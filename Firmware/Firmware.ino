#include "dhtHandler.h"           //temperature and humidity functions
#include "mpu6050Handler.h"       //IMU MPU6050 functions
#include "currentSesnorHandler.h" //current sensor functions
#include "headers.h"              //all misc. headers and functions
#include "esp32InternalTime.h"
#include "MQTTFuncs.h"            //MQTT related functions
#include "webApp.h"               //Captive Portal webpages
#include <FS.h>                   //ESP32 File System

const long interval = 1000 * 60 * 5;        // Interval at which to read sensors//5 mintues
Neotimer dataAcqTimer = Neotimer(interval); // Set timer's preset

IPAddress ipV(192, 168, 4, 1);
String loadParams(AutoConnectAux &aux, PageArgument &args) //function to load saved settings
{
    (void)(args);
    File param = FlashFS.open(PARAM_FILE, "r");
    String v1 = "";
    String v2 = "";
    if (param)
    {
        Serial.println("load params func");
        aux.loadElement(param);
        Serial.println(param);
        //Serial.println(args);
        AutoConnectText &vibSValueElm = aux["vibSValue"].as<AutoConnectText>();
        AutoConnectText &curSValueElm = aux["curSValue"].as<AutoConnectText>();
        //vibSValueElm.value="VibS:91122";#
        Serial.println(vibSValueElm.value);
        v1 = String(vibSValueElm.value);
        v2 = String(curSValueElm.value);
        if (v1.length() > 0)
        {
            vibSValueElm.value = String("Vibration: ") + getMPU6050Data();
        }
        if (v2.length() > 0)
        {
            curSValueElm.value = String("Current: ") + getCurrentWatts();
        }
        // curSValueElm.value="CurS:7788";
        param.close();
    }
    else
        Serial.println(PARAM_FILE " open failed");
    return String("");
}

String saveParams(AutoConnectAux &aux, PageArgument &args) //save the settings
{
    serverName = args.arg("mqttserver"); //broker
    serverName.trim();

    channelId = args.arg("channelid");
    channelId.trim();

    userKey = args.arg("userkey"); //user name
    userKey.trim();

    apiKey = args.arg("apikey"); //password
    apiKey.trim();

    minActiveValue = args.arg("minActiveValue");
    minActiveValue.trim();

    ampSensorType = args.arg("ampSensorType");
    ampSensorType.trim();
    ampSensorType = ampSensorType.substring(0, 2);

    tempUnits = args.arg("tempUnits");
    tempUnits.trim();

    String upd = args.arg("period");
    upd = upd.substring(0, 2);
    sensorSelection = upd;

    apPass = args.arg("apPass"); //ap pass
    apPass.trim();

    settingsPass = args.arg("settingsPass"); //ap pass
    settingsPass.trim();

    hostName = args.arg("hostname");
    hostName.trim();

    // The entered value is owned by AutoConnectAux of /mqtt_setting.
    // To retrieve the elements of /mqtt_setting, it is necessary to get
    // the AutoConnectAux object of /mqtt_setting.
    File param = FlashFS.open(PARAM_FILE, "w");
    portal.aux("/mqtt_setting")->saveElement(param, {"mqttserver", "channelid", "userkey", "apikey", "period", "minActiveValue", "ampSensorType", "tempUnits", "hostname", "apPass", "settingsPass"});
    param.close();

    // Echo back saved parameters to AutoConnectAux page.
    AutoConnectText &echo = aux["parameters"].as<AutoConnectText>();
    echo.value = "Server: " + serverName + "<br>";
    echo.value += "Channel ID: " + channelId + "<br>";
    echo.value += "Username: " + userKey + "<br>";
    echo.value += "Password: " + apiKey + "<br>";
    echo.value += "Sensor Settings: " + String(upd) + "<br>";
    echo.value += "Min Active Value: " + minActiveValue + "<br>";
    echo.value += "Amp Sensor Type: " + ampSensorType + "<br>";
    echo.value += "Temperature Units: " + tempUnits + "<br>";
    echo.value += "ESP host name: " + hostName + "<br>";
    echo.value += "AP Password: " + apPass + "<br>";
    echo.value += "Settings Page Password: " + settingsPass + "<br>";

    return String("");
}
bool loadAux(const String auxName) //load defaults from data/*.json
{
    bool rc = false;
    Serial.println("load aux func");
    String fn = auxName + ".json";
    File fs = FlashFS.open(fn.c_str(), "r");
    if (fs)
    {
        rc = portal.load(fs);
        fs.close();
    }
    else
        Serial.println("Filesystem open failed: " + fn);
    return rc;
}
uint8_t inAP = 0;
bool whileCP()
{

    if (inAP == 0)
    {
        ledState(AP_MODE);
        inAP = 1;
    }
    // Serial.println("AP MODE");

    loopLEDHandler();
}

void setup() //main setup functions
{
    Serial.begin(115200);
    delay(1000);
    setupDHT22();
    setupMPU6050();
    setupCurrentSensor();

    if (!MDNS.begin("esp32")) //starting mdns so that user can access webpage using url `esp32.local`(will not work on all devices)
    {
        Serial.println("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
#if defined(ARDUINO_ARCH_ESP8266)
    FlashFS.begin();
#elif defined(ARDUINO_ARCH_ESP32)
    FlashFS.begin(true);
#endif
    loadAux(AUX_MQTTSETTING);
    loadAux(AUX_MQTTSAVE);
    AutoConnectAux *setting = portal.aux(AUX_MQTTSETTING);
    if (setting) //get all the settings parameters from setting page on esp32 boot
    {
        Serial.println("Setting loaded");
        PageArgument args;
        AutoConnectAux &mqtt_setting = *setting;
        loadParams(mqtt_setting, args);
        AutoConnectInput &hostnameElm = mqtt_setting["hostname"].as<AutoConnectInput>();
        AutoConnectInput &apPassElm = mqtt_setting["apPass"].as<AutoConnectInput>();
        AutoConnectInput &serverNameElm = mqtt_setting["mqttserver"].as<AutoConnectInput>();
        AutoConnectInput &channelidElm = mqtt_setting["channelid"].as<AutoConnectInput>();
        AutoConnectInput &userkeyElm = mqtt_setting["userkey"].as<AutoConnectInput>();
        AutoConnectInput &apikeyElm = mqtt_setting["apikey"].as<AutoConnectInput>();
        AutoConnectInput &minActiveValueElm = mqtt_setting["minActiveValue"].as<AutoConnectInput>();
        AutoConnectRadio &ampSensorTypeElm = mqtt_setting["ampSensorType"].as<AutoConnectRadio>();
        AutoConnectRadio &tempUnitsElm = mqtt_setting["tempUnits"].as<AutoConnectRadio>();
        AutoConnectRadio &periodElm = mqtt_setting["period"].as<AutoConnectRadio>();
        AutoConnectText &vibSValueElm = mqtt_setting["vibSValue"].as<AutoConnectText>();
        AutoConnectText &curSValueElm = mqtt_setting["curSValue"].as<AutoConnectText>();
        AutoConnectInput &settingsPassElm = mqtt_setting["settingsPass"].as<AutoConnectInput>();
        //vibSValueElm.value="VibS:11";
        serverName = String(serverNameElm.value);
        channelId = String(channelidElm.value);
        userKey = String(userkeyElm.value);
        apiKey = String(apikeyElm.value);
        minActiveValue = String(minActiveValueElm.value);
        ampSensorType = String(ampSensorTypeElm.value());
        tempUnits = String(tempUnitsElm.value());
        sensorSelection = String(periodElm.value());
        hostName = String(hostnameElm.value);
        apPass = String(apPassElm.value);
        settingsPass = String(settingsPassElm.value);
        if (hostnameElm.value.length())
        {
            //hostName=hostName+ String("-") + String(GET_CHIPID(), HEX);
            //;
            //portal.config(hostName.c_str(), apPass.c_str());
            // portal.config(hostName.c_str(), "123456789AP");
            config.apid = hostName; //hostnameElm.value+ "-" + String(GET_CHIPID(), HEX);
            config.password = apPass;
            config.psk = apPass;
            // portal.config(hostName.c_str(), "123456789AP");
            Serial.println("(from hostELM) hostname set to " + hostName);
        }
        else
        {

            // hostName = String("OEE");;
            // portal.config(hostName.c_str(), "123456789AP");
            config.apid = hostName; //hostnameElm.value+ "-" + String(GET_CHIPID(), HEX);
            config.password = apPass;
            config.psk = apPass;
            //config.hostName = hostName;//hostnameElm.value+ "-" + String(GET_CHIPID(), HEX);
            // portal.config(hostName.c_str(), "123456789AP");
            Serial.println("hostname set to " + hostName);
        }
        config.homeUri = "/_ac";
        config.apip = ipV;

        portal.on(AUX_MQTTSETTING, loadParams);
        portal.on(AUX_MQTTSAVE, saveParams);
    }
    else
    {
        Serial.println("aux. load error");
    }
    //config.homeUri = "/_ac";
    config.apip = ipV;
    config.autoReconnect = true;
    config.reconnectInterval = 1;
    Serial.print("AP: ");
    Serial.println(hostName);
    Serial.print("Password: ");
    Serial.println(apPass);
    config.title = "OEE Sensing"; //set title of webapp

    //add different tabs on homepage
    portal.append("/api-now", "api-now");
    portal.append("/api", "api");
    portal.append("/LiveSensors", "LiveSensors");
    portal.append("/data", "DataTable");
    portal.disableMenu(AC_MENUITEM_DISCONNECT);
    server.on("/", handleRoot);
    server.on("/api-now", cmotsValues);
    server.on("/api", api);
    server.on("/LiveSensors", live);
    server.on("/data", dataTable);
    // Starts user web site included the AutoConnect portal.

    config.auth = AC_AUTH_DIGEST;
    config.authScope = AC_AUTHSCOPE_PARTIAL;
    config.username = hostName;
    config.password = settingsPass;

    portal.config(config);
    portal.whileCaptivePortal(whileCP);
    portal.onDetect(atDetect);
    portal.load(FPSTR(PAGE_AUTH));
    if (portal.begin())
    {
        Serial.println("Started, IP:" + WiFi.localIP().toString());
        ledState(AP_MODE);
    }
    else
    {
        Serial.println("Connection failed.");
        while (true)
        {
            yield();
            delay(100);
        }
    }

    MDNS.addService("http", "tcp", 80);
    mqttConnect(); //start mqtt
}

void loop()
{
    server.handleClient();
    portal.handleRequest();
    loopLEDHandler();
    if (dataAcqTimer.repeat())
    {
        OEEValue = ss.getOEEValue();
        ss.addSensorValue(getTimestamp(), OEEValue, getTemp(tempUnits), getHumid(), getCurrentWatts());
    }
    if (millis() - lastPub > updateInterval) //publish data to mqtt server
    {
        mqttPublish("OEE/" + String(hostName), getTimestamp() + String(";") + getTempHumid(tempUnits) + String(";") + getMPU6050Data() + String(";") + getCurrentWatts()); //publish data to mqtt broker
        ledState(ACTIVE_MODE);
        //uncomment the lines below for debugging
        // Serial.println(ampSensorType);
        // Serial.println(sensorSelection);
        // Serial.println(minActiveValue);
        // Serial.println(channelId);
        // Serial.println(userKey);
        // Serial.println(apiKey);
        // Serial.println(apid);
        // Serial.println(hostName);
        // Serial.println(apPass);
        // Serial.println(tempUnits)

        lastPub = millis();
    }
    if (!mqttClient.connected())
    {
        reconnect();
    }
    mqttClient.loop();
    if (WiFi.status() == WL_IDLE_STATUS)
    {
        ledState(IDLE_MODE);
        ESP.restart();

        delay(1000);
    }
}
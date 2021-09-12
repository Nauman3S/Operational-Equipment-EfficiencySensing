#include "dhtHandler.h"           //temperature and humidity functions
#include "mpu6050Handler.h"       //IMU MPU6050 functions
#include "currentSesnorHandler.h" //current sensor functions
#include "headers.h"              //all misc. headers and functions
#include "MQTTFuncs.h"            //MQTT related functions
#include "webApp.h"               //Captive Portal webpages
#include <FS.h>                   //ESP32 File System

String loadParams(AutoConnectAux &aux, PageArgument &args) //function to load saved settings
{
    (void)(args);
    File param = FlashFS.open(PARAM_FILE, "r");
    if (param)
    {
        aux.loadElement(param);
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

    String upd = args.arg("period");
    upd = upd.substring(0, 2);
    sensorSelection = upd;

    apPass = args.arg("apPass"); //ap pass
    apPass.trim();

    hostName = args.arg("hostname");
    hostName.trim();

    // The entered value is owned by AutoConnectAux of /mqtt_setting.
    // To retrieve the elements of /mqtt_setting, it is necessary to get
    // the AutoConnectAux object of /mqtt_setting.
    File param = FlashFS.open(PARAM_FILE, "w");
    portal.aux("/mqtt_setting")->saveElement(param, {"mqttserver", "channelid", "userkey", "apikey", "period", "minActiveValue", "ampSensorType", "hostname", "apPass"});
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
    echo.value += "ESP host name: " + hostName + "<br>";
    echo.value += "AP Password: " + apPass + "<br>";

    return String("");
}
bool loadAux(const String auxName) //load defaults from data/*.json
{
    bool rc = false;
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
        AutoConnectRadio &periodElm = mqtt_setting["period"].as<AutoConnectRadio>();

        serverName = String(serverNameElm.value);
        channelId = String(channelidElm.value);
        userKey = String(userkeyElm.value);
        apiKey = String(apikeyElm.value);
        minActiveValue = String(minActiveValueElm.value);
        ampSensorType = String(ampSensorTypeElm.value());
        sensorSelection = String(periodElm.value());
        hostName = String(hostnameElm.value);
        apPass = String(apPassElm.value);
        if (hostnameElm.value.length())
        {
            //hostName=hostName+ String("-") + String(GET_CHIPID(), HEX);
            hostName = String("OEE") + String("-") + String(GET_CHIPID(), HEX);
            portal.config(hostName.c_str(), apPass.c_str());
            //config.hostName = hostName;//hostnameElm.value+ "-" + String(GET_CHIPID(), HEX);
            Serial.println("hostname set to " + hostName);
        }
        config.homeUri = "/_ac";

        portal.on(AUX_MQTTSETTING, loadParams);
        portal.on(AUX_MQTTSAVE, saveParams);
    }
    else
    {
        Serial.println("aux. load error");
    }
    config.homeUri = "/_ac";
    config.autoReconnect = true;
    config.reconnectInterval = 1;
    Serial.print("AP: ");
    Serial.println(hostName);
    Serial.print("Password: ");
    Serial.println(apPass);
    config.title = "OEE Sensing"; //set title of webapp
    //add different tabs on homepage
    portal.append("/api-now", "api-now");
    portal.append("/LiveSensors", "LiveSensors");
    server.on("/", handleRoot);
    server.on("/api-now", cmotsValues);
    server.on("/LiveSensors", live);
    // Starts user web site included the AutoConnect portal.
    portal.config(config);
    portal.onDetect(atDetect);
    if (portal.begin())
    {
        Serial.println("Started, IP:" + WiFi.localIP().toString());
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
    if (millis() - lastPub > updateInterval) //publish data to mqtt server
    {
        mqttPublish("OEE/" + String(hostName), getTempHumid() + String(";") + getMPU6050Data() + String(";") + getCurrentWatts()); //publish data to mqtt broker
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

        lastPub = millis();
    }
    if (!mqttClient.connected())
    {
        reconnect();
    }
    mqttClient.loop();
    if (WiFi.status() == WL_IDLE_STATUS)
    {
        ESP.restart();

        delay(1000);
    }
}
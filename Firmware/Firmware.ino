#include "dhtHandler.h"
#include "mpu6050Handler.h"
#include "currentSesnorHandler.h"
#include "headers.h"
#include "MQTTFuncs.h"
#include "webApp.h"
unsigned long lastPub = 0;
unsigned int  updateInterval = 2000;
void setup()
{
    Serial.begin(115200);
    delay(1000);
    setupDHT22();
    //setupMPU6050();
    setupCurrentSensor();

    if (!MDNS.begin("esp32"))
    {
        Serial.println("Error setting up MDNS responder!");
        while (1)
        {
            delay(1000);
        }
    }
    portal.config("ESP32AP", "123456789AP");
    config.title = "SmartTControl";
    portal.append("/cmots", "Cmots");
    server.on("/", handleRoot);
    // server.on("/io", handleGPIO);
    server.on("/dev", handleDEV);
    server.on("/conn", handleCON);
    server.on("/cmots", cmotsValues);

    // Starts user web site included the AutoConnect portal.
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
        }
    }

    MDNS.addService("http", "tcp", 80);
    mqttConnect();
}

void loop()
{
    server.handleClient();
    portal.handleRequest();
    if (millis() - lastPub > updateInterval) {
    mqttPublish("channels/0011","data here");
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
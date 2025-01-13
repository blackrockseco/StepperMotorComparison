// include/wifi_config.h
#pragma once
#include <Arduino.h>

#if defined(ESP32)
    #include <WiFi.h>
    #include <ArduinoOTA.h>

    // WiFi credentials
    const char* WIFI_SSID = "The Rabbit Hole";
    const char* WIFI_PASSWORD = "BurningMan";
    const char* OTA_HOSTNAME = "ESP32-Stepper-Test";

    class WifiManager {
    public:
        static bool setupWiFiAndOTA() {
            Serial.println("Setting up WiFi and OTA...");
            
            // Connect to WiFi with timeout
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
            int wifi_attempt = 0;
            while (WiFi.status() != WL_CONNECTED && wifi_attempt < 20) {
                delay(500);
                Serial.print(".");
                wifi_attempt++;
            }
            
            if (WiFi.status() != WL_CONNECTED) {
                Serial.println("\nWiFi connection failed!");
                return false;
            }

            Serial.println("\nConnected to WiFi!");
            Serial.print("IP address: ");
            Serial.println(WiFi.localIP());

            // Configure OTA
            ArduinoOTA.setHostname(OTA_HOSTNAME);
            
            ArduinoOTA.onStart([]() {
                String type = ArduinoOTA.getCommand() == U_FLASH ? "sketch" : "filesystem";
                Serial.println("Starting OTA update (" + type + ")");
            });
            
            ArduinoOTA.onEnd([]() {
                Serial.println("\nOTA update complete!");
            });
            
            ArduinoOTA.onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("End Failed");
            });

            ArduinoOTA.begin();
            Serial.println("OTA Ready!");
            return true;
        }

        static void handle() {
            if (WiFi.status() == WL_CONNECTED) {
                ArduinoOTA.handle();
            }
        }
    };
#endif
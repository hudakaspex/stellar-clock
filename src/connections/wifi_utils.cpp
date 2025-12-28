#include <Arduino.h>
#include <WiFiManager.h>
#include "wifi_utils.h"

WiFiManager wm;

const char *apName = "feeder";
const char *apPassword = "12345678";


void WifiUtils::connect() {
    boolean isConnect = wm.autoConnect(apName, apPassword); // connect using
    if (isConnect) {
        Serial.println("Connected to WiFi!");
    } else {
        Serial.println("Failed to connect to WiFi.");
        Serial.println("Connect to default AP with SSID: " + String(apName) + " Password: " + String(apPassword));
    }
    Serial.println(WiFi.localIP());
}

boolean WifiUtils::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

void WifiUtils::reset() {
    wm.disconnect();
    wm.resetSettings();
    wm.startConfigPortal(apName, apPassword);
}
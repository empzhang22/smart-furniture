#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "08 307";
const char* password = "coolR0bots13!";

const int LED_PIN = 2;
const int UDP_PORT = 4210;

WiFiUDP udp;

unsigned long timeOn = 1000;
unsigned long timeOff = 1000;

unsigned long previousMillis = 0;
bool ledState = false;

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Connected!");
    Serial.print("ESP32 IP: ");
    Serial.println(WiFi.localIP());

    udp.begin(UDP_PORT);

    Serial.print("Listening on UDP port ");
    Serial.println(UDP_PORT);
}

void loop() {
    // -----------------------------
    // Check for commands from PC
    // -----------------------------

    int packetSize = udp.parsePacket();

    if (packetSize > 0) {
        char buffer[128];

        int len = udp.read(buffer, sizeof(buffer) - 1);

        if (len > 0) {
            buffer[len] = '\0';
        }

        Serial.print("Received: ");
        Serial.println(buffer);

        // Expected format:
        // timeOn,timeOff
        //
        // Example:
        // 1000,2000

        char* comma = strchr(buffer, ',');

        if (comma != nullptr) {
            *comma = '\0';

            unsigned long newTimeOn = strtoul(buffer, nullptr, 10);
            unsigned long newTimeOff = strtoul(comma + 1, nullptr, 10);

            if (newTimeOn > 0 && newTimeOff > 0) {
                timeOn = newTimeOn;
                timeOff = newTimeOff;

                Serial.print("New ON time: ");
                Serial.println(timeOn);

                Serial.print("New OFF time: ");
                Serial.println(timeOff);
            }
        }
    }

    // -----------------------------
    // LED timing
    // -----------------------------

    unsigned long currentMillis = millis();

    unsigned long interval;

    if (ledState) {
        interval = timeOn;
    } else {
        interval = timeOff;
    }

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        ledState = !ledState;

        digitalWrite(
            LED_PIN,
            ledState ? HIGH : LOW
        );
    }
}
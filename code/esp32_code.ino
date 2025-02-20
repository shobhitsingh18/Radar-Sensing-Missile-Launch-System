#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* mqtt_server = "your_raspberry_pi_ip";

WiFiClient espClient;
PubSubClient client(espClient);

const int radarPin = 34;  
const int trigPin = 5;    
const int echoPin = 18;   

void setup_wifi() {
    delay(10);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void reconnect() {
    while (!client.connected()) {
        if (client.connect("ESP32_Client")) {
            // Subscribe if needed
        } else {
            delay(5000);
        }
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);

    pinMode(radarPin, INPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
}

void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    
    int radarValue = analogRead(radarPin);

    //for ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;


    String payload = "{\"radar\":" + String(radarValue) + ", \"distance\":" + String(distance) + "}";
    client.publish("sensors/data", payload.c_str());

    delay(1000);
}

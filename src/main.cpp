// esp8266 deepsleep
#include <ESP8266WiFi.h>
#include <credentials.h>

#define PIN_SENSOR_ON_OFF_CONTROL D2
#define OFF_PIN D7
#define DATA_INTERVAL 3600e6

String apiWritekey = APIKEY;
char ssid[] = WIFI_SSID;
char password[] = WIFI_PASSWD;
char server[] = "api.thingspeak.com";
WiFiClient client;
void connect1() {
    WiFi.disconnect();
    delay(10);
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("NodeMcu connected to wifi...");
}

void sendData(int value) {
    if (client.connect(server, 80)) {
        String Data = apiWritekey;
        Data += "&field1=";
        Data += String(value);
        Data += "\r\n\r\n";

        client.print("POST /update HTTP/1.1\n");
        client.print("Host: api.thingspeak.com\n");
        client.print("Connection: close\n");
        client.print("X-THINGSPEAKAPIKEY: " + apiWritekey + "\n");
        client.print("Content-Type: application/x-www-form-urlencoded\n");
        client.print("Content-Length: ");
        client.print(Data.length());
        client.print("\n\n");
        client.print(Data);
        delay(10);
        // // print out response 
        // while (client.connected() || client.available()) {
        //     if (client.available()) {
        //         String line = client.readStringUntil('\n');
        //         Serial.println(line);
        //     }
        // }
        Serial.println("uploaded to Thingspeak server....");
    }
    client.stop();
}
void setup() {
    pinMode(OFF_PIN, OUTPUT);
    digitalWrite(OFF_PIN, LOW);
    Serial.begin(115200);
    Serial.println("setup...");
    pinMode(PIN_SENSOR_ON_OFF_CONTROL, OUTPUT);
    while (!Serial) {
    }
    connect1();
    digitalWrite(PIN_SENSOR_ON_OFF_CONTROL, HIGH); // turn sensor on
    delay(2000); 
    int value = analogRead(0);
    digitalWrite(PIN_SENSOR_ON_OFF_CONTROL, LOW);
    Serial.print("Value: ");
    Serial.println(value);
    sendData(value);
    delay(10);
    digitalWrite(OFF_PIN, HIGH); // turn aduino off
    delay(50);
    digitalWrite(OFF_PIN, LOW); // not really necessary
    delay(50);
}

void loop()
{
}
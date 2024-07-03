#include <WiFiS3.h>
#include <Servo.h>

char ssid[] = "wiranphat_2.4GHz";
char pass[] = "0947195386";
const char *host = "192.168.1.200";
int status = WL_IDLE_STATUS;

WiFiClient client;
Servo S1;
Servo S2;
Servo S3;
Servo S4;

int servoPin = 3;
int servoPin1 = 5;
int servoPin2 = 6;
int servoPin3 = 9;

uint8_t msg[5];

void setup() {
  Serial.begin(9600);
  Serial.println("Servo control with WiFi");

  S1.attach(servoPin);
  S2.attach(servoPin1);
  S3.attach(servoPin2);
  S4.attach(servoPin3);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("Connected to WiFi");

  IPAddress serverIP;
  serverIP.fromString(host);
  if (client.connect(serverIP, 80)) {
    Serial.println("Connected to server");
  } else {
    Serial.println("Connection to server failed");
  }
}

void loop() {
  if (client.connected()) {
    if (client.available()) {
      msg[0] = client.read();
      msg[1] = client.read();
      msg[2] = client.read();
      msg[3] = client.read();

      Serial.println("Received Flex Sensor Value: " + String(msg[0]));
      Serial.println("Received Flex Sensor Value1: " + String(msg[1]));
      Serial.println("Received Flex Sensor Value2: " + String(msg[2]));
      Serial.println("Received Flex Sensor Value3: " + String(msg[3]));

      S1.write(msg[0]);
      S2.write(msg[1]);
      S3.write(msg[2]);
      S4.write(msg[3]);
      delay(500);
    }
  } else {
    Serial.println("Lost connection to server, reconnecting...");
    IPAddress serverIP;
    serverIP.fromString(host);
    if (client.connect(serverIP, 80)) {
      Serial.println("Reconnected to server");
    } else {
      Serial.println("Reconnection to server failed");
    }
  }
}

#include "WiFiS3.h"

char ssid[] = "wiranphat_2.4GHz";
char pass[] = "0947195386";
int status = WL_IDLE_STATUS;

WiFiServer server(80);
int flexPin = A0; 
int flexPin1 = A1;
int flexPin2 = A2;
int flexPin3 = A3;
int flexValue;
int flexValue1;
int flexValue2;
int flexValue3;

uint8_t msg[5];

void setup() {
  Serial.begin(9600);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }

  server.begin();
  printWifiStatus();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Send HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");
            client.println();
            
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<head><title>Flex Sensor Data</title></head>");
            client.println("<body><h1>Flex Sensor Values</h1>");
            
            flexValue = analogRead(flexPin);
            flexValue = map(flexValue, 0, 1023, 0, 180);
            flexValue1 = analogRead(flexPin1);
            flexValue1 = map(flexValue1, 0, 1023, 0, 180);
            flexValue2 = analogRead(flexPin2);
            flexValue2 = map(flexValue2, 0, 1023, 0, 180);
            flexValue3 = analogRead(flexPin3);
            flexValue3 = map(flexValue3, 0, 1023, 0, 180);

            client.println("<p>Flex Sensor Value: " + String(flexValue) + "</p>");
            client.println("<p>Flex Sensor Value1: " + String(flexValue1) + "</p>");
            client.println("<p>Flex Sensor Value2: " + String(flexValue2) + "</p>");
            client.println("<p>Flex Sensor Value3: " + String(flexValue3) + "</p>");

            msg[0] = flexValue;
            msg[1] = flexValue1;
            msg[2] = flexValue2;
            msg[3] = flexValue3;
            client.write(msg, 4);
            
            // client.println("</body></html>");
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    delay(1);
    client.stop();
    Serial.println("Client disconnected");
  }
}

void printWifiStatus() {
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

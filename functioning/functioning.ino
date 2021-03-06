#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
const char* ssid     = "madar";
const char* password = "nyiccsdkianyad";
int characterCount;

WiFiClient client;

const char server[] = "192.168.1.24";

void setup() {
  Serial.println("Attempting to connect to wifi");
  connect_to_wifi();
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Wire.begin();        // join i2c bus (address optional for master)

  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void connect_to_wifi() {
  Serial.begin(9600);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

}

void send_data(String url) {
  if (client.connect(server, 3000)) {
    if (client) {
      while (client.connected()) {
        //String url = "/api/store?left=" + String(left) + "&center=" + String(center) + "&right=" + String(right) + "";
        
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host:  192.168.1.24\r\n" +
                     "Connection: close\r\n\r\n");

        /*Serial.println("SENT");
          Serial.println("Response:");

          while(client.available()){
          String line = client.readStringUntil('\r');
          Serial.print(line);
          }*/
        //delay(80);
        break;
      }
    }
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {  
    Wire.requestFrom(8, 6);    // request 6 bytes from slave device #8
  char message[50]= "";
  characterCount = 0;
  
  while (Wire.available()) { // slave may send less than requested
    message[characterCount] = Wire.read(); // receive a byte as character
    characterCount += 1;
  }
  String messageString(message);
  Serial.print("Received message :" + messageString);

  send_data(messageString);
  
}

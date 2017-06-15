#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
const char* ssid     = "madar";
const char* password = "nyiccsdkianyad";
int characterCount;
String url;

volatile int left_a;volatile int left_b;volatile int left;

volatile int center_a;volatile int center_b;volatile int center;

volatile int right_a;volatile int right_b;volatile int right;

WiFiClient client;

const char server[] = "192.168.0.102";

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
                     "Host:  192.168.0.102\r\n" +
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
  
  // ~~ START Inquire value from SlaveID#11 ~~ //
  Wire.requestFrom(11, 6);
  while (Wire.available())
  {
    left_a = Wire.read(); 
    left_b = Wire.read();
    left = (left_b << 8) | left_a;
    Serial.print("left:"); Serial.print(left);

    center_a = Wire.read(); 
    center_b = Wire.read();
    center = (center_b << 8) | center_a;
    Serial.print("center:"); Serial.print(center);

    right_a = Wire.read(); 
    right_b = Wire.read();
    right = (right_b << 8) | right_a;
    Serial.print("right:"); Serial.print(right);
    Serial.println("");
  }

  url = "/api/store?left=" + String(left) + "&center=" + String(center) + "&right=" + String(right) + "";

  send_data(url);
  delay(200);

  
}

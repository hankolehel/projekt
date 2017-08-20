#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
const char* ssid     = "DIGI-6D5M";
const char* password = "exx257Cr";
String url;

volatile int left_a;volatile int left_b;volatile int left;

volatile int center_a;volatile int center_b;volatile int center;

volatile int right_a;volatile int right_b;volatile int right;

WiFiClient client;

char *server = "192.168.100.2";

void setup() {
  Serial.println("Attempting to connect to wifi");
  connect_to_wifi();
  client.setTimeout(150);
  
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
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendMessageToArduino(String message){
  byte x;
  if (message.equals("work"))            //if we got the command to work, we send a 0
      x = 0;             
  else
    if (message.equals("stop"))          //if we got a command to stop, we send a 1
      x = 1;
    
  
  Wire.beginTransmission(11); // transmit to device #11
  Wire.write(x);              // sends one byte
  Wire.endTransmission();    // stop transmitting
}

void send_data(String url) {
  if (client.connect(server, 3000)) {
    if (client) {
      if (client.connected()) {
        
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host:  "+String(server)+"\r\n" +
                     "Connection: close\r\n\r\n");

          //Serial.print("Response:");

          while(client.available()){
          String line = client.readStringUntil('\r');
            if (line.equals("\n")){
              line = client.readStringUntil('\r');              
              line.trim();
              if (line.equals("work") || line.equals("stop")){
                sendMessageToArduino(line); 
              }
            }
          }
        //delay(80);
      }
    }
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

void loop() {  
  
  // ~~ START Inquire value from SlaveID#11 ~~ //
  Serial.println("Starting request");
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
  
  delay(100);

  
}

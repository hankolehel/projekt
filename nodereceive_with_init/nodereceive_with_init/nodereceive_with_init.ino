#include <ESP8266WiFi.h>
#include <json_arduino.h>
#include <Wire.h>

//variables used for initialization
 
const char *ssid = "ESP8266 Access Point"; // The name of the Wi-Fi network that will be created
const char *password = "thereisnospoon";   // The password required to connect to it, leave blank for an open network
token_list_t *token_list = NULL;
String systemMessage = "";
WiFiServer server(80);

//variables used for twi and communication

WiFiClient clientToServer;
volatile int left_a;volatile int left_b;volatile int left;
volatile int center_a;volatile int center_b;volatile int center;
volatile int right_a;volatile int right_b;volatile int right;

String serverAddress;
String url;

void setup() {
  Serial.begin(9600);
  delay(10);
 
  clientToServer.setTimeout(100);
  WiFi.softAP(ssid, password);             // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP());         // Send the IP address of the ESP8266 to the computer
  
  // Start the server
  server.begin();
  token_list = create_token_list(25);
  bool keepHosting = true;
  
  while(keepHosting){
    // Check if a client has connected
    WiFiClient client;
    while ((client = server.available()) == false){}
   
    // Wait until the client sends some data
    Serial.println("new client");
    int timeout;
    while(!client.available()){
      Serial.print(".");    
      delay(1);   
      timeout++;
      if(timeout>10000) {Serial.print("INFINITE LOOP BREAK!");  break;}
    }
      timeout=0;
    
        while (client.available ()) {                   //read every line of the request
          String line = client.readStringUntil('\r');  
          line.trim();
          //Serial.println(line);
          if ((line.indexOf("SSID") != -1) && (line.indexOf("password") != -1))  {              //if the request is a POST request
            char formJSON[line.length() + 1];       
            line.toCharArray(formJSON, line.length()+1);
            json_to_token_list(formJSON, token_list); // Convert JSON String to a Hashmap of Key/Value Pairs

            char *inputSSID =  json_get_value(token_list, (char *)"SSID");
            char *inputPassword = json_get_value(token_list, (char *)"password");
            char *svAddress = json_get_value(token_list, (char *)"serverAddress");
            serverAddress = String(svAddress);
            Serial.println("Received data:");
            Serial.print("SSID: ");
            Serial.println(inputSSID);
            Serial.print("Password: ");
            Serial.println(inputPassword);
            Serial.print("serverAddress: ");
            Serial.println(serverAddress);
            
            int connectionResult = connect_to_wifi(inputSSID, inputPassword);
            if (connectionResult == 0){
              keepHosting = false;
              Serial.println("Successfully connected, stopping Access Point");
            }
            
          }     //end ssid and password if
          
        }       // end available client while
  display_page(client, systemMessage);

  }
// at this point we are connected to a wifi and we know the IP address of the server
Wire.begin();             // join i2c bus (address optional for master)

}
void display_page(WiFiClient client, String text){
      client.flush();
     
    // Return the response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println(""); //  do not forget this one
    client.println("<html>");
  
      client.println("<h3>SSID:</h3>");
        client.println("<input type='text' id='SSID' required><br>");  
      client.println("<h3>Password:</h3>");
        client.println("<input type='password' id='password' required><br><br>");
      client.println("<h3>Server IP:</h3>");
        client.println("<input type='text' id='serverAddress' required><br><br>");
      client.println("<input type='button' name='submit' value='Submit' onclick='return sendForm()' />");
    client.println("<h2 id='StatusHeader'>"+ text + "</h2>");
    client.println("<script>");
    client.println("function isValid(text) {return text.match(/^[^\"<>&\\{}]*$/);}");
    client.println("function sendForm(){var ssid = document.getElementById('SSID').value;");
    client.println("var password = document.getElementById('password').value;");
    client.println("var serverAddress = document.getElementById('serverAddress').value;");
    client.println("if(isValid(ssid) && isValid(password) && serverAddress.match(/^(?:(?:2[0-4]\\d|25[0-5]|1\\d{2}|[1-9]?\\d)\\.){3}(?:2[0-4]\\d|25[0-5]|1\\d{2}|[1-9]?\\d)$/)){");
    client.println("document.getElementById('StatusHeader').innerHTML = 'Working on it..';");
    client.println("var xmlhttp = new XMLHttpRequest();");
    client.println("var url = 'URLTEST';");
    client.println(" var params = '{\"SSID\" : \"'+ssid +'\" , \"password\" : \"'+ password  +'\" , \"serverAddress\" : \"' + serverAddress +'\"}';");
    client.println("xmlhttp.open(\"POST\", url, true);");
    client.println("xmlhttp.send(params);");
    client.println("}else{document.getElementById('StatusHeader').innerHTML = 'Illegal character found in text input.'; }  ");
    client.println("}");
     client.println("</script>");
    client.println("</html>");
   
  
    Serial.println("Client disonnected");
    Serial.println("");
   
}

int connect_to_wifi(char *inputSSID, char *inputPassword) {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(inputSSID);

  WiFi.begin(inputSSID, inputPassword);
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (counter == 20){
      systemMessage = "Connection timed out. Maybe the provided SSID/password was wrong ?";
      WiFi.disconnect();
      WiFi.softAPdisconnect(true);
      WiFi.mode(WIFI_AP_STA);
      delay(100);
      WiFi.softAP(ssid, password);             // Start the access point
      Serial.print("Access Point \"");
      Serial.print(ssid);
      Serial.println("\" started");
    
      Serial.print("IP address:\t");
      Serial.println(WiFi.softAPIP());   
      break;
    }
    delay(500);
    Serial.print(".");
    counter = counter + 1;
  };
  if(counter < 20){
    systemMessage = "Connection successful!";
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP()); 
    return 0;
  }
  return -1;
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
  char charBuf[15];
  serverAddress.toCharArray(charBuf, 15); 
  
  if (clientToServer.connect(charBuf, 3000)) {
    if (clientToServer) {
      if (clientToServer.connected()) {
        //String url = "/api/store?left=" + String(left) + "&center=" + String(center) + "&right=" + String(right) + "";
        
        clientToServer.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host:  "+String(serverAddress)+"\r\n" +
                     "Connection: close\r\n\r\n");

        Serial.print("Response:");

          while(clientToServer.available()){
          String line = clientToServer.readStringUntil('\r');
            if (line.equals("\n")){
              line = clientToServer.readStringUntil('\r');              
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
   //  if you didn't get a connection to the server:
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
  //Serial.println(url);
  send_data(url);
  delay(200);
}






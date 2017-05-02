#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid     = "Big_Cats";
const char* password = "big cats gatekeeper";

unsigned int timerertek;
double tavolsag1,tavolsag2,tavolsag3;
unsigned char value;
int timerNumberCount;
double tavolsag[] = {0, 0, 0};

//int echo[4] = {16, 4, 14};
//int trigger[4] = {5, 0, 12};


int echo[4] = {4, 14, 16};
int trigger[4] = {0, 12, 5};
s
int control [4] = {0, 0, 0};
int measurementSave [4] = {0, 0, 0};


/*#define echo1 16
#define trigger1 5

#define echo2 4
#define trigger2 0

#define echo3 14
#define trigger3 12*/

WiFiClient client;

const char server[] = "192.168.1.24";

void connect_to_wifi(){
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

void send_data(int left, int center, int right){
  if (client.connect(server, 3000)) {
        if (client) {
          while (client.connected()) {
            String url = "/api/store?left="+String(left)+"&center="+String(center)+"&right="+String(right)+"";
            client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host:  192.168.1.24\r\n" + 
                 "Connection: close\r\n\r\n");
                 
            /*Serial.println("SENT");
            Serial.println("Response:");
           
            while(client.available()){
              String line = client.readStringUntil('\r');
              Serial.print(line);
            }*/
            delay(100);
            break;
          }
        }
    }else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
    }
}

void setup() {

  pinMode(13, OUTPUT);//trigger
  pinMode(15, OUTPUT);//trigger
  
  
  pinMode(trigger[0],OUTPUT);//trigger
  pinMode(echo[0],INPUT); //echo 
  pinMode(trigger[1],OUTPUT);//trigger
  pinMode(echo[1],INPUT); //echo 
  pinMode(trigger[2],OUTPUT);//trigger
  pinMode(echo[2],INPUT); //echo 
  
  Serial.println("Ultrahangos meres");
  connect_to_wifi(); 
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  init_validator();
  // give the Ethernet shield a second to initialize:
  delay(1000);
}

void init_validator(){
    int i,j;
    read_sensors();
      for(i=0; i<3; i++){     
        control[i] = tavolsag[i]/2;
        measurementSave[i] = tavolsag[i];
      }
    for(i=0; i<5; i++){     
      read_sensors();
      for(j=0; j<3; j++){     
        control[j] = (control[j] + tavolsag[j])/2;
      }
    }
    
}

void read_sensors(){
  for(timerNumberCount=0; timerNumberCount<3; timerNumberCount++){
    digitalWrite(trigger[timerNumberCount],HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger[timerNumberCount],LOW);    //trigger vege

    value = 0;
    //varom amig az echon 1-est latok   

    timerertek = 0;
    
    while (value!=1)
      value = digitalRead(echo[timerNumberCount]);
    
    timerertek = 0;
    while (value!=0)
      {
      value = digitalRead(echo[timerNumberCount]); 
      timerertek++;
      delayMicroseconds(1);
      }

    tavolsag[timerNumberCount] = timerertek * 0.0388;
  }
    
}

bool disregard_invalid_data(){
  bool disregard = false;
  int i;
    for (i=0; i<3; i++){
      if (tavolsag[i] < 100){
        if ( abs(tavolsag[i] - control[i]) > 35 ){
            disregard = true;
        }      
        control[i] = (control[i] + tavolsag[i]) / 2;     
      }else{
          tavolsag[i] = 100;     
      }   
    }

  return disregard;
}

  void loop() {

    read_sensors();
    bool disregard = disregard_invalid_data();
    if (!disregard){

      send_data(tavolsag[0],tavolsag[1],tavolsag[2]);
     
    }else{
      Serial.println("DISREGARDED DATA");

    }
      Serial.print("elso = ");
      Serial.print(tavolsag[0]);
      Serial.print(" mm ");
  
      Serial.print("masodik = ");    
      Serial.print(tavolsag[1]);
      Serial.print(" mm ");
          
      Serial.print("harmadik = ");
      Serial.print(tavolsag[2]);
      Serial.println(" mm");

 
 /* Serial.println(validator.left);
    Serial.println(validator.center);
    Serial.println(validator.right);
    delay(500);*/
  
}

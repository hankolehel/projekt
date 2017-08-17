
#include <Wire.h>

int turn_direction = 0; // 0 means hypothetical middle, 1: right, -1:left
unsigned int turn_intensity = 0; // the intensity of the turn

unsigned char value;
unsigned int timerertek;
double tavolsag[] = {0, 0, 0};
int echo[4] = {4, 14, 16};
int trigger[4] = {0, 12, 5};
int B = 2;
int A = 3;
int trigger_common = 7;
int echo_common = 8;
char url[50];
int distance_average[4] = {0, 0, 0};

void setup() 
  {
  Serial.begin(9600);    
  Serial.println("Starting setup");
  
  Wire.begin(11);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  
  int left_right_distance_delta = 0;
  int turn_direction = 0;
  unsigned int turn_intensity = 0;
  
  Serial.println("Initializing trigger/echo pins");
    // put your setup code here, to run once:
  Serial.println("Setting pins");
  pinMode(B, OUTPUT);     //B
  pinMode(A, OUTPUT);     //A

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  pinMode(trigger_common, OUTPUT);     //TRIGGER COMMON
  pinMode(echo_common, INPUT);      //ECHO COMMON


  Serial.println("Trigger/echo pin initialization complete");
  Serial.println("Initializing distance average array validator");
  init_validator();
  Serial.println("Distance average array initialized");

  Serial.println("Setup complete");
}

void init_validator() {
  int i, j;
  multiplexed_reading();
  for (i = 0; i < 5; i++) {
    multiplexed_reading();
    distance_average[j] = (distance_average[j] + tavolsag[j]) / 2;
  }
}

void read_sensor(int sensorNumber) 
  {
      digitalWrite(trigger_common, HIGH);
      delayMicroseconds(10);
      //10 us is the trigger pulse
      digitalWrite(trigger_common, LOW);  
      
      value = 0;
      //varom amig az echon 1-est latok
  
      timerertek = 0;
  
      while (value != 1)
        value = digitalRead(echo_common);
 
      
      timerertek = 0;
      while ((value != 0)&&(timerertek <= 40000))
      {
        value = digitalRead(echo_common);
        timerertek++;
        delayMicroseconds(1);
      }


      if (timerertek>40000){
        Serial.println("reached over");
      }
  
      //if (timerertek==40000)
      
      //we calibrated this with measurements
      tavolsag[sensorNumber] = timerertek * 0.0786;
}

void output_sensors(int sensorNumber){
  Serial.print(tavolsag[sensorNumber]);
  Serial.print(" mm (");
  Serial.print(distance_average[sensorNumber]);
  Serial.print(" mm ");
}

bool disregard_invalid_data() {
  bool disregard = false;
  int i;
  for (i = 0; i < 3; i++) {
    if (tavolsag[i] < 250) {
      if ( abs(tavolsag[i] - distance_average[i]) > 150 ) {
        disregard = true;
      }
      distance_average[i] = tavolsag[i];// (distance_average[i] + tavolsag[i]) / 2;
    } else {
      tavolsag[i] = 250;
    }
  }
  return disregard;
}

void multiplexed_reading(){
 //Serial.print("Left: ");
 digitalWrite(A, LOW);
 digitalWrite(B, LOW);
 read_sensor(0);
 output_sensors(0);


 //Serial.print("Center: ");
 digitalWrite(A, LOW);
 digitalWrite(B, HIGH);
 read_sensor(1);
 output_sensors(1);


 //Serial.print("Right: ");
 digitalWrite(A, HIGH);
 digitalWrite(B, LOW);
 read_sensor(2);
 output_sensors(2);
  Serial.println();  
  
}

void decide(int* turn_direction, unsigned int* turn_intensity) {
  *turn_intensity = 255;
  
  if (tavolsag[1] > 25) {     // ha van elore hely
    if (tavolsag[0] < 20) {   // balra 10nel kozelebb
      *turn_direction = 1;
    }
    else {
      if (tavolsag[2] < 20){ // jobbra 10 nel kozelebb
        *turn_direction = -1;
      }
      else {                    //ha balra sem surgos es jobbra sem kanyarodni  
        
        if (tavolsag[0] > 40 && tavolsag[2] > 40)           //ha mindket iranyban 50+, tarthatja egyenesen
          *turn_direction = 0;
        else{
          if (tavolsag[0] > tavolsag[2]){    //nem surgos, de balra kozelebb van
            *turn_direction = -1;       // balra kanyarodik
            *turn_intensity = 180;
          }else{                              //nem surgos de jobbra kozelebb van
            *turn_direction = 1;          //kulonben jobbra 
            *turn_intensity = 180;
          }
        }
      }

    }
  }else { // elore nincs hely
    *turn_direction = -2;    //kozepet allitok be es majd hatramegyek az applynal
  }
}

void apply_decisions(int turn_direction, int turn_intensity){
  switch (turn_direction) {
    case -1:
      digitalWrite(6,LOW);
      analogWrite(5,turn_intensity);
      break;
    case 0:
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);          //steering tires look straight
      break;
    case 1:
      analogWrite(6,turn_intensity);
      digitalWrite(5,LOW);
      break;
    case -2:            //no space ahead
      digitalWrite(6,LOW);
      digitalWrite(5,LOW);          //steering tires look straight
      
//      digitalWrite(11,LOW);
//      analogWrite(10,40);        //go back for 400 miliseconds
//      delay(200);
      digitalWrite(10,LOW);
      digitalWrite(11,LOW);          //steering tires look straight

  }

}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write(distance_average[0]); // lower byte
  Wire.write(distance_average[0]>>8); // upper byte

  Wire.write(distance_average[1]); // lower byte
  Wire.write(distance_average[1]>>8); // upper byte
  
  Wire.write(distance_average[2]); // lower byte
  Wire.write(distance_average[2]>>8); // upper byte
}

void loop() {
  multiplexed_reading();
  bool disregard = disregard_invalid_data();
  if (!disregard) {
    //decide(&turn_direction, &turn_intensity);
    //apply_decisions(turn_direction, turn_intensity);
    
  } else {
    Serial.println("DISREGARDED DATA");
  }
  delay(150);
//digitalWrite(5, LOW);
//for (int i = 0; i< 255; i+=30){
//  analogWrite(6,220);
//  Serial.println(String(i));
//  delay(3000);
//}

}

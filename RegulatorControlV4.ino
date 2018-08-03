#include <SPI.h>
//Clock line is pin 13. MOSI is pin 11, MISO is pin 12

const byte inWavePin = 9; //Define pin number for input wave
const byte inSensorPin = 8; //Define pin number for sensor input voltage
const byte outRead = A5; //Define pin number for feedback
const byte sensorPinV1 = A4; //Define pin number for V+ from sensor
const byte sensorPinV2 = A3; //Define pin number for V- from sensor

const byte slaveselectPin = 10;
const byte digipotPowerPin = 6; 
unsigned long timer; 
unsigned long previoustimer; // for resetting time

//DEFINE REQUIREMENTS
float frequencyDesired = 1;// In Hz - refrain from using greater than 10
float choosePSI = 5;//PSI (do not use until the DAC is integrated)

int stepTime = 500/frequencyDesired; 
int startTime = 500;// choose start time

int level = 158;
int channel = 0;

int pass =0;
int counter = 0; // counter for various uses
int pressureHIGH =0;// top pressure
float regValue = 0; // regulator output
float sensorValue1 = 0; // V+ sensor output
float sensorValue2 = 0; // V- sensor output
float filteredSensor1 =0; // digitally filtered (use when serial plotter is used instead of MATLAB
float filteredSensor2 =0; //

void setup() {
  // put your setup code here, to run once:
pinMode(inWavePin, OUTPUT);
pinMode(slaveselectPin, OUTPUT);
SPI.begin();
Serial.begin(19200);
delay(50);
SPI.transfer(0);
SPI.transfer(0);
delay(10);
digitalWrite(digipotPowerPin,HIGH);

digitalPotWrite(channel,level);
}

void loop() {
  
  // put your main code here, to run repeatedly:
timer = millis();
  previoustimer = startTime;
  while (timer >= startTime){
   
     timer = millis();
     regValue = analogRead(outRead); //get value from regulator feedback
     sensorValue1 = analogRead(sensorPinV1);
     sensorValue2 = analogRead(sensorPinV2);
      
     filteredSensor1 = ((sensorValue1)*1.0694)+ ((0.1423*1024)/5);
     filteredSensor2 = (sensorValue2*0.8268)+ ((0.6896*1024)/5);
     
     // Serial.print(timer); Serial.print(' '); //print out timer
     Serial.print(regValue); Serial.print(' '); //print out regulator value
     Serial.print(filteredSensor1); Serial.print(' '); //print out V1
     Serial.println (filteredSensor2); //print out V2

     // The following controls the Step function
             //following allows pressure range control
                    //calculate top pressure
               pressureHIGH = (level/255)*1024;
             digitalWrite(inWavePin, HIGH);
              if(regValue >= pressureHIGH){
                digitalWrite(inWavePin,LOW);
              }
              if(regValue <= 40){
                digitalWrite(inWavePin,HIGH);
              }
             //following allows frequency control
//               if ((timer - previoustimer) >= stepTime){
//                  previoustimer = timer;
//                    
//                    if (pass == 0){
//                    digitalWrite(inWavePin, HIGH);
//        
//                    pass = 1;
//                    }
//                
//                    else if(pass == 1){
//                   
//                    digitalWrite(inWavePin,LOW);
//                    pass = 0;
//                    }
//             
//              }
            }
  
}

void digitalPotWrite(int address, int value) {
  // take the SS pin low to select the chip:
  digitalWrite(slaveselectPin, LOW);
  //  send in the address and value via SPI:
  SPI.transfer(address);
  SPI.transfer(value);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveselectPin, HIGH);
}



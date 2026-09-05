 
/*

WRITE 
  stty -F /dev/ttyUSB6 raw speed 9600
  while [ 1 ]; do         READ=`dd if=/dev/ttyUSB6 count=1`;         echo $READ; done
READ
  cat  > /dev/ttyUSB6

wiring
  green - RX
  white - TX

 */
  
#include <SoftwareSerial.h>



SoftwareSerial mySerial(11, 12); // RX, TX




#define dirPin 2
#define stepPin 3
#define stepsPerRevolution 20

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);  

  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);


  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}


int modteller=0;


char input;

int toggle=0;

int cutflag=2;
// send serial signal of char 0 to cut

void loop() 
{
  
  
    int sensorValue = analogRead(A4);
    // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
      float voltage = sensorValue * (5.0 / 1023.0);
    // print out the value you read:
    Serial.println(voltage);
    Serial.println(modteller);
        
    if(Serial.available()){
        input = Serial.read();
        if(input=='0'){
        cutflag=0;
        }
        Serial.print("You typed: " );
        Serial.println(input);
    }
    
    modteller = (1 + modteller) % 10;   // modulo operator rolls over variable  
    if(modteller==0){
      
        

        if(cutflag<2){
          if(voltage>2.10 && voltage<2.7 ){
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);   
          }else{
                 // These four lines result in 1 step:
              toggle = !toggle;
              if(toggle){
                digitalWrite(dirPin, HIGH);
              }else{
                digitalWrite(dirPin, LOW);
              }          // These four lines result in 1 step:
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);          // These four lines result in 1 step:
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);
            digitalWrite(stepPin, HIGH);
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);
            cutflag++;
          }
        } 
        
    }
 

}




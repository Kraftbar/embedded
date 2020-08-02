 
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



char input;

int toggle=0;
void loop() 
{
    if(Serial.available()){
        input = Serial.read();
        Serial.print("You typed: " );
        Serial.println(input);
    }
    
    
    toggle = !toggle;
    if(toggle){
      digitalWrite(dirPin, HIGH);
    }else{
      digitalWrite(dirPin, LOW);
    }
    delay(1000);
      
    // Spin the stepper motor 1 revolution slowly:
    for (int i = 0; i < stepsPerRevolution; i++) {
      // These four lines result in 1 step:
      digitalWrite(stepPin, HIGH);
      delayMicroseconds(4000);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(4000);
    }
 

}


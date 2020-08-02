 
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

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);  

  Serial.begin(9600);
  Serial.println("Goodnight moon!");

  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}



char input;


void loop() // run over and over
{
    if(Serial.available()){
        input = Serial.read();
        Serial.print("You typed: " );
        Serial.println(input);
    }
 

}


/*
stty -F /dev/ttyS1 raw speed 9600
while [ 1 ]; do         READ=`dd if=/dev/ttyUSB3 count=1`;         echo $READ; done

cat  > /dev/ttyUSB3
##wite something###

 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(11, 12); // RX, TX

void setup()
{
        pinMode(LED_BUILTIN, OUTPUT);  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() // run over and over
{
  if (mySerial.available())
    Serial.write(mySerial.read());
  if (Serial.available())
    mySerial.write(Serial.read());
    
	if (Serial.available() > 0) {
             Serial.println("serial is  available");

		}
       else{
           Serial.println("serial is not available");
}    
    
 
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on 
    delay(500);                       // wait for half a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off 
    delay(500);     

}



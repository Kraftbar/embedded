

#include "myWifi.h"
int valHall = 0;
int valTouch = 0;


#include "Waveforms.h"
#include "esp32-hal-dac.h"
#define oneHzSample 1000000/maxSamplesNum  // sample for the 1Hz signal expressed in microseconds 
int i = 0;
int sample;
enum State {
  myopen,
  myopenNotOK,
  myclosed
};
State myState=myopen;


#include "muisc.h"

void setup() {
  Serial.begin(115200);
  mySetup();
  myLoop(); // trenger egentlig ikke
}

void playSound(){
  sample = map(analogRead(A0), 0, 4095, 0, oneHzSample);
  sample = constrain(3, 0, oneHzSample);
  dacWrite(25, waveformsTable[1][i]);  // write the selected waveform on DAC1
  i++;
  if(i == maxSamplesNum)  // Reset the counter to repeat the wave
    i = 0;
  delayMicroseconds(sample);  // Hold the sample value for the sample time
}




float MAhall;
void loop() {
  valHall = hallRead();
  valTouch = touchRead(T0);
  int hour=myLoop();
  MAhall= (0.02 * valHall ) + (1.0 - 0.02) * MAhall;


  Serial.println((String)  "Hall:" + valHall + " | Hall:" + MAhall + "  | Touch:" + valTouch + "|  Hour: "+hour );



  if(MAhall<0){
    myState=myclosed;
    
  }else if(MAhall>1 && hour>21){
    myState=myopenNotOK;
    
  }else{
    myState=myopen;
  }
  
  switch (myState) {
  
    case myopen:

      break;
    case myclosed:
      // statements
      break;
    case myopenNotOK:
      playSound();
      break;
    default:
      // statements
      break;
}

  
}

/*Sends signal to the receiver to trigger the hose or feeder turn at regular time intervals. Currently the feeder is set to  trigger every 6 seconds*/
#include "nRF905Transceiver.cpp"
#include <ADCTouch.h>
#include <Time.h>

nRF905Tranceiver transmitter(0x000000F0,0x000000F1);


//Input A0 triggers the feeder and A1 triggers the hose 
#define INPUT0 A0
#define INPUT1 A1


void setup() {
  Serial.begin(9600);
  transmitter.setup();
  transmitter.onResponse(onResponse);
  pinMode(INPUT0, INPUT_PULLUP);
  pinMode(INPUT1, INPUT_PULLUP);

}

void loop() {

 // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
 // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
 // button's pressed, and off when it's not:

 //When the buton A0 is pushed and A1 is not, the feeder is triggered
  if((digitalRead(INPUT0) == LOW) && (digitalRead(INPUT1) == HIGH)){
    Serial.println("The feder is triggered");
    transmitter.send("TURN 90");  
    transmitter.send("TURN HOSE 0");
 //When the buton A1 is pushed and A0 is not, the hose is triggered    
  } else if((digitalRead(INPUT1) == LOW) && (digitalRead(INPUT0) == HIGH)){
    Serial.println("The hose is triggered");
    transmitter.send("TURN HOSE 120");
    transmitter.send("TURN 0");
 //When the butons A0 & A1 are not pushed, none of the components are triggered      
  } else if ((digitalRead(INPUT1) == HIGH) && (digitalRead(INPUT0) == HIGH)){
    transmitter.send("TURN HOSE 0");
    transmitter.send("TURN 0");
  //When the butons A0 & A1 are both pushed, both the components are triggered 
  } else if ((digitalRead(INPUT1) == LOW) && (digitalRead(INPUT0) == LOW)){
     transmitter.send("TURN HOSE 120");
    transmitter.send("TURN 90");
  }
  
   
  transmitter.trySend();

}

/* DO NOT MODIFY*/

void onResponse(String command) {

}

void NRF905_CB_RXCOMPLETE(void) {
  transmitter.onComplete();
  nRF905_standby();
}

void NRF905_CB_RXINVALID(void) {
  transmitter.onInvalid();
  nRF905_standby();
}


//Turns the servo in the hose to either 360 or 0 depending on input from transmitter. 
//To facilitate debugging, the LED light is turned on when the hose is to turn 360 and off when it's at turn 0.
#include "tranceiver.cpp"
#include <Servo.h>

nRF905Tranceiver receiver(0x000000F1, 0x000000F0);
int count = 0;

#define SERVO_PIN 5
Servo valve;
int angle = 0;

void setup() {
  Serial.begin(115200);
  receiver.setup();
  receiver.onResponse(onResponse);
  valve.attach(SERVO_PIN);
  pinMode(LED_BUILTIN, OUTPUT);
}

void onResponse(String command) {

  if(command == "TURN HOSE 360"){
    Serial.println("Turn hose 360"); 
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level) to indicate the hose is being turned
    angle = 360;
  }else if(command == "TURN HOSE 0"){
    Serial.println("Turn hose 0");
    digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW to indicate hose is not being turned
    angle = 0;
  }
}

void loop() {
  receiver.tryReceive();
  //Prints the turn angle 
  Serial.println(angle);
  valve.write(angle);
}

void NRF905_CB_RXCOMPLETE(void) {
  receiver.onComplete();
}

void NRF905_CB_RXINVALID(void) {
  receiver.onInvalid();
}


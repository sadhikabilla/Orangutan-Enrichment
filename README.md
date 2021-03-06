# Orangutan-Enrichment

#### An enrichment system designed for the Orangutans at the Singapore Zoo

<img src="https://github.com/sadhikabilla/Orangutan-Enrichment/blob/master/orang.jpg" width=500>

### Purpose
Studies in the wild and in captivity indicate that the great apes, chimpanzees, gorillas, and orangutans, are among the smartest land animals on the planet. These animals require constant cognitive and physical enrichment to maintain their sharp cognitive skills and physical strength. 

#### Take a look at the development process here! 

[Link to youtube video](https://youtu.be/oGEmEEOKdpw)

### Designing a cohesive system
The setup consists of 3 components (Vines, feeder and sprinkler) that work together as a comperhensive enrichment system to encourage cooperation between the animals and promote their physical & mental well being.


<img src= "https://github.com/sadhikabilla/Orangutan-Enrichment/blob/master/components.png" width=500>

A detailed explanation of each of the components can be found in the zoo manual.pdf.  

### Repository Organisation

The repository consists of 3 folders - Manual_mode, Debug_mode and Automatic_mode. In order to run the code you will need to install the Arduino software. 

### Automatic mode
This folder contains the Receiver (feeder receiver), Transmitter (from the touch sensors) and the valve-servo-control_receiver (for the sprinkler system). The nr905Tranceiver.cpp files are important to include for wireless transmission.  

The automatic mode is the default mode. When both wires from pins A0 and A2 (refer to circut diagram in manual) are touched, a signal is sent from the tramsmitter to the feeder and the sprinkler to trigger accordingly. Currently, the feeder is set to dispense at every 6 second interval. This can be adjusted from the ```timeInterval``` variable in the Transmitter file. 
```
float currentTime = 0;
boolean check90 = false;
float timeDiff = 0; //Controls the amount of time the screw rotates for - currently set to 3 seconds. The value can be modified accordingly. 
float timeInterval = 6000; //The time interval between subsequent turns of the feeder. The time interval is currently set to 6 seconds and can be modified accordingly
```

When the feeder (Receiver) receives the signal 'TURN 90', the motor in the feeder is triggered. 
```
void onResponse(String command) {

    //Changes state based on signal from transmitter 
    if(command == "TURN 90"){
        state = HIGH;
    }else if (command == "TURN 0"){
        state = LOW;

    }
}
```

Similarly, when the sprinkler (valve-servo-control_receiver) receives a signal 'TURN HOSE 120', the servo in the sprinkler is trigger to turn. 
```
void onResponse(String command) {

  if(command == "TURN HOSE 120"){
    angle = 120;
  }else if(command == "TURN HOSE 0"){
    angle = 0;
  }
}
```
### Manual mode
This folder contains the FeederReceiver (feeder receiver), Transmitter (from the touch sensors) and the HoseReceiver (for the sprinkler system). The nr905Tranceiver.cpp files are important to include for wireless transmission.  

Based on the appropriate button input (A0 for feeder, A1 for sprinkler), either the feeder or the sprinkler is triggerd. The touch sensors are emiminated for the manual mode. 
```
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
```
### Debug mode
This folder contains the Stand_alone_hose and the Stand_alone_feeder folders. The Stand_alone_hose contains the HoseTransmitter and the HoseReceiver, while the Stand_alone_feeder contains the FeederTransmitter and the FeederReceiver. 

In the debug mode each component is seperated and works as a stand alone device. The hose transmitter sends signals to the hose receiver while the feeder transmitter sends signals to the feeder receiver.  

# Orangutan-Enrichment

#### An enrichment system designed for the Orangutans at the Singapore Zoo

<img src="https://github.com/sadhikabilla/Orangutan-Enrichment/blob/master/orang.jpg" width=500>

### Purpose
Studies in the wild and in captivity indicate that the great apes, chimpanzees, gorillas, and orangutans, are among the smartest land animals on the planet. These animals require constant cognitive and physical enrichment to maintain their sharp cognitive skills and physical strength. 

### Designing a cohesive system
The setup consists of 3 components (Vines, feeder and sprinkler) that work together as a comperhensive enrichment system to encourage cooperation between the animals and promote their physical & mental well being. 

A detailed explanation of each of the components can be found in the zoo manual.pdf.  

### Repository Organisation

The repository consists of 3 folders - Manual_mode, Debug_mode and Automatic_mode. In order to run the code you will need to install the Arduino software from <a href="https://www.arduino.cc/en/Main/Software">. 

## Automatic mode
This folder contains the Receiver (feeder receiver), Transmitter (from the touch sensors) and the valve-servo-control_receiver (for the sprinkler system). The nr905Tranceiver.cpp files are important to include for wireless transmission.  

The automatic mode is the default mode. When both wires from pins A0 and A2 (refer to circut diagram in manual) are touched, a signal is sent from the tramsmitter to the feeder and the sprinkler to trigger accordingly. Currently, the feeder is set to dispense at evry 6 second interval. This can be adjusted from the timeInterval variable in the Transmitter file. When the feeder (Receiver) receives the signal 'TURN 90', the motor in the feeder is triggered. Similarly, when the sprinkler (valve-servo-control_receiver) receives a signal 'TURN HOSE 120', the servo in the sprinkler is trigger to turn. 

## Manual mode
This folder contains the FeederReceiver (feeder receiver), Transmitter (from the touch sensors) and the HoseReceiver (for the sprinkler system). The nr905Tranceiver.cpp files are important to include for wireless transmission.  

Based on the appropriate button input (A0 for feeder, A1 for sprinkler), either the feeder or the sprinkler is triggerd. The touch sensors are emiminated for the manual mode. 

## Debug mode
This folder contains the Stand_alone_hose and the Stand_alone_feeder folders. The Stand_alone_hose contains the HoseTransmitter and the HoseReceiver, while the Stand_alone_feeder contains the FeederTransmitter and the FeederReceiver. 

In the debug mode each component is seperated and works as a stand alone device. The hose transmitter sends signals to the hose receiver while the feeder transmitter sends signals to the feeder receiver.  

import processing.serial.*;

// declaring each alarm variable
int ZONE1_ALARM = 0;
int ZONE2_ALARM = 0;
int ENTRY_EXIT_ALARM = 0;

Serial serialPort;  // serial port object
int baudRate = 9600;  // baud rate for serial communication

// setting up the gui size
void setup() {
  size(500, 500);
  
  // open the first serial port in the list
  String port = Serial.list()[2];
  serialPort = new Serial(this, port, baudRate);
}

// this function will run as the gui starts. It will generate 3 squares refering to each sensor o the alarm project
void draw() {
  monitorReader();
  // background color
  background(255);
   
  // Draw ENTRY_EXIT rectangle
  fill(ENTRY_EXIT_ALARM == 1 ? color(255, 0, 0) : color(0, 255, 0));
  rect(50, 50, 100, 100);
  textSize(20);
  textAlign(CENTER, CENTER);
  textFont(createFont("ProximaNova-Bold", 16));
  fill(255);
  text("ENTRY_EXIT", 100, 100);
  

  // Draw ZONE1 rectangle
  fill(ZONE1_ALARM == 1 ? color(255, 0, 0) : color(0, 255, 0));
  rect(175, 50, 100, 100);
  textSize(20);
  textFont(createFont("ProximaNova-Bold", 16));
  textAlign(CENTER, CENTER);
  fill(255);
  text("ZONE1", 225, 100);
  
  // Draw ZONE2 rectangle
  fill(ZONE2_ALARM == 1 ? color(255, 0, 0) : color(0, 255, 0));
  rect(300, 50, 100, 100);
  textSize(20);
  textFont(createFont("ProximaNova-Bold", 16));
  textAlign(CENTER, CENTER);
  fill(255);
  text("ZONE2", 350, 100);
}

// this function is going to read monitor and determine the states of each alarm variable
void monitorReader() {
  // checking if serial port is available
  if (serialPort.available() > 0) {
    // reading data from serial monitor undtil the ne wline
    String data = serialPort.readStringUntil('\n');
    if (data != null) {
      data = data.trim();  // remove any whitespace characters
      println("Received data: " + data);
      if(data.equals("ENTRY_EXIT_STATE")){ // if the string before the newline equals to ENTRY_EXIT_STATE, set the ENTRY_EXIT_ALARM 1. Otherwise 0 so that the rectangle can remain green
        ENTRY_EXIT_ALARM = 1;
      }else{ENTRY_EXIT_ALARM = 0;}
      if(data.equals("ZONE1_STATE")){ // if the string before the newline equals to ZONE1_STATE, set the ZONE1_ALARM 1. Otherwise 0 so that the rectangle can remain green
        ZONE1_ALARM = 1;
      }else{ZONE1_ALARM = 0;}
      if(data.equals("ZONE2_STATE")){ // if the string before the newline equals to ZONE2_STATE, set the ZONE2_ALARM 1. Otherwise 0 so that the rectangle can remain green
        ZONE2_ALARM = 1;
      }else{ZONE2_ALARM = 0;}
      if(data.equals("Alarm disarmed")){ // if alarm is disabled by the user, set all the varibles to 0 so that all alarm areas can be green
        ENTRY_EXIT_ALARM = 0;
        ZONE1_ALARM = 0;
        ZONE2_ALARM = 0;
      }
    }
  }
}

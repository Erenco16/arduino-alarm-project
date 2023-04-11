import processing.serial.*;

Serial myPort; // Create object from Serial class

int ENTRY_EXIT_STATE = 0;
int ZONE1_STATE = 0;
int ZONE2_STATE = 0;

void setup() {
  size(600, 300);
  noStroke();
  myPort = new Serial(this, Serial.list()[2], 9600); // Open the first serial port
}

void draw() {
  background(255);

  // Draw rectangles for sensors
  print("BAK ENTRY_EXIT BU AMK == ");
  println(ENTRY_EXIT_STATE);
  print("BAK ZONE1 BU AMK == ");
  println(ZONE1_STATE);  
  print("BAK ZONE2 BU AMK == ");
  println(ZONE2_STATE);
  fill(ENTRY_EXIT_STATE == 1 ? color(255, 0, 0) : color(0, 255, 0)); // Change color based on sensor state
  rect(50, 50, 100, 100);
  fill(ZONE1_STATE == 1 ? color(255, 0, 0) : color(0, 255, 0)); // Change color based on sensor state
  rect(175, 50, 100, 100);
  fill(ZONE2_STATE == 1 ? color(255, 0, 0) : color(0, 255, 0)); // Change color based on sensor state
  rect(300, 50, 100, 100);
}

void serialEvent(Serial myPort) {
  // Read the data from Arduino and set the state variables accordingly
  String data = myPort.readStringUntil('\n');
  if (data != null) {
    //println(data.trim());
    if(data.trim() == "ENTRY_EXIT_STATE"){
      ENTRY_EXIT_STATE = 1;
    }
    else if(data.trim() == "ZONE1_STATE"){
      ZONE1_STATE = 1;
    }
    else if(data.trim() == "ZONE2_STATE"){
      ZONE2_STATE = 1;
    }
  }
}

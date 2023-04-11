import processing.serial.*;

Serial myPort;  // Create object from Serial class

void setup() {
  // List all the available serial ports
  println(Serial.list());
  // Open the port you are using at the rate you want:
  myPort = new Serial(this, Serial.list()[2], 9600);
}

void draw() {
  if (myPort.available() > 0) {
    // Read the serial data until a newline character is received
    byte[] serialData = myPort.readBytesUntil('\n');
    if (serialData != null) {
      // Convert the byte array to a string and print it to the console
      String dataString = new String(serialData);
      if(dataString != null){
          String [] values = split(dataString.trim(), ',');
    }
  }
}
}

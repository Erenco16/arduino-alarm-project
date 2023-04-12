/*
  Basic Alarm System with keypad
*/
#include <Keypad.h>
#include <Password.h>
#include <LiquidCrystal.h>


String newPasswordString;  //hold the new password
char newPassword[6];       //character string of newPasswordString


//pir1 connected to 49 -> zone1
//pir2 connected to 41 -> entry_exit
// ultrasonic sensor connected to 10 -> zone2
const int ENTRY_EXIT = 49;
const int ZONE1 = 41;
int LED = 47; // led pin connected to 47
const int buzzerPin = 48; // buzzer pin connected to 48
const int rs = 52, en = 53, d4 = 4, d5 = 5, d6 = 6, d7 = 7; // lcd oins are connected
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// variables for ultrasonic sensor
const int trigPin = 9;
const int ZONE2 = 10; // echoPin
// variables to check the distance for ultrasonic sensor
long duration;
int distance;
int distanceCheck = 100;

int ON_OFF_STATE = LOW;
int ENTRY_EXIT_STATE = HIGH;
int ENTRY_EXIT_ACTIVATED = 0;
int ZONE1_STATE;
int ZONE2_STATE;


//initialize password to 1234
Password password = Password("1234");

byte maxPasswordLength = 6;
byte currentPasswordLength = 0;
const byte ROWS = 4;  // Four rows
const byte COLS = 4;  // Four columns

//Define the keymap
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

//// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = {23,25,27,29};  //connect to row pinouts

// Connect keypad COL0, COL1, COL2 and COL3 to these Arduino pins.
byte colPins[COLS] = {31,33,35,37};  //connect to column pinouts

// Create the Keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


unsigned long time_now = 0;
int period = 1000;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(ZONE2, INPUT_PULLUP); // Sets the ZONE2 as an Input
  Serial.begin(9600);
  lcd.begin(16,2);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);

  pinMode(ENTRY_EXIT, INPUT_PULLUP);
  pinMode(ZONE1, INPUT_PULLUP);

  lcd.clear();
  lcd.print(" Enter password ");
}

// the loop function runs over and over again forever
void loop() {
  my_key();
  // if the correct password has been inputted, go in that condition
  if (ON_OFF_STATE) {
    // checking the alarm statuses
    ENTRY_EXIT_STATE = digitalRead(ENTRY_EXIT);
    ZONE1_STATE = digitalRead(ZONE1);
    // if distance is less than 20, trigger the alarm for ultrasonic sensor
    distanceCheck = distanceReturner();
    if(distanceCheck<=20){
        ZONE2_STATE = HIGH;
      }
      else{
          ZONE2_STATE = LOW;
        }
    // entry exit is being triggered here
    if (ENTRY_EXIT_STATE == 1) {
      countdown(); // 5 seconds of countdown
      ENTRY_EXIT_STATE = 0;
      ENTRY_EXIT_ACTIVATED++;
      if (ENTRY_EXIT_ACTIVATED%2) { // if ENTRY_EXIT_ACTIVATED is 2, trigger the alarm, otherwise do not. Because we want the alarm to trigger on the second attempt
        ENTRY_EXIT_STATE = HIGH;
      }
    }
    my_key(); // geting password from the user
    while (ENTRY_EXIT_STATE && ON_OFF_STATE || ZONE1_STATE && ON_OFF_STATE || ZONE2_STATE && ON_OFF_STATE) { //prompting the alarm messages for Processing and LCD display. Also our alarm triggers here as well.
      if(ENTRY_EXIT_STATE == 1){
          Serial.println("ENTRY_EXIT_STATE");
          lcd.clear();
          lcd.print("ENTRY_EXIT");
        }
      else if(ZONE1_STATE == 1){
          Serial.println("ZONE1_STATE");
          lcd.clear();
          lcd.print("ZONE1_STATE");
        }
      else if(ZONE2_STATE == 1){
          Serial.println("ZONE2_STATE");
          lcd.clear();
          lcd.print("ZONE2_STATE");
        }
      Flash(); // led flashing and buzzer
      my_key(); // getting password from the user again
    }
  }
}

// 5 seconds of countdown
void countdown() {
  for (int x = 1; x < 6; x++) {
    if(ON_OFF_STATE==LOW){ // stop counting if password is correct
       break;
      }
    my_key();
    lcd.clear();
    lcd.print("Countdown: "); lcd.print(6-x);
    My_Delay(); // millis() is used so that the user can input password while the countdown is triggered.
    ENTRY_EXIT_STATE = digitalRead(ENTRY_EXIT);
  }
}

void my_key() { // reading the password here and then checking and processing
  char key = keypad.getKey();
  if (key != NO_KEY) {
    delay(60);
    switch (key) {
      case 'A': break;
      case 'B': break;
      case 'C': break;
      case 'D': break;
      case '#': checkPassword(); break;
      case '*': resetPassword(); break;
      default: processNumberKey(key);
    }
  }
}

void processNumberKey(char key) { // appending each key to a password so that it can be checked whether is true or not
  currentPasswordLength++;
  password.append(key);
  lcd.setCursor(0,1);
  lcd.write(key);
  if (currentPasswordLength == maxPasswordLength) {
    checkPassword();
  }
}

void checkPassword() { // checking if the password is whether true or not
  if (password.evaluate()) {
    ON_OFF_STATE = ON_OFF_STATE ^ 1;
    if(ON_OFF_STATE==HIGH){
        lcd.clear();
        lcd.print("Alarm is armed");
      }
    else{
      Serial.println("Alarm disarmed");
      lcd.clear();
      lcd.print("Alarm disarmed");
      }
  } else {
    lcd.clear();
    lcd.print(" Wrong password!");
  }
  resetPassword();
}

void resetPassword() {
  password.reset();
  currentPasswordLength = 0;
}


void Flash() {
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  tone(buzzerPin, 2500, 600); // turning the buzzer on
  My_Delay();               // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  tone(buzzerPin, 2500, 600); // turning the buzzer off
  My_Delay();               // wait for a second
}


void My_Delay() {
  time_now = millis();
  while (millis() < time_now + period) {
    my_key(); // for user to input password
    // wait appox. [period] ms
  }
}

int distanceReturner(){ // returns the distance between the object and the ultrasonic sensor
    // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW); 
  // Reads the ZONE2, returns the sound wave travel time in microseconds
  duration = pulseIn(ZONE2, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // 0.034 is the speed of the sound wave and divided by 2 because the waves reach the object and bounces back so the measure would be counted twice
  return distance;
  }

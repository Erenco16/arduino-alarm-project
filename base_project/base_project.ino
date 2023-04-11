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
int LED = 47;
const int buzzerPin = 48;
const int rs = 52, en = 53, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
// variables for ultrasonic sensor
const int trigPin = 9;
const int ZONE2 = 10;
long duration;
int distance;
int distanceCheck = 100;

int ON_OFF_STATE = LOW;
int ENTRY_EXIT_STATE = HIGH;
int ENTRY_EXIT_ACTIVATED = 0;
int ZONE1_STATE;
int ZONE2_STATE;


//initialize password to 1234
//you can use password.set(newPassword) to overwrite it
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
  if (ON_OFF_STATE) {
    ENTRY_EXIT_STATE = digitalRead(ENTRY_EXIT);
    /*
    lcd.clear();
    lcd.print("ENTRY_EXIT_ACTIVATED"); */
    ZONE1_STATE = digitalRead(ZONE1);
    distanceCheck = distanceReturner();
    if(distanceCheck<=20){
        ZONE2_STATE = HIGH;
      }
      else{
          ZONE2_STATE = LOW;
        }
    Serial.print(ENTRY_EXIT_STATE); Serial.print(", ");
    Serial.print(ZONE1_STATE); Serial.print(", ");
    Serial.println(ZONE2_STATE);
    if (ENTRY_EXIT_STATE == 1) {
      countdown();
      ENTRY_EXIT_STATE = 0;
      ENTRY_EXIT_ACTIVATED++;
      if (ENTRY_EXIT_ACTIVATED%2) {
        ENTRY_EXIT_STATE = HIGH;
      }
    }
      if(ENTRY_EXIT_STATE == HIGH){
      lcd.clear();
      lcd.print("ENTRY_EXIT");
    }
   else if(ZONE1_STATE == HIGH){
      lcd.clear();
      lcd.print("ZONE1");
    }
    else if(ZONE2_STATE == HIGH){
      lcd.clear();
      lcd.print("ZONE2");
    }
    my_key();
    while (ENTRY_EXIT_STATE && ON_OFF_STATE || ZONE1_STATE && ON_OFF_STATE || ZONE2_STATE && ON_OFF_STATE) {
      if(ENTRY_EXIT_STATE == 1){
          Serial.println("ENTRY_EXIT_STATE");
        }
      else if(ZONE1_STATE == 1){
          Serial.println("ZONE1_STATE");
        }
      else if(ZONE2_STATE == 1){
          Serial.println("ZONE2_STATE");
        }
      Flash();
      my_key();
    }
  }
}

void countdown() {
  for (int x = 1; x < 6; x++) {
    if(ON_OFF_STATE==LOW){
       break;
      }
    my_key();
    lcd.clear();
    lcd.print("Countdown: "); lcd.print(6-x);
    My_Delay();
    ENTRY_EXIT_STATE = digitalRead(ENTRY_EXIT);
  }
}

void my_key() {
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

void processNumberKey(char key) {
  currentPasswordLength++;
  password.append(key);
  lcd.setCursor(0,1);
  lcd.write(key);
  if (currentPasswordLength == maxPasswordLength) {
    checkPassword();
  }
}

void checkPassword() {
  if (password.evaluate()) {
    //Serial.println(" OK.");
    ON_OFF_STATE = ON_OFF_STATE ^ 1;
    if(ON_OFF_STATE==HIGH){
       // Serial.println("The alarm is armed.");
        lcd.clear();
        lcd.print("Alarm is armed");
      }
    else{
      //Serial.println("The alarm disarmed.");
      lcd.clear();
      lcd.print("Alarm disarmed");
      }
  } else {
    //Serial.println(" Wrong password!");
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
    my_key();
    // wait appox. [period] ms
  }
}

int distanceReturner(){
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
  distance = duration * 0.034 / 2;
  return distance;
  }

#include <Wire.h>

// assigning components to pins
int redPin = 9;
int greenPin = 10;
int bluePin = 11;
int whitePin = 12;
int startButton = 3;
int stopButton = 2;

// declaring variables
int start = 0;
int colour = 0;
int change = 0;

//Setup
void setup() {
  //initialising the transfer of data between arduinos
  Wire.begin(8);
  Wire.onReceive(receiveEvent);

  //setting up leds for output
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(whitePin, OUTPUT);

  //specifying interrupt routine for both buttons
  attachInterrupt(1, startInput, FALLING);
  attachInterrupt(0, stopInput, FALLING);

  //setting up buttons for input
  pinMode(startButton, INPUT);
  pinMode(stopButton, INPUT);
}

//Main loop
void loop() {
 delay(100);
 //Checks status of white light
 if(digitalRead(whitePin) == HIGH){
  start = 1;
 }
 else if(digitalRead(whitePin) == LOW){
  start = 0;
 }

 /*If white light has changed status then transmission of data occurs
  * Begins the skittle sorting process.
  */
 if(change == 1){
  Wire.beginTransmission(9);
  Wire.write(start);
  Wire.endTransmission();
  change = 0;
 }
}

/*Interrput commands for start button
 * Turns white LED on and if the sorting hasn't started
 * it prepares for a change in status.
 */
void startInput() {
  digitalWrite(whitePin, HIGH);
  if(start == 0){
    change = 1;
  }
}


/*Interrput commands for stop button
 * Turns white LED off and if the sorting has started
 * it prepares for a change in status.
 */
void stopInput() {
 digitalWrite(whitePin, LOW);
 if(start == 1){
  change = 1;
 }
}

/*
 * Commands for when a transmission of data is received.
 * Recieves which colour of skittle is being sorted
 * Alters the RGB LED to show this colour.
 */
void receiveEvent() {
 colour = Wire.read();
 if(colour == 1){//Red
   analogWrite(redPin, 255);
   analogWrite(bluePin, 0);
   analogWrite(greenPin, 0);
 }
 else if(colour == 2){//Orange
   analogWrite(redPin, 255);
   analogWrite(bluePin, 0);
   analogWrite(greenPin, 50);
 }
 else if(colour == 3){//Green
   analogWrite(redPin, 0);
   analogWrite(bluePin, 0);
   analogWrite(greenPin, 255);
 }
 else if(colour == 4){//Yellow
   analogWrite(redPin, 170);
   analogWrite(bluePin, 0);
   analogWrite(greenPin, 80);
 }
 else if(colour == 5){//Purple
   analogWrite(redPin, 180);
   analogWrite(bluePin, 180);
   analogWrite(greenPin, 0);
 }
 else if(colour == 6){//No Skittle
   analogWrite(redPin, 255);
   analogWrite(bluePin, 255);
   analogWrite(greenPin, 255);
 }
}



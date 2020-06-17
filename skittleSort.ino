#include <Servo.h>
#include <Wire.h>


// assigning components to pins
int S0 = 4;
int S1 = 5;
int S2 = 6;
int S3 = 7;
int sensorOut = 8;

// declaring variables
int red = 0;
int green = 0;
int blue = 0;
int start = 0;
int colour;

//assigning servos
Servo mover;
Servo sorter;

//Setup
void setup() {
  
  //attaching servo to pin and setting it to inital point
  mover.attach(9);
  mover.write(0);

  //attaching servo to pin and setting it to inital point
  sorter.attach(10);
  sorter.write(0);
  
  //Setting up the colour sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  //initialising the serial monitor and printing starting messages
  Serial.begin(9600);
  Serial.println("Welcome to the Skittle Sorter!");

  //initialising the transfer of data between arduinos
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
}

//Main loop
void loop() { 
 /*If start button on connected arduino has been pressed
  * then sorting should begin.
  */  
 while(start == 1){ 
  moverRotate();
  
  delay(1000);
  
  colourPicker();
  colourSorter();
  
  delay(1000);
  
  moverReturn();
  
  delay(2000);
 }
}

/*
 * Commands for when a transmission of data is received.
 * Recevies an integer 1 or 0.
 * 1 if the sorting should begin, and 0 if it shouldn't be sorting.
 */
void receiveEvent(){
  start = Wire.read();
}

/*
 * Function for sensing the colour of the Skittle.
 * Transmits this data to the connected Arduino.
 */
void colourPicker()
{
  //Detecting the 'red' of the Skittle.
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  red = pulseIn(sensorOut, LOW);
  delay(100);

  //Detecting the 'green' of the Skittle.
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  green = pulseIn(sensorOut, LOW);
  delay(100);

  //Detecting the 'blue' of the Skittle.
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  blue = pulseIn(sensorOut, LOW);
  delay(100);

  /*
   * Colour calacualtion.
   * Based on amount of red, green and blue light reflected.
   */
  Serial.print("Skittle: ");
  if(red < blue && blue < green)
  {
    if(red > 205)
    {
      colour = 1;
      Serial.print("Red");
      
      Wire.beginTransmission(8);
      Wire.write(colour);
      Wire.endTransmission();
    }
    else
    {
      colour = 2;
      Serial.print("Orange");
      
      Wire.beginTransmission(8);
      Wire.write(colour);
      Wire.endTransmission();
    }
  }
  else if(green < blue && green < red)
  {
    colour = 3;
    Serial.print("Green");
    
    Wire.beginTransmission(8);
    Wire.write(colour);
    Wire.endTransmission();
  }
  else if(red < green && green < blue)
  {
    colour = 4;
    Serial.print("Yellow");
    
    Wire.beginTransmission(8);
    Wire.write(colour);
    Wire.endTransmission();
  }
  else if(blue < red && red < green)
  {
    colour = 5;
    Serial.print("PURPLE");
    
    Wire.beginTransmission(8);
    Wire.write(colour);
    Wire.endTransmission();
  }
  else
  {
    colour = 6;
    Serial.print("No Skittle");
    
    Wire.beginTransmission(8);
    Wire.write(colour);
    Wire.endTransmission();
  }
  Serial.println("  ");
  delay(500);
  
  //Moves servo to push the skittle to be sorted into correct section.
  mover.write(170);
}

/*
 * Function for sorting the skittles on their colour.
 * Rotates the sorting servo to correct position.
 */
void colourSorter(){
 if(colour == 1){//Red
   sorter.write(0);
 }
 else if(colour == 2){//Orange
   sorter.write(18);
 }
 else if(colour == 3){//Green
  sorter.write(36);
 }
 else if(colour == 4){//Yellow
   sorter.write(54);
 }
 else if(colour == 5){//Purple
  sorter.write(72);
 }
 else if(colour == 6){//No Skittles
  sorter.write(0);
 }
}

/*
 * Function to rotate servo to a position to be colour sensored.
 */
void moverRotate(){
  mover.write(90);
}

/*
 * Function to return servo to orginal position to recieve another skittle.
 */
void moverReturn(){
  mover.write(0);
}

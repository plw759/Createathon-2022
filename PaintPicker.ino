// Include the AccelStepper Library
#include <AccelStepper.h>
#include <Servo.h>

// Define pin connections
const int dirPin = 2;
const int stepPin = 3;
const int buttonPin = 8;
const int limitPin = 7;
const int redLedPin = 10;
const int greenLedPin = 11;
const int blueLedPin = 12;
int posW = 0;
int posC = 0;
int posM = 0;
int posY = 0;
int posK = 0;
int valW = 0;
int valC = 0;
int valM = 0;
int valY = 0;
int valK = 0;
int buttonState = 0;
int limitState = 0;
#define motorInterfaceType 1
#define Servo_PWM 4
#define S0 5
#define S1 6
#define S2 9
#define S3 13
#define sensorOut 22

// Creates an instance
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);
Servo myServo;

void setup() {
  // set the maximum speed, acceleration factor,
  // initial speed and the target position
  myStepper.setMaxSpeed(1000);
  myStepper.setAcceleration(20000);
  myStepper.setSpeed(1000);
  myStepper.moveTo(0);
  myStepper.runToNewPosition(0);
  
  pinMode(buttonPin, INPUT);
  pinMode(limitPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin,OUTPUT);
  pinMode(blueLedPin,OUTPUT);

    // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Set Pulse Width scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  myServo.attach(Servo_PWM);
  myServo.write(0);
  delay(2000);
  Serial.begin(9600);
}

void loop() {
  //wait for color sensor
  buttonState = digitalRead(buttonPin);
  while (buttonState != HIGH) {
    delay(50);
    buttonState = digitalRead(buttonPin);
  }
  Serial.print("Done");
  

  //light up led based on values
  int redLed = getRedPW(); //from sensor directly
  Serial.println(redLed);
  redLed = map(redLed, 39, 380, 240, 0);
  Serial.println(redLed);
  if(redLed > 255){
    redLed = 255;
  }else if(redLed < 0){
    redLed = 5;
  }
  delay(200);
  int blueLed = getBluePW();
  Serial.println(blueLed);
  blueLed = map(blueLed, 25, 290, 240, 0);
  if(blueLed > 255){
    blueLed = 255;
  }else if(blueLed < 0){
    blueLed = 5;
  }
  delay(200);
  int greenLed = getGreenPW();
  Serial.println(greenLed);
  greenLed = map(greenLed, 35, 330, 240, 0);
  if(greenLed > 255){
    greenLed = 255;
  }else if(greenLed < 0){
    greenLed = 5;
  }
  delay(200);
  analogWrite(redLedPin, redLed * .8);
  analogWrite(greenLedPin, greenLed * .8);
  analogWrite(blueLedPin, blueLed * .8);
  Serial.println(redLed);
  Serial.println(greenLed);
  Serial.println(blueLed);

  valK = 255-max(max(redLed,greenLed),blueLed);
  if(valK == 255){ //kval 0 -- 0 percent -- 100 is 5 percent black -- 255 is 100
    valC = 0;
    valM = 0;
    valY = 0;
    valW = 0;
  }else{
    valC = (255-redLed-valK) / (1-(double)(valK/255));
    valM = (255-greenLed-valK) / (1-(double)(valK/255));
    valY = (255-blueLed-valK) / (1-(double)(valK/255));
    valW = 255 - valK;
  }
  
  
//    //find valC, valW...
//  valW = 200;
//  valC = 200;
//  valM = 200;
//  valY = 200;
//  valK = 200;


  
  myServo.write(0); //white
  delay(2000);
  
  myStepper.runToNewPosition(0);
  limitState = digitalRead(limitPin);
  myStepper.setSpeed(800);
  while (limitState != LOW) {
    myStepper.runSpeed();
    limitState = digitalRead(limitPin);
    Serial.print(limitState);
  }
  //myStepper.stop();
  myStepper.setMaxSpeed(100);
  myStepper.moveTo(valW+myStepper.currentPosition()+200);
  myStepper.runToPosition();
  //return to zero
  myStepper.setMaxSpeed(1000);
  myStepper.runToNewPosition(0);


  myServo.attach(Servo_PWM);
  myServo.write(45); //cyan
  delay(2000);
   
  myStepper.runToNewPosition(0);
  limitState = digitalRead(limitPin);
  myStepper.setSpeed(800);
  while (limitState != LOW) {
    myStepper.runSpeed();
    limitState = digitalRead(limitPin);
  }
  //myStepper.stop();
  myStepper.setMaxSpeed(100);
  myStepper.moveTo(valC+myStepper.currentPosition());
  myStepper.runToPosition();
  //return to zero
  myStepper.setMaxSpeed(1000);
  myStepper.runToNewPosition(0);



  
  myServo.write(90); //magenta
  delay(2000);

  myStepper.runToNewPosition(0);
  limitState = digitalRead(limitPin);
  myStepper.setSpeed(800);
  while (limitState != LOW) {
    myStepper.runSpeed();
    limitState = digitalRead(limitPin);
  }
  //myStepper.stop();
  myStepper.setMaxSpeed(100);
  myStepper.moveTo(valM+myStepper.currentPosition());
  myStepper.runToPosition();
  //return to zero
  myStepper.setMaxSpeed(1000);
  myStepper.runToNewPosition(0);

  
  myServo.write(135); //yellow
  delay(2000);
  
  myStepper.runToNewPosition(0);
  limitState = digitalRead(limitPin);
  myStepper.setSpeed(800);
  while (limitState != LOW) {
    myStepper.runSpeed();
    limitState = digitalRead(limitPin);
  }
  //myStepper.stop();
  myStepper.setMaxSpeed(100);
  myStepper.moveTo(valY+myStepper.currentPosition());
  myStepper.runToPosition();
  //return to zero
  myStepper.setMaxSpeed(1000);
  myStepper.runToNewPosition(0);



  
  myServo.write(180); //black
  delay(2000);
  
  myStepper.runToNewPosition(0);
  limitState = digitalRead(limitPin);
  myStepper.setSpeed(800);
  while (limitState != LOW) {
    myStepper.runSpeed();
    limitState = digitalRead(limitPin);
  }
  //myStepper.stop();
  myStepper.setMaxSpeed(100);
  myStepper.moveTo(valK+myStepper.currentPosition());
  myStepper.runToPosition();
  //return to zero
  myStepper.setMaxSpeed(1000);
  myStepper.runToNewPosition(0);

  //reset
  myServo.write(0);
  delay(3000);

}

//void loop() {
//  // Read Red Pulse Width
//  int redPW = getRedPW();
//  // Delay to stabilize sensor
//  delay(200);
//
//  // Read Green Pulse Width
//  int greenPW = getGreenPW();
//  // Delay to stabilize sensor
//  delay(200);
//
//  // Read Blue Pulse Width
//  int bluePW = getBluePW();
//  // Delay to stabilize sensor
//  delay(200);
//
//  // Print output to Serial Monitor
//  Serial.print("Red PW = ");
//  Serial.print(redPW);
//  Serial.print(" - Green PW = ");
//  Serial.print(greenPW);
//  Serial.print(" - Blue PW = ");
//  Serial.println(bluePW);
//}


//// Function to read Red Pulse Widths
int getRedPW() {
  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

// Function to read Green Pulse Widths
int getGreenPW() {
  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

// Function to read Blue Pulse Widths
int getBluePW() {
  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;
}

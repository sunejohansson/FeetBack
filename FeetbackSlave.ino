#include <Arduino.h>
#include <SparkFunAutoDriver.h>
#include <SPI.h>
#include <Wire.h>


// uncomment the desired mode

//#define POTENTIOMETERMODE
#define USERSTUDYMODE
#define ISLEFTFOOT // only set this if USERSTUDYMODE is activated and motors selected are 4, 5 or 6. 
//#define DEMOMODE

const int wireAddress = 6; // EACH ARDUINO NEEDS A UNIQUE ADDRESS

#ifdef ISLEFTFOOT
bool motorDirection = FWD;
#else
bool motorDirection = REV;
#endif

// THIS CODE IS FOR THE RECEIVING ARDUINOS
// THIS CODE IS FOR THE RECEIVING ARDUINOS
// THIS CODE IS FOR THE RECEIVING ARDUINOS

AutoDriver boardA(0, 10, 7);

int motorSpeed = 1000;
int index = 0;
int wireInput;
byte inputArray[2];
bool newCommandReceived;
bool uniqueMotorCommand;
bool isMotorSpeedCommand;

void setup()
{
  pinMode(7, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  digitalWrite(7, LOW);       // This low/high is a reset of the L6470 chip on the
  digitalWrite(7, HIGH);      //  Autodriver board, and is a good thing to do at
  //  the start of any Autodriver sketch, to be sure
  //  you're starting the Autodriver from a known state.
  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  dSPINConfig();
  Serial.begin(9600);
  Wire.begin(wireAddress);
  Wire.onReceive(receiveEvent); // register event, this will run whenever something is received on the bus
  newCommandReceived = false;
  uniqueMotorCommand = false;
  isMotorSpeedCommand = false;
}

void loop()
{

  if (newCommandReceived) {

#ifdef USERSTUDYMODE
    // Feetback user study code. Will run a selected pattern function
    switch (wireInput) {
      case 1:
        pattern1();
        break;
      case 2:
        pattern2();
        break;
      case 3:
        pattern3();
        break;
      case 4:
        pattern4();
        break;
      case 5:
        pattern5();
        break;
      case 6:
        pattern6();
        break;
      case 7:
        pattern7();
        break;
      case 8:
        pattern8();
        break;
      case 9:
        stopMotor();
        break;
      case 0:
        homeMotors();
        break;
      default:
        break;
    }
#endif

#ifdef POTENTIOMETERMODE
    rotateSpeed(wireInput);
#endif

#ifdef DEMOMODE
    // set the motor speed
    if (uniqueMotorCommand && isMotorSpeedCommand) {
      motorSpeed = wireInput;
      uniqueMotorCommand = false;
      isMotorSpeedCommand = false;
    }

    // is the input a motor direction change?
    if (uniqueMotorCommand && wireInput == 1) {
      motorDirection = !motorDirection;
      uniqueMotorCommand = false;
    }

    // is the input a motor speed change?
    else if (uniqueMotorCommand && wireInput == 2) {
      isMotorSpeedCommand = true;
    }


    // is the input a move command?
    else if (uniqueMotorCommand && wireInput != 1) {
      rotateMotor();
      //rotateSteps(wireInput);
      uniqueMotorCommand = false;
    }

    else if (wireInput == 1) {
      rotateMotor();
    }
    else if (wireInput == 2) {
      rotateMotorReverse();
    }
    else if (wireInput == 3) {
      stopMotor();
    }
    else if (wireInput == 4) {
      uniqueMotorCommand = true;
    }
#endif

    newCommandReceived = false;
  }
}

void receiveEvent(int howMany) {
  // Interrupt event. Will store input in wireInput
  readFromWire();
}

void readFromWire() {
  index = 0;
  while (Wire.available() > 0)
  {
    inputArray[index] = Wire.read();
    index++;
  }
  // stitch bytes in array to int
  wireInput = inputArray[1];
  wireInput = wireInput << 8;
  wireInput |= inputArray[0];
  newCommandReceived = true;
}

#ifdef USERSTUDYMODE
void pattern1() {
  boardA.setMaxSpeed(500);
  boardA.move(motorDirection, 690000);
}
void pattern2() {
  boardA.setMaxSpeed(1000);
  boardA.move(motorDirection, 690000);
}
void pattern3() {
  boardA.setMaxSpeed(500);
  boardA.move(REV, 690000);
}
void pattern4() {
  boardA.setMaxSpeed(1000);
  boardA.move(REV, 230000);
}
void pattern5() {
  boardA.setMaxSpeed(500);
  boardA.move(FWD, 690000);
}
void pattern6() {
  boardA.setMaxSpeed(1000);
  boardA.move(FWD, 230000);
}
void pattern7() {
  boardA.setMaxSpeed(500);
  boardA.move(!motorDirection, 690000);
}
void pattern8() {
  boardA.setMaxSpeed(1000);
  boardA.move(!motorDirection, 690000);
}
void homeMotors() {
  boardA.setMaxSpeed(1000);
  boardA.goHome();
}
#endif

void rotateMotor() {
  boardA.run(motorDirection, motorSpeed);
}

void rotateMotorReverse() {
  boardA.run(!motorDirection, motorSpeed);
}

void rotateSteps(int steps) {
  boardA.move(motorDirection, steps);
  while (boardA.busyCheck());
}

void rotateSpeed(int inputSpeed) {
  boardA.run(motorDirection, inputSpeed);
}

void stopMotor() {
  boardA.softStop();
  while (boardA.busyCheck());
}

#include <Wire.h>

//#define POTENTIOMETERMODE
//#define USERSTUDYMODE
//#define DEMOMODE
#define USERSTUDYMODEV2
const int patternRight = 6;
const int patternLeft = 7;

bool isMotorsRunning = false;

// change this to control delay between motors running
int motorDelay = 500;

byte byteArray[2];
int serialInput;
int driverSelection;
int directionSelection;
int numberOfSteps;

const int potPin = A0;
int potValue;
int temp;

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Setup done, input is ready");
  pinMode(potPin, INPUT);
  pinMode(patternRight, INPUT);
  pinMode(patternLeft, INPUT);
}

void loop() {

#ifdef POTENTIOMETERMODE

  potValue = analogRead(potPin);
  temp = map(potValue, 0, 1023, 0, 2000);
  sendCommandToAll(temp);
  Serial.println(temp);
  delay(100);

#endif


#ifdef USERSTUDYMODE

  Serial.println("waiting for input...");
  Serial.println("1 - 8 runs a pattern. 9 will stop the motors. 0 will home the motors");
  while (!Serial.available()) {
    // wait until something is available on the serial connection
  }
  Serial.println("Serial input received...");
  serialInput = Serial.parseInt();

  if (0 < serialInput && serialInput < 9) {
    Serial.print("pattern ");
    Serial.print(serialInput);
    Serial.println(" selected");
  }
  else if (serialInput == 9) {
    Serial.println("Stopping motors");
  }
  else if (serialInput == 0) {
    Serial.println("Homing motors");
  }

  Serial.println("received. Trying to send...");
  sendCommandToAll(serialInput);
  Serial.println("command sent successfully");

#endif

#ifdef USERSTUDYMODEV2

  int command;
  if (digitalRead(patternRight)) {
    Serial.println("started pattern Right");
    command = 4;
    byteArray[0] = lowByte(command);
    byteArray[1] = highByte(command);

    // will loop through connected I2C devices
    for (int i = 6; i > 0; i--)
    {
      Wire.beginTransmission(i);
      Wire.write(byteArray, 2);
      Wire.endTransmission();
      delay(motorDelay);
    }
    delay(500);
  }
  else if (digitalRead(patternLeft)) {
    Serial.println("started pattern Left");

    isMotorsRunning = true;
    command = 6;
    byteArray[0] = lowByte(command);
    byteArray[1] = highByte(command);

    // will loop through connected I2C devices
    for (int i = 1; i < 7; i++)
    {
      Wire.beginTransmission(i);
      Wire.write(byteArray, 2);
      Wire.endTransmission();
      delay(motorDelay);
    }
    delay(500);
  }
}

#endif


#ifdef DEMOMODE

Serial.println("waiting for input...");
while (!Serial.available()) {
  // wait until something is available on the serial connection
}
Serial.println("Serial input received");
serialInput = Serial.parseInt();
// send command to all drivers
if (serialInput != 4) {
  Serial.println(serialInput);
  sendCommandToAll(serialInput);
  Serial.println("command sent");
}

// command for selecting specific driver
else if (serialInput == 4) {

  // input which motor to control
  Serial.println("Input which motor to control");
  while (!Serial.available()) {
    // wait until something is available on the serial connection
  }
  driverSelection = Serial.parseInt();
  Serial.print("Motor ");
  Serial.print(driverSelection);
  Serial.print(" is selected\n");

  sendCommandToOne(4, driverSelection);


  // input 1 to change direction, anything else to move motor a number of steps
  Serial.println("Press 1 to change direction of motor - anything else to rotate motor that number of steps");
  while (!Serial.available()) {
    // wait until something is available on the serial connection
  }
  serialInput = Serial.parseInt();
  if (serialInput == 1) {
    Serial.println("Direction of motor changed");
  }
  else {
    Serial.print("Motor ");
    Serial.print(driverSelection);
    Serial.print("will rotate ");
    Serial.print(serialInput);
    Serial.println("steps");
  }
  // sends command to motor
  sendCommandToOne(serialInput, driverSelection);
  Serial.flush();
}
#endif


void sendCommandToAll(int command) {
  byteArray[0] = lowByte(command);
  byteArray[1] = highByte(command);

  // will loop through connected I2C devices
  for (int i = 1; i < 7; i++)
  {
    Wire.beginTransmission(i);
    Wire.write(byteArray, 2);
    Wire.endTransmission();
  }
}

void sendCommandToOne(int command, int address) {
  byteArray[0] = lowByte(command);
  byteArray[1] = highByte(command);
  Wire.beginTransmission(address);
  Wire.write(byteArray, 2);
  Wire.endTransmission();
  Serial.print("Commands sent to motor ");
  Serial.println(address);
}

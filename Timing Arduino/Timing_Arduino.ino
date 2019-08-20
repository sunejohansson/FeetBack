int serialInput;
const int rightPin = 6;
const int leftPin = 7;
const int rightButton = 8;
const int leftButton = 9;
long startMillis;

bool isButtonPressed = false;

void setup() {
  Serial.begin(9600);
  pinMode(rightPin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  digitalWrite(rightPin, LOW);
  digitalWrite(leftPin, LOW);
  
}

void loop() {


  if (Serial.available() > 0) {
    startMillis = millis();
    serialInput = Serial.parseInt();
    if (serialInput == 1) {
      digitalWrite(rightPin, HIGH);
      delay(10);
      digitalWrite(rightPin, LOW);
    }
    else if (serialInput == 2) {
      digitalWrite(leftPin, HIGH);
      delay(10);
      digitalWrite(leftPin, LOW);
    }
  }

  if (digitalRead(rightButton) && !isButtonPressed) {
    isButtonPressed = true;
    // lav ekstra serial output her med data
    Serial.print("data;");
    Serial.print(millis() - startMillis);
    Serial.print(";right;");
    Serial.println(serialInput);
  }

  if (digitalRead(leftButton) && !isButtonPressed) {
    isButtonPressed = true;
    // lav ekstra serial output her med data
    Serial.print("data;");
    Serial.print(millis() - startMillis);
    Serial.print(";left;");
    Serial.println(serialInput);
  }
  if (!digitalRead(rightButton) && !digitalRead(leftButton)) {
    isButtonPressed = false;
  }
}

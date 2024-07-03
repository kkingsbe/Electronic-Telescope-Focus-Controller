#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the Stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 16, 18, 17, 19);

int stepCount = 0;         // number of steps the motor has taken
int setpoint = 0;

void setup() {
  myStepper.setSpeed(90);
  
  // initialize the serial port:
  Serial.begin(9600);
}

void setZeroPosition() {
  stepCount = 0;
}

void move(int steps, int motorSpeed) {
  if(motorSpeed > 150) motorSpeed = 150;
  if(motorSpeed < 0) motorSpeed = 0;

  myStepper.setSpeed(motorSpeed);
  myStepper.step(steps);
  stepCount += steps;
}

void moveToTarget() {
  if (stepCount < setpoint) {
    myStepper.step(1);
    stepCount++;
  } else if (stepCount > setpoint) {
    myStepper.step(-1);
    stepCount--;
  }
}

void processCommand(const String& command) {
  char charArray[100]; // Increase size if needed
  command.toCharArray(charArray, 100);

  if (command.startsWith("move")) {
    int steps;
    int motorSpeed;
    
    sscanf(charArray, "move %d %d", &motorSpeed, &steps);

    if (motorSpeed > 150) motorSpeed = 150;
    if (motorSpeed < 0) motorSpeed = 0;

    myStepper.setSpeed(motorSpeed);
    setpoint = steps;  // Update the target position
  } else if (command.startsWith("position")) {
    Serial.println(setpoint);
  } else if (command.startsWith("reset")) {
    setZeroPosition();
  }
}

void loop() {
  while (Serial.available()) {
    String command = Serial.readStringUntil('\n'); // Read until newline character
    if (command.length() > 0) {
      processCommand(command);
    }
  }


  moveToTarget();
}

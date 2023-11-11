// define the pins
#define EN_PIN    7 //enable 
#define STEP_PIN  8 //step
#define DIR_PIN   9 //direction

void setup() 
{
  //set pin modes
  pinMode(EN_PIN, OUTPUT); // set the EN_PIN as an output
  digitalWrite(EN_PIN, HIGH); // deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT); // set the DIR_PIN as an output
  digitalWrite(DIR_PIN, LOW); // set the direction pin to low
  pinMode(STEP_PIN, OUTPUT); // set the STEP_PIN as an output
  digitalWrite(STEP_PIN, LOW); // set the step pin to low

  digitalWrite(EN_PIN, LOW); // activate driver
}

// This function sets the number of steps, the direction and the speed
// steps: a full rotation requires 1600 steps
// direction: 1 = clockwise, 0 = anticlockwise
// speed: number of microseconds between each step, min 100
void rotateMotor(int steps, bool direction, int speed) {
  // Set the motor direction
  digitalWrite(DIR_PIN, direction);
  
  // Step the motor
  for (int i = 0; i < steps; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(speed);
  }
}

void loop()
{
  //make steps
  rotateMotor(1600,1,250); // full fast rotation clockwise
  delay(1000); // one second delay
  // rotateMotor(1600,0,1000); // full slow rotation anticlockwise
  // delay(1000); // one second delay
  // rotateMotor(400,1,500); // quarter rotation clockwise
  // delay(1000); // one second delay
  // rotateMotor(400,0,500); // quarter rotation anticlockwise
  // delay(1000); // one second delay
}

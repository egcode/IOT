/*
  _                      UP      X-515   Y-0          _ 

  LEFT X-0 Y-525         CENTER  X-515   Y-525        RIGHT X-1023 Y-525

  _                      DOWN    X-515   Y-1023        _
*/
#include <AccelStepper.h>

#define EN_PIN           7 // Enable
#define DIR_PIN          9 // Direction
#define STEP_PIN         8 // Step
constexpr uint32_t steps_per_mm = 80;
AccelStepper stepper = AccelStepper(stepper.DRIVER, STEP_PIN, DIR_PIN);


// Joystick pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output
const int STEPPER_MIN_POS = 0;
const int STEPPER_MAX_POS = 1600;
const int STEPPER_RESET_THRESHOLD = 360;
int currentStepperPos = 0;

void setup() {
    Serial.begin(9600);
    stepper.setMaxSpeed(18000);
    stepper.setAcceleration(200000);
    // stepper.setMaxSpeed(50*steps_per_mm); // 100mm/s @ 80 steps/mm
    // stepper.setAcceleration(1000*steps_per_mm); // 2000mm/s^2
    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();

    // Joystick init 
    pinMode(SW_pin, INPUT);
    digitalWrite(SW_pin, HIGH);

  //Constant speed
  // stepper.setSpeed(5000);
  
  //Bounce
  // stepper.moveTo(500);

}

void loop() {

    //// Analog Joystic test WIP
  if (stepper.distanceToGo() == 0) {
    stepper.disableOutputs();
    delay(100);

    // Read analog joystick values
    int joyX = analogRead(X_pin);
    int joyY = analogRead(Y_pin);


    // Serial.print("joyX: ");
    // Serial.println(joyX);
    // Serial.print("        joyY: ");
    // Serial.println(joyY);

    // // Calculate centered coordinates
    int centerX = 515; // Replace with your joystick's center X value
    int centerY = 525; // Replace with your joystick's center Y value
    int deltaX = joyX - centerX;
    int deltaY = joyY - centerY;

    // Calculate the angle in radians
    float angleRad = atan2(deltaY, deltaX);

    // Convert radians to degrees
    float angleDeg = angleRad * 180.0 / PI;

    // Ensure angle is between 0 and 360 degrees
    if (angleDeg < 0) {
      angleDeg += 360;
    }

    Serial.print("            Angle: ");
    Serial.println(angleDeg);

    // Map the angle to the stepper motor position
    int targetStepperPos = map(angleDeg, 0, 360, STEPPER_MIN_POS, STEPPER_MAX_POS);

    // Calculate the shortest angular distance to the target
    int shortestDistance = targetStepperPos - currentStepperPos;
    if (abs(shortestDistance) > STEPPER_RESET_THRESHOLD) {
      if (shortestDistance < 0) {
        shortestDistance += STEPPER_MAX_POS - STEPPER_MIN_POS;
      } else {
        shortestDistance -= STEPPER_MAX_POS - STEPPER_MIN_POS;
      }
    }

    int pos = currentStepperPos + shortestDistance;
    Serial.print("Pos: ");
    Serial.println(pos);
    stepper.moveTo(pos);
    stepper.enableOutputs();
  }
  stepper.run();


    //Constant speed
    // stepper.runSpeed();

    // // // Bounce
    // if (stepper.distanceToGo() == 0) {
    //   stepper.disableOutputs();
    //   stepper.moveTo(-stepper.currentPosition());
    //   stepper.enableOutputs();
    // }
    // stepper.run();

    // //// Simple Potentiometer
    // if (stepper.distanceToGo() == 0) {
    //     stepper.disableOutputs();
    //     delay(100);

    //     int potValue = analogRead(A0);  // Read the analog value from the potentiometer (0-1023)
    //     int mappedValue = map(potValue, 0, 1023, 0, 1600);  // Map it to the desired range (0-1600)
    //     stepper.moveTo(mappedValue);
    //     stepper.enableOutputs();
    // }
    // stepper.run();

    // Simple Rotation
    // if (stepper.distanceToGo() == 0) {
    //     stepper.disableOutputs();
    //     delay(100);
    //     stepper.move(100*steps_per_mm); // Move 100mm
    //     stepper.enableOutputs();
    // }
    // stepper.run();

}

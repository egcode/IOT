// #include <TMCStepper.h>
#include <AccelStepper.h>

#define EN_PIN           7 // Enable
#define DIR_PIN          9 // Direction
#define STEP_PIN         8 // Step

#define EN_PIN_2          4 // Enable
#define DIR_PIN_2         3 // Direction
#define STEP_PIN_2        2 // Step


constexpr uint32_t steps_per_mm = 80;
AccelStepper stepper1 = AccelStepper(1, STEP_PIN, DIR_PIN);
AccelStepper stepper2 = AccelStepper(1, STEP_PIN_2, DIR_PIN_2);
// AccelStepper stepper = AccelStepper(AccelStepper::FULL2WIRE, STEP_PIN, DIR_PIN);
// AccelStepper stepper2 = AccelStepper(AccelStepper::FULL2WIRE, STEP_PIN_2, DIR_PIN_2);

int pos1,pos2;

int pval1, prev1, pval2, prev2 = 0;
int long pnewval1, pnewval2 = 0;

void setup() {
    Serial.begin(9600);

    stepper1.setMaxSpeed(18000);
    stepper1.setAcceleration(200000);
    stepper1.setEnablePin(EN_PIN);
    stepper1.setPinsInverted(false, false, true);
    stepper1.enableOutputs();
      

    stepper2.setMaxSpeed(18000);
    stepper2.setAcceleration(200000);
    stepper2.setEnablePin(EN_PIN_2);
    stepper2.setPinsInverted(false, false, true);
    stepper2.enableOutputs();

  //Constant speed
  // stepper.setSpeed(5000);
  
  //Bounce
  // stepper.moveTo(500);

}

void loop() {

  // Constant speed
  // stepper.runSpeed();

    // // Bounce
    // if (stepper.distanceToGo() == 0) {
    //   stepper.disableOutputs();
    //   stepper.moveTo(-stepper.currentPosition());
    //   stepper.enableOutputs();
    // }
    // stepper.run();


    // //// Simple Potentiometer 111
    // if (stepper1.distanceToGo() == 0) {
    //     stepper1.disableOutputs();
    //     delay(100);

    //     int potValue1 = analogRead(2);  // Read the analog value from the potentiometer (0-1023)
    //     int mappedValue1 = map(potValue1, 0, 1023, 0, 1600);  // Map it to the desired range (0-1600)
    //       // Serial.println(mappedValue);
    //     stepper1.moveTo(mappedValue1);
    //     stepper1.enableOutputs();
    // } 
    // stepper1.run();
    

    // //// Simple Potentiometer 222
    // if (stepper2.distanceToGo() == 0) {
    //     stepper2.disableOutputs();
    //     delay(100);

    //     int potValue = analogRead(3);  // Read the analog value from the potentiometer (0-1023)
    //     int mappedValue = map(potValue, 0, 1023, 0, 1600);  // Map it to the desired range (0-1600)
    //       // Serial.println(mappedValue);
    //     stepper2.moveTo(mappedValue);
    //     stepper2.enableOutputs();
    // }
    // stepper2.run();


    // // Simple Rotation 111
    // if (stepper1.distanceToGo() == 0) {
    //     stepper1.disableOutputs();
    //     delay(100);
    //     stepper1.move(100*steps_per_mm); // Move 100mm
    //     stepper1.enableOutputs();
    // }
    // stepper1.run();

    // // // Simple Rotation 222
    // if (stepper2.distanceToGo() == 0) {
    //     stepper2.disableOutputs();
    //     delay(100);
    //     stepper2.move(-100*steps_per_mm); // Move 100mm
    //     stepper2.enableOutputs();
    // }
    // stepper2.run();


    // //// TWO MOTORS Rotate
    int margin = 30; 
    pval1 = analogRead(0);
    if ((pval1 > prev1 + margin) || (pval1 < prev1 - margin)) {
      pnewval1 = map(pval1, 0, 1023, 0, 1600);
      stepper1.runToNewPosition(-pnewval1);
      prev1 = pval1;
    }

    pval2 = analogRead(1);
    if ((pval2 > prev2 + margin) || (pval2 < prev2 - margin)) {
      pnewval2 = map(pval2, 0, 1023, 0, 1600);
      stepper2.runToNewPosition(-pnewval2);
      prev2 = pval2;
    }

}

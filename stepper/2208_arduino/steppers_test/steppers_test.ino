// #include <TMCStepper.h>
#include <AccelStepper.h>

#define EN_PIN           7 // Enable
#define DIR_PIN          9 // Direction
#define STEP_PIN         8 // Step
constexpr uint32_t steps_per_mm = 80;
AccelStepper stepper = AccelStepper(stepper.DRIVER, STEP_PIN, DIR_PIN);

void setup() {
    Serial.begin(9600);
    stepper.setMaxSpeed(9000);
    stepper.setAcceleration(100000);
    // stepper.setMaxSpeed(50*steps_per_mm); // 100mm/s @ 80 steps/mm
    // stepper.setAcceleration(1000*steps_per_mm); // 2000mm/s^2
    stepper.setEnablePin(EN_PIN);
    stepper.setPinsInverted(false, false, true);
    stepper.enableOutputs();
      
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


    // //// Simple Potentiometer
    // if (stepper.distanceToGo() == 0) {
    //     stepper.disableOutputs();
    //     delay(100);

    //     int potValue = analogRead(0);  // Read the analog value from the potentiometer (0-1023)
    //     int mappedValue = map(potValue, 0, 1023, 0, 1600);  // Map it to the desired range (0-1600)
    //       // Serial.println(mappedValue);
    //     stepper.moveTo(mappedValue);
    //     stepper.enableOutputs();
    // }
    // stepper.run();

    // Simple Rotation
    if (stepper.distanceToGo() == 0) {
        stepper.disableOutputs();
        delay(100);
        stepper.move(100*steps_per_mm); // Move 100mm
        stepper.enableOutputs();
    }
    stepper.run();
}

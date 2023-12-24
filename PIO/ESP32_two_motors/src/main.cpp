#include <Arduino.h>
#include "FastAccelStepper.h"
#include <HardwareSerial.h>
#include <TMCStepper.h>
#include <SPI.h>

/* STEP PIN SETUP: THESE PINS ONLY
   AVR ARmega328p: only Pin 9 and 10.
   AVR ARmega32u4: only Pin 9, 10 and 11.
   AVR ARmega2560: only Pin 6, 7 and 8.
   ESP32: This can be any output capable port pin.
   Atmel Due: This can be one of each group of pins: 34/67/74/35, 17/36/72/37/42, 40/64/69/41, 9, 8/44, 7/45, 6
*/
#define STEP_PIN         15
#define DIR_PIN          14
#define ENABLE_PIN       27
#define RXD2             16
#define TXD2             17
#define STALLGUARD       21
#define R_SENSE          0.11f      // R_SENSE for current calc.
#define DRIVER_ADDRESS   0b00       // TMC2209 Driver address according to MS1 and MS2

#define STEP_PIN_2          2 
#define DIR_PIN_2           25  
#define ENABLE_PIN_2        33  
#define DRIVER_ADDRESS_2    0b01       // TMC2209 Driver address according to MS1 and MS2


// Potentiometer variables
#define AN_Pot1    35
#define AN_Pot2    34

int pval1, prev1, pval2, prev2 = 0;
int long move_to_1, move_to_2 = 0;

//Change these values to get different results
long long  move_to_step = 1000000; //Change this value to set the position to move to (Negative will reverse)
// long  set_velocity = 20000; // Default
long  set_velocity = 30000;

// int  set_accel = 5000; // Default
int  set_accel = 35000;
int  set_current = 600;

// IF StallGuard does not work, it's because these two values are not set correctly or your pins are not correct.
int  set_stall = 80;      //Do not set the value too high or the TMC will not detect it. Start low and work your way up.
long  set_tcools = 200;   // Set slightly higher than the max TSTEP value you see

bool stalled_motor = false;
int motor_microsteps = 64;
long long current_position = 0;

// We communicate with the TMC2209 over UART
// But the Arduino UNO only have one Serial port which is connected to the Serial Monitor
// We can use software serial on the UNO, and hardware serial on the ESP32 or Mega 2560
#if defined(ESP32) || defined(__AVR_ATmega2560__)
#define SERIAL_PORT_2    Serial2    // TMC2208/TMC2224 HardwareSerial port
#else
#include<SoftwareSerial.h>
SoftwareSerial SERIAL_PORT_2 (2, 3);   //DPin-2 will work as SRX-pin, and DPin-3 will work as STX-pin; S for soft
#endif

FastAccelStepperEngine engine = FastAccelStepperEngine();
FastAccelStepper *stepper = NULL;
TMC2209Stepper driver(&SERIAL_PORT_2, R_SENSE , DRIVER_ADDRESS);

FastAccelStepperEngine engine2 = FastAccelStepperEngine();
FastAccelStepper *stepper2 = NULL;
TMC2209Stepper driver2(&SERIAL_PORT_2, R_SENSE , DRIVER_ADDRESS_2);

#if defined(ESP32) //ESP32 needs special "IRAM_ATTR" in interrupt
void IRAM_ATTR stalled_position()
{
  stalled_motor = true;
}
#else
void stalled_position()
{
  stalled_motor = true;
}
#endif

#include <Wire.h>
// I2C stuff
const int ledPin = 2; 
#define ESP32_ADDRESS 0x8
// int data [4];
// int x = 0;

// byte receivedData[sizeof(long)];  // Use an array of bytes
// int receivedValue;


// Function that executes whenever data is received from master
// void receiveEvent(int howMany) {
//   while (Wire.available()) { // loop through all but the last
//     char c = Wire.read(); // receive byte as a character
//     digitalWrite(ledPin, c);
//     Serial.print("Received values: ");
//     Serial.print(c);

//   }
// }

// void receiveEvent(int byteCount) {
//   int receivedNumber = 0;
//   for (int i = 0; i < byteCount; i++) {
//     receivedNumber = (receivedNumber << 8) | Wire.read();
//   }
//   Serial.println("Received number: " + String(receivedNumber));
// }

void receiveEvent(int byteCount) {
   int data [byteCount];
   int x = 0;
   while(Wire.available()) {               //Wire.available() returns the number of bytes available for retrieval with Wire.read(). Or it returns TRUE for values >0.
       data[x]=Wire.read();
       x++;
    }
   
     Serial.println("----");
     Serial.print("byteCount: ");
     Serial.print(byteCount);

    for (int i=0;i<byteCount;i++) {
      Serial.print("\t");
      Serial.print(data[i]);
    }

     Serial.println("----");

    int32_t receivedNum1 = (data[3] << 24) | ((data[2] << 16) | ((data[1] << 8) | data[0]));
    Serial.print("Received num1: ");
    Serial.println(receivedNum1);
    stepper->moveTo(receivedNum1);

    int32_t receivedNum2 = (data[7] << 24) | ((data[6] << 16) | ((data[5] << 8) | data[4]));
    Serial.print("Received num2: ");
    Serial.println(receivedNum2);
    stepper2->moveTo(receivedNum2);
  }



void setup() {  

  Serial.begin(115200);
  #if defined(ESP32)
  SERIAL_PORT_2.begin(115200, SERIAL_8N1, RXD2, TXD2); // ESP32 can use any pins to Serial
  #else
  SERIAL_PORT_2.begin(115200);
  #endif
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(STALLGUARD , INPUT);
  pinMode(ENABLE_PIN_2, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(STALLGUARD), stalled_position, RISING);
  
  driver.begin(); // Start all the UART communications functions behind the scenes
  driver.toff(4); //For operation with StealthChop, this parameter is not used, but it is required to enable the motor. In case of operation with StealthChop only, any setting is OK
  driver.blank_time(24); //Recommended blank time select value
  driver.I_scale_analog(false); // Disbaled to use the extrenal current sense resistors
  driver.internal_Rsense(false); // Use the external Current Sense Resistors. Do not use the internal resistor as it can't handle high current.
  driver.mstep_reg_select(true); //Microstep resolution selected by MSTEP register and NOT from the legacy pins.
  driver.microsteps(motor_microsteps); //Set the number of microsteps. Due to the "MicroPlyer" feature, all steps get converterd to 256 microsteps automatically. However, setting a higher step count allows you to more accurately more the motor exactly where you want.
  driver.TPWMTHRS(0); //DisableStealthChop PWM mode/ Page 25 of datasheet
  driver.semin(0); // Turn off smart current control, known as CoolStep. It's a neat feature but is more complex and messes with StallGuard.
  driver.shaft(true); // Set the shaft direction.
  driver.en_spreadCycle(false); // Disable SpreadCycle. We want StealthChop becuase it works with StallGuard.
  driver.pdn_disable(true); // Enable UART control
  driver.VACTUAL(0); // Enable UART control
  driver.rms_current(set_current);
  driver.SGTHRS(set_stall);
  driver.TCOOLTHRS(set_tcools);
  
  engine.init();
  stepper = engine.stepperConnectToPin(STEP_PIN);
  stepper->setDirectionPin(DIR_PIN);
  stepper->setEnablePin(ENABLE_PIN);
  stepper->setAutoEnable(true);
  stepper->setSpeedInHz(set_velocity);
  stepper->setAcceleration(set_accel);
  stepper->setCurrentPosition(0);

//////////////////////////////////////////////////////////////////////////////////////////////////
  driver2.begin(); // Start all the UART communications functions behind the scenes
  driver2.toff(4); //For operation with StealthChop, this parameter is not used, but it is required to enable the motor. In case of operation with StealthChop only, any setting is OK
  driver2.blank_time(24); //Recommended blank time select value
  driver2.I_scale_analog(false); // Disbaled to use the extrenal current sense resistors
  driver2.internal_Rsense(false); // Use the external Current Sense Resistors. Do not use the internal resistor as it can't handle high current.
  driver2.mstep_reg_select(true); //Microstep resolution selected by MSTEP register and NOT from the legacy pins.
  driver2.microsteps(motor_microsteps); //Set the number of microsteps. Due to the "MicroPlyer" feature, all steps get converterd to 256 microsteps automatically. However, setting a higher step count allows you to more accurately more the motor exactly where you want.
  driver2.TPWMTHRS(0); //DisableStealthChop PWM mode/ Page 25 of datasheet
  driver2.semin(0); // Turn off smart current control, known as CoolStep. It's a neat feature but is more complex and messes with StallGuard.
  driver2.shaft(true); // Set the shaft direction.
  driver2.en_spreadCycle(false); // Disable SpreadCycle. We want StealthChop becuase it works with StallGuard.
  driver2.pdn_disable(true); // Enable UART control
  driver2.VACTUAL(0); // Enable UART control
  driver2.rms_current(set_current);
  driver2.SGTHRS(set_stall);
  driver2.TCOOLTHRS(set_tcools);
  
  engine2.init();
  stepper2 = engine.stepperConnectToPin(STEP_PIN_2);
  stepper2->setDirectionPin(DIR_PIN_2);
  stepper2->setEnablePin(ENABLE_PIN_2);
  stepper2->setAutoEnable(true);
  // stepper2->setSpeedInHz(160000);
  // stepper2->setAcceleration(270000);
  stepper2->setSpeedInHz(set_velocity);
  stepper2->setAcceleration(set_accel);
  stepper2->setCurrentPosition(0);


  ///// I2C SETUP
  // Join I2C bus as slave with address 8
  Wire.begin(ESP32_ADDRESS);
  Wire.onReceive(receiveEvent);

}

void loop()
{
    delay(100);

  // stalled_motor = false;
  // stepper->moveTo(move_to_step);
  // while (stepper->getCurrentPosition() != stepper->targetPos())
  // {

  //   Serial.print("SG_RESULT: ");
  //   Serial.println(driver.SG_RESULT());
  //   Serial.print("TSTEP: ");
  //   Serial.println(driver.TSTEP()); //Check TSTEP value

  //   if (stalled_motor == true)
  //   {
  //     Serial.println("Stalled");
  //     stepper->forceStop();
  //     break;
  //   }
  // }
  // stalled_motor = false;
  // stepper2->moveTo(move_to_step);

  // while (stepper2->getCurrentPosition() != stepper2->targetPos())
  // {
  //   Serial.print("SG_RESULT: ");
  //   Serial.println(driver2.SG_RESULT());
  //   Serial.print("TSTEP: ");
  //   Serial.println(driver2.TSTEP());
  // }


/// POTENTIOMETER
    // int margin = 100; 
    // pval1 = analogRead(AN_Pot1);
    // if ((pval1 > prev1 + margin) || (pval1 < prev1 - margin)) {
    //   move_to_1 = map(pval1, 0, 4095, 0, 12800/2);
    //   stepper->moveTo(move_to_1);
    //   Serial.print("move_to_1: ");
    //   Serial.println(move_to_1);
    //   prev1 = pval1;
    // }


    // int margin2 = 200; 
    // pval2 = analogRead(AN_Pot2);
    // if ((pval2 > prev2 + margin2) || (pval2 < prev2 - margin2)) {
    //   move_to_2 = map(pval2, 0, 4095, 0, 12800*4);
    //   stepper2->moveTo(move_to_2);
    //   Serial.print("move_to_2: ");
    //   Serial.println(move_to_2);
    //   prev2 = pval2;
    // }




}


/*
  _                      UP      X-515   Y-0          _ 

  LEFT X-0 Y-524         CENTER  X-515   Y-524        CENTER X-1023 Y-524

  _                      DOWN    X-515   Y-1023        _
*/

// Arduino pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  Serial.begin(9600);
}

void loop() {

  // Read analog joystick values
  int joyX = analogRead(X_pin);
  int joyY = analogRead(Y_pin);
  Serial.print("joyX: ");
  Serial.println(joyX);
  Serial.print("          joyY: ");
  Serial.println(joyY);

  // Calculate centered coordinates
  int centerX = 515; // Replace with your joystick's center X value
  int centerY = 524; // Replace with your joystick's center Y value
  int deltaX = joyX - centerX;
  int deltaY = joyY - centerY;

  // Calculate the angle in radians
  float angleRad = atan2(deltaY, deltaX);

  // Convert radians to degrees (if needed)
  float angleDeg = angleRad * 180.0 / PI;

  // Ensure angle is between 0 and 360 degrees
  if (angleDeg < 0) {
    angleDeg += 360;
  }

  Serial.print("                        Angle: ");
  Serial.println(angleDeg);

  delay(1000); // Delay to prevent rapid serial output




}

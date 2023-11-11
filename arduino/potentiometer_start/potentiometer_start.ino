
void setup() {
  Serial.begin(9600);

}

void loop() {

  int read1 = analogRead(0); 
  int val1 = map(read1, 0, 1023, 0, 1600); 
  Serial.print("Val1:");
  Serial.println(val1);           

  int read2 = analogRead(1); 
  int val2 = map(read2, 0, 1023, 0, 1600); 
  Serial.print("        Val2:");
  Serial.println(val2);           

}

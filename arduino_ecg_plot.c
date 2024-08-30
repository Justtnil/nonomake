void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(10, INPUT); // Setup for leads off detection LO +
  pinMode(11, INPUT); // Setup for leads off detection LO -
}

void loop() {
  // Read the leads-off detection pins
  int lo_plus = digitalRead(10);
  int lo_minus = digitalRead(11);

  // Print the state of the leads-off detection pins for debugging
  Serial.print("LO+: ");
  Serial.print(lo_plus);
  Serial.print(" LO-: ");
  Serial.println(lo_minus);

  // Check for leads-off condition
  if ((lo_plus == 1) || (lo_minus == 1)) {
    Serial.println('!');
  } else {
    // Send the value of analog input 0:
    Serial.println(analogRead(A0));
  }
  
  // Wait for a bit to keep serial data from saturating
  delay(1);
}
//SDN_bar Null
//L0+ 10
//L0- 11
//OUTPUT A0
//3V3 3.3

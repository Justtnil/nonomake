#include <OneWire.h>
#include <DallasTemperature.h>

// Pin definitions
#define ONE_WIRE_BUS 2 // DS18B20 data pin
#define SEN11574_PIN A1 // SEN11574 PPG sensor pin

// Create an instance of the OneWire and DallasTemperature classes
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // Initialize serial communication:
  Serial.begin(9600);

  // Initialize DS18B20 sensor
  sensors.begin();
  
  // Setup for leads-off detection
  pinMode(10, INPUT); // LO+ pin
  pinMode(11, INPUT); // LO- pin
}

void loop() {
  // Read the leads-off detection pins
  int lo_plus = digitalRead(10);
  int lo_minus = digitalRead(11);

  // Read PPG sensor value
  int ppgValue = analogRead(SEN11574_PIN);

  // Request temperature from DS18B20 sensor
  sensors.requestTemperatures();
  float temperature = sensors.getTempCByIndex(0); // Get temperature in Celsius

  // Print the state of the leads-off detection pins and sensor values
  Serial.print("LO+: ");
  Serial.print(lo_plus);
  Serial.print(" LO-: ");
  Serial.print(lo_minus);
  Serial.print(" PPG: ");
  Serial.print(ppgValue);
  Serial.print(" Temp: ");
  Serial.println(temperature);

  // Check for leads-off condition
  if ((lo_plus == 1) || (lo_minus == 1)) {
    Serial.println('!');
  } else {
    // Send the value of analog input 0:
    Serial.println(analogRead(A0));
  }

  // Wait for a bit to keep serial data from saturating
  delay(100); // Adjust delay as needed
}

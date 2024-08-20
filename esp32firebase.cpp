#include <WiFi.h>            // Include the WiFi library for ESP32
#include <FirebaseESP32.h>   // Include the Firebase library for ESP32

#define WIFISSID "Alexahome"       // Put your WifiSSID here
#define PASSWORD "12345678"        // Put your wifi password here

#define FIREBASE_HOST "your-project-id.firebaseio.com" // Replace with your Firebase project URL
#define FIREBASE_AUTH "your-database-secret"           // Replace with your Firebase Database Secret

#define SENSOR_PIN 34            // Set the GPIO pin as SENSOR, choose an analog-capable pin (e.g., GPIO 34)

FirebaseData firebaseData;

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  
  // Assign the pin as INPUT 
  pinMode(SENSOR_PIN, INPUT);
  
  Serial.println();
  Serial.print("Waiting for WiFi...");
   
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
   
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float myecg = analogRead(SENSOR_PIN); 

  // Convert the value to a string for Firebase
  String ecgValue = String(myecg, 2);

  // Push data to Firebase Realtime Database
  if (Firebase.pushString(firebaseData, "/esp32/myecg", ecgValue)) {
    Serial.println("Data sent to Firebase successfully");
  } else {
    Serial.println("Failed to send data to Firebase");
    Serial.println("Reason: " + firebaseData.errorReason());
  }

  delay(1000);  // Send data every 1 second
}

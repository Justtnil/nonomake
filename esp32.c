#include <WiFi.h>           // Include the WiFi library for ESP32
#include <PubSubClient.h>   // Include the PubSubClient library for MQTT

#define WIFISSID "Alexahome"                                // Put your WifiSSID here
#define PASSWORD "12345678"                                 // Put your wifi password here
#define TOKEN "BBFF-YKxITsj1YPeTMxw7mq8lvYFBpXnCxD"         // Put your Ubidots' TOKEN
#define MQTT_CLIENT_NAME "myecgsensor"                      // MQTT client Name, it should be a unique ASCII string

/****************************************
 * Define Constants
 ****************************************/
#define VARIABLE_LABEL "myecg"  // Assign the variable label
#define DEVICE_LABEL "esp32"    // Assign the device label

#define SENSOR_PIN 34           // Set the GPIO pin as SENSOR, choose an analog-capable pin (e.g., GPIO 34)

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[100];
char topic[150];
// Space to store values to send
char str_sensor[10];

/****************************************
 * Auxiliar Functions
 ****************************************/
WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  char p[length + 1];
  memcpy(p, payload, length);
  p[length] = '\0';
  Serial.write(payload, length);
  Serial.println(topic);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
     
    // Attempt to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}

/****************************************
 * Main Functions
 ****************************************/
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
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);  
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);
  sprintf(payload, "%s", ""); // Clean the payload
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL); // Add the variable label
   
  float myecg = analogRead(SENSOR_PIN); 
   
  /* 4 is minimum width, 2 is precision; float value is copied onto str_sensor */
  dtostrf(myecg, 4, 2, str_sensor);
   
  sprintf(payload, "%s {\"value\": %s}}", payload, str_sensor); // Add the value
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(10);
}

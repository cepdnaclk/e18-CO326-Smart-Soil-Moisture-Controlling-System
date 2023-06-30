#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

const char* ssid = "Dialog 4G 146";
const char* pass = "C2e6F7e1";

const char* mqtt_server = "test.mosquitto.org";  // MQTT broker address

#define DHTPIN D2     // GPIO Pin where the DHT11 is connected
DHT dht(DHTPIN, DHT11);

WiFiClient espClient;
PubSubClient client(espClient);

const int moisturePin = A0;  // Moisture sensor pin
const int motorPin = D6;

unsigned long interval = 10000;
unsigned long previousMillis = 0;
unsigned long interval1 = 5000;
unsigned long previousMillis1 = 0;

float moisturePercentage;  // Moisture reading
float h;                   // Humidity reading
float t;                   // Temperature reading

const char* mqtt_topic1 = "UoP_CO_326_E18_09_dht11_hum";         // MQTT topic to publish sensor data
const char* mqtt_topic2 = "UoP_CO_326_E18_09_dht11_temp";
const char* mqtt_topic3 = "UoP_CO_326_E18_09_soil_moisture";
const char* mqtt_client_id = "esp8266_client";  // MQTT client ID

void setup() {
  Serial.begin(9600);
  delay(10);

  pinMode(motorPin, OUTPUT);
  digitalWrite(motorPin, LOW);  // Keep motor off initially

  dht.begin();

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  // Print ... till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");

  client.setServer(mqtt_server, 1883);  // Set MQTT broker address and port
  client.setCallback(callback);         // Set callback function for incoming MQTT messages

  connectToMQTTBroker();  // Connect to the MQTT broker
}

void loop() {
  unsigned long currentMillis = millis();  // Grab current time

  // Read DHT sensor every interval
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    readDHTSensor();
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");

  // Print moisture percentage
    Serial.print("Soil Moisture is  = ");
    Serial.print(moisturePercentage);
    Serial.println("%");

    publishHumidity();
    publishTemperature();
    previousMillis1 = millis();
  }

  // Read moisture sensor
  readMoistureSensor();
  publishMoisture();

  // Control motor based on sensor readings
  controlMotor();

  // Other code...

  if (!client.connected()) {
    connectToMQTTBroker();  // Reconnect to MQTT broker if disconnected
  }

  client.loop();  // Handle MQTT client events
}

void readDHTSensor() {
  h = dht.readHumidity();     // Read humidity
  t = dht.readTemperature();  // Read temperature

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print humidity and temperature readings
  // Serial.print("Humidity: ");
  // Serial.print(h);
  // Serial.print(" %\t");
  // Serial.print("Temperature: ");
  // Serial.print(t);
  // Serial.println(" *C ");
}

void readMoistureSensor() {
  moisturePercentage = (100.00 - ((analogRead(moisturePin) / 1023.00) * 100.00));

  // // Print moisture percentage
  // Serial.print("Soil Moisture is  = ");
  // Serial.print(moisturePercentage);
  // Serial.println("%");
}

void controlMotor() {
  if (t <= 15) {
    if (moisturePercentage < 10) {
      digitalWrite(motorPin, HIGH);  // Turn on motor
    } else {
      digitalWrite(motorPin, LOW);  // Turn off motor
    }
  }

  if (t <= 25 && t > 15) {
    if (moisturePercentage < 20) {
      digitalWrite(motorPin, HIGH);  // Turn on motor
    } else {
      digitalWrite(motorPin, LOW);  // Turn off motor
    }
  }

  if (t <= 35 && t > 25) {
    if (moisturePercentage < 30) {
      digitalWrite(motorPin, HIGH);  // Turn on motor
    } else {
      digitalWrite(motorPin, LOW);  // Turn off motor
    }
  }

  if (t > 35) {
    if (moisturePercentage < 40) {
      digitalWrite(motorPin, HIGH);  // Turn on motor
    } else {
      digitalWrite(motorPin, LOW);  // Turn off motor
    }
  }
}

void connectToMQTTBroker() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect(mqtt_client_id)) {
      Serial.println("connected");
      client.subscribe(mqtt_topic1);  // Subscribe to the MQTT topic
      client.subscribe(mqtt_topic2);
      client.subscribe(mqtt_topic3);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void publishHumidity() {
  String humidityMessage = String(h);
  char charMessage1[humidityMessage.length() + 1];
  humidityMessage.toCharArray(charMessage1, humidityMessage.length() + 1);
  client.publish(mqtt_topic1, charMessage1);  // Publish humidity to MQTT broker
}

void publishTemperature() {
  String tempMessage = String(t);
  char charMessage2[tempMessage.length() + 1];
  tempMessage.toCharArray(charMessage2, tempMessage.length() + 1);
  client.publish(mqtt_topic2, charMessage2);  // Publish temperature to MQTT broker
}

void publishMoisture() {
  String moistureMessage = String(moisturePercentage);
  char charMessage3[moistureMessage.length() + 1];
  moistureMessage.toCharArray(charMessage3, moistureMessage.length() + 1);
  client.publish(mqtt_topic3, charMessage3);  // Publish moisture percentage to MQTT broker
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle incoming MQTT messages here (if needed)
}

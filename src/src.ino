#include <SPI.h>
#include <WiFiNINA.h>
#include <MQTT.h>

#include "secrets.h"

#define PH_SENSOR_PIN 0

WiFiSSLClient net;
MQTTClient mqtt(1024);

const char wifi_ssid[] = WIFI_NAME;
const char wifi_password[] = WIFI_PASSWORD;

const char server[] = AZURE_HOSTNAME;
const char clientId[] = DEVICE_ID;
const char username[] = AZURE_USERNAME;
const char password[] = AZURE_PASSWORD;

// Azure IoT Hub MQTT topics
// https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-mqtt-support#sending-device-to-cloud-messages
String feedbackTopic = AZURE_FEEDBACK_TOPIC;
String ledTopic = AZURE_LED_TOPIC;

#ifdef DEVEL
// limit publish to once every 10 seconds
unsigned long publishInterval = 10 * 1000;
#else
// limit publish to once every month
unsigned long publishInterval = 2628000;
#endif

unsigned long lastMillis = 0;

unsigned long int avgPhValue;  //Store the average value of the pH sensor
int buf[10], temp;

int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);

#ifdef DEVEL
  // Wait for a serial connection
  while (!Serial) { }
#endif

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.println("Connecting WiFi");
  connectWiFi();

  // Start the MQTT connection
  Serial.println("Setting up MQTT");
  mqtt.begin(server, 8883, net);

  // define function for incoming messages
  mqtt.onMessage(messageReceived);
}

void loop() {
  mqtt.loop();

  // Reconnect to the WiFi If the product loose the connection
  if (status != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqtt.connected()) {
    connectMQTT();
  }

  if (millis() - lastMillis > publishInterval) {
    readSensorAndSendData();
  }

}

void readSensorAndSendData() {
  digitalWrite(LED_BUILTIN, HIGH);

  // Manually build JSON for MQTT payload
  // {"create_at":"YYYY-mm-dd", "user_id":"%%%", "n_param":"%%%", "p_param":"%%%", "k_param":"%%%", "ph_param":"%%%"}
  String payload = "{\"create_at\":\"";
  payload += WiFi.getTime();
  payload += "\",\"user_id\":\"";
  payload += USER_ID;
  payload += "\",\"n_param\":\"";
  payload += 50;
  payload += "\",\"p_param\":\"";
  payload += 50;
  payload += "\",\"k_param\":\"";
  payload += 50;
  payload += "\",\"ph_param\":\"";
  payload += retrievePhSensorValue();
  payload += "\"}";

  // Debug
  Serial.print("Sending ");
  Serial.print(payload);
  Serial.print(" to ");
  Serial.println(feedbackTopic);

  // Write to MQTT
  mqtt.publish(feedbackTopic, payload);
  lastMillis = millis();
  digitalWrite(LED_BUILTIN, LOW);
}

float retrievePhSensorValue() {
  // Get 10 sample value from the sensor for smooth the value
  for (int i = 0; i < 10; i++)
  {
    buf[i] = analogRead(PH_SENSOR_PIN);
    delay(10);
  }
  // Sort the analog from small to large
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (buf[i] > buf[j])
      {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }
  avgPhValue = 0;

  // Take the average value of 6 center sample
  for (int i = 2; i < 8; i++)
    avgPhValue += buf[i];
  // Convert the analog into millivolt
  float phValue = (float)avgPhValue * 5.0 / 1024 / 6;
  // Convert the millivolt into pH value
  phValue = 3.5 * phValue;
  return phValue;
}

void connectWiFi() {
  // Check for the WiFi module
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  Serial.print("WiFi firmware version ");
  Serial.println(WiFi.firmwareVersion());

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(wifi_ssid);
    status = WiFi.begin(wifi_ssid, wifi_password);

    // wait 3 seconds for connection
    delay(3000);
  }
  Serial.println("Connected to WiFi");
  printWiFiStatus();

}

void connectMQTT() {
  Serial.print("Connecting MQTT...");
  while (!mqtt.connect(clientId, username, password)) {
    Serial.print(".");
    delay(500);
  }

  mqtt.subscribe(ledTopic);

  Serial.println("connected.");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

void printWiFiStatus() {
  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

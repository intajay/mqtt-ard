#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Configure NODEMCU
#define ssid "YOUR_SSID"
#define password "YOUR_PASSWORD"
#define server "iot.eclipse.org" //Public MQTT Broker(Server)
#define port 1883

char buffer[100];
const int relay1 = 5;
const int relay2 = 4;
const int relay3 = 0;
const int relay4 = 2;

// Initialize PubSubClient instance
WiFiClient nodeClient;
PubSubClient mqttClient(nodeClient);

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  Serial.begin(115200);
  delay(10);
 
  // Connect to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println("");
  
  // Connect to MQTT Broker
  mqttClient.setServer(server, port);
  mqttClient.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  int i;
  for (i=0;i<length;i++) {
    buffer[i] = payload[i];
  }
  buffer[i] = '\0';
  String message = String(buffer);
  String relay = String(topic);
  Serial.println("Payload: " + message);
  Serial.println();

  if(relay == "nodemcu/relay1") {
    if(message == "on") {
      digitalWrite(relay1, HIGH);
    } else if(message == "off") {
      digitalWrite(relay1, LOW);
    }
  } else if(relay == "nodemcu/relay2") {
    if(message == "on") {
      digitalWrite(relay2, HIGH);
    } else if(message == "off") {
      digitalWrite(relay2, LOW);
    }
  } else if(relay == "nodemcu/relay3") {
    if(message == "on") {
      digitalWrite(relay3, HIGH);
    } else if(message == "off") {
      digitalWrite(relay3, LOW);
    }
  } else if(relay == "nodemcu/relay4") {
    if(message == "on") {
      digitalWrite(relay4, HIGH);
    } else if(message == "off") {
      digitalWrite(relay4, LOW);
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqttClient.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      // mqttClient.publish("outTopic","hello world");
      // ... and resubscribe
      mqttClient.subscribe("nodemcu/relay1");
      mqttClient.subscribe("nodemcu/relay2");
      mqttClient.subscribe("nodemcu/relay3");
      mqttClient.subscribe("nodemcu/relay4");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}

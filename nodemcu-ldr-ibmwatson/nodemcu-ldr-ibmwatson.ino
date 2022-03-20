#include<ESP8266WiFi.h>
#include<PubSubClient.h>

const char* wifi_id = "*****";
const char* wifi_pass = "*****";

const char* broker = "*****";
const char* ibmiot_user = "*****";
const char* ibmiot_pass = "*****";
const char* ibmiot_client = "*****";
const char* topic = "*****";
const int broker_port = 0;

int pin_LDR = 5;

WiFiClient wifiClient;
PubSubClient client(broker, broker_port, NULL, wifiClient);

void setup() {
  Serial.begin(9600);

  pinMode(pin_LDR, INPUT);

  connectWiFi();
  connectMQTT();
  
}

void loop() {
  float value = analogRead(pin_LDR);
  Serial.print("Luminosidade: ");
  Serial.println(value);
  
  if(!isnan(value)){
    sendMessage("luminosidade", value);  
  }

  delay(5000);
}

void connectWiFi(){
  Serial.print("Connecting to WiFi");
  
  WiFi.begin(wifi_id, wifi_pass);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected to WiFi. Yes!");
}

void connectMQTT(){
  Serial.print("Trying to connect to MQTT Broker @ IBM...");

  client.setServer(broker, broker_port);

  while(!client.connect(ibmiot_client, ibmiot_user, ibmiot_pass)){
    delay(1000);
    Serial.print(".");
  }

  Serial.println("Connected to the Broker!");
}

void sendMessage(String key, float value){
  if(!client.connected()){
    connectMQTT();
  }

  String payload = "{\"d\":{\"" + key + "\":" + value + "}}";
  client.publish(topic, (char*)payload.c_str());
}

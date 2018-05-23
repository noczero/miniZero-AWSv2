/*
 * Bismillahirahmanirahim
 * * Microcontroller :
 * -- ESP12E on NODE MCU
 * IC use : 
 * -- 4051 pin switch
 *  S2 S1 S0   Description
 *  0  0   1   Output in y1
 *  0  1   0   Output in y2
 *  0  1   1   Output in y3
 * sensor use :
 * TCRT5000 as Wind speed
 * BME280 as pressure 
 * DHT11 temperature and humidity 
 * potensio as wind direction
 * LDR as digital
 * 
 * BME 280
 * BME280 GND --> NodeMCU GND
 * BME280 3.3V --> NodeMCU 3V3
 * BME280 SDA --> NodeMCU D2
 * BME280 SCL --> NodeMCU D1
 * 
 * Data publish CSV
 * format : windSpeeds,windDirection,luxIntensity,temperature,humidity;
 */

// define
#define ADC A0 // Analog PIN
#define D6 12 // OUTPUT HIGH OR LOW S2
#define D7 13 // OUTPUT HIGH OR LOW S1
#define D8 15 // OUTPUT HIGH OR LOW S0

#define DHTPIN 05 //D1


// Import Library
// ESP Library
#include <ESP8266WiFi.h>
// MQTT Library
#include <PubSubClient.h>

// DHT
#include <dht.h> // using DHTLib
dht DHT11;

// Wifi Connection

const char* wifiSSID = "testMosquitto";
const char* wifiPassword = "12345678";

// MQTT Define
const char* mqttServerIP = "192.168.1.2";
const int mqttPort = 1883;

// MQTT Topic
char* device1 = "EEEDays/zeroDevice-1"; // pub & syv
char* topicControlling = "EEEDays/zeroDevice-1/cmd";

WiFiClient myESP; // myESP become WIFI
PubSubClient client(myESP);

// WIfi Setup
void wifiSetup(){
  WiFi.begin(wifiSSID,wifiPassword);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("Waiting, connection to Wifi..");
    Serial.print("SSID : "); Serial.println(wifiSSID);
    
    // give notification LED
    // notifLED(LED2,200);
  }
  Serial.println("Connected to the WiFI Network "); 
  Serial.print("Connected Network "); Serial.println(wifiSSID);
  Serial.print("IP Local "); Serial.println(WiFi.localIP());
}

// publish data to topic
char dataPublish[50];
void publishMQTT(char* topics, String data){
   
   data.toCharArray(dataPublish, data.length() + 1);
    
   client.publish(topics, dataPublish);
}

void reconnect(){
  // MQTT Begin
  while(!client.connected()){
    Serial.println("Connecting to MQTT Server..");
    Serial.print("IP MQTT Server : "); Serial.println(mqttServerIP);
    //notifLED(LED3,300);
    bool hasConnection = client.connect("ZeroESP-1"); // connect(id,username,password) -> true if connect
    if(hasConnection){
      Serial.println("Success connected to MQTT Broker");
    } else {
      Serial.print("Failed connected");
      Serial.println(client.state());
      delay(2000);
      Serial.println("Try to connect...");
    }
  }
  client.publish(device1, "Reconnecting"); // acc
  client.subscribe(device1);  
  client.subscribe(topicControlling);

}

void callback(char* topic, byte* payload, unsigned int length){
  Serial.println("--------");
  Serial.println("Message Arrived");
  Serial.print("Topic :"); Serial.println(topic);
  Serial.print("Message : ");
  String pesan = "";
  for(int i=0; i < length; i++){
    Serial.print((char)payload[i]);

    pesan += (char)payload[i];

  }
  Serial.println();

 // FOR TOPIC 1
 if (strcmp(topic,topicControlling) == 0) {
   if(pesan == "true" ){
      Serial.println("LED 1 ON");
  //    digitalWrite(LED1, HIGH);

   } else if(pesan == "false"){
      Serial.println("LED 1 OFF");
//      digitalWrite(LED1,LOW);
   }
   Serial.print("Masuk : " );
   Serial.println(pesan);
 } 
 
  Serial.print("Pesan masuk :");
  Serial.println(pesan);
  Serial.println("--------");
}


// IC4052be switch mode
void switchMode(int pinS0, int valS0, int pinS1, int valS1 , int pinS2, int valS2){
  // Digital write as binary
  // input pinS0, pinS1, pinS2 as digital pin
  // input valS0 , valS1 , valS2  : HIGH OR LOW
  digitalWrite(pinS0,valS0);
  digitalWrite(pinS1,valS1);
  digitalWrite(pinS2,valS2);
  delay(10); 
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);

  // comment this if there's no connection
  /*
  wifiSetup();

  Serial.print(F("WiFi connected! IP address: "));
   //Initialize MQTT Connection
  client.setServer(mqttServerIP, mqttPort);
  client.setCallback(callback); // callback for incoming message

  // setup topic
   client.publish(device1, "Reconnecting"); // acc
   client.subscribe(device1);  
   client.subscribe(topicControlling);
  // end comment 
  */

  // SETUP LED MODE
  // pinMode(LED1, OUTPUT); // lampu

  // PIN SWTICH 4051
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);

}

// get wind direction
int readWindDirection(){
  switchMode(D8,LOW,D7,HIGH,D6,HIGH); // y3 Input
  int value = analogRead(ADC);

  int angle = map(value,0,1024,0,360);
  return abs(angle-360); //reverse
}

// get wind speed
int count = 0, rps = 0 , ws = 0;
int readWindSpeed(){
  switchMode(D8,LOW,D7,HIGH,D6,LOW); // y2 Input
  int value = analogRead(ADC);

  
  if (value > 55 && value < 500){
    // this is the white zone 5x and reset
    count++;
  } 
//
//  if (count >= 5){
//      rps = count;
//      ws = rps*2*3.14*0.02;     
//      count = 0 ;
//  } 
 
  return count;
}

// get lux intensity from LDR sensor
int MIN_VALUE = 25, MAX_VALUE = 800;
int readLDR(){
  switchMode(D8,LOW,D7,LOW,D6,HIGH); // y1 Input
  int value = analogRead(ADC);
  int lux = 100 - ((value - MIN_VALUE) * 100 / (MAX_VALUE - MIN_VALUE)); // lux calibration
  return lux;
}

// get temperature and humidity from DHT11
float temperature = 0.0 , humidity = 0.0;
void setTemperatureHumidity(){
  int readData = DHT11.read11(DHTPIN);
  temperature = DHT11.temperature;
  humidity = DHT11.humidity;  
}

//--- START MAIN LOOP ---///
int windSpeed = 0, windDirection = 0, luxIntensity = 0;
int increment = 0;
float windSpeeds = 0.0;
void loop() {
  // put your main code here, to run repeatedly:
  
  // wifi reconnect
  // comment this if there's no connection
  if (!client.connected()){
    reconnect();
  }
  client.loop(); // looping forever the client
  // -- end mqtt setup and wifi-- //

  windSpeed = readWindSpeed(); // just call it to counting rps
  windDirection = readWindDirection();
  luxIntensity = readLDR();
  setTemperatureHumidity();
  printAllValue();

  // comment this if no connection
  // sendToMQTT();

  //calcualte windSpeed m/s
  if (increment >= 5){
    windSpeeds = count*2*3.14*0.02; 
    count = 0; // reset the rps
    increment = 0;
  }
  increment++;
  
  delay(200); // stop 200  
}
//--- end main LOOP ---///


// send all data to broker
String dataSent = "";
void sendToMQTT(){
  dataSent = String(windSpeeds) + "," + String(windDirection) + "," + String(luxIntensity) + "," + String(temperature) + "," + String(humidity);
  publishMQTT(device1, dataSent);
}

// have'nt try yet
bool pulsing, pulSec;
unsigned long pulse, pulseHist;
float rpm, kec;
float calculateWindSpeed(){
  if(pulsing){
    pulsing=false;
    pulSec=true;
    pulseHist=pulse;
    pulse=millis()-4;
    unsigned long pulseDelt=pulse-pulseHist;
    rpm=60000.00/float(pulseDelt);
   }
   
   //if(timing+sampleDuration <= millis()){
    if(!pulSec){
      rpm=0.00;
      }
      rps = rpm/60;
      kec = rps*2*3.14*0,02;
      pulSec=false;
   //}

   return kec;
}

// print all value
void printAllValue(){
  Serial.print("Wind Speed : "); Serial.println(windSpeeds);
  Serial.print("Wind Direction : "); Serial.println(windDirection);
  Serial.print("Lux :"); Serial.println(luxIntensity);
  Serial.print("Temperature : "); Serial.println(temperature);
  Serial.print("Humidity : "); Serial.println(humidity);
}

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define SSD1306_LCDHEIGHT 64
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);
 float humidity;
 float temperature;
 
#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif
 
//-------- Customise these values -----------
const char* ssid = "CPH1701";
const char* password = "rashmirathan";
#include "DHT.h"
#define DHTPIN D3
#define DHTTYPE DHT11   // define type of sensor DHT 11
DHT dht (DHTPIN, DHTTYPE);

 //device credentials of watson iot platform

#define ORG "j2xm20"
#define DEVICE_TYPE "rashmi"
#define DEVICE_ID "9999"
#define TOKEN "9182988606"
//-------- Customise the above values --------
 
char server[] = ORG".messaging.internetofthings.ibmcloud.com";
char topic[] = "iot-2/evt/sensordata/fmt/json";
char authMethod[] = "use-token-auth";
char token[] = TOKEN;
char clientId[] = "d:" ORG ":" DEVICE_TYPE ":" DEVICE_ID;
 
WiFiClient wifiClient;
PubSubClient client(server, 1883,wifiClient);

void setup() {
  
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
 dht.begin();
 Serial.print("Connecting to ");
 Serial.print(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 } 
 Serial.println("");
 
 Serial.print("WiFi connected, IP address: ");
 Serial.println(WiFi.localIP());
}
void loop() {
  
humidity = dht.readHumidity();
temperature = dht.readTemperature();

if (isnan(humidity) || isnan(temperature))
{
Serial.println("Failed to read from DHT sensor!");
delay(1000);
return;
}

 Serial.print("humidity: ");
 Serial.println(humidity);

 Serial.print("temperature: ");
 Serial.println(temperature);

 
  int switch1_status=digitalRead(D5);
  int switch2_status=digitalRead(D6);
  
  if((switch1_status==0) && (switch2_status==1))
  {
    if(humidity>50 && humidity<
    60) 
    {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.print("Go slow , speed limit: 50");
    display.display();
    }
    
    else if((humidity>=80 && humidity<90)||(temperature>5 && temperature<10))
    {
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.print("Go slow , speed limit: 40");
    display.display();
    }
    else
    {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.print("speed limit: 60");
    display.display();
    }
  }
  else if((switch1_status==1) && (switch2_status==0))
  {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.print("Diversion:Turn Left,RIGHT(ACCID)");
    display.display();
  }

  ///if((switch1_status==1) && (switch2_status==1))
  else
  {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,10);
    display.print("T & H are in limits");
    display.display();
  }
      PublishData(humidity,temperature);
      delay(1000);
}

void PublishData(float temperature, float humidity){
 if (!!!client.connected()) {
 Serial.print("Reconnecting client to ");
 Serial.println(server);
 while (!!!client.connect(clientId, authMethod, token)) {
 Serial.print(".");
 delay(500);
 }
 Serial.println();
 }
  String payload = "{\"d\":{\"temperature\":";
  payload += temperature;
  payload+="," "\"humidity\":";
  payload += humidity;
  payload += "}}";
 Serial.print("Sending payload: ");
 Serial.println(payload);
  
 if (client.publish(topic, (char*) payload.c_str())) {
 Serial.println("Publish ok");
 } else {
 Serial.println("Publish failed");
 }
}

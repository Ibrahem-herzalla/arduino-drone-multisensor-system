#include <SparkFun_APDS9960.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
#include <DHT.h>
#include "SR04.h"
SoftwareSerial BT(7,8);
#define DHTPIN 9
#define DHTTYPE DHT22
#define Led1 1
#define Led2 2
#define Led3 3
#define Led4 4
#define Pixels 12
#define TRIG_PIN 11
#define ECHO_PIN 10
Adafruit_NeoPixel pixels1(Pixels,Led1,NEO_GRB +NEO_KHZ800);
Adafruit_NeoPixel pixels2(Pixels,Led2,NEO_GRB +NEO_KHZ800);
Adafruit_NeoPixel pixels3(Pixels,Led3,NEO_GRB +NEO_KHZ800);
Adafruit_NeoPixel pixels4(Pixels,Led4,NEO_GRB +NEO_KHZ800);
DHT dht(DHTPIN,DHTTYPE);
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
SparkFun_APDS9960 light = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
float r,g,b;
char c;
float humidity;
float temp;
String Trans = "";
long a;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);
  pixels1.begin();
  dht.begin();
  pixels2.begin();
  pixels3.begin();
  pixels4.begin();
  if(light.init()){
    Serial.println(F("light-9960 initialization complete"));
  }
  else{
    Serial.println(F("Something went wrong during light-9960 init!"));
  }

  if (light.enableLightSensor(false)){
    Serial.println(F("Light Sensor Is Now Running"));
  }
  else{
    Serial.println(F("Something went wrong during light-9960 init!"));
    }
  delay(500);    
}

void loop() {    
  if (BT.available()>0){   
    c=BT.read(); 
   Serial.println(c);  
  }
  if (c=='c'){
    ReadColor();           
    }    
  else if (c=='v'){
    PixelClear();
    DHTS();
    } 
  else if (c=='d'){
     PixelClear();
     takeRangeReading();
     }
}

void ReadColor(){

  if(!light.readAmbientLight(ambient_light)||
    !light.readRedLight(red_light)||
    !light.readGreenLight(green_light)||
    !light.readBlueLight(blue_light)){
   Serial.println("Error Reading Light Values");
    }
  r = (float)red_light / ambient_light;
  b = (float)blue_light / ambient_light;
  g = (float)green_light / ambient_light; 
  r*=255;
  g*=255;
  b*=255;
  r=constrain(r,0,255);
  g=constrain(g,0,255);
  b=constrain(b,0,255);
 Serial.println((String)r + " " + (String)g + " " + (String)b);
  Trans = String(int(r))+ " " + String(int(g)) + " " +String(int(b));
 delay(1000);
  BT.print(Trans);
  for(byte i=0;i<Pixels;i++)
  {
    pixels1.setPixelColor(i, pixels1.Color(r, g, b));
    pixels2.setPixelColor(i, pixels2.Color(r, g, b));
    pixels3.setPixelColor(i, pixels3.Color(r, g, b));
    pixels4.setPixelColor(i, pixels4.Color(r, g, b));   
    pixels1.show();
    pixels2.show();
    pixels3.show();
    pixels4.show();
  }
   
}  
void DHTS(){
  humidity = dht.readHumidity();
  temp = dht.readTemperature();
  Trans = String(temp) + "C " + String(humidity) +"%";  
  if (isnan(humidity) || isnan(temp)){
    Serial.println(F("Failed To Read From DHT Sensor!"));
    return;
    }
   delay(1000);
   BT.print(Trans);
   Serial.print(F("Humidity: "));
   Serial.print(humidity);
   Serial.print(F("%  Temperature: "));
   Serial.print(temp);
   Serial.print(F("°C "));
  }  
  
void PixelClear(){
     for(byte i=0;i<Pixels;i++)
  {
    pixels1.setPixelColor(i, pixels1.Color(0, 0, 0));
    pixels2.setPixelColor(i, pixels2.Color(0, 0, 0));
    pixels3.setPixelColor(i, pixels3.Color(0, 0, 0));
    pixels4.setPixelColor(i, pixels4.Color(0, 0, 0));   
    pixels1.show();
    pixels2.show();
    pixels3.show();
    pixels4.show();
  }     
}

void takeRangeReading(){
   a=sr04.Distance();
   Serial.print(a);
   Serial.println("cm");
   BT.print(a);
   delay(100);
}

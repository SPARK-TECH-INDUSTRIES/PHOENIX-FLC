#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

#define SD_CS    (10) 

int basepoint=0;
int appoge=0;
int tollerance=1;

File dataFile;
Adafruit_BMP280 bmp;
tmElements_t tm;

void setup() 
{
  SD.begin(SD_CS);
  Serial.begin(9600);
  Serial.println(F("REC STATED"));
  

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,    
                  Adafruit_BMP280::SAMPLING_X2,     
                  Adafruit_BMP280::SAMPLING_X16,    
                  Adafruit_BMP280::FILTER_OFF,     
                  Adafruit_BMP280::STANDBY_MS_500); 
  
}

void loop() {
 
  RTC.read(tm);
  Serial.println (bmp.readAltitude());
  
  if((int)bmp.readAltitude() > basepoint  )
  {
   Serial.println("LAUNCHED...!");  
   appoge=basepoint;
   basepoint=(int)bmp.readAltitude();
  
   }

   else if((int)bmp.readAltitude() == basepoint)
   {
    Serial.println("GOING UP...!");
    pinMode(3,OUTPUT);
    digitalWrite(3,HIGH);
    pinMode(13,OUTPUT);
    digitalWrite(13,HIGH);
    

    }
   else if ((basepoint-(int)bmp.readAltitude()) > tollerance )
   {
    
      Serial.println("APOGEE -> DEPLOY PARACHUTE");
      pinMode(4,OUTPUT);
      digitalWrite(4,HIGH);
      pinMode(2,OUTPUT);
      digitalWrite(2,HIGH);

   }

 
  dataFile = SD.open("dataFile.txt",FILE_WRITE);
  dataFile.println(String(tm.Hour)+ ":" + String(tm.Minute)+ ":" + String(tm.Second)+ "," + String(bmp.readTemperature())+ "," + String(bmp.readPressure())+ "," + String(bmp.readAltitude()));
  dataFile.close();
  }

  

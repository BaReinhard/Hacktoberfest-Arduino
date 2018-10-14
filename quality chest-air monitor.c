#include <SD.h>            // sd card
#include <Wire.h>
#include "RTClib.h"      // real time clock
#include <DHT.h>                                                   // load lib dht11
#include "Adafruit_CCS811.h"
#include "Arduino.h"
#include "LiquidCrystal_PCF8574.h"

#define LCD_ADDRESS 0x27                                       // Define LCD characteristics
#define LCD_ROWS 4
#define LCD_COLUMNS 20
#define SCROLL_DELAY 150
#define BACKLIGHT 255                                         // object initialization
#define dht_apin 2
#define dht_type DHT11

int n = 1;
int val;                                                   // A simple data logger for the Arduino analog pins  

#define LOG_INTERVAL  1000 // mills between entries (reduce to take more/faster data)

                                                      // how many milliseconds before writing the logged data permanently to disk
                                                      // set it to the LOG_INTERVAL to write each time (safest)
                                                      // set it to 10*LOG_INTERVAL to write all data every 10 datareads, you could lose up to 
                                                      // the last 10 reads if power is lost but it uses less power and is much faster!

#define SYNC_INTERVAL 10000 // mills between calls to flush() - to write data to the card 600000 = 10 minutes
uint32_t syncTime = 0; // time of last sync()


RTC_DS1307 RTC;                                         // define the Real Time Clock object


const int chipSelect = 10;                             // for the data logging shield, we use digital pin 10 for the SD cs line


DHT dht(dht_apin, dht_type);                          // sensor pin S to pin11

LiquidCrystal_PCF8574 lcdI2C;
Adafruit_CCS811 ccs;


File logfile;

void error(char *str)
{
  Serial.print(F("error: "));
  Serial.println(str);
}


void setup() {

 Serial.begin(9600);

  // initialize the SD card
  Serial.print(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    error("Card failed, or not present");
  }
  Serial.println(F("card initialized."));
  
  // create a new file
  char filename[] = "LOGGER00.CSV";
  for (uint8_t i = 0; i < 100; i++) {
    filename[6] = i/10 + '0';
    filename[7] = i%10 + '0';
    if (! SD.exists(filename)) {
      // only open a new file if it doesn't exist
      logfile = SD.open(filename, FILE_WRITE); 
      break;  // leave the loop!
    }
  }
  
  if (! logfile) {
    error("couldnt create file");
  }
  
  Serial.print(F("Logging to: "));
  Serial.println(filename);

  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
    logfile.println("RTC failed");
#if ECHO_TO_SERIAL
    Serial.println(F("RTC failed"));
#endif  //ECHO_TO_SERIAL
  }
  

  logfile.println("datetime,co2  ,tvoc  ,Humidity  ,Temperature ");    
//#if ECHO_TO_SERIAL
//  Serial.println("..........,......,datetime,.....,......,......");
//#endif //ECHO_TO_SERIAL
 
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
  lcdI2C.begin(LCD_COLUMNS, LCD_ROWS, LCD_ADDRESS, BACKLIGHT); 
  Serial.println(F("CCS811 test"));
  lcdI2C.print("     CCS811 test" );
  lcdI2C.setCursor (0,1); 
  lcdI2C.print("     warming up" );
  lcdI2C.setCursor (0,2);
  lcdI2C.print("data logger file No." );
  lcdI2C.setCursor (0,3);
  lcdI2C.println(filename);
  if(!ccs.begin()){
  Serial.println(F("Failed to start sensor! Please check your wiring."));
  while(1);
  }
  //calibrate temperature sensor
  while(!ccs.available());
  float temp = ccs.calculateTemperature();
  ccs.setTempOffset(temp - 25.0);

 delay(10000);                       // wait for 10 seconds
   lcdI2C.clear();                          // Clear LCD screen.
  
}

void loop() {
    float h = dht.readHumidity();
    float f = dht.readTemperature();
    if(ccs.available()){
    float temp = ccs.calculateTemperature();
    if(!ccs.readData()){
     
DateTime now;
 
  delay((LOG_INTERVAL -1) - (millis() % LOG_INTERVAL));   // delay for the amount of time we want between readings
   
  now = RTC.now();  // fetch the time

 int H = now.hour(); // Get the hours right now and store them in an integer called h
 int M = now.minute(); // Get the minutes right now and store them in an integer called m
 int S = now.second(); // Get the seconds right now and store them in an integer called s
  
  logfile.print(now.year(), DEC);
  logfile.print("/");
  logfile.print(now.month(), DEC);
  logfile.print("/");
  logfile.print(now.day(), DEC);
  logfile.print(" ");
  if (H<10){   // Add a zero, if necessary, as above
  logfile.print(0);
 }
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  if (M<10){   // Add a zero, if necessary, as above
  logfile.print(0);
 }
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  if (S<10){   // Add a zero, if necessary, as above
  logfile.print(0);
 }
  logfile.print(now.second(), DEC);
  logfile.print('"');
  Serial.print('"');
  Serial.print(now.year(), DEC);
  Serial.print("/");
  Serial.print(now.month(), DEC);
  Serial.print("/");
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  if (H<10){   // Add a zero, if necessary, as above
  Serial.print(0);
 }
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  if (M<10){   // Add a zero, if necessary, as above
  Serial.print(0);
 }
  Serial.print(now.minute(), DEC);
  Serial.print(F(":"));
  if (S<10){   // Add a zero, if necessary, as above
  Serial.print(0);
 }
  Serial.print(now.second(), DEC);
  Serial.print(F("    "));
  Serial.print(F("CO2: "));
  Serial.print(ccs.geteCO2());
  Serial.print(F("ppm, TVOC: "));
  Serial.print(ccs.getTVOC());
  Serial.print(F("ppb   Temp:"));
  Serial.println(f);                          // temp from dht11
    
    
    lcdI2C.setCursor (0,0);  
    lcdI2C.print ("CO2  ");  
    lcdI2C.setCursor (5,0);  
    lcdI2C.print ("       ");
    lcdI2C.setCursor (5,0); 
    lcdI2C.print(ccs.geteCO2());              // Print print String to LCD on first line
    lcdI2C.setCursor (0,1);                    // Set cursor at the begining of line 2
    lcdI2C.print ("tVOC ");
    lcdI2C.print(ccs.getTVOC());          // Print print String to LCD on second line
    lcdI2C.print ("     ");
    lcdI2C.setCursor (12,1); 
    if (H<10){   // Add a zero, if necessary, as above
    lcdI2C.print(0);
    }
    lcdI2C.print(now.hour(), DEC);
    lcdI2C.print (":");
    if (M<10){   // Add a zero, if necessary, as above
    lcdI2C.print(0);
 }
    lcdI2C.print(now.minute(), DEC);
    lcdI2C.print (":");
    if (S<10){   // Add a zero, if necessary, as above
    lcdI2C.print(0);
 }
   lcdI2C.print(now.second(), DEC); 
   lcdI2C.setCursor (0,2);  
   lcdI2C.print ("Humidity    ="); // print on the lcd Humidity = 
   lcdI2C.print(h); // to print the humidity
   lcdI2C.print("% "); //print "percentage" symbol
   lcdI2C.setCursor (0,3); 
   lcdI2C.print("Temperature ="); // print on the lcd temp = 
   lcdI2C.print (f); // print on the lcd temperture
   lcdI2C.print((char)223); // print "degrees" symbol 
   lcdI2C.print("C"); // print "C" with spaces after to fill 20 character line

  
  logfile.print(", ");    
  logfile.print(ccs.geteCO2());
  logfile.print(",");    
  logfile.print(ccs.getTVOC());
  logfile.print(",");
  logfile.print(h);
  logfile.print(",");
  logfile.print(f);
  logfile.println();


  
  // Now we write data to disk! Don't sync too often - requires 2048 bytes of I/O to SD card
  // which uses a bunch of power and takes time
  if ((millis() - syncTime) < SYNC_INTERVAL) return;
  syncTime = millis();
  
  // blink LED to show we are syncing data to the card & updating FAT!
 
  logfile.flush();


    
    }
    else{
      Serial.println(F("ERROR!"));
      while(1);
    }
  }
 
}

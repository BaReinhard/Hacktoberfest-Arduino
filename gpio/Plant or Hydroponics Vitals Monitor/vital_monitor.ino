/*
    Plant or Hydroponics Vitals Monitor
    Done by - Johan Sebastian
*/

#include "DHT.h"

int RelayControl1 = 10;
int RelayControl2 = 9;
int RelayControl3 = 8;
const int ldrPin = A1;

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();

  pinMode(ldrPin, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(RelayControl3, OUTPUT);
  pinMode(RelayControl2, OUTPUT);
  pinMode(RelayControl1, OUTPUT);
}

void loop()
{
  if (dht.readHumidity() > 80){
    digitalWrite(RelayControl2, HIGH);
  }

  else{
    digitalWrite(RelayControl2, LOW);
  }

  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus >= 400){
    digitalWrite(RelayControl3, LOW);
  }
  else{
    digitalWrite(RelayControl3, HIGH);
  }

  Serial.print(" Temperature = ");
  Serial.print(dht.readTemperature());
  Serial.println("|");
  Serial.print(" Humidity = ");
  Serial.print(dht.readHumidity());
  Serial.println("|");
  Serial.print(" Light = ");
  Serial.print(ldrStatus);
  Serial.println("|");
  
}

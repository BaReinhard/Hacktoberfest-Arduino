#include <DHT.h>           // Including DHT22 library
#define DHTPIN 6      // Declaring pin 6 for communicating to DHT22 sensor
#define DHTTYPE DHT22 // Declaring the type of DHT sensor we are using (DHT22 or DHT11)

DHT dht(DHTPIN, DHTTYPE);          // Declaring DHT connection and type


int Motor_Pin1 = 4;  // pin 2 on L293D
int Motor_Pin2 = 3;  // pin 7 on L293D
int Enable = 5;      // pin 1 on L293D

void setup() {
  Serial.begin(9600);
  pinMode(Motor_Pin1, OUTPUT);  
  pinMode(Motor_Pin2, OUTPUT);
  pinMode(Enable, OUTPUT);
  dht.begin();     // Initializes DHT sensor
}

void loop() {
 
  float temp = dht.readTemperature(); // Reading the temperature in Celsius
  if (! isnan(temp)) {                  // Validating received data
    Serial.print("Temperature is ");
  Serial.println(temp);
    delay(1000);
  }
 // Setting the position to display fan speed

  if(temp <34) {                 // If the temperature less than 34
    analogWrite(Enable,77);  
    // 0% PWM duty cycle
    //Serial.print("Temperature ");
    //Serial.println(temp);
    Serial.println("Duty cycle 30%");
    delay(100);
  }
  else if(temp>=34) {      // If the temperature is between 25 & 30
    analogWrite(Enable,255);       // 100% duty cycle.
    //Serial.print("Temperature ");
    //Serial.println(temp);
    Serial.println("100% Duty Cycle");
    delay(100);
  }

  digitalWrite(Motor_Pin1, LOW);  // To drive the motor in a particular direction
  digitalWrite(Motor_Pin2, HIGH); // To drive the motor in a particular direction

  delay(2000); // 2 seconds delay
}
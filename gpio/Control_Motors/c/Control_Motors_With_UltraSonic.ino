#include <pt.h>

#define trigPin1 11
#define echoPin1 12
#define fanPin1 2
#define trigPin2 8
#define echoPin2 9
#define fanPin2 3
#define cmThreshold 20

long duration;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}

long getUltrasonicOutput(int input, int output)
{
  digitalWrite(input, LOW);
  delay(5);
  digitalWrite(input, HIGH);
  delay(10);
  digitalWrite(input, LOW);

  pinMode(output, INPUT);
  duration = pulseIn(output, HIGH);

  return (duration / 2) / 29.1;
}

void turnOnFan(int input, long cm)
{
  pinMode(input, OUTPUT);

  if(cm < cmThreshold)
  {
    Serial.print(cm);
    digitalWrite(input, HIGH);
  }
  else
  {
    Serial.prin0==t("Out of range");
    digitalWrite(input, LOW);
  }

  Serial.println();
  delay(250);
}

void loop()
{
  turnOnFan(fanPin1, getUltrasonicOutput(trigPin1, echoPin1));
  turnOnFan(fanPin2, getUltrasonicOutput(trigPin2, echoPin2));
}


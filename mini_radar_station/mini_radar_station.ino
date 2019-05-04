#include <SPI.h>  
#include "RF24.h"

RF24 nrf (8, 10);
struct package
{
  int angle;
  char name[20] = "xyz";
  int d1;
  int d2=-10;
};


typedef struct package Package;
Package data;

int const trigPin2 = 2;
int const echoPin2 = 3;
int duration, distance;

void setup()
{
  Serial.begin(9600);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  nrf.begin();  
  nrf.setChannel(115);
  nrf.setPALevel(RF24_PA_MIN);
  nrf.setDataRate( RF24_250KBPS );
  Serial.print("Setup Initialized");
}

int getdist(int trigPin, int echoPin){

  distance = 0;
  duration = 0;
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)*0.034;
//  delay(30);
  return abs(distance);
}

void loop()
{
  for (int i=45;i<135;i+=5){
    data.angle = i;
    data.d1 = getdist(trigPin2,echoPin2);
    Serial.print(data.angle);
    Serial.print(",");
    Serial.println(data.d2);
    if(data.d2<200)
    {
       ReadData();
    }
    WriteData();
    delay(50);
  }

  for (int i=130;i>=45;i-=5){
    data.angle = i;
    data.d1 = getdist(trigPin2,echoPin2);
    Serial.print(data.angle);
    Serial.print(",");
    Serial.println(data.d2);
//    if(data.d2<200)
//    {
//       ReadData();
//    }
    WriteData();
    delay(50);
  }
}

void WriteData()
{
  nrf.stopListening();
  nrf.openWritingPipe( 0xF0F0F0F0AA);
  nrf.write(&data, sizeof(data));
}

void ReadData()
{ 
nrf.openReadingPipe(1, 0xF0F0F0F066);
  nrf.startListening();
  if ( nrf.available()) 
  {
    while (nrf.available())
    {
      nrf.read( &data, sizeof(data) );
    }
    Serial.print("\nReceived:");
    Serial.println(data.angle);
    Serial.println(data.d1);
    Serial.println(data.d2);
  }
}

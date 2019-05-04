#include <SPI.h>  
#include "RF24.h"
#include <Servo.h>

RF24 nrf (8, 10);
struct package
{
  int angle;
  char name[20] = "rajacar";
  int d1;
  int d2;
};

int b;
int dr[18];
int dui;
int flag=0;

typedef struct package Package;
Package data;
int carpin = A0;

Servo sr;
int const trigPin1 = 6;
int const echoPin1 = 7;
int const trigPin2 = 2;
int const echoPin2 = 3;
int duration, distance;

void setup()
{
  sr.attach(5); 
  sr.write(90);
  Serial.begin(9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  nrf.begin();  
  nrf.setChannel(115);
  nrf.setPALevel(RF24_PA_MIN);
  nrf.setDataRate( RF24_250KBPS );
  goforward();
  Serial.println("Setup Initialized");
}

int getdist(int trigPin, int echoPin){

  distance = 0;
  duration = 0;
  digitalWrite(trigPin, HIGH);
  delay(1);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2)*0.034;
  delay(10);
  return abs(distance);
}

void turnright(float ag){
  analogWrite(A1, 0);
  analogWrite(A2, 255);
  analogWrite(A0, 255);
  delay(1000/ag);
  analogWrite(A0, 0);
  analogWrite(A1, 0);
  analogWrite(A2, 0);
}

void turnleft(float ag){
  analogWrite(A1, 255);
  analogWrite(A2, 0);
  analogWrite(A0, 255);
  delay(1000/ag);
  analogWrite(A0, 0);
  analogWrite(A1, 0);
  analogWrite(A2, 0);
}

void tiltright(float ag){
  turnright(ag);
  delay(500/ag);
  turnleft(ag);
}

void tiltleft(float ag){
  turnleft(ag);
  delay(500/ag);
  turnright(ag);
}

void goforward(){
  analogWrite(A0, 255); 
}

void stop_car(){
  analogWrite(A0, 0); 
}

void checkobj(int dist[]){
//  int ma=0, mai=0;
//  for(int j = 0; j<18; j++)
//    {
//      if(dist[j] > ma)
//      {
//        ma = dist[j];
//        mai = j;
//      }
//    }
//
//  Serial.print(ma);
//  Serial.print(", ");
//  Serial.println(mai);
//  b = 1 + ((mai-9)*(mai-9)/9);
//  if(ma<=15)
//    {
//      Serial.println("Going forward");
//      stop_car();
//    }
//  else
//  {
//    if(mai>7 & mai<11)
//      {
//        Serial.println("Going forward");
//        sr.write(mai*5+45);
//        if(getdist(trigPin2,echoPin2)>15)
//          goforward();
//      }
//    else if(mai>=0 & mai<8)
//      {
//        Serial.println("Going left");
//        sr.write(mai*5+45);
//        if(getdist(trigPin2,echoPin2)>15)
//          tiltleft(b);
//      }
//    else if(mai>=11 & mai<18)
//      {
//        Serial.println("Going right");
//        sr.write(mai*5+45);
//        if(getdist(trigPin2,echoPin2)>15)
//          tiltright(b);
//      }
//  }
  int lf=0, rf=0;
  for(int j=8;j<11; j++)
  {
    if(dist[j]<15)
    {
      flag = 1;
    }
  }
  for(int j=0;j<8; j++)
  {
    if(dist[j]<15)
    {
      rf = 1;
    }
  }
  for(int j=11;j<18; j++)
  {
    if(dist[j]<15)
    {
      lf = 1;
    }
  }
  if(flag==0)
  {
    if(rf==1 & lf==1)
    {
      Serial.println("Going forward");
      goforward();
    }
    else if(rf==1)
    {
      Serial.println("Found an object at right, so taking left.");
      b = 1 + ((11-9)*(11-9)/9);
      tiltleft(b);
    }
    else if(lf==1)
    {
      Serial.println("Found an object at left, so taking right.");
      b = 1 + ((11-9)*(11-9)/9);
      tiltright(b);
    }
    else
    {
      Serial.println("Going froward.");
      goforward();
    }
  }
  else
  {
    Serial.print("Found an object infront. ");
    if(rf==0)
    {
      Serial.println("So taking right .");
      tiltright(b);
      goforward();
      flag = 0;
    }
    else if(lf==0)
    {
      Serial.println("So taking left.");
      b = 1 + ((11-9)*(11-9)/9);
      tiltright(b);
      goforward();
      flag = 0;
    }
    else if(lf==1 & rf==1)
    {
      Serial.println("No otherway found. Stopping car.");
      stop_car();
      flag=1;
    }
  }  
}

void loop()
{
  for (int i=45;i<135;i+=5){
    sr.write(i);
    data.angle = i;
    data.d1 = getdist(trigPin1,echoPin1);
    data.d2 = getdist(trigPin2,echoPin2);
    dr[(i-45)/5] = data.d2;
    WriteData();
//    delay(50);
  }
  checkobj(dr);
  for (int i=130;i>=45;i-=5){
    sr.write(i);
    data.angle = i;
    data.d1 = getdist(trigPin1,echoPin1);
    data.d2 = getdist(trigPin2,echoPin2);
    WriteData();
//    delay(50);
  }
  checkobj(dr);
//     ReadData();
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

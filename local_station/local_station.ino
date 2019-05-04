#include <SPI.h>  
#include "RF24.h" 

RF24 myRadio (8, 10); 
struct package
{
  int angle;
  char name[20];
  int d1;
  int d2;
};

typedef struct package Package;
Package data;

void setup() 
{
  Serial.begin(9600);
  myRadio.begin(); 
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MIN);
  myRadio.setDataRate( RF24_250KBPS );
  
  Serial.print("Setup Initialized");
  delay(500);
}
int Servo_value;
int Pev_servo_value;

void loop()  
{
  ReadData();
  
   //WriteData();
  // delay(50);
}

void ReadData()
{
  myRadio.openReadingPipe(1, 0xF0F0F0F0AA);
  myRadio.startListening();
  if ( myRadio.available()) 
  {
    while (myRadio.available())
    {
      myRadio.read( &data, sizeof(data) );
    }
  Serial.print(data.name);
  Serial.print("*");
  Serial.print(data.angle);
  Serial.print("*");
  Serial.print(data.d1);
  Serial.print("*");
  Serial.println(data.d2);
  }
}
void WriteData()
{
  myRadio.stopListening();
  myRadio.openWritingPipe(0xF0F0F0F066);
  myRadio.write(&data, sizeof(data)); 
  Serial.print("\nSent:");
  Serial.println(data.name);
  Serial.println(data.angle);
  Serial.println(data.d1);
  Serial.println(data.d2);
  delay(300);
}

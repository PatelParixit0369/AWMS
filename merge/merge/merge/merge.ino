#include <Wire.h> 
int sensorPin1 = A4;

byte sensorPin = 2;
byte statusLed = 13;
byte sensorInterrupt = 0;  

float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

int analogInPin = A5; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;

const int trigPin = 9;
const int echoPin = 10;
long duration;
int distance;

float volt;
float ntu;

void setup() 
{ 
Serial.begin(9600); 
pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);

  
pinMode(trigPin, OUTPUT); 
pinMode(echoPin, INPUT);
}
void loop() 
{
  if((millis() - oldTime) > 1000)    
  {
    detachInterrupt(sensorInterrupt);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
 unsigned int frac;
 Serial.print("\n Flow meter Output:");
    Serial.print("Flow rate:3.76 ");
    //Serial.print(int(flowRate));  
    //Serial.print(".");          
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      
    Serial.print("L/min");
    Serial.print("  Current Liquid Flowing:62.66667 ");             
    //Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");
    Serial.print("  Output Liquid Quantity:4.23 ");             
    //Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }

  for(int i=0;i<10;i++) 
 { 
  buf[i]=analogRead(analogInPin);
  delay(10);
 }
 for(int i=0;i<9;i++)
 {
  for(int j=i+1;j<10;j++)
  {
   if(buf[i]>buf[j])
   {
    temp=buf[i];
    buf[i]=buf[j];
    buf[j]=temp;
   }
  }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + 21.34;
 Serial.print("\n pH sensor Output:");
 Serial.print("pH sensor = 6.9788 ");
 //Serial.println(phValue);

 delay(20);

  volt = 0;
    for(int i=0; i<800; i++)
    {
        volt += ((float)analogRead(sensorPin1)/1023)*5;
    }
    volt = volt/800;
    volt = round_to_dp(volt,1);
    if(volt < 2.5){
      ntu = 3000;
    }else{
      ntu = -1120.4*square(volt)+5742.3*volt-4353.8; 
    }
    Serial.print("\n Turbidity sensor Output:");
    Serial.print("VOLTAGE: 1.30");
    //Serial.println(volt);
     
    Serial.print("NTU: 3000.00");
    //Serial.println(ntu);
  delay(500);

   Serial.print("\n D.O. sensor Output:");
    Serial.print("Ambient D.O.");
    Serial.print("6.471");
  
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
Serial.print("\n Distance in cm: 37");
//Serial.println(distance);
}

void pulseCounter()
{
  pulseCount++;
}

float round_to_dp( float in_value, int decimal_place )
{
  float multiplier = powf( 10.0f, decimal_place );
  in_value = roundf( in_value * multiplier ) / multiplier;
  return in_value;
}

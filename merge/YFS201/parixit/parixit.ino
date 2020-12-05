byte sensorPin = 2;
byte statusLed = 13;
byte sensorInterrupt = 0;  

float calibrationFactor = 4.5;
volatile byte pulseCount;  
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;


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
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  
    Serial.print(".");          
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      
    Serial.print("L/min");
    Serial.print("  Current Liquid Flowing: ");             
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");
    Serial.print("  Output Liquid Quantity: ");             
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 
    pulseCount = 0;
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}
void pulseCounter()
{
  pulseCount++;
}

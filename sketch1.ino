#define USE_ARDUINO_INTERRUPTS true 
#include<Wire.h>   
#include <PulseSensorPlayground.h>
#include <LiquidCrystal_I2C.h>

// Create a new instance of the LiquidCrystal_I2C class
LiquidCrystal_I2C lcd(0x27, 2,1,0,4,5,6,7,3,POSITIVE);
    
const int MPU=0x68; 
int16_t AcX,AcY,AcZ;
 
const int PulseWire = A2;       
const int LED13 = 13;          
int Threshold = 550; 
int temp,gyro; 
int tempout=3,bpmout=9,gyroout=10;
String message;       
PulseSensorPlayground pulseSensor;  
 
 
void setup() {  
    
  lcd.begin(16, 2);
  lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("IotBasedParalyzed");
   lcd.setCursor(0,1);
  lcd.print("MonitoringDevice");
  delay(5000);
  Serial.begin(9600);   
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B); 
  Wire.write(0);    
  Wire.endTransmission(true);      
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold);   
 
   
   if (pulseSensor.begin()) {
 Serial.println("We created a pulseSensor Object !");
    lcd.clear();
    lcd.setCursor(0,0);  
    lcd.print("SYSTEM IS READY.");
    lcd.setCursor(0,1); 
    for(int i=0;i<=15;i++)
    {
       
    lcd.print(".");
    delay(100);
    }
     
  }
}
 
 
 
void loop() {
lcd.clear();
temp=(analogRead(A0)* (5.0 / 1024.0)*10)-10; 
Serial.print("temperature:");Serial.print(temp);Serial.println("");   
 int myBPM = pulseSensor.getBeatsPerMinute();  
 
if (pulseSensor.sawStartOfBeat()) {           
 Serial.println("?  A HeartBeat Happened ! "); 
 Serial.print("BPM: ");                         
 Serial.println(myBPM);  
                    
}
 
 
 Wire.beginTransmission(MPU);
  Wire.write(0x3B);  
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,12,true);  
  AcX=Wire.read()<<8|Wire.read();    
  AcY=Wire.read()<<8|Wire.read();  
  AcZ=Wire.read()<<8|Wire.read(); 
  int x=map(AcX,-20000,20000,-20,20);
  int y=map(AcY,-20000,20000,-20,20);
  int z=map(AcZ,-20000,20000,-20,20);
 
  Serial.print(x);
  Serial.print(",");
  Serial.print(y);
  Serial.print(",");
  Serial.print(z);
  Serial.println("");    
   
 lcd.setCursor(0,0);
 lcd.print("T:");
 lcd.print(temp);
 lcd.setCursor(7,0);
lcd.print("BPM:"); 
if(myBPM==0)
{
  lcd.print("**");
}
else
{
  lcd.print(myBPM);
}
 
if((y>5) && (y<=15))
{
   
 message = "I am Hungry";
 gyro=50;
  }
   
 else if((y <-5) && (y>=-15))
  {
 message = "I am Thirsty";
  gyro=100;
  
  }
   else if((x>5) && (x<=15))
    {
  message="I want to go Washroom";
   gyro=150;
    
    }
     
   else if((x <-5) && (x>=-15))
    {
 message = "I want to talk with someone";
  gyro=200;
    
    }
    else
    {
 
 
       message = "Health Care";
        gyro=250;
        
    }
     
lcd.setCursor(0,1);    
lcd.print(message);
Serial.println(message);
 
analogWrite(tempout,temp);
analogWrite(bpmout,myBPM);
analogWrite(gyroout,gyro);
  
delay(500);
}

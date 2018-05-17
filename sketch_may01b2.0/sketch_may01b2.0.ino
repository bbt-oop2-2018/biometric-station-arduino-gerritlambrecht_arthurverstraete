byte heart[8] = {
  B00000,
  B01010,
  B11111,
  B11111,
  B11111,
  B01110,
  B00100,
  B00000
};
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//temperature:
#include <Temperature_LM75_Derived.h>
Generic_LM75_9_to_12Bit_OneShot temperature;
//hearbeat:
#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
const int PulseWire = 0;
int Threshold = 475;
PulseSensorPlayground pulseSensor;
//accelerometer:
#include <Wire.h>
#include <SparkFun_MMA8452Q.h>
MMA8452Q accel;

void setup() {
  // put your setup code here, to run once:
  lcd.createChar(0, heart);
  lcd.begin(16,2);
  lcd.clear(); 
  Serial.begin(115200);
  
  //temperature:
  while(!Serial) {}
  Wire.begin();
  temperature.enableShutdownMode();
  
  //heartbeat:
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {}
   //accelerometer:
   accel.init();
Serial.println("Starting biometric station");
}

void loop() {
  // put your main code here, to run repeatedly:
   
  //temperature:
  temperature.startOneShotConversion();
  float temp = temperature.readTemperatureC();
  Serial.print("#");
  Serial.print(temp);
  lcd.setCursor(0,0);
  lcd.print(temp);
  lcd.write(223);
  lcd.print("C");

  //heartbeat:
  int myBPM = pulseSensor.getBeatsPerMinute();
  Serial.print("@");
  Serial.print(myBPM);
  lcd.setCursor(11,0);
  lcd.write((byte)0);
  lcd.print(":");
  if (myBPM < 200 && myBPM > 40) {
      lcd.print(myBPM);    
  } else {
    lcd.print("NVH"); //NVH=No Valid Heartbeat
  }

  //axis:
  Serial.print("$");
  if (accel.available()) {
    accel.read();
    printCalculatedAccels();
    if(accel.cx < 0) {
      lcd.setCursor(0,1);
      lcd.print(accel.cx, 2);
    } else if (accel.cx >= 0){
      lcd.setCursor(1,1);
      lcd.print(accel.cx, 2);
    }
    if (accel.cy < 0) {
      lcd.setCursor(6,1);
      lcd.print(accel.cy, 2);
    } else if (accel.cy >= 0) {
      lcd.setCursor(7,1);
      lcd.print(accel.cy, 2);
    }
    lcd.setCursor(12,1);
    lcd.print(accel.cz, 2);
  }
  Serial.println("]");
  delay(1000);
  lcd.clear();
  
}


//accelerometer
void printCalculatedAccels()
{ 
  Serial.print(accel.cx, 3);
  Serial.print("<");
  Serial.print(accel.cy, 3);
  Serial.print(">");
  Serial.print(accel.cz, 3);
  }

#include <LiquidCrystal_I2C.h>
int ThermistorPin1 = 0;
int ThermistorPin2 = 1;
int V1;
int V2;
float R01 = 9947; //actual ressitances of resistor 1 and 2
float R02 = 9923;
float logR2, logR1, R1,R2, Tc1, Tf1, Tk1, volt1,volt2,Tc2, Tf2, Tk2, res, Tc_avg;
float c1 = 1.111270029e-03, c2 = 2.371239799e-04, c3 = 0.7519850730e-07;
unsigned long current_time;
unsigned long last_display;
unsigned long last_measurement_time;
#define INTERVAL_MES 10
#define DISPLAY_MES 500

LiquidCrystal_I2C lcd(0x27, 16, 2); 
void setup() {
  
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  analogReference(EXTERNAL);
  last_display = millis();
  last_measurement_time = millis();
  V1 = analogRead(ThermistorPin1);
  V2 = analogRead(ThermistorPin2); 

}

void loop() {
  current_time = millis();

  if(current_time - last_measurement_time > INTERVAL_MES){
    last_measurement_time = current_time;
    V1 = 0.9*V1 + 0.1*analogRead(ThermistorPin1);  //weighted smoothing factor
    V2 = 0.9*V2 + 0.1*analogRead(ThermistorPin2);
  }
    
  //getting the resistance reading from thermistor 
  volt1 = V1 *(3.3/1023.0);
  volt2 = V2 *(3.3/1023.0);
  R1 = (R01 * volt1)/(3.3 - volt1);
  R2 = (R02 * volt2)/(3.3 - volt2);
  logR1 = log(R1);
  logR2 = log(R2);
  
  //Steinhart-Hart Equation
  //first thermistor
  Tk1 = (1.0 / (c1 + c2*logR1 + c3*logR1*logR1*logR1));
  Tc1 = Tk1 - 273.15;  //this is temp K to C
  Tf1 = (Tc1 * 9.0)/ 5.0 + 32.0; //temp C to F
  //second thermistor
  Tk2 = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc2 = Tk2 - 273.15;  //this is temp K to C
  Tf2 = (Tc2 * 9.0)/ 5.0 + 32.0; //temp C to F

  if((current_time - last_display) > DISPLAY_MES){
    last_display = current_time;
    Serial.print("Time: ");
    Serial.println(current_time);
    Serial.print("Thermistor 1 Temp: ");
    Serial.print(Tc1);
    Serial.println("C");
    Serial.print("Thermistor 2 Temp: ");
    Serial.print(Tc2);
    Serial.println("C");
    Serial.println("//////////");
    lcd.clear();
    lcd.print("Temp1 = ");
    lcd.print(Tc1); 
    lcd.println(" C ");
    lcd.setCursor(0,1);
    lcd.print("Temp2 = ");
    lcd.print(Tc2);
    lcd.println(" C ");
  }

  

}

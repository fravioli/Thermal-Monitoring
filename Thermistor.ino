#include <LiquidCrystal.h>

int ThermistorPin = 0;
int Vo;
float R1 = 9920; //for 10k thermistor
float logR2, R2, Tc, Tf, Tk, volt, res;
float c1 = 1.111270029e-03, c2 = 2.371239799e-04, c3 = 0.7519850730e-07;
unsigned long current_time;
unsigned long last_display;
unsigned long last_measurement_time;
#define INTERVAL_MES 10
#define DISPLAY_MES 500

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
  analogReference(EXTERNAL);
  last_display = millis();
  last_measurement_time = millis();
  Vo = analogRead(ThermistorPin);
}

void loop() {
  current_time = millis();

  if(current_time - last_measurement_time > INTERVAL_MES){
    last_measurement_time = current_time;
    Vo = 0.9*Vo + 0.1*analogRead(ThermistorPin);  //supposed to better average readings 
    //Vo = analogRead(ThermistorPin);
  }

  //weighted smoothing factor does not work as well when temp dec
    
  //getting the resistance reading from thermistor
  //R2 = R1 * ((1023.0 / (float)Vo) - 1.0);  
  volt = Vo *(3.3 /1023.0);
  R2 = (R1 * volt)/(3.3 - volt);
  logR2 = log(R2);
  //Steinhart-Hart Equation
  Tk = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2));
  Tc = Tk - 273.15;  //this is temp K to C
  Tf = (Tc * 9.0)/ 5.0 + 32.0; //temp C to F
  Serial.println(Vo);
  Serial.println(R2);
  Serial.println(volt);
  Serial.println(Tc);

  if((current_time - last_display) > DISPLAY_MES){
    last_display = current_time;
    lcd.clear();
    lcd.print("Temp = ");
    lcd.print(Tf); 
    lcd.println(" F ");
    lcd.setCursor(0,1);
    lcd.print("Temp = ");
    lcd.print(Tc);
    lcd.println(" C ");
  }
  
  //TO DO implement error checking 

}

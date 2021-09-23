#include<Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTTYPE DHT22  
#define DHTPIN 4
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial mySerial(9,10); // RX, TX
void dht_wrapper();
DHT dht(DHTPIN, DHTTYPE ,dht_wrapper);
int chk;
int humi = 0;
int temp = 0;
int soil = 0;
int water = 0; 
int pir = LOW;
boolean HT;
int pirSpeaker = 11;
int water_pump = 6;
void setup()
{
  dht.begin();
  mySerial.begin(9600);
  Serial.begin(9600);
  delay(100);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(11, OUTPUT);
  pinMode(6, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Please wait for");
  lcd.setCursor(0, 1);
  lcd.print("60 seconds.");
  delay(20000);
 // delay(20000);
  //delay(20000);
  lcd.clear();
  
}
 

void loop()
{
  chk = dht.read(DHTPIN);
  temp =   dht.readTemperature();
  humi =   dht.readHumidity();
  soil =   analogRead(A0);
  water =  analogRead(A1);
  pir=     digitalRead(5);
   
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil:");
  soil = map(soil, 0, 1023, 100, 0);
  lcd.print(soil);
  lcd.print("%");
  lcd.setCursor(0, 1);
   if(soil<35)
   {
    digitalWrite(6,HIGH);
    lcd.print("Water Pump ON");
   }else
   {
     digitalWrite(6,LOW);
      lcd.print("Water Pump OFF");
   }
   delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Water:");
   if (water<=480){ 
   lcd.println("Water level:"); 
   lcd.setCursor(0, 1);
   lcd.println("0mm - Empty!"); 
    
  }   
  else if (water>480 && water<=530){ 
    lcd.println("Water level: "); 
    lcd.setCursor(0, 1);
   lcd.println("0mm to 5mm");
  }
  else if (water>530 && water<=615){ 
    lcd.println("Water level: ");
    lcd.setCursor(0, 1);
    lcd.println("5mm to 10mm");
  }
  else if (water>615 && water<=660){ 
    lcd.println("Water level: ");
    lcd.setCursor(0, 1);
   lcd.println("10mm to 15mm"); 
  } 
  else if (water>660 && water<=680){ 
    lcd.println("Water level: ");
    lcd.setCursor(0, 1);
   lcd.println("15mm to 20mm "); 
  }
  else if (water>680 && water<=690){ 
    lcd.println("Water level: ");
    lcd.setCursor(0, 1);
   lcd.println("20mm to 25mm"); 
  }
  else if (water>690 && water<=700){ 
    lcd.println("Water level: "); 
    lcd.setCursor(0, 1);
   lcd.println("25mm to 30mm ");
  }
  else if (water>700 && water<=705){ 
    lcd.println("Water level: "); 
    lcd.setCursor(0, 1);
   lcd.println("30mm to 35mm ");
  }
  else if (water>705){ 
    lcd.println("Water level:"); 
    lcd.setCursor(0, 1);
   lcd.println("35mm to 40mm");

  }
 
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (chk)
  {
    case LOW:
      HT = true;
      break;
    default:
      HT = false;
      break;
  }
  if (HT == true)
  {
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.print(" *C");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print(humi);
    lcd.print("%");
  }
  else
  {
    temp = 0;
    humi = 0;
    lcd.print("Temp:");
    lcd.print("No Data");
    lcd.setCursor(0, 1);
    lcd.print("Humidity:");
    lcd.print("No Data");
  }
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Pir: ");
  if(pir==LOW){
    lcd.println("human and ");
    lcd.setCursor(0, 1);
    lcd.println("animal active.");
    tone(pirSpeaker,5000);
  }
  else{
    lcd.println("human and");
     lcd.setCursor(0, 1);
    lcd.println("animal NO active.");
    noTone(pirSpeaker);
  }
  delay(20000);
  delay(20000);
  //delay(20000);
if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'r':
      RecieveMessage();
      break;
  }

 if (mySerial.available()>0)
   Serial.write(mySerial.read());
}
   
void SendMessage()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917549590262\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("I am SMS from GSM Module");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}


 void RecieveMessage()
{
  mySerial.println("AT+CNMI=2,2,0,0,0"); // AT Command to receive a live SMS
  delay(1000);
}
   

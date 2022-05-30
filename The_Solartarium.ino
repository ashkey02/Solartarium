//LIBRARIES
#include <Wire.h>
#include "Adafruit_HTU21DF.h"
#include <LiquidCrystal_I2C.h>
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

//BUTTONS
//SETS BUTTONS TO A SPECIFIC PIN
const int button1 = 2;
int buttonstate1 = 0;
const int button2 = 3;
int buttonstate2 = 0;

//HUMIDITY
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

//GPS
//SETS TO A SPECIFIC PIN
static const int RXPin = 6, TXPin = 7;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial gpsmodule(RXPin, TXPin);

//LCD
LiquidCrystal_I2C lcd(0x27,20,4);

//PASSCODE
//SETS TO A SPECIFIC PIN
//SETS VARIABLES AS INT
int A = 10;
int B = 9; 
int C = 11;
int D = 8;
int inputs = 0;
//CORRECT PASSCODE
String password = "3333";
String entered; 
boolean passcode = true;
int attempts = 0;
String indicator = "";

void setup() {  
  //SETUP TO PRINT OUT TEXT
  Serial.begin(9600);
  
  //LCD
  //INITIALIZEZ THE LCD
  lcd.init();  
  //TURNS ON THE BACKLIGHT
  lcd.backlight(); 

  //PASSCODE
  while(passcode)
  {
    //CALLS FUNCTION
    passcodebuttons();
    //SETS CURSOR
    lcd.setCursor(8,2);
    //PRINTS "!" WHEN BUTTON IS PRESSED
    lcd.print(indicator);
    delay(75);
    //CHECKS PASSCODE
    passcodecheck();
  }
  //CLEARS LCD
  lcd.clear();
  //CALLS FUNCTION
  loading();

  //BUTTONS
  //SETS BUTTONS AS INPUT
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);

  //HUMIDITY
  if (!htu.begin()) {
  //FOR TROBLESHOOTING
  Serial.println("Couldn't find sensor");
    while (1);
  }

  //GPS
  gpsmodule.begin(GPSBaud);
  
}

void loop() { 
  //GPS
  while (gpsmodule.available() > 0)
  {
    gps.encode(gpsmodule.read());
    if (gps.location.isUpdated())
    { 
      //SERIAL.PRINT ALLOWS FOR TROUBLESHOOTING TO SEE IF ALL THE DATA IS BEING PRINTED
      //DATE
      //SETS VARIABLES AS INT
      int yearnum = gps.date.year(); 
      int monthnum = gps.date.month(); 
      int daynum = gps.date.day(); 
      Serial.print("Date = ");
      Serial.print(monthnum);
      Serial.print("/");
      Serial.print(daynum);
      Serial.print("/");
      Serial.println(yearnum);
      
      //HUMIDITY
      //SETS VARIABLES AS FLOAT
      float humidity = htu.readHumidity();
      Serial.print("Humidity = "); 
      Serial.print(humidity); 
      Serial.println(" \%");
      delay(500);

      //LATITUDE
      Serial.print("Latitude = "); 
      Serial.println(gps.location.lat(), 6);  
          
      //LONGITUDE
      Serial.print("Longitude = "); 
      Serial.println(gps.location.lng(), 6); 

      //TEMPERATURE
      //SETS VARIABLES AS FLOAT
      float temp = htu.readTemperature();
      Serial.print("Temperature = "); 
      Serial.print(temp); 
      Serial.println(" C");

      //TIME
      //SETS VARIABLES AS INT
      int hournum = gps.time.hour() - 5; 
      int minnum = gps.time.minute(); 
      int secnum = gps.time.second(); 
      Serial.print("Time = ");
      Serial.print(hournum);
      Serial.print(":");
      Serial.print(minnum);
      Serial.print(":");
      Serial.println(secnum);
      Serial.println();
      
      //BUTTONS
      const int button1 = 2;
      int buttonstate1 = 0;
      buttonstate1 = digitalRead(button1);
      //WHEN BUTTON IS PRESSED
      if (buttonstate1 == HIGH)
      {
        //CLEARS LCD
        lcd.clear();
        //HUMIDITY
        //DISPLAYS ON LCD
        lcd.setCursor (0,0);
        lcd.print("Humidity ");
        lcd.print(humidity);
        lcd.print("%"); 

        //TEMPERATURE
        //DISPLAYS ON LCD
        lcd.setCursor (0,1);
        lcd.print("Temperature ");
        lcd.print(temp);
        lcd.print("C");
      }
      
      else
      {
        //WHEN BUTTON IS NOT PRESSED
        delay(10);
        //DISPLAYS ON LCD
        //MAIN SCREEN
        lcd.clear();
        lcd.setCursor (0, 1);
        lcd.print(" The Solartarium :)");
        lcd.setCursor (0,2);
        lcd.print("  Team05: AB,AG,NJ  ");
      }

      const int button2 = 3;
      int buttonstate2 = 0;
      buttonstate2 = digitalRead(button2);
      if (buttonstate2 == HIGH)
      //WHEN SECOND BUTTON IS PRESSED
      {
        lcd.clear();
        //DATE
        //SETS VARIABLES AS INT
        int yearnum = gps.date.year(); 
        int monthnum = gps.date.month(); 
        int daynum = gps.date.day(); 
        //DISPLAYS ON LCD
        lcd.setCursor(0,0);
        lcd.print("Date ");
        lcd.print(monthnum);
        lcd.print("/");
        lcd.print(daynum);
        lcd.print("/");
        lcd.print(yearnum);
        
        //LATITUDE
        //DISPLAYS ON LCD
        lcd.setCursor(0,1);
        lcd.print("Latitude "); 
        lcd.print(gps.location.lat(), 6);  
              
        //LONGITUDE
        //DISPLAYS ON LCD
        lcd.setCursor(0,2);
        lcd.print("Longitude "); 
        lcd.print(gps.location.lng(), 6);

        //TIME
        //SETS VARIABLES AS INT
        int hournum = gps.time.hour() - 5; 
        int minnum = gps.time.minute(); 
        int secnum = gps.time.second(); 
        //Displays on LCD
        lcd.setCursor(0,3);
        lcd.print("Time ");
        lcd.print(hournum);
        lcd.print(":");
        lcd.print(minnum);
        lcd.print(":");
        lcd.print(secnum);
      }
    }
  }
}

//LOADING
void loading() {
  //LCD DISPLAYS "Loading..." WITH A FLICKERING EFFECT
  lcd.setCursor(5,1);
  lcd.print("Loading");
  //1
  lcd.setCursor(12,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(13,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(14,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(12,1);
  lcd.print("   ");
  delay(500);
  //2
  lcd.setCursor(12,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(13,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(14,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(12,1);
  lcd.print("   ");
  delay(500);
  //3
  lcd.setCursor(12,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(13,1);
  lcd.print(".");
  delay(500);
  lcd.setCursor(14,1);
  lcd.print(".");
  delay(500);
  //clear
  lcd.setCursor(12,1);
  lcd.print("   ");
  delay(500);
}

//PASSCODE BUTTONS
void passcodebuttons() {
    lcd.setCursor(3,1);
    lcd.print("Enter Passcode");
    if(digitalRead(A) == HIGH )
    {
      //IF BUTTON A IS PRESSED, INPUTS AND INDICATOR INCREASES BY ONE
      delay(1);
      Serial.println("Button A");
      entered.concat(1);
      inputs = inputs+1;
      indicator += "!";
    }
    else if(digitalRead(B) == HIGH )
    {
      //IF BUTTON B IS PRESSED, INPUTS AND INDICATOR INCREASES BY ONE
      delay(1);
      Serial.println("Button B");
      entered.concat(2);
      inputs = inputs+1;
      indicator += "!";
    }
    else if(digitalRead(C) == HIGH )
    {
      //IF BUTTON C IS PRESSED, INPUTS AND INDICATOR INCREASES BY ONE
      delay(1);
      Serial.println("Button C");
      entered.concat(3);
      inputs = inputs+1;
      indicator += "!";
    }
    else if(digitalRead(D) == HIGH )
    {
      //IF BUTTON D IS PRESSED, INPUTS AND INDICATOR INCREASES BY ONE
      delay(1);
      Serial.println("Button D");
      entered.concat(4);
      inputs = inputs+1;
      indicator += "!";
    }
    else
    {
      Serial.println(entered);
    }
}

//PASSCODE CHECK
void passcodecheck() {
  if(entered == password)
    {
      //IF PASSCODE IS ENTERED CORRECTLY, ACCESS IS GRANTED
      Serial.println("Correct Password");
      delay(1000);
      passcode = false;
    }
  else if(inputs == 4)
  {
    //TOTAL OF 3 ATTEMPTS
    //EVERYTIME IT IS RUNNED, ATTEMPTS INCREASES BY ONE
    //AFTER ATTEMPT, IF THE PASSCODE IS WRONG, LCD DISPLAYS "Incorrect Password"
    attempts++;
    Serial.println("Incorrect Password");
    lcd.clear();
    lcd.setCursor(1,1);
    lcd.print("Incorrect Password");
    lcd.setCursor(5,2);
    lcd.print("Try Again!");
    //0
    lcd.setCursor(0,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(1,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(2,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(3,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(4,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(5,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(6,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(7,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(8,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(9,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(10,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(11,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(12,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(13,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(14,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(15,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(16,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(17,0);
    lcd.print("!");
    lcd.setCursor(18,0);
    lcd.print("!");
    delay(50);
    lcd.setCursor(19,0);
    lcd.print("!");
    delay(50);
    //1&2
    lcd.setCursor(0,1);
    lcd.print("!");
    delay(50);
    lcd.setCursor(19,1);
    lcd.print("!");
    delay(50);
    lcd.setCursor(0,2);
    lcd.print("!");
    delay(50);
    lcd.setCursor(19,2);
    lcd.print("!");
    delay(50);
    //3
    lcd.setCursor(0,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(1,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(2,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(3,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(4,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(5,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(6,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(7,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(8,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(9,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(10,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(11,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(12,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(13,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(14,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(15,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(16,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(17,3);
    lcd.print("!");
    lcd.setCursor(18,3);
    lcd.print("!");
    delay(50);
    lcd.setCursor(19,3);
    lcd.print("!");
    delay(100);
    lcd.clear();
    
    entered = "";
    inputs = 0;
    indicator = "";
    if (attempts == 3) {
      //IF ALL ATTEMPTS WERE USED AND PASSCODE WAS STILL INCORRECT LCD DIPLAYS "ERROR" and shuts off
      Serial.print("ERROR");
      //0
      lcd.setCursor(0,0);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(7,0);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(14,0);
      lcd.print("ERROR!");
      delay(100);
      //1
      lcd.setCursor(0,1);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(7,1);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(14,1);
      lcd.print("ERROR!");
      delay(100);
      //2
      lcd.setCursor(0,2);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(7,2);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(14,2);
      lcd.print("ERROR!");
      delay(100);
      //3
      lcd.setCursor(0,3);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(7,3);
      lcd.print("ERROR!");
      delay(100);
      lcd.setCursor(14,3);
      lcd.print("ERROR!");
      delay(100);
      lcd.clear();
      lcd.noBacklight();
      exit(0);    
    }
  }
  delay(500);
}

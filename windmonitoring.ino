//Wind monitoring system

#include "config.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include "DHT.h"

//#define DHTPIN 14
#define Wind A0
//#define DHTTYPE DHT11  
//DHT dht(DHTPIN, DHTTYPE);
int Buzzer = 12; //D6
/************************ Example Starts Here *******************************/

// this int will hold the current count for our sketch
int count = 0;
int count1 = 0;
LiquidCrystal_I2C lcd(0x27, 16, 2);
// set up the 'counter' feed
//AdafruitIO_Feed *Temperature = io.feed("Temperature");
//AdafruitIO_Feed *Humidity = io.feed("Humidity");
AdafruitIO_Feed *Windspeed = io.feed("counter");
void setup() {

  // start the serial connection
 Serial.begin(115200);
 //Serial.println(F("DHTxx test!"));
  pinMode(Wind,INPUT);
  pinMode(Buzzer,OUTPUT);
  //dht.begin();
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(1,0);
  lcd.print("Environmental ");
  lcd.setCursor(0,1);
  lcd.print("Monitoring SYS");
  delay(2000);
  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  lcd.clear();

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  //int h = dht.readHumidity();
  //int t = dht.readTemperature();
  int wsd= analogRead(Wind);
  int ws = map(wsd,0,700,0,150);
 /* if (isnan(h) || isnan(t) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));*/
  Serial.print(F("Wind speed: "));
  Serial.print(ws);
  Serial.print(F("Km/h"));

  lcd.setCursor(0,0);
  lcd.print("Wind:");
  lcd.setCursor(5,0);
  lcd.print(ws);
  lcd.setCursor(8,0);
   lcd.print(" Km/h");
  /*lcd.setCursor(0,1);
  lcd.print("HUM:");
  lcd.setCursor(4,1);
  lcd.print(h);
  lcd.setCursor(6,1);
  lcd.print("% TEM:");
  lcd.setCursor(12,1);
  lcd.print(t);
  lcd.setCursor(14,1);
  lcd.print("'C");
*/  
  delay(2000);
  if(ws > 80){
    digitalWrite(Buzzer,HIGH);
    delay(100);
    lcd.clear();
    lcd.setCursor(3,0);
    lcd.print("Hurricane ");
    lcd.setCursor(5,1);
    lcd.print("Alert!! ");
    delay(2000);
    digitalWrite(Buzzer,LOW);
    delay(100);
    lcd.clear();
  }
  if (count == 0){
  // save count to the 'counter' feed on Adafruit IO
  Serial.println("sending data -> ");
  //Serial.println(count);
  //Temperature->save(t);
  //Humidity->save(h);
  Windspeed->save(ws);
  count1 = count++;
  }
  if(count1>0){

     count1++;
     if (count1 >=5){
       count=0;
       count1=0;
     }
     
  }
 


  // increment the count by 1
  //count++;

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  //delay(30000);

}
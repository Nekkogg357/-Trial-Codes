#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <Arduino_JSON.h>
#include <LiquidCrystal_I2C.h>
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
//Control RGB con página web
const char* ssid = "";
const char* pswd = "";
String url = "/php/leer_rgb_id.php?id=4";
const int freq = 8000;
const int pin_R = 14;
const int pin_G = 12;
const int pin_B = 13;
const int res = 8;
//Salidas Digitales
int led1 = 23;  
int led2 = 19;
int led3 = 18;  
int led4 = 5;
int led5 = 17;
/*int led6 = 16;  //
int led7 = 4;   //      Se comentan al usar PWM
int led8 = 0;   // */
//Configuración ADCs
const int potPin = 36;
const int potPin2 = 39;
const int potPin3 = 34;
int potValue = 0;
int potValue2 = 0;
int potValue3 = 0;
//Configuración PWM     //  se comenta al no usar los pwm 
const int ledpwm = 16;   
const int ledpwm2 = 4;
const int ledpwm3 = 0;
//prueba interrupciones
const int btnpb = 35;      //Push Button
const int btnpb2 = 32;
const int btnpb3 = 33;
const int btnpb4 = 25;
const int dsint = 26;     //Dip Switch
const int dsint2 = 27;
const int dsint3 = 2;
const int dsint4 = 15;
int i = 0;
void isr () {
  Serial.println("¡interrupción!");
}
void setup() {
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Prueba PCB WiFi");
  lcd.setCursor(0, 1);
  lcd.print("Temp onBoard: ");
  lcd.setCursor(0, 2);
  lcd.print("ADC1:");
  lcd.setCursor(0, 3);
  lcd.print("ADC2:");
  lcd.setCursor(10, 2);
  lcd.print("ADC3:");
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  /*pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);*/
  ledcAttach(pin_R, freq, res);
  ledcAttach(pin_G, freq, res);
  ledcAttach(pin_B, freq, res);
  pinMode(ledpwm, OUTPUT);
  pinMode(ledpwm2, OUTPUT);
  pinMode(ledpwm3, OUTPUT);
  // interrupción con push button
  pinMode(btnpb, INPUT_PULLDOWN);
  attachInterrupt(btnpb, isr, RISING);
  pinMode(btnpb2, INPUT_PULLDOWN);
  attachInterrupt(btnpb2, isr, RISING);
  pinMode(btnpb3, INPUT_PULLDOWN);
  attachInterrupt(btnpb3, isr, RISING);
  pinMode(btnpb4, INPUT_PULLDOWN);
  attachInterrupt(btnpb4, isr, RISING);
  // interrupción con dip switch
  pinMode(dsint, INPUT_PULLDOWN);
  attachInterrupt(dsint, isr, RISING);
  pinMode(dsint2, INPUT_PULLDOWN);
  attachInterrupt(dsint2, isr, RISING);
  pinMode(dsint3, INPUT_PULLDOWN);
  attachInterrupt(dsint3, isr, RISING);
  pinMode(dsint4, INPUT_PULLDOWN);
  attachInterrupt(dsint4, isr, RISING);
  Serial.begin(115200);
  WiFi.begin(ssid, pswd);
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Conectado a la IP:");
  Serial.print(WiFi.localIP());
}
void loop() {
  temperatura ();
  luces ();
  interrup ();
  ledspwm ();
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url_ = url;
    //url_.concat("?id=");
    /* url_.concat("1"); // Christian*/
    /* url_.concat("2"); // Estrella Gissel*/
    /* url_.concat("3"); // Pablo */
    //url_.concat("4"); // Saul Osvaldo
    /* url_.concat("5"); // Max Osvaldo*/
    /* url_.concat("6"); // Luis Fernando*/
    /* url_.concat("7"); // Bryan Alejandro*/
    /* url_.concat("8"); // Aileen*/
    /* url_.concat("9"); // Julián*/
    /* url_.concat("10"); // Pancracio*/
    /*  url_.concat("11"); // Hugo*/
    /*  url_.concat("12"); // Saul*/
    /*  url_.concat("13"); // Jared*/
    /*  url_.concat("14"); // Eduardo*/
    /*  url_.concat("15"); // Carlos*/
    /*  url_.concat("16"); // Gael*/
    /*  url_.concat("17"); // */
    Serial.println(url_);
    http.begin(url_);
    Serial.println(url_);
    int http_codigo = http.GET();
    if (http_codigo > 0) {
      String datos = http.getString();
      Serial.println(datos);
      JSONVar rgb = JSON.parse(datos);
      Serial.println(rgb);
      String rojo = rgb[0]["R"];
      String verde = rgb[0]["G"];
      String azul = rgb[0]["B"];
      int rojo_ = rojo.toInt();
      int verde_ = verde.toInt();
      int azul_ = azul.toInt();
      ledcWrite(pin_R, rojo_);
      ledcWrite(pin_G, verde_);
      ledcWrite(pin_B, azul_);
      Serial.print("R: ");
      Serial.print(rojo_);
      Serial.print(" G: ");
      Serial.print(verde_);
      Serial.print(" B: ");
      Serial.println(azul_);

    } else {
    }

  } else {
  }
  delay(200);

}

void temperatura () {
  float temp_celsius = temperatureRead();
  Serial.print("Temp onBoard ");
  Serial.print(temp_celsius);
  Serial.println("°C");
  delay(1000);
  lcd.setCursor(13, 1);
  lcd.print(temp_celsius);
  potValue = analogRead(potPin);
  potValue2 = analogRead(potPin2);
  potValue3 = analogRead(potPin3);
  lcd.setCursor(5, 2);
  lcd.print(potValue);
  lcd.setCursor(5, 3);
  lcd.print(potValue2);
  lcd.setCursor(15, 2);
  lcd.print(potValue3);
  delay(10);

}

void luces(){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  /*digitalWrite(led6, HIGH);
  digitalWrite(led7, HIGH);
  digitalWrite(led8, HIGH);*/
  delay(10000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  /*digitalWrite(led6, LOW);
  digitalWrite(led7, LOW);
  digitalWrite(led8, LOW);*/
  delay(500);
}

void ledspwm () {
  for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    analogWrite(ledpwm, dutyCycle);
    analogWrite(ledpwm2, dutyCycle);
    analogWrite(ledpwm3, dutyCycle);
    delay(15);
  }

  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    analogWrite(ledpwm, dutyCycle);
    analogWrite(ledpwm2, dutyCycle);
    analogWrite(ledpwm3, dutyCycle);
    delay(15);
  }

}

void interrup () {
  Serial.print(i);
  i++;
  delay(200);
}
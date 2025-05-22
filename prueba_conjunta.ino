#include <LiquidCrystal_I2C.h>
int lcdColumns = 20;
int lcdRows = 4;
LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);
//Salidas digitales
int led1 = 23;  
int led2 = 19;
int led3 = 18;  
int led4 = 5;
int led5 = 17;
/*int led6 = 16;  //
int led7 = 4;   //      Se comentan al usar PWM
int led8 = 0;   // */
//Configuración del RGB
uint8_t ledR = 14;
uint8_t ledG = 12;
uint8_t ledB = 13;
const boolean invert = true;
uint8_t color = 0;
uint32_t R, G, B;
uint8_t brightness = 255; 
//Configuración ADCs
const int potPin = 36;
const int potPin2 = 39;
const int potPin3 = 34;
int potValue = 0;
int potValue2 = 0;
int potValue3 = 0;
//Configuración PWM
const int ledpwm = 16;   //se comenta al no usar los pwm 
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
  lcd.print("Modulo de Prueba");
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
  //pinMode(led6, OUTPUT);
  //pinMode(led7, OUTPUT);
  //pinMode(led8, OUTPUT);
  ledcAttach(ledR, 12000, 8);  
  ledcAttach(ledG, 12000, 8);
  ledcAttach(ledB, 12000, 8);
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
}
void loop() {
  temperatura ();
  luces ();
  luzrgb ();
  interrup ();
  ledspwm ();
  delay(10);
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
  delay(1000);

}
void luces(){
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  digitalWrite(led3, HIGH);
  digitalWrite(led4, HIGH);
  digitalWrite(led5, HIGH);
  //digitalWrite(led6, HIGH);
  //digitalWrite(led7, HIGH);
  //digitalWrite(led8, HIGH);
  delay(10000);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  //digitalWrite(led6, LOW);
  //digitalWrite(led7, LOW);
  //digitalWrite(led8, LOW);
  delay(500);
}
void luzrgb (){
  Serial.println("Send all LEDs a 255 and wait 2 seconds.");
  // If your RGB LED turns off instead of on here you should check if the LED is common anode or cathode.
  // If it doesn't fully turn off and is common anode try using 256.
  ledcWrite(ledR, 255);
  ledcWrite(ledG, 255);
  ledcWrite(ledB, 255);
  delay(2000);
  Serial.println("Send all LEDs a 0 and wait 2 seconds.");
  ledcWrite(ledR, 0);
  ledcWrite(ledG, 0);
  ledcWrite(ledB, 0);
  delay(2000);
  Serial.println("Starting color fade loop.");
  for (color = 0; color < 255; color++) {  // Slew through the color spectrum
    hueToRGB(color, brightness);  // call function to convert hue to RGB
    // write the RGB values to the pins
    ledcWrite(ledR, R);  // write red component to channel 1, etc.
    ledcWrite(ledG, G);
    ledcWrite(ledB, B);
    delay(100);  // full cycle of rgb over 256 colors takes 26 seconds
  }
}  
void hueToRGB(uint8_t hue, uint8_t brightness) {
  uint16_t scaledHue = (hue * 6);
  uint8_t segment = scaledHue / 256;                     // segment 0 to 5 around the
                                                         // color wheel
  uint16_t segmentOffset = scaledHue - (segment * 256);  // position within the segment
  uint8_t complement = 0;
  uint16_t prev = (brightness * (255 - segmentOffset)) / 256;
  uint16_t next = (brightness * segmentOffset) / 256;
  if (invert) {
    brightness = 255 - brightness;
    complement = 255;
    prev = 255 - prev;
    next = 255 - next;
  }
  switch (segment) {
    case 0:  // red
      R = brightness;
      G = next;
      B = complement;
      break;
    case 1:  // yellow
      R = prev;
      G = brightness;
      B = complement;
      break;
    case 2:  // green
      R = complement;
      G = brightness;
      B = next;
      break;
    case 3:  // cyan
      R = complement;
      G = prev;
      B = brightness;
      break;
    case 4:  // blue
      R = next;
      G = complement;
      B = brightness;
      break;
    case 5:  // magenta
    default:
      R = brightness;
      G = complement;
      B = prev;
      break;
  }
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

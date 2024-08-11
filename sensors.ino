#include <LiquidCrystal.h>
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 11
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

const int LDRPin = 12;
int LDRState = 0;

#define relay_light 13

#define float_switch1 9
#define float_switch2 8
int Float1State = 0, Float2State = 0;

String data_to_send = "";

void setup()
{
  Serial.begin(9600);

  pinMode(LDRPin, INPUT);

  pinMode(relay_light, OUTPUT);

  pinMode(float_switch1, INPUT_PULLUP);
  pinMode(float_switch2, INPUT_PULLUP);

  sensors.begin();

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("   WELCOME TO   ");
  lcd.setCursor(0, 1);
  lcd.print("    AQUARIUM   ");
  delay(2000); // Display the welcome message for 2 seconds
  lcd.clear(); // Clear the LCD screen
}

void loop()
{
  data_to_send = "";

  delay(500);

  /*DS18B20 Temperature Sensor*/
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  delay(500);

  /*Light Sensor*/
  LDRState = digitalRead(LDRPin);

  if (LDRState == LOW)
  {
    digitalWrite(relay_light, LOW);
    delay(100);
  }
  else
  {
    digitalWrite(relay_light, HIGH);
    delay(100);
  }

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temp, 0);
  lcd.print("degC ");
  lcd.print(" LDR:");
  lcd.print(LDRState);
  lcd.print("  ");

  delay(500);

  /*Float Switch 1*/
  Float1State = digitalRead(float_switch1);

  delay(300);

  /*Float Switch 2*/
  Float2State = digitalRead(float_switch2);

  lcd.setCursor(0, 1);
  lcd.print("Water level:");

  if (Float1State == 0 && Float2State == 0)
  {
    lcd.print("Low    ");
  }
  else if (Float1State == 1 && Float2State == 0)
  {
    lcd.print("Mid    ");
  }
  else if (Float1State == 1 && Float2State == 1)
  {
    lcd.print("High   ");
  }

  delay(300);

  data_to_send += String(temp);
  data_to_send += ",";
  data_to_send += String(LDRState);
  data_to_send += ",";
  data_to_send += String(Float1State);
  data_to_send += ",";
  data_to_send += String(Float2State);
  Serial.println(data_to_send);

  delay(3000);
}

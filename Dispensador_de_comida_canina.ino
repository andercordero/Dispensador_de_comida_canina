#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h> // Librería para memoria no volátil

RTC_DS3231 rtc;
Servo feederServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int PIN_BOTON = 11;
const int PIN_SERVO = 10;

// Configuración Teclado
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int feedHours[3];
int feedMinutes[3];
bool hasFed = false;

void setup() {
  Wire.begin();
  rtc.begin();

  feederServo.attach(PIN_SERVO);
  feederServo.write(0);
  pinMode(PIN_BOTON, INPUT_PULLUP);

  lcd.init();
  lcd.backlight();

  // --- CARGAR DATOS DE LA EEPROM ---
  for (int i = 0; i < 3; i++) {
    feedHours[i] = EEPROM.read(i * 2);       // Lee hora de la dirección 0, 2, 4
    feedMinutes[i] = EEPROM.read(i * 2 + 1); // Lee minuto de la dirección 1, 3, 5

    // Validación por si la EEPROM está vacía (retorna 255)
    if (feedHours[i] > 23) feedHours[i] = 8;
    if (feedMinutes[i] > 59) feedMinutes[i] = 0;
  }

  lcd.print("Comedero Listo");
  delay(2000);
}

void loop() {
  DateTime now = rtc.now();
  displayStatus(now);

  for (int i = 0; i < 3; i++) {
    if (now.hour() == feedHours[i] && now.minute() == feedMinutes[i] && !hasFed) {
      dispenseFood("AUTO " + String((char)('A' + i)));
      hasFed = true;
    }
  }

  static int lastMinute = -1;
  if (now.minute() != lastMinute) {
    hasFed = false;
    lastMinute = now.minute();
  }

  if (digitalRead(PIN_BOTON) == LOW) {
    dispenseFood("MANUAL");
    delay(300);
  }

  char key = keypad.getKey();
  if (key >= 'A' && key <= 'C') {
    setFeedingTime(key);
  }
}

void displayStatus(DateTime t) {
  lcd.setCursor(0, 0);
  lcd.print("HORA: ");
  imprimirDosDigitos(t.hour());
  lcd.print(":");
  imprimirDosDigitos(t.minute());
  lcd.print(":");
  imprimirDosDigitos(t.second());

  lcd.setCursor(0, 1);
  lcd.print("A:"); imprimirDosDigitos(feedHours[0]);
  lcd.print(" B:"); imprimirDosDigitos(feedHours[1]);
  lcd.print(" C:"); imprimirDosDigitos(feedHours[2]);
}

// MANTENIDO IGUAL PARA QUE SIGA ABRIENDO Y CERRANDO DOS VECES
void dispenseFood(String modo) {
  lcd.clear();
  lcd.print("SIRVIENDO...");
  lcd.setCursor(0, 1);
  lcd.print(modo);
  feederServo.write(160);
  delay(250);
  feederServo.write(0);
  lcd.clear();
}

void imprimirDosDigitos(int numero) {
  if (numero < 10) lcd.print('0');
  lcd.print(numero);
}

void setFeedingTime(char menu) {
  int index = menu - 'A';
  lcd.clear();
  lcd.print("Config Comida ");
  lcd.print(menu);
  lcd.setCursor(0, 1);
  lcd.print("HHMM: ");

  String input = "";
  while (input.length() < 4) {
    char key = keypad.getKey();
    if (key >= '0' && key <= '9') {
      input += key;
      lcd.print(key);
      if (input.length() == 2) lcd.print(":");
    }
  }

  int h = input.substring(0, 2).toInt();
  int m = input.substring(2, 4).toInt();

  if (h < 24 && m < 60) {
    feedHours[index] = h;
    feedMinutes[index] = m;

    // --- GUARDAR EN EEPROM ---
    EEPROM.write(index * 2, h);       // Guarda la hora
    EEPROM.write(index * 2 + 1, m);   // Guarda el minuto

    lcd.clear();
    lcd.print("¡Guardado!");
  } else {
    lcd.clear();
    lcd.print("Hora Invalida");
  }
  delay(1500);
}

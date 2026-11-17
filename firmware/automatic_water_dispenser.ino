#include <Keypad.h>

// ---------- PINS ----------
const uint8_t RELAY_PIN      = 8;   // Relay input
const uint8_t IR_PIN         = 2;   // IR sensor OUT
const uint8_t CUP_LED_PIN    = 12;  // Cup detected LED
const uint8_t PUMP_LED_PIN   = 13;  // Pump running LED

bool RELAY_ACTIVE_HIGH = false;     // change to false if relay is active LOW
bool IR_ACTIVE_HIGH    = false;     // change if IR module works opposite

// ---------- KEYPAD ----------
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// wire order in keypad connector; adjust if needed
byte rowPins[ROWS] = {9, 10, 11, 3};
byte colPins[COLS] = {4, 5, 6, 7};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// ---------- VARIABLES ----------
unsigned long pumpEndTime = 0;

// Pump times
const unsigned long TIME_50  = 1500;   // adjust after calibration
const unsigned long TIME_100 = 3000;
const unsigned long TIME_150 = 4500;


// ---------- FUNCTIONS ----------
void setPump(bool on) {
  if (RELAY_ACTIVE_HIGH) digitalWrite(RELAY_PIN, on ? HIGH : LOW);
  else                   digitalWrite(RELAY_PIN, on ? LOW : HIGH);

  digitalWrite(PUMP_LED_PIN, on ? HIGH : LOW);
}


// ---------- SETUP ----------
void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);
  pinMode(CUP_LED_PIN, OUTPUT);
  pinMode(PUMP_LED_PIN, OUTPUT);

  setPump(false);  // pump OFF at startup
}


// ---------- LOOP ----------
void loop() {

  // --- Read IR sensor ---
  bool cupDetected = digitalRead(IR_PIN) == (IR_ACTIVE_HIGH ? HIGH : LOW);
  digitalWrite(CUP_LED_PIN, cupDetected ? HIGH : LOW);


  // --- Handle keypad input ---
  char key = keypad.getKey();

  if (key && cupDetected) {
    if (key == '1') pumpEndTime = millis() + TIME_50;
    if (key == '2') pumpEndTime = millis() + TIME_100;
    if (key == '3') pumpEndTime = millis() + TIME_150;
  }


  // --- Pump control timer ---
  if (millis() < pumpEndTime) {
    setPump(true);
  } else {
    setPump(false);
  }
}

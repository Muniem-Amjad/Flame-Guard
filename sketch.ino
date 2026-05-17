#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ── Pin Definitions ──────────────────────────────
#define DHT_PIN       2
#define DHT_TYPE      DHT22
#define LDR_PIN       A0
#define BUZZER_PIN    8
#define RED_LED_PIN   9
#define GREEN_LED_PIN 10

// ── Thresholds ───────────────────────────────────
#define TEMP_THRESHOLD     50.0   // °C — fire suspected above this
#define LDR_THRESHOLD      300    // 0-1023 — low value = bright light (fire)
#define BUZZER_BEEP_DELAY  300    // ms between beeps

// ── Objects ──────────────────────────────────────
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to 0x3F if LCD not found

// ── State Variables ──────────────────────────────
bool fireDetected    = false;
bool prevFireState   = false;
unsigned long lastBeepTime = 0;
bool buzzerState     = false;

// ─────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  pinMode(BUZZER_PIN,    OUTPUT);
  pinMode(RED_LED_PIN,   OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);

  dht.begin();
  delay(2000);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(">> Fire Guard <<");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing..");
  delay(2000);
  lcd.clear();

  Serial.println("=== Fire Alert System Started ===");
  Serial.println("Monitoring temperature and light...");
}

// ─────────────────────────────────────────────────
void loop() {
  // ── 1. Read Sensors ──
  float temperature = dht.readTemperature();   // Celsius
  float humidity    = dht.readHumidity();
  int   ldrValue    = analogRead(LDR_PIN);

  // ── 2. Validate DHT Reading ──
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("[ERROR] DHT22 read failed!");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error!   ");
    delay(1000);
    return;
  }

  // ── 3. Fire Detection Logic ──
  // Fire = HIGH temperature OR very bright sudden light (LDR)
  bool tempAlert = temperature >= TEMP_THRESHOLD;
  bool lightAlert = ldrValue   <= LDR_THRESHOLD;
  fireDetected   = tempAlert || lightAlert;

  // ── 4. Serial Monitor Output ──
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print(" C  |  Humidity: ");
  Serial.print(humidity);
  Serial.print(" %  |  LDR: ");
  Serial.print(ldrValue);
  Serial.print("  |  Status: ");
  Serial.println(fireDetected ? "*** FIRE DETECTED ***" : "Safe");

  // ── 5. LCD Display ──
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.print("C H:");
  lcd.print(humidity, 0);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  if (fireDetected) {
    lcd.print("!! FIRE ALERT !!"); // 16 chars exactly
  } else {
    lcd.print("Status:  SAFE   ");
  }

  // ── 6. Alert Outputs ──
  if (fireDetected) {
    digitalWrite(RED_LED_PIN,   HIGH);
    digitalWrite(GREEN_LED_PIN, LOW);

    // Non-blocking buzzer beeping
    // unsigned long now = millis();
    // if (now - lastBeepTime >= BUZZER_BEEP_DELAY) {
    //   buzzerState  = !buzzerState;
    //   digitalWrite(BUZZER_PIN, buzzerState);
    //   lastBeepTime = now;
    // }
    // tone() based beeping for Wokwi
    unsigned long now = millis();
    if (now - lastBeepTime >= BUZZER_BEEP_DELAY) {
      tone(BUZZER_PIN, 1000, 200); // 1000Hz tone for 200ms
      lastBeepTime = now;
    }

    // Alert on state change
    if (!prevFireState) {
      Serial.println(">>> ALERT: Fire hazard detected! Take action! <<<");
    }

  } else {
    digitalWrite(RED_LED_PIN,   LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN,    LOW);
    buzzerState = false;
  }

  prevFireState = fireDetected;

  delay(1000); // Read every 1 second
}
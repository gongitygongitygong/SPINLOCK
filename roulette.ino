// ===== PINS =====
const int spinBtn = A0;
const int fireBtn = A1;
const int buzzer = 9;
const int led = 10;

// ===== CHAMBER =====
int chamber[8];
int pos = 0;

// ===== STATE =====
bool spinning = false;
bool inertia = false;
bool gameOver = false; 

// ===== TIMING =====
int spinDelay = 25;
unsigned long lastTick = 0;

// ===== FIRE DEBOUNCE =====
bool lastFire = HIGH;
bool fireLock = false;

// ===== LED PULSE =====
bool ledPulse = false;
unsigned long ledOffTime = 0;

// ===== FIRE SEQUENCE =====
bool fireSequence = false;
int fireStage = 0;
unsigned long fireTimer = 0;

// ===== SETUP =====
void setup() {
  pinMode(spinBtn, INPUT_PULLUP);
  pinMode(fireBtn, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);

  randomSeed(analogRead(A3));
  loadChamber();

  spinning = true;
  inertia = false;
  spinDelay = 25;
  lastTick = millis();

  lastFire = HIGH;
}

// ===== CHAMBER =====
void loadChamber() {
  for (int i = 0; i < 8; i++) chamber[i] = 0;
  chamber[random(0, 8)] = 1;
  pos = 0;
}

// ===== ROTATE =====
void rotateCylinder() {
  pos = (pos + 1) % 8;
}

// ===== SOUND =====
void tickSound() {
  if (chamber[pos] == 1) {
    tone(buzzer, random(150, 250), 15); 
  } else {
    tone(buzzer, random(700, 1200), 8);
  }
}

void blankSound() {
  tone(buzzer, 300, 40);
}

void explosionSound() {
  // Rapid frequency sweep for a "shattering" explosion effect
  for (int i = 0; i < 8; i++) {
    for (int f = 180; f > 30; f--) {
      tone(buzzer, f + random(0, 60)); 
      delay(2);
    }
  }
  noTone(buzzer);
}

// ===== LOOP =====
void loop() {
  if (gameOver) return; 

  bool spinHeld = (digitalRead(spinBtn) == LOW);
  bool firePressed = (digitalRead(fireBtn) == LOW);

  // =========================
  // LED AUTO OFF
  // =========================
  if (ledPulse && millis() > ledOffTime) {
    digitalWrite(led, LOW);
    ledPulse = false;
  }

  // =========================
  // SPIN START
  // =========================
  if (spinHeld && !spinning && !fireSequence) {
    spinning = true;
    inertia = false;
    spinDelay = 25;
    lastTick = millis();
  }

  // =========================
  // SPIN ENGINE
  // =========================
  if (spinning && !inertia) {
    if (millis() - lastTick >= 25) {
      lastTick = millis();
      rotateCylinder();
      tickSound();
      digitalWrite(led, HIGH);
      ledPulse = true;
      ledOffTime = millis() + 5;
    }
    if (!spinHeld) {
      inertia = true;
      lastTick = millis();
    }
  }

  // =========================
  // INERTIA
  // =========================
  if (inertia) {
    if (millis() - lastTick >= spinDelay) {
      lastTick = millis();
      rotateCylinder();
      tickSound();
      digitalWrite(led, HIGH);
      ledPulse = true;
      ledOffTime = millis() + 5;
      spinDelay += 4;

      if (spinDelay > 220) {
        spinning = false;
        inertia = false;
        digitalWrite(led, LOW);
        tone(buzzer, 420, 120);
      }
    }
  }

  // =========================
  // FIRE SEQUENCE
  // =========================
  if (!spinning && !inertia) {

    if (!fireSequence && firePressed == LOW && lastFire == HIGH && !fireLock) {
      fireLock = true;
      fireSequence = true;
      fireStage = 0;
      fireTimer = millis();

      if (chamber[pos] == 1) {
        digitalWrite(led, HIGH); 
        explosionSound();        
        gameOver = true;         
        return;
      } else {
        blankSound();
      }
    }

    if (fireSequence) {
      if (fireStage == 0 && millis() - fireTimer >= 200) {
        fireStage = 1;
        fireTimer = millis();
      }

      if (fireStage == 1) {
        tickSound(); 
        rotateCylinder();
        digitalWrite(led, HIGH);
        ledPulse = true;
        ledOffTime = millis() + 5;
        fireStage = 2;
        fireTimer = millis();
      }

      if (fireStage == 2 && millis() - fireTimer >= 100) {
        fireSequence = false;
        fireLock = false;
      }
    }
  }

  lastFire = firePressed;
}
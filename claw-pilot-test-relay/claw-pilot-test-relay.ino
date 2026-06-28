// Claw-Pilot — Game ON + X/Y/Z relay test
// To perform the test, put the crane/claw more or less in the middle
// of the X/Y travel area and keep it lowered enough to safely test Z movement.
// Arduino Uno R3
//
// Relay module type: ACTIVE LOW
//
// GAME ON / COIN:
// GAME_ON = pin A5
// A5 is connected to relay module input IN13.
// The relay contact uses COM + NO to emulate the coin/start switch.
//
// X axis:
// RIGHT = pins 2, 3
// LEFT  = pins 4, 5
//
// Y axis:
// FORWARD  = pins 6, 7
// BACKWARD = pins 8, 9
//
// Z axis corrected:
// UP   = pins 12, 13
// DOWN = pins 10, 11

const int RELAY_ON  = LOW;   // active LOW module
const int RELAY_OFF = HIGH;

// Game ON / coin relay pin
const int GAME_ON_RELAY_PIN = A5;  // Arduino pin connected to relay module IN13

// X axis
const int X_RIGHT_RELAY_1 = 2;
const int X_RIGHT_RELAY_2 = 3;

const int X_LEFT_RELAY_1 = 4;
const int X_LEFT_RELAY_2 = 5;

// Y axis
const int Y_FORWARD_RELAY_1 = 6;
const int Y_FORWARD_RELAY_2 = 7;

const int Y_BACKWARD_RELAY_1 = 8;
const int Y_BACKWARD_RELAY_2 = 9;

// Z axis — corrected
const int Z_DOWN_RELAY_1 = 10;
const int Z_DOWN_RELAY_2 = 11;

const int Z_UP_RELAY_1 = 12;
const int Z_UP_RELAY_2 = 13;

const unsigned long GAME_ON_PULSE_MS = 300;
const unsigned long MOVE_TIME_MS = 2000;
const unsigned long DEAD_TIME_MS = 300;

const int relayPins[] = {
  GAME_ON_RELAY_PIN,

  X_RIGHT_RELAY_1,
  X_RIGHT_RELAY_2,
  X_LEFT_RELAY_1,
  X_LEFT_RELAY_2,

  Y_FORWARD_RELAY_1,
  Y_FORWARD_RELAY_2,
  Y_BACKWARD_RELAY_1,
  Y_BACKWARD_RELAY_2,

  Z_DOWN_RELAY_1,
  Z_DOWN_RELAY_2,
  Z_UP_RELAY_1,
  Z_UP_RELAY_2
};

const int NUM_RELAYS = sizeof(relayPins) / sizeof(relayPins[0]);

void allRelaysOff() {
  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(relayPins[i], RELAY_OFF);
  }
}

void safeDelayBeforeMove() {
  allRelaysOff();
  delay(DEAD_TIME_MS);
}

void triggerGameOn(unsigned long durationMs) {
  Serial.println("GAME ON / COIN: ON");

  allRelaysOff();
  delay(DEAD_TIME_MS);

  digitalWrite(GAME_ON_RELAY_PIN, RELAY_ON);
  delay(durationMs);

  digitalWrite(GAME_ON_RELAY_PIN, RELAY_OFF);

  Serial.println("GAME ON / COIN: OFF");
}

void moveRight(unsigned long durationMs) {
  Serial.println("X RIGHT: ON");
  safeDelayBeforeMove();

  digitalWrite(X_RIGHT_RELAY_1, RELAY_ON);
  digitalWrite(X_RIGHT_RELAY_2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  Serial.println("X RIGHT: OFF");
}

void moveLeft(unsigned long durationMs) {
  Serial.println("X LEFT: ON");
  safeDelayBeforeMove();

  digitalWrite(X_LEFT_RELAY_1, RELAY_ON);
  digitalWrite(X_LEFT_RELAY_2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  Serial.println("X LEFT: OFF");
}

void moveForward(unsigned long durationMs) {
  Serial.println("Y FORWARD: ON");
  safeDelayBeforeMove();

  digitalWrite(Y_FORWARD_RELAY_1, RELAY_ON);
  digitalWrite(Y_FORWARD_RELAY_2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  Serial.println("Y FORWARD: OFF");
}

void moveBackward(unsigned long durationMs) {
  Serial.println("Y BACKWARD: ON");
  safeDelayBeforeMove();

  digitalWrite(Y_BACKWARD_RELAY_1, RELAY_ON);
  digitalWrite(Y_BACKWARD_RELAY_2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  Serial.println("Y BACKWARD: OFF");
}

void moveUp(unsigned long durationMs) {
  Serial.println("Z UP: ON");
  safeDelayBeforeMove();

  digitalWrite(Z_UP_RELAY_1, RELAY_ON);
  digitalWrite(Z_UP_RELAY_2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  Serial.println("Z UP: OFF");
}

void moveDown(unsigned long durationMs) {
  Serial.println("Z DOWN: ON");
  safeDelayBeforeMove();

  digitalWrite(Z_DOWN_RELAY_1, RELAY_ON);
  digitalWrite(Z_DOWN_RELAY_2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  Serial.println("Z DOWN: OFF");
}

void setup() {
  Serial.begin(9600);

  // Active LOW relay modules can flicker during startup.
  // Set outputs HIGH before pinMode OUTPUT.
  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(relayPins[i], RELAY_OFF);
    pinMode(relayPins[i], OUTPUT);
  }

  allRelaysOff();

  Serial.println("Claw-Pilot Game ON + X/Y/Z relay test starting...");
  delay(1000);

  Serial.println("Step 0: Trigger GAME ON / COIN");
  triggerGameOn(GAME_ON_PULSE_MS);

  // Wait for the toy's timer/music/control system to become active
  delay(1500);

  Serial.println("Test 1: X RIGHT for 2 seconds");
  moveRight(MOVE_TIME_MS);

  delay(1000);

  Serial.println("Test 2: X LEFT for 2 seconds");
  moveLeft(MOVE_TIME_MS);

  delay(1000);

  Serial.println("Test 3: Y FORWARD for 2 seconds");
  moveForward(MOVE_TIME_MS);

  delay(1000);

  Serial.println("Test 4: Y BACKWARD for 2 seconds");
  moveBackward(MOVE_TIME_MS);

  delay(1000);

  Serial.println("Test 5: Z UP for 2 seconds");
  moveUp(MOVE_TIME_MS);

  delay(1000);

  Serial.println("Test 6: Z DOWN for 2 seconds");
  moveDown(MOVE_TIME_MS);

  Serial.println("Test complete. All relays OFF.");
  allRelaysOff();
}

void loop() {
  // Test runs once on startup/reset.
}
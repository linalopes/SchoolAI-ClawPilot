// Claw-Pilot — Temporal Calibration Sketch
// Arduino Uno R3
//
// Purpose:
// Test movement duration per axis using Serial Monitor.
// Use this to estimate travel time for X, Y, and Z axes.
//
// Home convention:
// X = 0: left
// Y = 0: front / prize chute side
// Z = 0: claw fully up
//
// Relay module type: ACTIVE LOW
//
// GAME ON / COIN:
// GAME_ON = pin A5
//
// X axis:
// RIGHT = pins 2, 3
// LEFT  = pins 4, 5
//
// Y axis:
// FORWARD  = pins 6, 7
// BACKWARD = pins 8, 9
//
// Z axis:
// DOWN = pins 10, 11
// UP   = pins 12, 13

const int RELAY_ON  = LOW;   // active LOW module
const int RELAY_OFF = HIGH;

// Game ON / coin relay pin
const int GAME_ON_RELAY_PIN = A5;

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

// Z axis
const int Z_DOWN_RELAY_1 = 10;
const int Z_DOWN_RELAY_2 = 11;

const int Z_UP_RELAY_1 = 12;
const int Z_UP_RELAY_2 = 13;

// Timing
const unsigned long DEFAULT_MOVE_MS = 500;
const unsigned long GAME_ON_PULSE_MS = 300;
const unsigned long DEAD_TIME_MS = 150;

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

String inputLine = "";

void allRelaysOff() {
  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(relayPins[i], RELAY_OFF);
  }
}

void prepareMove() {
  allRelaysOff();
  delay(DEAD_TIME_MS);
}

void activatePair(int relay1, int relay2, unsigned long durationMs, const char* label) {
  Serial.print(label);
  Serial.print(" ON for ");
  Serial.print(durationMs);
  Serial.println(" ms");

  prepareMove();

  digitalWrite(relay1, RELAY_ON);
  digitalWrite(relay2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();

  Serial.print(label);
  Serial.println(" OFF");
}

void triggerGameOn() {
  Serial.print("GAME ON / COIN pulse for ");
  Serial.print(GAME_ON_PULSE_MS);
  Serial.println(" ms");

  prepareMove();

  digitalWrite(GAME_ON_RELAY_PIN, RELAY_ON);
  delay(GAME_ON_PULSE_MS);
  digitalWrite(GAME_ON_RELAY_PIN, RELAY_OFF);

  Serial.println("GAME ON / COIN OFF");
}

void moveRight(unsigned long durationMs) {
  activatePair(X_RIGHT_RELAY_1, X_RIGHT_RELAY_2, durationMs, "X RIGHT");
}

void moveLeft(unsigned long durationMs) {
  activatePair(X_LEFT_RELAY_1, X_LEFT_RELAY_2, durationMs, "X LEFT");
}

void moveForward(unsigned long durationMs) {
  activatePair(Y_FORWARD_RELAY_1, Y_FORWARD_RELAY_2, durationMs, "Y FORWARD");
}

void moveBackward(unsigned long durationMs) {
  activatePair(Y_BACKWARD_RELAY_1, Y_BACKWARD_RELAY_2, durationMs, "Y BACKWARD");
}

void moveDown(unsigned long durationMs) {
  activatePair(Z_DOWN_RELAY_1, Z_DOWN_RELAY_2, durationMs, "Z DOWN");
}

void moveUp(unsigned long durationMs) {
  activatePair(Z_UP_RELAY_1, Z_UP_RELAY_2, durationMs, "Z UP");
}

void printHelp() {
  Serial.println();
  Serial.println("Claw-Pilot Temporal Calibration");
  Serial.println("--------------------------------");
  Serial.println("Commands:");
  Serial.println("  game");
  Serial.println("  right <ms>");
  Serial.println("  left <ms>");
  Serial.println("  forward <ms>");
  Serial.println("  backward <ms>");
  Serial.println("  down <ms>");
  Serial.println("  up <ms>");
  Serial.println("  stop");
  Serial.println("  help");
  Serial.println();
  Serial.println("Examples:");
  Serial.println("  game");
  Serial.println("  right 1000");
  Serial.println("  left 500");
  Serial.println("  down 200");
  Serial.println();
  Serial.println("Home convention:");
  Serial.println("  X=0 left, Y=0 front/prize chute side, Z=0 claw fully up");
  Serial.println();
}

unsigned long parseDuration(String command, String keyword) {
  command.trim();

  int spaceIndex = command.indexOf(' ');
  if (spaceIndex == -1) {
    return DEFAULT_MOVE_MS;
  }

  String value = command.substring(spaceIndex + 1);
  value.trim();

  unsigned long duration = value.toInt();

  if (duration == 0) {
    Serial.println("Invalid or missing duration. Using default 500 ms.");
    return DEFAULT_MOVE_MS;
  }

  return duration;
}

void handleCommand(String command) {
  command.trim();
  command.toLowerCase();

  if (command.length() == 0) {
    return;
  }

  if (command == "help") {
    printHelp();
    return;
  }

  if (command == "stop") {
    allRelaysOff();
    Serial.println("All relays OFF");
    return;
  }

  if (command == "game") {
    triggerGameOn();
    return;
  }

  if (command.startsWith("right")) {
    moveRight(parseDuration(command, "right"));
    return;
  }

  if (command.startsWith("left")) {
    moveLeft(parseDuration(command, "left"));
    return;
  }

  if (command.startsWith("forward")) {
    moveForward(parseDuration(command, "forward"));
    return;
  }

  if (command.startsWith("backward")) {
    moveBackward(parseDuration(command, "backward"));
    return;
  }

  if (command.startsWith("down")) {
    moveDown(parseDuration(command, "down"));
    return;
  }

  if (command.startsWith("up")) {
    moveUp(parseDuration(command, "up"));
    return;
  }

  Serial.print("Unknown command: ");
  Serial.println(command);
  Serial.println("Type 'help' for command list.");
}

void setup() {
  Serial.begin(9600);

  // Active LOW relay modules may flicker during startup.
  // Set pins HIGH before declaring them OUTPUT.
  for (int i = 0; i < NUM_RELAYS; i++) {
    digitalWrite(relayPins[i], RELAY_OFF);
    pinMode(relayPins[i], OUTPUT);
  }

  allRelaysOff();

  delay(1000);

  printHelp();
  Serial.println("Ready.");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n' || c == '\r') {
      if (inputLine.length() > 0) {
        handleCommand(inputLine);
        inputLine = "";
      }
    } else {
      inputLine += c;
    }
  }
}
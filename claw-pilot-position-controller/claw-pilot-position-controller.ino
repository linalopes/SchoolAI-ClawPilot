// Claw-Pilot — Percentage Position Controller
// Arduino Uno R3
//
// This sketch uses temporal calibration to move the claw by estimated position.
//
// IMPORTANT:
// There are no limit switches or encoders.
// Position is estimated by time only.
// Always place the claw manually at HOME before sending "home".
//
// Home convention:
// X = 0%   -> left
// Y = 0%   -> front / prize chute side
// Z = 0%   -> claw fully up
//
// X = 100% -> right
// Y = 100% -> back
// Z = 100% -> claw fully down
//
// Relay module type: ACTIVE LOW
//
// GAME ON / COIN:
// GAME_ON = A5
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

const int RELAY_ON  = LOW;   // active LOW relay module
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

// Temporal calibration values
const unsigned long X_FULL_TRAVEL_MS = 10800;
const unsigned long Y_FULL_TRAVEL_MS = 5000;
const unsigned long Z_FULL_TRAVEL_MS = 2500;

// Timing
const unsigned long GAME_ON_PULSE_MS = 300;
const unsigned long DEAD_TIME_MS = 150;

// Estimated current position, in percent
float currentX = 0.0;
float currentY = 0.0;
float currentZ = 0.0;

bool isHomed = false;

String inputLine = "";

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

float clampPercent(float value) {
  if (value < 0.0) return 0.0;
  if (value > 100.0) return 100.0;
  return value;
}

unsigned long percentDeltaToMs(float deltaPercent, unsigned long fullTravelMs) {
  deltaPercent = abs(deltaPercent);
  return (unsigned long)((deltaPercent / 100.0) * fullTravelMs);
}

void prepareMove() {
  allRelaysOff();
  delay(DEAD_TIME_MS);
}

void activatePair(int relay1, int relay2, unsigned long durationMs, const char* label) {
  if (durationMs == 0) {
    Serial.print(label);
    Serial.println(": no movement needed");
    return;
  }

  Serial.print(label);
  Serial.print(" for ");
  Serial.print(durationMs);
  Serial.println(" ms");

  prepareMove();

  digitalWrite(relay1, RELAY_ON);
  digitalWrite(relay2, RELAY_ON);

  delay(durationMs);

  allRelaysOff();
  delay(DEAD_TIME_MS);

  Serial.print(label);
  Serial.println(" done");
}

void triggerGameOn() {
  Serial.print("GAME ON / COIN pulse for ");
  Serial.print(GAME_ON_PULSE_MS);
  Serial.println(" ms");

  prepareMove();

  digitalWrite(GAME_ON_RELAY_PIN, RELAY_ON);
  delay(GAME_ON_PULSE_MS);
  digitalWrite(GAME_ON_RELAY_PIN, RELAY_OFF);

  Serial.println("GAME ON / COIN done");
}

void setHome() {
  currentX = 0.0;
  currentY = 0.0;
  currentZ = 0.0;
  isHomed = true;

  Serial.println("HOME SET");
  Serial.println("Current estimated position: X=0%, Y=0%, Z=0%");
}

void printStatus() {
  Serial.println();
  Serial.println("Estimated position:");
  Serial.print("  X = ");
  Serial.print(currentX);
  Serial.println("%");

  Serial.print("  Y = ");
  Serial.print(currentY);
  Serial.println("%");

  Serial.print("  Z = ");
  Serial.print(currentZ);
  Serial.println("%");

  Serial.print("  Homed = ");
  Serial.println(isHomed ? "YES" : "NO");
  Serial.println();
}

void moveXTo(float targetX) {
  targetX = clampPercent(targetX);

  if (!isHomed) {
    Serial.println("WARNING: Position is not homed. Manually place claw at HOME and send 'home'.");
  }

  float delta = targetX - currentX;
  unsigned long durationMs = percentDeltaToMs(delta, X_FULL_TRAVEL_MS);

  if (delta > 0) {
    activatePair(X_RIGHT_RELAY_1, X_RIGHT_RELAY_2, durationMs, "X RIGHT");
  } else if (delta < 0) {
    activatePair(X_LEFT_RELAY_1, X_LEFT_RELAY_2, durationMs, "X LEFT");
  } else {
    Serial.println("X already at target");
  }

  currentX = targetX;
  printStatus();
}

void moveYTo(float targetY) {
  targetY = clampPercent(targetY);

  if (!isHomed) {
    Serial.println("WARNING: Position is not homed. Manually place claw at HOME and send 'home'.");
  }

  float delta = targetY - currentY;
  unsigned long durationMs = percentDeltaToMs(delta, Y_FULL_TRAVEL_MS);

  // Convention:
  // Y = 0% front
  // Y = 100% back
  //
  // If your physical direction is inverted, swap FORWARD/BACKWARD here.
  if (delta > 0) {
    activatePair(Y_BACKWARD_RELAY_1, Y_BACKWARD_RELAY_2, durationMs, "Y BACKWARD");
  } else if (delta < 0) {
    activatePair(Y_FORWARD_RELAY_1, Y_FORWARD_RELAY_2, durationMs, "Y FORWARD");
  } else {
    Serial.println("Y already at target");
  }

  currentY = targetY;
  printStatus();
}

void moveZTo(float targetZ) {
  targetZ = clampPercent(targetZ);

  if (!isHomed) {
    Serial.println("WARNING: Position is not homed. Manually place claw at HOME and send 'home'.");
  }

  float delta = targetZ - currentZ;
  unsigned long durationMs = percentDeltaToMs(delta, Z_FULL_TRAVEL_MS);

  // Convention:
  // Z = 0% up
  // Z = 100% down
  if (delta > 0) {
    activatePair(Z_DOWN_RELAY_1, Z_DOWN_RELAY_2, durationMs, "Z DOWN");
  } else if (delta < 0) {
    activatePair(Z_UP_RELAY_1, Z_UP_RELAY_2, durationMs, "Z UP");
  } else {
    Serial.println("Z already at target");
  }

  currentZ = targetZ;
  printStatus();
}

void goToPosition(float targetX, float targetY, float targetZ) {
  targetX = clampPercent(targetX);
  targetY = clampPercent(targetY);
  targetZ = clampPercent(targetZ);

  Serial.println();
  Serial.println("GOTO command received");
  Serial.print("Target: X=");
  Serial.print(targetX);
  Serial.print("%, Y=");
  Serial.print(targetY);
  Serial.print("%, Z=");
  Serial.print(targetZ);
  Serial.println("%");

  // Safer order:
  // 1. Keep claw up before horizontal movement
  // 2. Move X
  // 3. Move Y
  // 4. Move Z last
  //
  // If targetZ is lower than currentZ, move X/Y first, then lower.
  // If currentZ is already down and we need horizontal movement, lift first.
  if (currentZ > 0 && (targetX != currentX || targetY != currentY)) {
    Serial.println("Lifting Z to 0% before horizontal movement");
    moveZTo(0);
  }

  moveXTo(targetX);
  moveYTo(targetY);
  moveZTo(targetZ);

  Serial.println("GOTO complete");
}

float getArgumentAsFloat(String command) {
  command.trim();
  int spaceIndex = command.indexOf(' ');

  if (spaceIndex == -1) {
    return -1;
  }

  String value = command.substring(spaceIndex + 1);
  value.trim();

  return value.toFloat();
}

void parseGoto(String command) {
  command.trim();

  int firstSpace = command.indexOf(' ');
  if (firstSpace == -1) {
    Serial.println("Invalid goto command. Use: goto <x> <y> <z>");
    return;
  }

  String rest = command.substring(firstSpace + 1);
  rest.trim();

  int secondSpace = rest.indexOf(' ');
  if (secondSpace == -1) {
    Serial.println("Invalid goto command. Use: goto <x> <y> <z>");
    return;
  }

  String xStr = rest.substring(0, secondSpace);
  String rest2 = rest.substring(secondSpace + 1);
  rest2.trim();

  int thirdSpace = rest2.indexOf(' ');
  if (thirdSpace == -1) {
    Serial.println("Invalid goto command. Use: goto <x> <y> <z>");
    return;
  }

  String yStr = rest2.substring(0, thirdSpace);
  String zStr = rest2.substring(thirdSpace + 1);
  zStr.trim();

  float x = xStr.toFloat();
  float y = yStr.toFloat();
  float z = zStr.toFloat();

  goToPosition(x, y, z);
}

void printHelp() {
  Serial.println();
  Serial.println("Claw-Pilot Percentage Position Controller");
  Serial.println("-----------------------------------------");
  Serial.println("Commands:");
  Serial.println("  game");
  Serial.println("  home");
  Serial.println("  x <percent>");
  Serial.println("  y <percent>");
  Serial.println("  z <percent>");
  Serial.println("  goto <x_percent> <y_percent> <z_percent>");
  Serial.println("  status");
  Serial.println("  stop");
  Serial.println("  help");
  Serial.println();
  Serial.println("Examples:");
  Serial.println("  game");
  Serial.println("  home");
  Serial.println("  x 50");
  Serial.println("  y 75");
  Serial.println("  z 100");
  Serial.println("  goto 50 80 0");
  Serial.println();
  Serial.println("Calibration values:");
  Serial.print("  X full travel: ");
  Serial.print(X_FULL_TRAVEL_MS);
  Serial.println(" ms");

  Serial.print("  Y full travel: ");
  Serial.print(Y_FULL_TRAVEL_MS);
  Serial.println(" ms");

  Serial.print("  Z full travel: ");
  Serial.print(Z_FULL_TRAVEL_MS);
  Serial.println(" ms");
  Serial.println();
  Serial.println("Home convention:");
  Serial.println("  X=0 left, Y=0 front/prize chute side, Z=0 claw fully up");
  Serial.println();
}

void handleCommand(String command) {
  command.trim();
  command.toLowerCase();

  if (command.length() == 0) return;

  if (command == "help") {
    printHelp();
    return;
  }

  if (command == "stop") {
    allRelaysOff();
    Serial.println("All relays OFF");
    return;
  }

  if (command == "status") {
    printStatus();
    return;
  }

  if (command == "game") {
    triggerGameOn();
    return;
  }

  if (command == "home") {
    setHome();
    return;
  }

  if (command.startsWith("goto")) {
    parseGoto(command);
    return;
  }

  if (command.startsWith("x")) {
    float target = getArgumentAsFloat(command);
    moveXTo(target);
    return;
  }

  if (command.startsWith("y")) {
    float target = getArgumentAsFloat(command);
    moveYTo(target);
    return;
  }

  if (command.startsWith("z")) {
    float target = getArgumentAsFloat(command);
    moveZTo(target);
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
// Test code for two relay modules on Arduino UNO
// Replace the placeholder pins with your actual Arduino digital pins.

const int X_FRONT_RELAY_1_PIN = 2;
const int X_FRONT_RELAY_2_PIN = 3;
const int X_BACK_RELAY_1_PIN = 4;
const int X_BACK_RELAY_2_PIN = 5;
const int Y_RIGHT_RELAY_1_PIN = 6;
const int Y_RIGHT_RELAY_2_PIN = 7;
const int Y_LEFT_RELAY_1_PIN = 8;
const int Y_LEFT_RELAY_2_PIN = 9;
const int Z_UP_RELAY_1_PIN = 10;
const int Z_UP_RELAY_2_PIN = 11;
const int Z_DOWN_RELAY_1_PIN = 12;
const int Z_DOWN_RELAY_2_PIN = 13;
const int GAME_ON_RELAY_1_PIN = A5;
const int GAME_ON_RELAY_2_PIN = A4;

// Most relay modules are active LOW.
// If your relay is active HIGH, invert these two values.
const int RELAY_ON  = LOW;
const int RELAY_OFF = HIGH;

const unsigned long STEP_DELAY = 3000; // milliseconds

void setup() {
  Serial.begin(9600);

  pinMode(X_FRONT_RELAY_1_PIN, OUTPUT);
  pinMode(X_FRONT_RELAY_2_PIN, OUTPUT);
  pinMode(X_BACK_RELAY_1_PIN, OUTPUT);
  pinMode(X_BACK_RELAY_2_PIN, OUTPUT);
  pinMode(Y_RIGHT_RELAY_1_PIN, OUTPUT);
  pinMode(Y_RIGHT_RELAY_2_PIN, OUTPUT);
  pinMode(Y_LEFT_RELAY_1_PIN, OUTPUT);
  pinMode(Y_LEFT_RELAY_2_PIN, OUTPUT);
  pinMode(Z_UP_RELAY_1_PIN, OUTPUT);
  pinMode(Z_UP_RELAY_2_PIN, OUTPUT);
  pinMode(Z_DOWN_RELAY_1_PIN, OUTPUT);
  pinMode(Z_DOWN_RELAY_2_PIN, OUTPUT);
  pinMode(GAME_ON_RELAY_1_PIN, OUTPUT);
  pinMode(GAME_ON_RELAY_2_PIN, OUTPUT);

  // Start with both relays OFF
  digitalWrite2(X_FRONT_RELAY_1_PIN, X_FRONT_RELAY_2_PIN, RELAY_OFF);
  digitalWrite2(X_BACK_RELAY_1_PIN, X_BACK_RELAY_2_PIN, RELAY_OFF);
  digitalWrite2(Y_RIGHT_RELAY_1_PIN, Y_RIGHT_RELAY_2_PIN, RELAY_OFF);
  digitalWrite2(Y_LEFT_RELAY_1_PIN, Y_LEFT_RELAY_2_PIN, RELAY_OFF);
  digitalWrite2(Z_UP_RELAY_1_PIN, Z_UP_RELAY_2_PIN, RELAY_OFF);
  digitalWrite2(Z_DOWN_RELAY_1_PIN, Z_DOWN_RELAY_2_PIN, RELAY_OFF);
  digitalWrite2(GAME_ON_RELAY_1_PIN, GAME_ON_RELAY_2_PIN, RELAY_OFF);
  delay(1000);
  digitalWrite2(GAME_ON_RELAY_1_PIN, GAME_ON_RELAY_2_PIN, RELAY_ON);
  delay(1000);
  digitalWrite2(GAME_ON_RELAY_1_PIN, GAME_ON_RELAY_2_PIN, RELAY_OFF);

  Serial.println("Relay test started");
}

void digitalWrite2(int pin1, int pin2, int onoff)
{
  digitalWrite(pin1, onoff);
  digitalWrite(pin2, onoff);
}

void loop() {
  Serial.println("Both ON");
  digitalWrite2(Z_UP_RELAY_1_PIN, Z_UP_RELAY_2_PIN, RELAY_ON);
  delay(STEP_DELAY);

  Serial.println("Both OFF");
  digitalWrite2(Z_UP_RELAY_1_PIN, Z_UP_RELAY_2_PIN, RELAY_OFF);
  delay(STEP_DELAY);


 Serial.println("Both ON");
  digitalWrite2(Z_DOWN_RELAY_1_PIN, Z_DOWN_RELAY_2_PIN, RELAY_ON);
  delay(STEP_DELAY);

  Serial.println("Both OFF");
  digitalWrite2(Z_DOWN_RELAY_1_PIN, Z_DOWN_RELAY_2_PIN, RELAY_OFF);
  delay(STEP_DELAY);

}
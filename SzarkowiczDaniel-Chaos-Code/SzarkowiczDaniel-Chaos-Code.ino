#include <Adafruit_NeoPixel.h>

#define PIXELS_PIN 12
#define PIXELS_COUNT 4

#define LED_CHAIN 9

#define BUZZER_PIN 1 
#define LIGHT_PIN A10
#define TILT_PIN 3

int lightMin = 1023;
int lightMax = 0;
bool tilted = false;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(
  PIXELS_COUNT,
  PIXELS_PIN,
  NEO_GRB + NEO_KHZ800
);

void calibrateLightSensor() {
  // Calibrate minimum and maximum of current environment.
  unsigned long start = millis();
  while(millis() - start < 5000) {
    int reading = analogRead(LIGHT_PIN);
    if (reading < lightMin) lightMin = reading;
    if (reading > lightMax) lightMax = reading;

    delay(10);
  }
}

void setRandomSeed() {
  long seed = 0;
  // Subject to change once second sensor is obtained.
  seed ^= analogRead(LIGHT_PIN);
  // State of tilt pin contributes to randomness.
  seed ^= digitalRead(TILT_PIN) << 8;
  seed ^= millis();
  randomSeed(seed);
}

void setup() {
  // Setup pins 
  Serial.begin(9600);
  pinMode(LED_CHAIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TILT_PIN, INPUT_PULLUP);

  strip.begin();
  strip.setBrightness(50);
  strip.show();

  calibrateLightSensor();
}

void loop() {
  // Code using random() to power actuators.
  setRandomSeed();
  // Set random brightness, bare minimum is that it's still pretty visible.
  int brightness = random(40, 121);
  strip.setBrightness(brightness);
  int pixelIndex = random(PIXELS_COUNT);
  int r = random(256);
  int g = random(256);
  int b = random(256);
  strip.setPixelColor(pixelIndex, strip.Color(r, g, b));
  // Also assign random light values to the normal LED chain.
  analogWrite(LED_CHAIN, random(40, 180));

  strip.show();
  delay(200);

  tone(BUZZER_PIN, random(2500));
  delay(100);
  noTone(BUZZER_PIN);

}

#include <Adafruit_NeoPixel.h>

// Defining pins by how they are connected to the FLORA

#define PIXELS_PIN 12
#define PIXELS_COUNT 4

#define ONBOARD_PIXEL_PIN 8
#define ONBOARD_PIXEL_COUNT 1

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

Adafruit_NeoPixel onboardPixel = Adafruit_NeoPixel(
  ONBOARD_PIXEL_COUNT,
  ONBOARD_PIXEL_PIN,
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

  // Initialize the NeoPixels
  strip.begin();
  strip.setBrightness(50);
  strip.show();

  onboardPixel.begin();
  onboardPixel.setBrightness(50);
  onboardPixel.show();

  calibrateLightSensor();
}

void loop() {
  // Code using random() to power actuators.
  int light = analogRead(LIGHT_PIN);
  int tilt = digitalRead(TILT_PIN);

  // Additional chaos component influenced by light sensor readings.
  int chaos = map(light, lightMin, lightMax, 1, 10);
  chaos = constrain(chaos, 1, 10);

  Serial.print(light);
  Serial.print(" ");
  Serial.println(tilt);
  
  setRandomSeed();
  // Set random brightness, bare minimum is that it's still pretty visible.
  int brightness = random(40, 40 + chaos * 20);
  strip.setBrightness(brightness);
  // Gives a slight fading effect to onboard and external NeoPixels. More noticeable when delays are shorter which equals a darker environment.
  for (int i = 0; i < PIXELS_COUNT; i++) {
    uint32_t c = strip.getPixelColor(i);

    int r = (c >> 16) & 255;
    int g = (c >> 8) & 255;
    int b = c & 255;

    strip.setPixelColor(i, strip.Color(r * 0.8, g * 0.8, b * 0.8));
  }
  
  uint32_t oc = onboardPixel.getPixelColor(0);

  int orr = (oc >> 16) & 255;
  int og = (oc >> 8) & 255;
  int ob = oc & 255;

  onboardPixel.setPixelColor(0, onboardPixel.Color(orr * 0.8, og * 0.8, ob * 0.8));

  int pixelIndex = random(PIXELS_COUNT);
  int r = random(256);
  int g = random(256);
  int b = random(256);
  strip.setPixelColor(pixelIndex, strip.Color(r, g, b));
  onboardPixel.setPixelColor(0, onboardPixel.Color(r, g, b));

  // Additional random light color if dark
  for (int i = 0; i < chaos / 3; i++) {
    strip.setPixelColor(random(PIXELS_COUNT), strip.Color(random(256), random(256), random(256)));
    onboardPixel.setPixelColor(0, onboardPixel.Color(random(256), random(256), random(256)));
  }

  // Also assign random light values to the normal LED chain.
  analogWrite(LED_CHAIN, random(0, 25 * chaos));

  strip.show();
  onboardPixel.show();
  delay(random(20, 1 + chaos * 20));

  // Buzzer tone incorporates both the random call and the chaos factor determined by light.
  tone(BUZZER_PIN, random(500 * chaos));
  delay(random(20, 1 + chaos * 20));
  noTone(BUZZER_PIN);

}

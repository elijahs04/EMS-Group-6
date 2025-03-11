#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display settings
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ADXL335 Analog Pins
#define X_PIN A0
#define Y_PIN A1
#define Z_PIN A2

// ADXL335 calibration values
const float ZERO_G = 338.0; // Center point at rest
const float SCALE = 67.6;   // ADC units per g (676/10)

// Track min/max values for g-forces
float xMin = 3.0, xMax = -3.0;
float yMin = 3.0, yMax = -3.0;
float zMin = 3.0, zMax = -3.0;

// Function to round to the nearest 0.2
float roundToNearest(float value, float step) {
    return round(value / step) * step;
}

void setup() {
    Serial.begin(9600);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 5);
    display.println("Starting...");
    display.display();
    delay(1000);
}

void loop() {
    // Read raw values
    int xRaw = analogRead(X_PIN);
    int yRaw = analogRead(Y_PIN);
    int zRaw = analogRead(Z_PIN);

    // Convert to g-forces
    float xG = (xRaw - ZERO_G) / SCALE;
    float yG = (yRaw - ZERO_G) / SCALE;
    float zG = (zRaw - ZERO_G) / SCALE;

    // Update min/max values
    if (xG < xMin) xMin = xG;
    if (xG > xMax) xMax = xG;

    if (yG < yMin) yMin = yG;
    if (yG > yMax) yMax = yG;

    if (zG < zMin) zMin = zG;
    if (zG > zMax) zMax = zG;

    // Round values for display
    float xG_display = roundToNearest(xG, 0.1);
    float yG_display = roundToNearest(yG, 0.1);
    float zG_display = roundToNearest(zG, 0.1);

    // Display on OLED
    display.clearDisplay();

    // --- Real-time XYZ Readings ---
    display.setCursor(0, 0);
    display.print("X: ");
    display.print(xG, 1); // One decimal place after rounding
    display.print("g");
    display.setCursor(80, 0);
    display.print(yRaw);

    display.setCursor(0, 10);
    display.print("Y: ");
    display.print(yG, 1);
    display.print("g");
    display.setCursor(80, 10);
    display.print(yRaw);

    display.setCursor(0, 20);
    display.print("Z: ");
    display.print(zG, 1);
    display.print("g");
    display.setCursor(80, 20);
    display.print(zRaw);

    // --- Min/Max Values ---
    display.setCursor(0, 35);
    display.print("X Min/Max: ");
    display.print(xMin, 2);
    display.print("/");
    display.print(xMax, 2);

    display.setCursor(0, 45);
    display.print("Y Min/Max: ");
    display.print(yMin, 2);
    display.print("/");
    display.print(yMax, 2);

    display.setCursor(0, 55);
    display.print("Z Min/Max: ");
    display.print(zMin, 2);
    display.print("/");
    display.print(zMax, 2);

    display.display();

    // Serial Output for Debugging
    Serial.print("X: "); Serial.print(xG_display, 2);
    Serial.print("g | Y: "); Serial.print(yG_display, 2);
    Serial.print("g | Z: "); Serial.print(zG_display, 2);
    Serial.print(" || X Min/Max: "); Serial.print(xMin, 2); Serial.print("/"); Serial.print(xMax, 2);
    Serial.print(" | Y Min/Max: "); Serial.print(yMin, 2); Serial.print("/"); Serial.print(yMax, 2);
    Serial.print(" | Z Min/Max: "); Serial.print(zMin, 2); Serial.print("/"); Serial.println(zMax, 2);
}
Arduino Code
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// OLED display dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Create OLED display object
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT Sensor Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Gas Sensor Pin
#define SENSOR_PIN A0

// Buzzer and LED Pins
#define BUZZER_PIN 7
#define LED_PIN 6

int gasLevel = 0;         // Gas level variable
String airQuality = "";   // Air quality status

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 initialization failed!"));
    while (true); // Halt execution if OLED fails
  }
  Serial.println(F("SSD1306 initialized successfully!"));
  display.clearDisplay();

  // Initialize DHT sensor
  dht.begin();

  // Initialize Buzzer and LED
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // Display startup message
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("By PSR :D ");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop() {
  // Read Temperature and Humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return; // Skip the rest of the loop if sensor fails
  }

  // Read Air Quality (Gas Level)
  gasLevel = analogRead(SENSOR_PIN);
  if (gasLevel < 151) {
    airQuality = "GOOD";
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  } else if (gasLevel <= 200) {
    airQuality = "POOR";
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  } else if (gasLevel <= 300) {
    airQuality = "VERY BAD";
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  } else if (gasLevel <= 500) {
    airQuality = "TOXIC";
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    airQuality = "EXTREME";
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  }

  // Update OLED Display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("Air Quality Monitor:");

  display.setCursor(0, 15);
  display.print("Level: ");
  display.print(gasLevel);

  display.setCursor(0, 30);
  display.print("Status: ");
  display.println(airQuality);

  display.setCursor(0, 45);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.print(" C");

  display.setCursor(0, 55);
  display.print("Hum : ");
  display.print(humidity, 1);
  display.print(" %");

  display.display();

  delay(1000); // Delay for 1 second }

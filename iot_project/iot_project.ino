#define BLYNK_TEMPLATE_ID "TMPL3wJgpnzjw"
#define BLYNK_TEMPLATE_NAME "smart water heater"
#define BLYNK_AUTH_TOKEN "dSVIf5Bj4DOpx4mvARmcrkd6KqZI1dGF" // Replace with your Blynk Auth Token

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <MAX6675.h>

// Blynk credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "_.Ram";       // Replace with your WiFi SSID
char pass[] = "Rengalaksham76"; // Replace with your WiFi password

// Pins for MAX6675
int thermoSO = 19;
int thermoCS = 5;
int thermoSCK = 18;
MAX6675 thermocouple(thermoSCK, thermoCS, thermoSO);

// Pin for PIR Sensor (not used in this version)
#define PIR_PIN 23

// Pin for Relay
#define RELAY_PIN 22

// Virtual Pins
#define VIRTUAL_PIN_TEMP V1
#define VIRTUAL_PIN_MANUAL V2  // Virtual pin for manual control

// Variables
bool heaterState = false;
const double TEMP_THRESHOLD = 50.0; // Temperature threshold in Celsius

// Blynk Write function for manual control
BLYNK_WRITE(VIRTUAL_PIN_MANUAL) {
  bool manualControl = param.asInt();
  if (manualControl) {
    digitalWrite(RELAY_PIN, HIGH); // Turn on the heater only if requested manually
    heaterState = true;
  }
}

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  // Initialize GPIO
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Ensure the relay is off initially

  // Allow MAX6675 to stabilize
  delay(500);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // Read Temperature
  double celsius = thermocouple.readCelsius();
  Serial.print("C = ");
  Serial.println(celsius);
  Blynk.virtualWrite(VIRTUAL_PIN_TEMP, celsius);

  // Check if temperature exceeds threshold
  if (celsius > TEMP_THRESHOLD) {
    Blynk.logEvent("temp_exceeded", String("Temperature exceeded: ") + celsius + "°C");
    digitalWrite(RELAY_PIN, LOW); // Turn off the heater automatically
    heaterState = false;
  }

  // Delay between readings
  delay(1000);
}

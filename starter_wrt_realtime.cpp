#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

const int relayPin = 8; // Relay control pin

void setup() {
  pinMode(relayPin, OUTPUT); // Set relay pin as output
  digitalWrite(relayPin, HIGH); // Ensure relay starts OFF (active LOW relays)
  Serial.begin(9600);

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Stop if RTC is not found
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, resetting time.");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // Set RTC to compile time
  }
}

void loop() {
  DateTime now = rtc.now(); // Get the current time

  // Extract hours and minutes
  int currentHour = now.hour();
  int currentMinute = now.minute();

  // Relay ON/OFF logic
  if ((currentHour >= 18 && currentHour < 22) || // Evening 6 PM to 10 PM
      (currentHour == 22 && currentMinute <= 30) || // 10 PM to 10:30 PM
      (currentHour == 5)) { // Morning 5 AM to 6 AM
    Serial.println("Relay ON");
    digitalWrite(relayPin, LOW); // Relay ON (adjust if active HIGH)
  } else {
    Serial.println("Relay OFF");
    digitalWrite(relayPin, HIGH); // Relay OFF (adjust if active HIGH)
  }

  // Debugging: Print the current time and relay status
  Serial.print("Current Time: ");
  Serial.print(currentHour);
  Serial.print(":");
  Serial.println(currentMinute);

  delay(1000); // Check the time every second
}

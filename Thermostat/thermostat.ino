#include "Arduino_LED_Matrix.h"
#include <string.h>
#include <DHT.h>
ArduinoLEDMatrix matrix;

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  matrix.begin();
  dht.begin();

  Serial.println("DHT11 starting...");
}

byte frame[8][12] = {
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },

  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
};

float temps[] = { 0,0,0,0,0,0,0,0,0,0,0,0 };

float baseline_temp = 80;

void loop() {

  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = (dht.readTemperature() * 9.0 / 5) + 32;

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read DHT11");
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" F");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  memmove(temps, temps + 1, 11 * sizeof(float));

  temps[11] = temperature;

  memset(frame, 0, sizeof(frame));

  for (int i = 0; i < 12; i++) {
    int num = (int)((baseline_temp - temps[i]));
    num = calculate_row(-1 * num);
    //Serial.println(num);
    frame[num][i] = 1;
  }

  matrix.renderBitmap(frame, 8, 12);
  delay(500);
}

int calculate_row(int num) {
    if (num == 0) {
        return 4;
    }

    if (num > 0) {
        if (num > 10) {
            return 0;
        } else if (num > 5) {
            return 1;
        } else if (num > 1) {
            return 2;
        }

        return 3;
    }

    if (num > -5) {
        return 6;
    }

    return 7;
}

#include <SFE_BMP180.h>
#include <Wire.h>

SFE_BMP180 pressure;
#define ALTITUDE 943.7 // Altitude of Electropeak Co. in meters
int Valve = 4;

void setup() {
  pinMode(Valve, OUTPUT);
  Serial.begin(9600);

  if (pressure.begin()) {
    Serial.println("BMP180 init success");
  } else {
    Serial.println("BMP180 init fail\n\n");
    Serial.println("Check connection");
    while (1); // Halt execution
  }
}

void loop() {
  // Valve control through Serial
  if (Serial.available() > 0) {
    char data = Serial.read();
    switch (data) {
      case 'F':
        digitalWrite(Valve, HIGH); // Open valve
        break;
      case 'O':
        digitalWrite(Valve, LOW);  // Close valve
        break;
      default:
        break;
    }
    Serial.println(data);
  }

  // Read temperature and pressure
  char status;
  double T, P, p0, a;

  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);
    status = pressure.getTemperature(T);
    if (status != 0) {
      Serial.print("temperature: ");
      Serial.print(T, 2);
      Serial.println(" deg C");

      status = pressure.startPressure(3);
      if (status != 0) {
        delay(status);
        status = pressure.getPressure(P, T);
        if (status != 0) {
          Serial.print("absolute pressure: ");
          Serial.print(P, 2);
          Serial.print(" hPa = ");
          Serial.print(P * 100, 2);
          Serial.print(" Pa = ");
          Serial.print(P * 0.000986923, 2);
          Serial.print(" atm = ");
          Serial.print(P * 0.750063755, 2);
          Serial.print(" mmHg = ");
          Serial.print(P * 0.750061683, 2);
          Serial.print(" torr = ");
          Serial.print(P * 0.014503774, 2);
          Serial.println(" psi");

          p0 = pressure.sealevel(P, ALTITUDE);
          Serial.print("relative (sea-level) pressure: ");
          Serial.print(p0, 2);
          Serial.println(" hPa");

          a = pressure.altitude(P, p0);
          Serial.print("your altitude: ");
          Serial.print(a, 0);
          Serial.println(" meters");
        } else {
          Serial.println("error retrieving pressure measurement");
        }
      } else {
        Serial.println("error starting pressure measurement");
      }
    } else {
      Serial.println("error retrieving temperature measurement");
    }
  } else {
    Serial.println("error starting temperature measurement");
  }

  Serial.println("==========================================================================");
  delay(1000); // Delay before next reading
}

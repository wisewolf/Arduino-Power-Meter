#include <Arduino.h>

/*
MAX471
*/

#define max471VT A0
#define max471AT A1

float Voltage = 0;
float Current = 0;
float Power = 0;
float Energy = 0;

float minVoltage = 0;
float maxVoltage = 0;

float minCurrent = 0;
float maxCurrent = 0;

float minPower = 0;
float maxPower = 0;

float minEnergy = 0;
float maxEnergy = 0;

float SumCurrent = 0;
float AvgCurrent = 0;
float CurrentHour = 0;

const long interval = 1000;           // interval at which to blink (milliseconds)
unsigned long previousMillis = 0;

long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

void showData(){
      Serial.print("Voltage = ");       // shows the Voltage measured
      Serial.print(Voltage);
      Serial.print(" V");
      Serial.print(" Min = ");
      Serial.print(minVoltage,3);
      Serial.print(" V");
      Serial.print(" Max = ");
      Serial.print(maxVoltage,3);
      Serial.println(" V");

      Serial.print("Current = ");       // shows the Current measured
      Serial.print(Current,5);          // 5 digits after decimal point
      Serial.print(" A");
      Serial.print(" Min = ");
      Serial.print(minCurrent,5);
      Serial.print(" A");
      Serial.print(" Max = ");
      Serial.print(maxCurrent,5);
      Serial.println(" A");

      Serial.print("Power   = ");         // shows the Power measured
      Serial.print(Power,5);            // 5 digits after decimal point
      Serial.print(" W");
      Serial.print(" Min = ");
      Serial.print(minPower,5);
      Serial.print(" W");
      Serial.print(" Max = ");
      Serial.print(maxPower,5);
      Serial.println(" W");

      Serial.print("Energy  = ");         // shows the Energy measured
      Serial.print(Energy,5);            // 5 digits after decimal point
      Serial.print(" Wh");
      Serial.print(" Min = ");
      Serial.print(minEnergy,5);
      Serial.print(" Wh");
      Serial.print(" Max = ");
      Serial.print(maxEnergy,5);
      Serial.println(" Wh");

      Serial.println("Current per hour = " + String(CurrentHour) + " Ah");

      Serial.println("====================================================");
}

void setup(){
    pinMode(max471VT, INPUT);
    pinMode(max471AT, INPUT);
    Serial.begin(115200);
    minVoltage = 9999;
    maxVoltage = 0;
    minCurrent = 9999;
    maxCurrent = 0;
    minPower = 9999;
    maxPower = 0 ;
    minEnergy = 9999;
    maxEnergy = 0;
}

void loop(){

    unsigned long milisec = millis();  // calculate time in milliseconds
    long time = milisec/1000; // convert milliseconds to seconds

    int RawVT = analogRead(max471VT);
    int RawAT = analogRead(max471AT);
    float Vcc = readVcc() / 1000.0;                 // convert mV to V

    Voltage = ((RawVT * Vcc) / 1023) * 5 ;    // scale the ADC
    maxVoltage = max(Voltage, maxVoltage);
    minVoltage = min(minVoltage, Voltage);

    Current = (RawAT * Vcc) / 1023;           // scale the ADC
    maxCurrent = max(maxCurrent, Current);
    if (Current > 0) {
      minCurrent = min(minCurrent, Current);
    }

    Power = Voltage * Current;
    maxPower = max(maxPower, Power);
    if (Power > 0) {
      minPower = min(minPower, Power);
    }


    Energy = (Power * time) / 3600;           // Watt-sec is again convert to Watt-Hr by dividing 1hr (3600sec)
    maxEnergy = max(maxEnergy, Energy);
    if (Energy > 0) {
      minEnergy = min(minEnergy, Energy);
    }

    // Energy = (Power * time) / (1000*3600); for reading in kWh

    SumCurrent = SumCurrent + Current;        // calculate total current
    AvgCurrent = SumCurrent / time;           // average current
    CurrentHour = (AvgCurrent * time) / 3600; // Ah

    if (milisec - previousMillis >= interval) {
      previousMillis = milisec;
      showData();
    }

}

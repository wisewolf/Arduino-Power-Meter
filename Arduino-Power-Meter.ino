#include <Arduino.h>

/*
MAX471
*/

#define max471VT A0
#define max471AT A1

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

void setup(){
    pinMode(max471VT, INPUT);
    pinMode(max471AT, INPUT);
    Serial.begin(115200);
}

void loop(){

    long milisec = millis();  // calculate time in milliseconds
    long time = milisec/1000; // convert milliseconds to seconds

    int RawVT = analogRead(max471VT);
    int RawAT = analogRead(max471AT);
    float Vcc = readVcc() / 1000.0;                 // convert mV to V

    float Voltage = ((RawVT * Vcc) / 1023) * 5 ;    // scale the ADC
    float Current = (RawAT * Vcc) / 1023;           // scale the ADC
    float Power = Voltage * Current;
    float Energy = (Power * time) / 3600;           // Watt-sec is again convert to Watt-Hr by dividing 1hr (3600sec)
    // Energy = (Power * time) / (1000*3600); for reading in kWh

    float SumCurrent = SumCurrent + Current;        // calculate total current
    Serial.println("Current in Total = " + String(SumCurrent) + " A");
    float AvgCurrent = SumCurrent / time;           // average current
    Serial.println("Current in Average = " + String(AvgCurrent) + " A");
    float CurrentHour = (AvgCurrent * time) / 3600; // Ah
    Serial.println("Current per hour = " + String(CurrentHour) + " Ah");

    Serial.print("Voltage = ");       // shows the voltage measured
    Serial.print(Voltage,3);          // 3 digits after decimal point
    Serial.println(" V");             // 3 digits after decimal point

    Serial.print("Current = ");       // shows the voltage measured
    Serial.print(Current,3);          // 3 digits after decimal point
    Serial.println(" A");             // 3 digits after decimal point

    Serial.print("Power = ");         // shows the voltage measured
    Serial.print(Power,3);            // 3 digits after decimal point
    Serial.println(" W");             // 3 digits after decimal point

    Serial.print("Energy = ");         // shows the voltage measured
    Serial.print(Energy,5);            // 3 digits after decimal point
    Serial.println(" Wh");             // 3 digits after decimal point
    Serial.println("=====================");
    delay(1000);
}

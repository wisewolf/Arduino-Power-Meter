# Arduino Power Meter
While trying to measure the load of a circuit and having in my future plans to build a Lab PSU (hacking a ATX PSU or something like that) it was my excuse to make this.

It's based on a MAX471 module bought on ebay. You can find two kind of modules, one allows you to measure only current and other has a voltge divider included, so you also can measure voltage.

## Measuring Current
You can measure current from 0-3A and it's simple, as:

```C++
#define max471AT A1

void setup(){
  pinMode(max471AT, INPUT);
  Serial.begin(9600);
}

void loop(){
  int RawAT = analogRead(max471AT);
  float Current = (RawAT * 5.0) / 1023;
  Serial.print('Current = ');
  Serial.print(Current);
  Serial.println(' A');
}
```

## Measuring Voltage

The voltage divider configuration on this modules reduces to 1/5th of the input voltage, so the max voltage you can read safely on a 5V arduino board it's 25V and if you use a 3.3V arduino it's 16.5V.

The code it's very similar, but we need to multiply by 5 (remember.. 1/5th of the input voltage) and looks like this:

```C++
#define max471VT A0

void setup(){
  pinMode(max471VT, INPUT);
  Serial.begin(9600);
}

void loop(){
  int RawVT = analogRead(max471VT);
  float Current = ((RawVT * 5.0) / 1023) * 5;
  Serial.print('Voltage = ');
  Serial.print(Voltage);
  Serial.println(' V');
}
```

## Making accurate ADC readings

Apparently there's a built in precision voltage reference of 1.1V on Atmega 168 and 328, the brain in the Arduino, and I want precision in my readings, so after reading [this](http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/) I just added to my code the following:

```C++
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
```
Now, we need to tweak the code to use this, and this what I've done:

```C++
float Vcc = readVcc() / 1000.0;  // convert mV to V
float Voltage = ((RawVT * Vcc) / 1023) * 5 ;
float Current = (RawAT * Vcc) / 1023;
```

## Measuring Electric Power

To calculate the electric power we just need to use two known values, Voltage and Current, and do this:

```C++
float Power = Voltage * Current;
```

That was easy!

## Now what?

Well, I added some mambo jambo to the code, i2c lcd back support and some fairy powder for the Lab PSU project.

## External Links
[Secret Arduino Voltmeter – Measure Battery Voltage](http://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/)

[tinkerit - SecretVoltmeter.wiki](https://code.google.com/archive/p/tinkerit/wikis/SecretVoltmeter.wiki)

#include <OneWire.h>
#include <DallasTemperature.h>


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////           PINAGEM           /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////


int ledpin = 13;


//// DISPLAY
byte segmentos1 = 0;
byte segmentos2 = 0;

int pina = 31;
int pinb = 33;
int pinc = 35;
int pind = 37;
int pine = 39;
int pinf = 41;
int ping = 43;

int pincatodo1 = 51;
int pincatodo2 = 53;

int segmentPins[] = {pina, pinb, pinc, pind, pine, pinf, ping}; // a–g

// Segments bit order: a b c d e f g
// Index = digit; bits = abcdefg (bit 0 = a, bit 6 = g)
byte numeros[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};
////end DISPLAY






/////// SENSORS
#define PORT_TERMOMETRO 2

OneWire oneWire(PORT_TERMOMETRO);
DallasTemperature sensors(&oneWire);
///////end SENSORS





////// VENTILATORE
int out1temperature = 46;
int out2temperature = 48;
int out3vent = 50;
int out4vent = 52;

int sw1 = 47;
int sw2 = 49;
//////end VENTILATORE


///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////













void setup() {
  sensors.begin();

  Serial.begin(9600);

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(pincatodo1, OUTPUT);
  pinMode(pincatodo2, OUTPUT);

  pinMode(sw1, INPUT_PULLUP);
  pinMode(sw2, INPUT_PULLUP);

  pinMode(out1temperature, OUTPUT);
  pinMode(out2temperature, OUTPUT);
  pinMode(out3vent, OUTPUT);
  pinMode(out4vent, OUTPUT);
}





/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
int vel = 0;
int temp = 0;

void loop() {
  delay(100);
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);

  Serial.println(temp);


  ventilator();
  // showTempDisplays();
  // aquecedor();
}

/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////


void aquecedor(){
  if (temp > 40){
    analogWrite(out1temperature, 0);
    analogWrite(out2temperature, 0);
  }
  else{
    analogWrite(out1temperature, 255);
    analogWrite(out2temperature, 0);
  }
}





void ventilator(){
  if (digitalRead(sw1) == 0){
    vel += 1;
  }
  if (digitalRead(sw2) == 0){
    vel -= 1;
  }

  if (vel > 255){
    vel = 255;
  }
  if (vel < 0){
    vel = 0;
  }

  Serial.println(vel);
  analogWrite(out3vent, 0);
  analogWrite(out4vent, vel);
}


//////////////////////////////////////////////////////////////////

void writeSegment(byte value) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (value >> i) & 1);
  }
}

void showTempDisplays(){
  int unidade = temp % 10;
  int dezena = temp / 10;

  segmentos1 = numeros[unidade];
  segmentos2 = numeros[dezena];

  // Display tens
  digitalWrite(pincatodo1, LOW);
  digitalWrite(pincatodo2, HIGH);
  writeSegment(segmentos1);
  delay(5);

  // Display units
  digitalWrite(pincatodo1, HIGH);
  digitalWrite(pincatodo2, LOW);
  writeSegment(segmentos2);
  delay(5);
}
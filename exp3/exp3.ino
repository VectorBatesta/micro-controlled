byte segmentos1 = 0;
byte segmentos2 = 0;

int pina = 31;
int pinb = 33;
int pinc = 35;
int pind = 37;
int pine = 39;
int pinf = 41;
int ping = 43;

int ledpin = 13;

int pinpoten = A1;

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



void setup() {
  pinMode(pinpoten, INPUT);
  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(pincatodo1, OUTPUT);
  pinMode(pincatodo2, OUTPUT);

  pinMode(ledpin, OUTPUT);
}



/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
void loop() {
  // contador();
  // potenciometroum();
  ledpawn();
}
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////










void ledpawn(){
  // delayed = 300 -> 100 times of increments of 3 
  int delayed = 300;
  int dInc = delayed / 100;
  
  digitalWrite(ledpin, HIGH);
  for(int i = 0; i<100; i++){
    delay(dInc)
  }
  digitalWrite(ledpin, LOW);
  delay(delayed);
  //TODO -> analog output library
}






/////////////////////////////////////////
void potenciometroum(){
  int analogIN = analogRead(pinpoten);

  int valor99 = map(analogIN, 0, 1023, 0, 99);
  int unidade = valor99 % 10;
  int dezena = valor99 / 10;

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
/////////////////////////////////////////











void writeSegment(byte value) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (value >> i) & 1);
  }
}





void contador(){
  for (int i = 0; i < 100; i++) {
    int unidade = i % 10;
    int dezena = i / 10;

    segmentos1 = numeros[unidade];
    segmentos2 = numeros[dezena];

    for (int j = 0; j < 100; j++) { // Show each number for ~1s
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
  }
}

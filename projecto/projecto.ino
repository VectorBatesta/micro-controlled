
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////           PINAGEM           /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

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



//// BUTTONS
int btn_select0 = ;
int btn_select1 = ;
int btn_select2 = ;
int btn_select3 = ;
int btn_select4 = ;
int btn_select5 = ;
int btn_select6 = ;
int btn_select7 = ;
int btn_select8 = ;
int btn_select9 = ;
////end BUTTONS






//// LEDS



////end LEDS



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////











void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 7; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }
  pinMode(pincatodo1, OUTPUT);
  pinMode(pincatodo2, OUTPUT);

  pinMode(btn_select0, INPUT_PULLUP);
  pinMode(btn_select1, INPUT_PULLUP);
  pinMode(btn_select2, INPUT_PULLUP);
  pinMode(btn_select3, INPUT_PULLUP);
  pinMode(btn_select4, INPUT_PULLUP);
  pinMode(btn_select5, INPUT_PULLUP);
  pinMode(btn_select6, INPUT_PULLUP);
  pinMode(btn_select7, INPUT_PULLUP);
  pinMode(btn_select8, INPUT_PULLUP);
  pinMode(btn_select9, INPUT_PULLUP);
}




void loop() {
  if (digitalRead(sw1) == 0){
    //algo aqui botao 1
  }
  if (digitalRead(sw2) == 0){
    //algo aqui botao 2
  }
}








//////////////////////////////////////////////////////////////////
//DISPLAYS

void writeSegment(byte value) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], (value >> i) & 1);
  }
}

void showTempDisplays(int num){
  int unidade = num % 10;
  int dezena = num / 10;

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
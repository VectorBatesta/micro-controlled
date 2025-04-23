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

int pinled1 = 11;
int pinled2 = 12;
int pinled3 = 13;



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

  pinMode(pinled1, OUTPUT);
  pinMode(pinled2, OUTPUT);
  pinMode(pinled3, OUTPUT);
}



/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////
void loop() {
  // contador();
  // potenciometroum();

  // ledpawn300ms();
  // ledpawnPWMmanual();

  // ledpawnPWMnative();
  tres_leds();
}
/////////////////////////////////////////
/////////////////////////////////////////
/////////////////////////////////////////





/////////////////////////////////////////////
///////////////// exp 4 /////////////////////
/////////////////////////////////////////////

int lastTime_led1 = 0;
int lastTime_led2 = 0;
int lastTime_led3 = 0;

int valorLed1 = 0;
int valorLed2 = 0;
int valorLed3 = 0;
void tres_leds(){
  int tempoAtual = millis();

  if (tempoAtual - lastTime_led1 > 0){
    if (valorLed1 <= 150){
      analogWrite(pinled1, map(valorLed1, 0, 150, 0, 1023));
    }
    else if (valorLed1 <= 300){
      analogWrite(pinled1, map(valorLed1, 300, 150, 0, 1023));
    }

    valorLed1++;

    if (valorLed1 == 0){
      lastTime_led1 = millis();
    }
  }

  if (tempoAtual - lastTime_led2 > 200){
    lastTime_led2 = millis();

    analogWrite(pinled2, valorLed2 ? 1023 : 0);
    valorLed2 = !valorLed2;
  }

  if (tempoAtual - lastTime_led3 > 400){
    lastTime_led3 = millis();

    analogWrite(pinled3, valorLed3 ? 1023 : 0);
    valorLed3 = !valorLed3;
  }

  // analogWrite(pinled1, outer);
  // analogWrite(pinled2, outer);
  // analogWrite(pinled3, outer);
}







int flag = 1;
int i = 0;
void ledpawnPWMnative(){
  int lum;

  if(flag == 1){
    i++;
    if (i >= 300)
      flag = 0;
  }
  else{
    i--;
    if (i <= 0)
      flag = 1;
  }

  lum = map(i, 0, 300, 0, 255);
  analogWrite(ledpin, lum);
  delay(1);
}

























//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
////////////////////exp3//////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////


void ledpawnPWMmanual() {
  int cycleTime = 1000;        //PWM cycle time in microseconds (1ms)
  int steps = 1000;             //tempo total da subidadescida em ms
  steps /= 2; 
  int duty;

  //0% -> 100% em steps/2
  for (int i = 0; i <= steps; i++) {
    float duty = (float)i / steps;

    int onTime = cycleTime * duty;
    int offTime = cycleTime - onTime;

    digitalWrite(ledpin, HIGH);
    delayMicroseconds(onTime);
    digitalWrite(ledpin, LOW);
    delayMicroseconds(offTime);
  }

  //100% -> 0% em steps/2
  for (int i = steps; i >= 0; i--) {
    float duty = (float)i / steps;

    int onTime = cycleTime * duty;
    int offTime = cycleTime - onTime;

    digitalWrite(ledpin, HIGH);
    delayMicroseconds(onTime);
    digitalWrite(ledpin, LOW);
    delayMicroseconds(offTime);

  }
}

void ledpawn300ms(){
  int delayed = 300;
  int dInc = delayed / 100;
  
  digitalWrite(ledpin, HIGH);
  delay(dInc);
  digitalWrite(ledpin, LOW);
  delay(delayed);
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

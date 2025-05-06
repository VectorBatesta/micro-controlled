#define ledPin 13


int tempo_ms = 300;
int tempo_delay = 65536 - ((16000000 / 256 ) * tempo_ms / 1000);

void setup(){
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  
  // Configuração do timer1 
  TCCR1A = 0;	//confira timer para operação 
  //normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;	//limpa registrador
  
  TCCR1B |= (1 << CS12);

  TCNT1 = tempo_delay;
  
  TIMSK1 |= (1 << TOIE1);  // habilita a interrupção do TIMER1
}

int count;
void loop(){
  if (count){
    count=0;
    Serial.println(millis());  
    digitalWrite(ledPin, digitalRead(ledPin) ^ 1); 
    //inverte estado do led
    //a manipulação do led é feita na ISR
  }
}
  

ISR(TIMER1_OVF_vect){	//interrupção do TIMER1
  TCNT1 = tempo_delay;		// Renicia TIMER
  count++;
}


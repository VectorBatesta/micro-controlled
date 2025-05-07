#define ledPin 13
#define pinpoten A1
#define led2pin 9


// int tempo_ms = 300;
// int tempo_delay = 65536 - ((16000000 / 256 ) * tempo_ms / 1000);







void setup(){
  pinMode(ledPin, OUTPUT);
  Serial.begin(2000000);
  
  // Configuração do timer1 
  TCCR1A = 0;	//confira timer para operação 
  //normal pinos OC1A e OC1B desconectados
  TCCR1B = 0;	//limpa registrador
  
  TCCR1B |= (1 << CS10);

  TCNT1 = 0xC180; 
  
  TIMSK1 |= (1 << TOIE1);  // habilita a interrupção do TIMER1
}










//detecta se o timer interno da placa roda, ~1ms
int timerActivated = false;

//timer aumenta 1 desse para printar o valor do potenciometro
int timerPotenciometro = 0;

//controladores do fade do led2
bool fade = true;
double ledvalue = 0;
double fadeIncrement = 255.0/300.0;
// double incremento_ledvalue = tempo_delay / 255;

int milisegundos = 0;
int segundos = 0;
int minutos = 0;
int horas = 0;

int relogio = 0;

//conta quantidades de milisegundos
int count = 0;

void loop() {
  //executa a cada 1 ms, utilizando o timer da placa
  if (timerActivated) {
    timerActivated = false;

    //controle do fade do led2, executa a cada 1 ms
    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////
    if (fade == true){ //subindo led
      ledvalue += fadeIncrement;
    }
    else{ //descendo led
      ledvalue -= fadeIncrement;
    }
    analogWrite(led2pin, ledvalue);
    /////////////////////////////////////////////////////
    /////////////////////////////////////////////////////


    //se 300ms passar...
    if (count >= 300){
      //ativa fade pra controlar led2 ali em cima
      if (fade == true){
        fade = false;
      }
      else {
        fade = true;
      }

      //codigo para o pisca acende verde, não utilizado, roda no led1
      // digitalWrite(ledPin, digitalRead(ledPin) ^ 1);

      //reseta quando 300ms de volta pra 0ms
      count = 0;
    }


    ////////////////////////////////////////////////////////////////////////////////////////////
    if (timerPotenciometro >= 100){
      //lê o valor do potenciometro, depois printa ele logo em seguida
      int valor_potenciometro = analogRead(pinpoten);
      Serial.print("\t\tValor potenciometro = ");
      Serial.println(valor_potenciometro);

      timerPotenciometro = 0;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////

  
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    /////////////////////////////////////////////////////////////////////
    //relogio printa a cada 100ms, mas a cada 1000ms ele incrementa o segundo
    if (relogio >= 100){
      if (milisegundos >= 1000){
        segundos++;
        milisegundos = 0;
        if (segundos >= 60){
          minutos++;
          segundos = 0;
          if (minutos >= 60){
            horas++;
            minutos = 0;
            if (horas >= 24){
              horas = 0;
            }
          }
        }
      }

      Serial.print("\nmilisegundos = ");
      Serial.print(milisegundos);
      Serial.print(", segundos = ");
      Serial.print(segundos);
      Serial.print(", minutos = ");
      Serial.print(minutos);
      Serial.print(", horas = ");
      Serial.print(horas);
      /////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////
      relogio = 0;
    }
  }
}
  

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//timer interno da placa
ISR(TIMER1_OVF_vect){	//interrupção do TIMER1
  TCNT1 = 0xC180;		// Renicia TIMER pra subir/esperar 1ms de novo
  timerActivated = true; //1ms
  count++; //aumenta 1 a cada 1ms
  milisegundos++; //aumenta 1 a cada 1ms para o relogio
  timerPotenciometro++; //a cada 100 desse ele printa o potenciometro
  relogio++;
}


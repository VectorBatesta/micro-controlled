#include <LiquidCrystal.h>

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////           PINAGEM           /////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

#define BTN_MODO1 8   // botão modo 1
#define BTN_MODO2 9   // botão modo 2
#define BTN_MODO3 10  // botão modo 3

const int pinoLEDs[10]   = {A0, A1,  A2,  A3,  A4,  A5,  A6,  A7};    // pinos dos LEDs
const int pinoBotoes[10] = {A8, A9, A10, A11, A12, A13, A14, A15}; // pinos dos botões de jogo

//               RS, E, D4, D5, D6, D7
LiquidCrystal lcd(2, 3,  4,  5,  6,  7);

enum Estado { AGUARDANDO_SELECAO, MOSTRANDO_DESC, EXECUTANDO_JOGO, MOSTRANDO_RESULTADO };
Estado estadoAtual = AGUARDANDO_SELECAO;  //estado inicial

int jogoSelecionado = -1;       //índice do jogo
unsigned long tempoInicio;      //marca início de temporização
int pontuacao;                  //armazena pontos

///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void setup() {
  Serial.begin(9600);                     //inicializa serial
  
  lcd.begin(16, 2); 
  
  pinMode(BTN_MODO1, INPUT_PULLUP);       //configura botão modo 1 com pullup
  pinMode(BTN_MODO2, INPUT_PULLUP);       //configura botão modo 2 com pullup
  pinMode(BTN_MODO3, INPUT_PULLUP);       //configura botão modo 3 com pullup
  
  for(int i=0;i<10;i++){
    pinMode(pinoLEDs[i], OUTPUT);         //configura pino do led como saída
    pinMode(pinoBotoes[i], INPUT_PULLUP); //configura pino do botão com pullup
  }
  
  lcd.clear();                            //limpa tela
  lcd.setCursor(0,0);
  lcd.print("selecione jogo 1-3");        //mensagem inicial
}












void loop() {
  switch(estadoAtual){
    case AGUARDANDO_SELECAO:
      verificaBotoesModo();               //aguarda seleção de jogo
      break;
    case MOSTRANDO_DESC:
      exibeDescricao();                   //mostra descrição do jogo
      break;
    case EXECUTANDO_JOGO:
      executarJogoSelecionado();         //executa lógica do jogo
      break;
    case MOSTRANDO_RESULTADO:
      if(qualquerModoPressionado())       //aguarda reinício após resultado
        reiniciarParaSelecao();
      break;
  }
}




void verificaBotoesModo() {
  if(!digitalRead(BTN_MODO1)) aoBotaoModoPressionado(0);  //botão 1 pressionado?
  if(!digitalRead(BTN_MODO2)) aoBotaoModoPressionado(1);  //botão 2 pressionado?
  if(!digitalRead(BTN_MODO3)) aoBotaoModoPressionado(2);  //botão 3 pressionado?
}

bool qualquerModoPressionado() {
  //verifica qualquer botão de modo
  return !digitalRead(BTN_MODO1)
      || !digitalRead(BTN_MODO2)
      || !digitalRead(BTN_MODO3);
}





void aoBotaoModoPressionado(int idx) {
  delay(200);  //debounce
  if(estadoAtual == AGUARDANDO_SELECAO){
    jogoSelecionado = idx;             //define jogo selecionado
    estadoAtual = MOSTRANDO_DESC;      //muda para mostrar descrição
  }
  else if(estadoAtual == MOSTRANDO_DESC && jogoSelecionado == idx){
    estadoAtual = EXECUTANDO_JOGO;     //inicia jogo
  }
}






void exibeDescricao() {
  lcd.clear();                          //limpa lcd
  lcd.setCursor(0,0);

  
  if(jogoSelecionado==0){
    lcd.print("1: teste reacao");      //descrição jogo 1
    lcd.setCursor(0,1);
    lcd.print("pressione p iniciar");
  }
  else if(jogoSelecionado==1){
    lcd.print("2: jogo temporizado");  //descrição jogo 2
    lcd.setCursor(0,1);
    lcd.print("pressione p iniciar");
  }
  else {
    lcd.print("3: bate-toupeira");     //descrição jogo 3
    lcd.setCursor(0,1);
    lcd.print("pressione p iniciar");
  }


  
  while(estadoAtual == MOSTRANDO_DESC){
    if(qualquerModoPressionado())
      break;  //aguarda confirmação
    delay(50);
  }
}









void executarJogoSelecionado() {
  lcd.clear();                          //prepara tela para jogo
  pontuacao = 0;                        //zera pontuação
  switch(jogoSelecionado){
    case 0: jogoReacao();       break;  //chama teste de reação
    case 1: jogoTemporizado();  break;  //chama jogo temporizado
    case 2: jogoBateToupeira(); break;  //chama bate-toupeira
  }
  estadoAtual = MOSTRANDO_RESULTADO;    //muda para mostrar resultado
}

void reiniciarTodosLEDs(bool ligado) {
  for(int i = 0; i < 10; i++)
    digitalWrite(pinoLEDs[i], ligado);  //liga/desliga todos leds
}












///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////       GAMES       //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

void jogoReacao() {
  int alvo = random(0,10);              //escolhe led aleatório
  digitalWrite(pinoLEDs[alvo], HIGH);   //acende led alvo
  
  lcd.setCursor(0,0);
  lcd.print("reaja ao LED...");         //instrução
  
  tempoInicio = millis();               //marca início
  while(digitalRead(pinoBotoes[alvo])); //espera botão correto
  unsigned long dt = millis() - tempoInicio;  //calcula tempo
  
  digitalWrite(pinoLEDs[alvo], LOW);    //apaga led alvo
  
  lcd.clear();
  lcd.print("tempo: ");
  lcd.print(dt);   //mostra resultado
  lcd.print(" ms");
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void jogoTemporizado() {
  reiniciarTodosLEDs(HIGH);             //liga todos leds
  lcd.setCursor(0,0);
  lcd.print("prepare-se...");           //aviso de partida
  delay(1000);                          //tempo de preparação
  
  for(int i=0;i<10;i++){
    digitalWrite(pinoLEDs[i], LOW);     //desliga leds em sequência
    delay(100);
  }
  
  lcd.clear();
  lcd.print("AGORA!");     //início do timing
  tempoInicio = millis();               //marca início
  
  while(true){
    for(int i=0;i<10;i++){
      if(!digitalRead(pinoBotoes[i])){  //espera qualquer botão
        unsigned long dt = millis() - tempoInicio;
        
        lcd.clear();
        lcd.print("tempo:"); lcd.print(dt);
        lcd.print(" ms"); 
        return;
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void jogoBateToupeira() {
  unsigned long fim = millis() + 30000; //tempo total 30s
  
  while(millis() < fim){
    int toupeira = random(0,10);        //escolhe aleatório
    
    digitalWrite(pinoLEDs[toupeira], HIGH);  //acende por 1s
    unsigned long onTime = millis();
    
    bool acertou = false;
    while(millis() - onTime < 1000){
      for(int b=0;b<10;b++){
        if(!digitalRead(pinoBotoes[b])){ //verifica botão
          if(b==toupeira) pontuacao++;   //acerto
          else fim -= 5000;               //erro penaliza
          acertou = true;
          break;
        }
      }
      if(acertou) break;
    }
    digitalWrite(pinoLEDs[toupeira], LOW); //apaga led
    delay(200);                           //pausa breve
  }
  
  lcd.clear();
  lcd.print("pontuacao:"); lcd.print(pontuacao);  //mostra pontos
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////




void reiniciarParaSelecao() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("selecione jogo 1-3");        //retorna ao menu
  
  estadoAtual = AGUARDANDO_SELECAO;
}

#include <LiquidCrystal.h>

// PINAGEM E CONSTANTES
#define BTN_MODO1 8
#define BTN_MODO2 9
#define BTN_MODO3 10

const int NUM_LEDS_BOTOES = 8;
const int pinoLEDs[NUM_LEDS_BOTOES]   = {A0, A1,  A2,  A3,  A4,  A5,  A6,  A7};
const int pinoBotoes[NUM_LEDS_BOTOES] = {A8, A9, A10, A11, A12, A13, A14, A15};

LiquidCrystal lcd(2, 3,  4,  5,  6,  7);

// ESTADOS DO PROGRAMA
enum Estado { AGUARDANDO_SELECAO, MOSTRANDO_DESC, EXECUTANDO_JOGO, MOSTRANDO_RESULTADO };
Estado estadoAtual = AGUARDANDO_SELECAO;

// VARIÁVEIS GLOBAIS
int jogoSelecionado = -1;
unsigned long tempoInicio;
int pontuacao;

// VARIÁVEIS PARA LEITURA DE BOTÃO (LÓGICA MELHORADA)
bool modo1PressionadoAntes = false;
bool modo2PressionadoAntes = false;
bool modo3PressionadoAntes = false;


void setup() {
  Serial.begin(9600);
  Serial.println("--- INICIANDO DIAGNOSTICO ---");
  
  lcd.begin(16, 2); 
  
  pinMode(BTN_MODO1, INPUT_PULLUP);
  pinMode(BTN_MODO2, INPUT_PULLUP);
  pinMode(BTN_MODO3, INPUT_PULLUP);
  
  for(int i = 0; i < NUM_LEDS_BOTOES; i++){
    pinMode(pinoLEDs[i], OUTPUT);
    pinMode(pinoBotoes[i], INPUT_PULLUP);
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("selecione jogo 1-3");
  Serial.println("Setup completo. Estado inicial: AGUARDANDO_SELECAO");
}

void loop() {
  verificaBotoesModo();
  
  switch(estadoAtual){
    case AGUARDANDO_SELECAO:
     
      break;
    case MOSTRANDO_DESC:

      break;
    case EXECUTANDO_JOGO:
      executarJogoSelecionado();
      break;
    case MOSTRANDO_RESULTADO:
      if(qualquerModoPressionado())
        reiniciarParaSelecao();
      break;
  }
}

// ================================================================
//               FUNCOES DE CONTROLE E ESTADO
// ================================================================

void verificaBotoesModo() {
  bool modo1PressionadoAgora = !digitalRead(BTN_MODO1);
  bool modo2PressionadoAgora = !digitalRead(BTN_MODO2);
  bool modo3PressionadoAgora = !digitalRead(BTN_MODO3);

  if (modo1PressionadoAgora && !modo1PressionadoAntes) aoBotaoModoPressionado(0);
  if (modo2PressionadoAgora && !modo2PressionadoAntes) aoBotaoModoPressionado(1);
  if (modo3PressionadoAgora && !modo3PressionadoAntes) aoBotaoModoPressionado(2);

  modo1PressionadoAntes = modo1PressionadoAgora;
  modo2PressionadoAntes = modo2PressionadoAgora;
  modo3PressionadoAntes = modo3PressionadoAgora;
}

void aoBotaoModoPressionado(int idx) {

  Serial.println("------------------------------------");
  Serial.println("FUNCAO 'aoBotaoModoPressionado' CHAMADA!");
  Serial.print("  > Botao pressionado (idx): "); Serial.println(idx);
  Serial.print("  > Valor de 'estadoAtual' ANTES da logica: "); Serial.println(estadoAtual);
  Serial.print("  > Valor de 'jogoSelecionado' ANTES da logica: "); Serial.println(jogoSelecionado);

  if(estadoAtual == AGUARDANDO_SELECAO){
    jogoSelecionado = idx;
    estadoAtual = MOSTRANDO_DESC;
    Serial.println("  >> MUDANDO ESTADO PARA MOSTRANDO_DESC");
    exibeDescricao(); // Chama a função para desenhar a tela uma única vez
  }
  else if(estadoAtual == MOSTRANDO_DESC && jogoSelecionado == idx){
    estadoAtual = EXECUTANDO_JOGO;
    Serial.println("  >>> SUCESSO! MUDANDO ESTADO PARA EXECUTANDO_JOGO <<<");
  } else {
    Serial.println("  >> CONDICAO PARA INICIAR O JOGO FALHOU!");
  }
  Serial.println("------------------------------------");
}

void exibeDescricao() {
  lcd.clear();
  lcd.setCursor(0,0);
  switch(jogoSelecionado) {
    case 0: lcd.print("1: Teste Reacao"); break;
    case 1: lcd.print("2: Jogo Temporiz."); break;
    case 2: lcd.print("3: Bate-Toupeira"); break;
  }
  lcd.setCursor(0,1);
  lcd.print("Pressione p/ confirmar"); 
}

void executarJogoSelecionado() {
  lcd.clear();
  pontuacao = 0;
  
  Serial.print("EXECUTANDO JOGO: "); Serial.println(jogoSelecionado);

  switch(jogoSelecionado){
    case 0: jogoReacao(); break;
    case 1: jogoTemporizado(); break;
    case 2: jogoBateToupeira(); break;
  }
  estadoAtual = MOSTRANDO_RESULTADO;
}


bool qualquerModoPressionado() {
 return !digitalRead(BTN_MODO1) || !digitalRead(BTN_MODO2) || !digitalRead(BTN_MODO3);
}

void reiniciarTodosLEDs(bool ligado) {
  for(int i = 0; i < NUM_LEDS_BOTOES; i++)
    digitalWrite(pinoLEDs[i], ligado);
}

void jogoReacao() {
  int alvo = random(0, NUM_LEDS_BOTOES);
  
  Serial.print("  JOGO REACAO: Alvo eh o par LED/Botao de indice: ");
  Serial.println(alvo);

  digitalWrite(pinoLEDs[alvo], HIGH);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Reaja ao LED!");
  
  tempoInicio = millis();
  
  // Laço corrigido: espera o pino ir para LOW
  while(digitalRead(pinoBotoes[alvo]) == HIGH) {
    // Fica em loop esperando o pressionamento
    // Podemos adicionar um timeout aqui no futuro, se quisermos
  }
  
  // Assim que o botão é pressionado, sai do loop
  unsigned long dt = millis() - tempoInicio;
  
  // Um pequeno delay para debounce do botão
  delay(50);

  digitalWrite(pinoLEDs[alvo], LOW);
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Tempo: ");
  lcd.print(dt);
  lcd.setCursor(0,1);
  lcd.print("ms");
  
  Serial.print("  JOGO REACAO: Reacao em ");
  Serial.print(dt);
  Serial.println(" ms.");

  // Espera qualquer botão de modo ser pressionado para sair
  // e mostrar o resultado final antes de voltar ao menu.
  delay(1000); // Mostra o resultado por 1s
  while(qualquerModoPressionado()){ /* espera soltar o botão de jogo */ }
  Serial.println("Aguardando botao de modo para continuar...");
  while(!qualquerModoPressionado()){ /* espera pressionar um modo */ }
}


void jogoTemporizado() {
  reiniciarTodosLEDs(HIGH);
  lcd.setCursor(0,0);
  lcd.print("prepare-se...");
  delay(1000);
  for(int i = 0; i < NUM_LEDS_BOTOES; i++){
    digitalWrite(pinoLEDs[i], LOW);
    delay(100);
  }
  lcd.clear();
  lcd.print("AGORA!");
  tempoInicio = millis();
  while(true){
    for(int i = 0; i < NUM_LEDS_BOTOES; i++){
      if(!digitalRead(pinoBotoes[i])){
        unsigned long dt = millis() - tempoInicio;
        lcd.clear();
        lcd.print("tempo:"); lcd.print(dt);
        lcd.print(" ms"); 
        delay(2000); // Pausa para ver o resultado
        return;
      }
    }
  }
}

void jogoBateToupeira() {
  unsigned long fim = millis() + 30000;
  while(millis() < fim){
    int toupeira = random(0,NUM_LEDS_BOTOES);
    digitalWrite(pinoLEDs[toupeira], HIGH);
    unsigned long onTime = millis();
    bool acertou = false;
    while(millis() - onTime < 1000){
      for(int b=0;b<NUM_LEDS_BOTOES;b++){
        if(!digitalRead(pinoBotoes[b])){
          if(b==toupeira) pontuacao++;
          else fim -= 5000;
          acertou = true;
          break;
        }
      }
      if(acertou) break;
    }
    digitalWrite(pinoLEDs[toupeira], LOW);
    delay(200);
  }
  lcd.clear();
  lcd.print("pontuacao:"); lcd.print(pontuacao);
  delay(2000); // Pausa para ver o resultado
}

void reiniciarParaSelecao() {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("selecione jogo 1-3");
  estadoAtual = AGUARDANDO_SELECAO;
  Serial.println("--- REINICIANDO PARA O MENU ---");
}

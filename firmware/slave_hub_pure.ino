
#include "OneWireLib/OneWireHub.h"
#include "OneWireLib/OneWireItem.h"
#include "DJORNADA.h"
#include "FILA.h"

// Pinos dos Botões
constexpr uint8_t PIN_BOTAO_JORNADA{11};
constexpr uint8_t PIN_BOTAO_ALMOCO{8};
constexpr uint8_t PIN_BOTAO_LED{4};
constexpr uint8_t PIN_ONEWIRE{6};

// Configuração biblioteca
OneWireHub Wire = OneWireHub(PIN_ONEWIRE);

// Seriais de cada ação
auto SerialInicioJornada = DJORNADA(DJORNADA::family_code, 0x01, 0x01, 0x00,0x00, 0x00, 0x00);
auto SerialFimJornada = DJORNADA(DJORNADA::family_code, 0x01, 0x02, 0x00,0x00, 0x00, 0x00);
auto SerialAlmocoJornada = DJORNADA(DJORNADA::family_code, 0x01, 0x03, 0x00,0x00, 0x00, 0x00);

// Ação atual (EEPROM)
Fila FilaAcoes;

// Estados (EEPROM)
uint8_t status_jornada_addr = 0x02;
uint8_t status_almoco_addr = 0x03;
boolean em_jornada = false;
boolean em_almoco = false;
uint8_t reset = 0x00;

void setup()
{
    Serial.begin(9600);
    Serial.println("Dispositivo Jornada de Trabalho - GS10G (1-Wire)");

    // Carregar últimos estados
    em_jornada = EEPROM.read(status_jornada_addr) == 1 ? true : false;
    em_almoco = EEPROM.read(status_almoco_addr) == 1 ? true : false;
    Serial.println(EEPROM.read(status_almoco_addr) == 1 ? "Em almoço" : "Sem almoço");
    Serial.println(EEPROM.read(status_jornada_addr) == 1 ? "Em jornada" : "Sem jornada");

    // Carrega a fila de ações (caso não tenha enviado no 1-wire)
    FilaAcoes.iniciar_fila();
    FilaAcoes.mostrar_fila(FilaAcoes.em_fila());

    // Configura o pino de estado do botão
    pinMode(PIN_BOTAO_JORNADA, INPUT);
    pinMode(PIN_BOTAO_ALMOCO, INPUT);

    // Configura o pino do LED como saída
    pinMode(PIN_BOTAO_LED, OUTPUT);
    digitalWrite(PIN_BOTAO_LED, LOW);
}

void loop()
{
    // Segue os passos do protocolo 1-wire
    // 1. Verifica se o master está solicitando presença (Reset Pulse)
    // 2. Se houver, envia um pulso de presença (Presence Pulse)
    // 3. Le o comando enviado pelo master, no caso do dispositivo usado 0xF0 (SEARCH COMMAND)
    // 4. Envia os 8 bytes da ação para o master
    Wire.poll(&reset);

    // Verifica se reset foi feito
    // Tira da fila e libera vaga
    if (reset == 0xff) {
      FilaAcoes.seguir_fila();
      reset = 0x00;
    }

    // Verifica o botão que foi clicado
    // E a ação vinculada a ele
    verificar_botoes();

    // Processa as ações em fila a cada 2 segundos
    // Do primeiro ao último 0 -> 8
    if (FilaAcoes.em_fila() >= 1) {
      if (timing(2000)) {
        switch (FilaAcoes.ler_acao())
        {
        case 1: Wire.attach(SerialInicioJornada);
          break;
        case 2: Wire.attach(SerialFimJornada);
          break;
        case 3: Wire.attach(SerialAlmocoJornada);
          break;
        default:break;
        }
      }
    } 
    // Limpa fila se não houver mais
    else {
      FilaAcoes.limpar_fila();
    }
}

void verificar_botoes()
{
  uint8_t btnJornada  = digitalRead(PIN_BOTAO_JORNADA);
  uint8_t btnAlmoco  = digitalRead(PIN_BOTAO_ALMOCO);
  if ( (em_almoco && btnJornada) || (!em_jornada && btnJornada)) {
    if (!FilaAcoes.adicionar_acao(1)) return;
    mudar_status_almoco(false);
    mudar_status_jornada(true);
    Serial.println("Motorista iniciou jornada");
    piscar_led(PIN_BOTAO_LED, 1);
  }
  else if (em_jornada && btnJornada) {
    if (!FilaAcoes.adicionar_acao(2)) return;
    mudar_status_almoco(false);
    mudar_status_jornada(false);
    Serial.println("Motorista terminou jornada");
    piscar_led(PIN_BOTAO_LED, 3);
  }
  else if (btnAlmoco) {
    if (!FilaAcoes.adicionar_acao(3)) return;
    mudar_status_jornada(false);
    mudar_status_almoco(true);
    Serial.println("Motorista iniciou almoço");
    piscar_led(PIN_BOTAO_LED, 2);
  }
}

void piscar_led(uint8_t led, uint8_t rep)
{
  for (int i=0; i<rep; i++) {
    digitalWrite(led, HIGH);
    delay(500);
    digitalWrite(led, LOW);
    delay(500);
  }
}

void mudar_status_jornada(boolean acao)
{
  noInterrupts();
  em_jornada = acao;
  EEPROM.write(status_jornada_addr, acao ? 0x1 : 0x0);
  interrupts();
}

void mudar_status_almoco(boolean acao)
{
  noInterrupts();
  em_almoco = acao;
  EEPROM.write(status_almoco_addr, acao ? 0x1 : 0x0);
  interrupts();
}

bool timing(int interval)
{
  static uint32_t nextMillis = millis();
  if (millis() > nextMillis) {
    nextMillis += interval;
    return 1;
  }
  return 0;
}
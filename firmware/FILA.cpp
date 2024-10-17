#include "fila.h"

Fila::Fila() {
  iniciar_fila();
}

Fila::~Fila() {
  
}

void Fila::iniciar_fila() {
  sincronizar_fila(false);
  
  // Debug
  if (debug) {
    Serial.print("FilaAddr = 0x");
    Serial.print(FILA_ACOES_ADDR, HEX);
    Serial.println();
  }
}

void Fila::mostrar_fila(uint8_t qtd) {
  for (uint8_t i = 0; i < qtd; i++) {
    Serial.print("Fila[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(FILA_ACOES[i]);
  }
}

void Fila::mostrar_fila_mem(uint8_t qtd) {
  for (uint8_t i = 0; i < qtd; i++) {
    Serial.print("FilaROM[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(EEPROM[FILA_ACOES_ADDR + i]);
  }
}

bool Fila::adicionar_acao(uint8_t acao) {
  if (fila == FILA_TAMANHO) {
    // Debug
    if (debug) {
      //Serial.println("Limite da fila atingido");
    }
    return false;
  }
  FILA_ACOES[fila] = acao;
  salvar_fila();

  // Debug
  if (debug) {
    Serial.print("Ação: ");
    Serial.print(acao);
    Serial.print(" adicionada na Fila[");
    Serial.print(fila - 1);
    Serial.print("] com endereço: 0x");
    Serial.println((FILA_ACOES_ADDR + (fila-1)), HEX);
  }
  return true;
}

void Fila::salvar_fila() {
  sincronizar_fila(true);
}

void Fila::sincronizar_fila( bool to_eeprom ) {
  noInterrupts();
  for (uint8_t i=0; i<FILA_TAMANHO; i++) {
    if (to_eeprom) {
      EEPROM.write(FILA_ACOES_ADDR + i, FILA_ACOES[i]);
    } else {
      FILA_ACOES[i] = EEPROM.read(FILA_ACOES_ADDR + i);
    }
  }
  interrupts();
  fila = em_fila();
}

void Fila::limpar_fila() {
  memset(FILA_ACOES, 0xff, FILA_TAMANHO);
  fila = 0; // Reseta o contador da fila
}

void Fila::seguir_fila() {
  if (fila > 0) {
    memmove(&FILA_ACOES[0], &FILA_ACOES[1], (fila - 1) * sizeof(uint8_t));
    FILA_ACOES[fila - 1] = 0xFF; // Corrige a posição após o deslocamento
    fila--;
    salvar_fila();
    // Debug
    if (debug) {
      Serial.println("Fila Andou!");
      Serial.println("Hora De Almoco");
    }

  }
}

uint8_t Fila::em_fila() {
  uint8_t total = 0;
  for (uint8_t i = 0; i < FILA_TAMANHO; i++) {
    if (FILA_ACOES[i] > 0 && FILA_ACOES[i] < 4) {
      total++;
    }
  }
  return total;
}

uint8_t Fila::ler_acao() {
  return FILA_ACOES[0]; // Retorna 0 se a fila estiver vazia
}

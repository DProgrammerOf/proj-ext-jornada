#ifndef FILA_H
#define FILA_H

#include <Arduino.h>
#include <EEPROM.h>

class Fila {
public:
    Fila();
    ~Fila();

    void iniciar_fila();
    void mostrar_fila(uint8_t qtd);
    void mostrar_fila_mem(uint8_t qtd);
    bool adicionar_acao(uint8_t acao);
    void salvar_fila();
    void limpar_fila();
    void seguir_fila();
    uint8_t em_fila();
    uint8_t Fila::ler_acao();
    
    void sincronizar_fila( bool to_eeprom );

private:
    uint32_t FILA_ACOES_ADDR = 0x100; // Endereço da fila ROM
    static const size_t FILA_TAMANHO = 5; // Tamanho da fila
    uint8_t FILA_ACOES[FILA_TAMANHO];
    uint8_t fila;
    bool debug = true; // Mude para false para desativar o debug
};

#endif // FILA_H

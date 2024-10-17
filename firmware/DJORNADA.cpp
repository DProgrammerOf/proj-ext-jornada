#include "DJORNADA.h"

DJORNADA::DJORNADA(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6,
               uint8_t ID7)
    : OneWireItem(ID1, ID2, ID3, ID4, ID5, ID6, ID7)
{
    // disable bus-features:
    fast_read_rom   = false;
    fast_search_rom = false;
}

void DJORNADA::duty(OneWireHub *const hub)
{
    uint8_t cmd;
    
    if (hub->recv(&cmd)) return;

    switch (cmd)
    {
        default: hub->raiseDeviceError(cmd);
    }
}

void DJORNADA::setSerial(uint8_t index, uint8_t value)
{
    ID[index] = value;
    refreshCRC();
}

void DJORNADA::refreshCRC()
{
    ID[7] = crc8(ID, 7);
}
// Serial Number
// Works
// native bus-features: none

#ifndef ONEWIREHUB_DJORNADA_H
#define ONEWIREHUB_DJORNADA_H

#include "OneWireLib/OneWireItem.h"

class DJORNADA : public OneWireItem
{
public:
    static constexpr uint8_t family_code{0x01};
    DJORNADA(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5, uint8_t ID6, uint8_t ID7);

    void duty(OneWireHub *hub) final;
    void setSerial(uint8_t index, uint8_t value);
    void refreshCRC();
};

#endif
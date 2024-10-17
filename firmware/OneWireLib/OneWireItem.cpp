#include "OneWireItem.h"

OneWireItem::OneWireItem(uint8_t ID1, uint8_t ID2, uint8_t ID3, uint8_t ID4, uint8_t ID5,
                         uint8_t ID6, uint8_t ID7)
{
    ID[0] = ID1;
    ID[1] = ID2;
    ID[2] = ID3;
    ID[3] = ID4;
    ID[4] = ID5;
    ID[5] = ID6;
    ID[6] = ID7;
    ID[7] = crc8(ID, 7);
}

void OneWireItem::sendID(OneWireHub *const hub) const { hub->send(ID, 8); }

//The CRC code was excerpted and inspired by the Dallas Semiconductor
//sample code bearing this copyright.
//---------------------------------------------------------------------------
// Copyright (C) 2000 Dallas Semiconductor Corporation, All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DALLAS SEMICONDUCTOR BE LIABLE FOR ANY CLAIM, DAMAGES
// OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// Except as contained in this notice, the name of Dallas Semiconductor
// shall not be used except as stated in the Dallas Semiconductor
// Branding Policy.
//--------------------------------------------------------------------------

// The 1-Wire CRC scheme is described in Maxim Application Note 27:
// "Understanding and Using Cyclic Redundancy Checks with Maxim iButton Products"
// fast but needs more storage:
//  https://github.com/PaulStoffregen/OneWire/blob/master/OneWire.cpp --> calc with table (EOF)


// INFO: this is the slow but memory saving version of the CRC() --> the calculation is not time-critical and happens offline
// alternative for AVR: http://www.atmel.com/webdoc/AVRLibcReferenceManual/group__util__crc_1ga37b2f691ebbd917e36e40b096f78d996.html

uint8_t OneWireItem::crc8(const uint8_t data[], const uint8_t data_size, const uint8_t crc_init)
{
    uint8_t crc = crc_init;

    for (uint8_t index = 0; index < data_size; ++index)
    {
        crc = _crc_ibutton_update(crc, data[index]);
    }
    return crc;
}


uint16_t OneWireItem::crc16(const uint8_t address[], const uint8_t length, const uint16_t init)
{
    uint16_t crc = init; // init value
    for (uint8_t i = 0; i < length; ++i) { crc = _crc16_update(crc, address[i]); }
    return crc;
}

uint16_t OneWireItem::crc16(uint8_t value, uint16_t crc)
{
    return _crc16_update(crc, value);
}

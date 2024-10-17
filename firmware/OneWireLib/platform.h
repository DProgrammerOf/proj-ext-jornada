// taken with modifications from: https://github.com/PaulStoffregen/OneWire/blob/master/OneWire.h
// Platform specific I/O definitions

#ifndef ONEWIREHUB_PLATFORM_H
#define ONEWIREHUB_PLATFORM_H

#if defined(ARDUINO) && (ARDUINO >= 100)
  #include <Arduino.h>
#endif

#define ONEWIREHUB_GCC_VERSION ((__GNUC__ * 10000) + (__GNUC_MINOR__ * 100) + __GNUC_PATCHLEVEL__)

#define PIN_TO_BASEREG(pin)            (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)            (digitalPinToBitMask(pin))
#define DIRECT_READ(base, mask)        (((*(base)) & (mask)) ? 1 : 0)
#define DIRECT_MODE_INPUT(base, mask)  ((*((base) + 1)) &= ~(mask))
#define DIRECT_MODE_OUTPUT(base, mask) ((*((base) + 1)) |= (mask))
#define DIRECT_WRITE_LOW(base, mask)   ((*((base) + 2)) &= ~(mask))
#define DIRECT_WRITE_HIGH(base, mask)  ((*((base) + 2)) |= (mask))
using io_reg_t = uint8_t; // define special datatype for register-access
constexpr uint8_t VALUE_IPL{13};

#endif //ONEWIREHUB_PLATFORM_H

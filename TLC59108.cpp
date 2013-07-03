extern "C"
{
#include <inttypes.h>
}

#include "TLC59108.h"

void TLC59108::setDefaultI2C(TwoWire i2c_default)
{
   TLC59108::i2c_default = i2c_default;
}

TLC59108::TLC59108(TwoWire i2c, const byte i2c_address): i2c(i2c), addr(i2c_address)
{
}

TLC59108::TLC59108(const byte i2c_address): addr(i2c_address)
{
   i2c = Wire;
}

uint8_t TLC59108::setRegister(const uint8_t reg, const uint8_t value)
{
   i2c.beginTransmission(addr);
   i2c.write(reg);
   i2c.write(value);
   return i2c.endTransmission();
}

uint8_t TLC59108::setRegisters(const uint8_t startReg, const uint8_t values[], const uint8_t numValues)
{
   i2c.beginTransmission(startReg | AUTO_INCREMENT::ALL);
   for(uint8_t i = 0; i < numValues; i++)
     i2c.write(values[i]);
   return i2c.endTransmission();
}

int TLC59108::readRegister(const uint8_t reg) const
{
   i2c.beginTransmission(addr);
   i2c.write(reg);
   if(!i2c.endTransmission())
     return -1;

   i2c.requestFrom(addr, (uint8_t) 1);
   if(i2c.available())
     return i2c.read();
   else
     return -1;
}

uint8_t TLC59108::init(const uint8_t hwResetPin)
{
   if(hwResetPin)
     {
	pinMode(hwResetPin, OUTPUT);
	digitalWrite(hwResetPin, LOW);
	delay(1);
	digitalWrite(hwResetPin, HIGH);
	delay(1);
     }

   return setRegister(REGISTER::MODE1::ADDR, REGISTER::MODE1::ALLCALL);
}

uint8_t TLC59108::setBrightness(const uint8_t pwmChannel, const uint8_t dutyCycle)
{
   if(pwmChannel > 7)
     return ERROR::EINVAL;

   return setRegister(pwmChannel + 2, dutyCycle);
}

uint8_t TLC59108::setLedOutputMode(const uint8_t outputMode)
{
   if(outputMode & 0xfc)
     return ERROR::EINVAL;

   byte regValue = (outputMode << 6) | (outputMode << 4) | (outputMode << 2) | outputMode;

   uint8_t retVal = setRegister(REGISTER::LEDOUT0::ADDR, regValue);
   retVal &= setRegister(REGISTER::LEDOUT1::ADDR, regValue);
   return retVal;
}

uint8_t TLC59108::setAllBrightness(const uint8_t dutyCycle)
{
   i2c.beginTransmission(addr);
   i2c.write(REGISTER::PWM0::ADDR | AUTO_INCREMENT::IND);
   for(uint8_t i=0; i<8; i++)
     i2c.write(dutyCycle);
   return i2c.endTransmission();
}

uint8_t TLC59108::setAllBrightness(const uint8_t dutyCycles[]) 
{
   i2c.beginTransmission(addr);
   i2c.write(REGISTER::PWM0::ADDR | AUTO_INCREMENT::IND);
   for(uint8_t i=0; i<8; i++)
     i2c.write(dutyCycles[i]);
   return i2c.endTransmission();
}

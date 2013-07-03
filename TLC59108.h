/*
 * TLC59108: Arduino library to control TI TLC59108/TLC59108F/TLC59208 LED drivers
 * 
 * (C) 2013 Christopher Smith <chrylis@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef TLC59108_H
# define TLC59108_H

# include <Arduino.h>
# include "Wire.h"

class TLC59108
{
 public:
   // default I2C addresses
   // datasheet, pp 12-13
   struct I2C_ADDR
     {
	static const byte BASE 		= 0x40;
	static const byte SWRESET	= 0x4b;
	static const byte ALLCALL	= 0x48;
	static const byte SUB1		= 0x49;
	static const byte SUB2		= 0x4a;
	static const byte SUB3		= 0x4c;
     };

   // register auto-increment modes for setting multiple registers
   // datasheet, p 13
   struct AUTO_INCREMENT
     {
	static const byte ALL		= 0x80; // increment through all registers (for initial setup)
	static const byte IND	        = 0xa0; // increment through individual brightness registers
	static const byte GLOBAL	= 0xc0; // increment through global control registers
	static const byte INDGLOBAL	= 0xe0; // increment through individual and global registers
     };

   struct LED_MODE
     {
	static const byte OFF		= 0;
	static const byte FULL_ON	= 1;
	static const byte PWM_IND	= 2;
	static const byte PWM_INDGRP	= 3;
     };

   // register names
   // datasheet, p 16
   struct REGISTER
     {
      public:
	struct MODE1
	  {
	     static const byte ADDR	= 0x00;

	     static const byte OSC_OFF	= 0x10;
	     static const byte SUB1	= 0x08;
	     static const byte SUB2	= 0x04;
	     static const byte SUB3	= 0x02;
	     static const byte ALLCALL	= 0x01;
	  };

	struct MODE2
	  {
	     static const byte ADDR	= 0x01;

	     static const byte EFCLR	= 0x80;
	     static const byte DMBLNK	= 0x20;
	     static const byte OCH	= 0x08;
	  };

	struct PWM0
	  {
	     static const byte ADDR	= 0x02;
	  };

	struct PWM1
	  {
	     static const byte ADDR	= 0x03;
	  };

	struct PWM2
	  {
	     static const byte ADDR	= 0x04;
	  };

	struct PWM3
	  {
	     static const byte ADDR	= 0x05;
	  };

	struct PWM4
	  {
	     static const byte ADDR	= 0x06;
	  };

	struct PWM5
	  {
	     static const byte ADDR	= 0x07;
	  };

	struct PWM6
	  {
	     static const byte ADDR	= 0x08;
	  };

	struct PWM7
	  {
	     static const byte ADDR	= 0x09;
	  };

	struct GRPPWM
	  {
	     static const byte ADDR	= 0x0a;
	  };

	struct GRPFREQ
	  {
	     static const byte ADDR	= 0x0b;
	  };

	struct LEDOUT0
	  {
	     static const byte ADDR	= 0x0c;
	  };

	struct LEDOUT1
	  {
	     static const byte ADDR	= 0x0d;
	  };

	struct SUBADR1
	  {
	     static const byte ADDR	= 0x0e;
	  };

	struct SUBADR2
	  {
	     static const byte ADDR	= 0x0f;
	  };

	struct SUBADR3
	  {
	     static const byte ADDR	= 0x10;
	  };

	struct ALLCALLADR
	  {
	     static const byte ADDR	= 0x11;
	  };

	struct IREF
	  {
	     static const byte ADDR	= 0x12;

	     static const byte CM	= 0x80; // current multiplier
	     static const byte HC	= 0x40; // subcurrent
	  };

	struct EFLAG
	  {
	     static const byte ADDR	= 0x13;
	  };
     };

   struct ERROR
     {
	static const uint8_t EINVAL	= 2;
     };

 public:
   // sets default I2C interface for pre-initialization commands (e.g., subaddress setting)
   static void setDefaultI2C(TwoWire i2c_default);

   // creates an instance managing drivers on the specified interface and address
   TLC59108(TwoWire i2c, const byte i2c_address);
   
   // creates an instance managing drivers on the specified address with the default interface
   TLC59108(const byte i2c_address);

   // initializes the driver by performing a hardware reset (if pin is specified) and enabling the oscillator
   uint8_t init(const uint8_t hwResetPin = 0);

   // writes a value into a single register
   uint8_t setRegister(const uint8_t reg, const uint8_t value);

   // writes values into multiple registers
   uint8_t setRegisters(const uint8_t startReg, const uint8_t values[], const uint8_t numValues);

   // reads a single register; returns -1 on error
   int readRegister(const uint8_t reg) const;

   uint8_t setBrightness(const uint8_t pwmChannel, const uint8_t dutyCycle);

   uint8_t setAllBrightness(const uint8_t dutyCycle);

   // requires an array of size 8
   uint8_t setAllBrightness(const uint8_t dutyCycles[]);

   // sets all channels to the output modes listed in REGISTER::LED_MODE
   uint8_t setLedOutputMode(const uint8_t outputMode);

 private:
   static TwoWire i2c_default;
   mutable TwoWire i2c;
   byte addr;

};

#endif // TLC59108_H

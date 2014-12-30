/*************************************************** 
  This is a library for the Adafruit DRV2605L Haptic Driver

  ----> http://www.adafruit.com/products/2306
 
  Check out the links above for our tutorials and wiring diagrams
  This motor/haptic driver uses I2C to communicate

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
  
  Modded by Jayson Owens/ja450n to include TinyWire support for ATtiny Boards
  
 ****************************************************/


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	#include <TinyWireM.h>
#else  
	#include <Wire.h>
#endif

#include <Adafruit_DRV2605.h>

/**************************************************************************/
/*! 
    @brief  Instantiates a new DRV2605 class
*/
/**************************************************************************/
// I2C, no address adjustments or pins
Adafruit_DRV2605::Adafruit_DRV2605() {
}


/**************************************************************************/
/*! 
    @brief  Setups the HW
*/
/**************************************************************************/
boolean Adafruit_DRV2605::begin() {
  
  #if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
  	TinyWireM.begin();
	#else
		Wire.begin();
	#endif
	
  uint8_t id = readRegister8(DRV2605_REG_STATUS);
  // Serial.print("Status 0x"); Serial.println(id, HEX);
  
  writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby
  
  writeRegister8(DRV2605_REG_RTPIN, 0x00); // no real-time-playback
  
  writeRegister8(DRV2605_REG_WAVESEQ1, 1); // strong click
  writeRegister8(DRV2605_REG_WAVESEQ2, 0);
  
  writeRegister8(DRV2605_REG_OVERDRIVE, 0); // no overdrive
  
  writeRegister8(DRV2605_REG_SUSTAINPOS, 0);
  writeRegister8(DRV2605_REG_SUSTAINNEG, 0);
  writeRegister8(DRV2605_REG_BREAK, 0);
  writeRegister8(DRV2605_REG_AUDIOMAX, 0x64);
  
  // ERM open loop
  
  // turn off N_ERM_LRA
  writeRegister8(DRV2605_REG_FEEDBACK, readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
  // turn on ERM_OPEN_LOOP
  writeRegister8(DRV2605_REG_CONTROL3, readRegister8(DRV2605_REG_CONTROL3) | 0x20);

  return true;
}

void Adafruit_DRV2605::setWaveform(uint8_t slot, uint8_t w) {
  writeRegister8(DRV2605_REG_WAVESEQ1+slot, w);
}

void Adafruit_DRV2605::selectLibrary(uint8_t lib) {
  writeRegister8(DRV2605_REG_LIBRARY, lib);
}

void Adafruit_DRV2605::go() {
  writeRegister8(DRV2605_REG_GO, 1);
}

void Adafruit_DRV2605::setMode(uint8_t mode) {
  writeRegister8(DRV2605_REG_MODE, mode);
}


/********************************************************************/

uint8_t Adafruit_DRV2605::readRegister8(uint8_t reg) {
	uint8_t x ;
	// use i2c
	 	#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	 	  // TinyWireM.beginTransmission(DRV2605_ADDR);
    	// TinyWireM.write((byte)reg);
    	// TinyWireM.endTransmission();
    	// TinyWireM.beginTransmission(DRV2605_ADDR);
    	// TinyWireM.requestFrom((byte)DRV2605_ADDR, (byte)1);
    	// x = TinyWireM.read();
    	// TinyWireM.endTransmission();
    	
  		TinyWireM.beginTransmission(DRV2605_ADDR);
	    TinyWireM.write(reg);
	    TinyWireM.endTransmission(false);
	    //while (TinyWireM.requestFrom(_i2caddr, 1) != 1);
	    TinyWireM.requestFrom(DRV2605_ADDR, 1);
	    return ( TinyWireM.read());
    	
	 	#else
    	Wire.beginTransmission(DRV2605_ADDR);
    	Wire.write((byte)reg);
    	Wire.endTransmission();
    	Wire.beginTransmission(DRV2605_ADDR);
    	Wire.requestFrom((byte)DRV2605_ADDR, (byte)1);
    	x = Wire.read();
    	Wire.endTransmission();
    #endif

  //  Serial.print("$"); Serial.print(reg, HEX); 
  //  Serial.print(": 0x"); Serial.println(x, HEX);
  
  return x;
}

void Adafruit_DRV2605::writeRegister8(uint8_t reg, uint8_t val) {
   // use i2c
   	#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
	 	  TinyWireM.beginTransmission(DRV2605_ADDR);
    	TinyWireM.write((byte)reg);
    	TinyWireM.write((byte)val);
    	TinyWireM.endTransmission();
	 	#else
    	Wire.beginTransmission(DRV2605_ADDR);
    	Wire.write((byte)reg);
    	Wire.write((byte)val);
    	Wire.endTransmission();
    #endif
}

/****************/


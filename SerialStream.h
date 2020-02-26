#pragma once
#ifndef INO_SERIALSTREAM_INCLUDED
#define INO_SERIALSTREAM_INCLUDED

#include "InoCore.h"

#include <Arduino.h>

namespace ino {

	class SerialStream
	{
	protected:
		HardwareSerial& SerialRef;
		unsigned long SerialBaud;

	public:
		SerialStream(HardwareSerial& SerialRef) : SerialRef(SerialRef) {}

    	virtual void begin(unsigned long SerialBaud, uint8_t Config = SERIAL_8N1) { SerialRef.begin(SerialBaud, Config); this->SerialBaud = SerialBaud; }
    	virtual void end() { SerialRef.end(); }

	};

}

#endif

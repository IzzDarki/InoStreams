#pragma once
#ifndef INO_PINSTREAM_INCLUDED
#define INO_PINSTREAM_INCLUDED

#include "InoCore.h"

#include <SoftwareSerial.h>

namespace ino {

	class PinStream
	{
	protected:
		SoftwareSerial& SerialRef;
		unsigned long SerialBaud;

	public:
		PinStream(SoftwareSerial& SerialRef) : SerialRef(SerialRef) {}

    	virtual void begin(unsigned long SerialBaud) { SerialRef.begin(SerialBaud); this->SerialBaud = SerialBaud; }
    	virtual void end() { SerialRef.end(); }

	};

}

#endif

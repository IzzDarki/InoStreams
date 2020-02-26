#pragma once
#ifndef INO_SERIALINSTREAM_INCLUDED
#define INO_SERIALINSTREAM_INCLUDED

#include "InoCore.h"
#include "InStream.h"
#include "SerialStream.h"

namespace ino {

	class SerialInStream : public InStream, virtual public SerialStream
	{
	protected:

		virtual inline bool NoDataAvailable() override { while(!Available()); return true; }
		
	public:
		SerialInStream(HardwareSerial& SerialRef) : SerialStream(SerialRef) {}

		virtual inline char Peek() const override { return SerialRef.peek(); }
		virtual inline unsigned int Available() const override { return SerialRef.available(); }
		virtual inline char Read() override { char Char = SerialRef.read(); delayMicroseconds(10000000 / SerialBaud); return Char; }

	};

}

#endif

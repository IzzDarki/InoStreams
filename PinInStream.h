#pragma once
#ifndef INO_PININSTREAM_INCLUDED
#define INO_PININSTREAM_INCLUDED

#include "InoCore.h"
#include "InStream.h"
#include "PinStream.h"

namespace ino {

	class PinInStream : public InStream, virtual public PinStream
	{
	protected:

		virtual inline bool NoDataAvailable() { while(!Available()); return true; }
		
	public:
		PinInStream(SoftwareSerial& SerialRef) : PinStream(SerialRef) {}

		virtual inline char Peek() const override { return SerialRef.peek(); }
		virtual inline unsigned int Available() const { return SerialRef.available(); }
		virtual inline char Read() override { char Char = SerialRef.read(); delayMicroseconds(10000000 / SerialBaud); return Char; }

	};

}

#endif

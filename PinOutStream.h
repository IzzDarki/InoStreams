#pragma once
#ifndef INO_PINOUTSTREAM_INCLUDED
#define INO_PINOUTSTREAM_INCLUDED

#include "InoCore.h"
#include "OutStream.h"
#include "PinStream.h"

namespace ino {

	class PinOutStream : public OutStream, virtual public PinStream
	{
	protected:
		virtual inline void Write(char Character) override { SerialRef.write(Character); }

	public:
		PinOutStream(SoftwareSerial& SerialRef) : PinStream(SerialRef) {}

	};

}

#endif
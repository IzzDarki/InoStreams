#pragma once
#ifndef INO_SERIALOUTSTREAM_INCLUDED
#define INO_SERIALOUTSTREAM_INCLUDED

#include "InoCore.h"
#include "OutStream.h"
#include "SerialStream.h"

namespace ino {

	class SerialOutStream : public OutStream, virtual public SerialStream
	{
	protected:
		virtual inline void Write(char Character) override { SerialRef.write(Character); }

	public:
		SerialOutStream(HardwareSerial& SerialRef) : SerialStream(SerialRef) {}

	};

}

#endif

#pragma once
#ifndef INO_PIOPININSTREAM_INCLUDED
#define INO_PIOPININSTREAM_INCLUDED

#include "InoCore.h"
#include "InStream.h"
#include "PinStream.h"

namespace ino {

	class PIOPinInStream : public InStream, virtual public PinStream
	{
	public:
		PIOPinInStream(SoftwareSerial& SerialRef) : PinStream(SerialRef) {}
		
		virtual inline char Peek() const override { return Buffer[0]; }
		virtual inline unsigned int Available() const override { return Buffer.length(); }

		bool Run() const;
		virtual inline char Read() override { char RetVal =  Buffer[0]; Buffer.remove(0, 1); return RetVal; }

	private:
		mutable String Buffer;

		char WaitAndRead() const;
		
	protected:

		virtual inline bool NoDataAvailable() override { while(!Run()); return true; }

	};

}

#endif

#pragma once
#ifndef INO_STRINGSTREAM_INCLUDED
#define INO_STRINGSTREAM_INCLUDED

#include "InoCore.h"
#include "OutStream.h"
#include "InStream.h"

#include <Arduino.h>

namespace ino {

	class StringStream : public OutStream, public InStream
	{
	public:
		using StringType = String;
		
	private:
		StringType Buffer;

	protected:
		virtual inline void Write(char Character) override { Buffer += Character; }

		virtual inline char Read() override { char Char = Buffer[0]; Buffer.remove(0, 1); return Char; }

	public:
		StringStream(const StringType& Buffer = "") : Buffer(Buffer) {}
		StringStream(StringType&& Buffer) : Buffer(std::move(Buffer)) {}

		inline StringType PeekBuffer() const { return Buffer; }
		
		virtual inline char Peek() const override { return Buffer[0]; }
		virtual inline unsigned int Available() const override { return Buffer.length(); }
		
	};

}

#endif

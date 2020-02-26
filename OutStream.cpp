#include "InoCore.h"
#include "OutStream.h"

namespace ino {

	char OutStream::NumToChar(uint8_t Num)
	{
		if (Num <= 9)
			return Num + 48;
		else
			return Num + 55;
	}

	char OutStream::NumToChar(uint8_t Num, const CaseFormats& Case)
	{
		if (Num <= 9)
			return Num + 48;
		else if (Case == Fmt::Uppercase)
			return Num + 55;
		else
			return Num + 87;
	}

#ifdef INO_OUTSTREAM_CURSORTRACKER
	void OutStream::InternalWrite(char Character)
	{
		if (Character == '\n')
		{
			CursorPos.Line++;
			CursorPos.Num = 0;
		}
		else if (Character == '\t')
		{
			CursorPos.Num += INO_OUTSTREAM_CURSORTRACKER_TABSIZE - (CursorPos.Num % INO_OUTSTREAM_CURSORTRACKER_TABSIZE);
		}
		else
			CursorPos.Num++;
		Write(Character);
	}
#endif

	// * ----- Default output operators (always decimal, char as character) -------------------------------------------------------

	OutStream& OutStream::operator<<(char Data) {
		InternalWrite(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(signed char Data) {
		DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(unsigned char Data) {
		DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(short Data) {
		DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(unsigned short Data) {
		DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(int Data) {
		DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(unsigned int Data) {
		DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(long Data) {
		DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(unsigned long Data) {
		DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(long long Data) {
		DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(unsigned long long Data) {
		DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(float Data) {
		DefaultFloat(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(double Data) {
		DefaultFloat(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(long double Data) {
		DefaultFloat(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(bool Data) {
		DefaultBool(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(const String& Data) {
		DefaultString(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(const volatile String& Data) {
		DefaultString(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(const char* Data) {
		DefaultCString(Data);
		FinishTransfer();
		return *this;
	}

	OutStream& OutStream::operator<<(const volatile char* Data) {
		DefaultCString(Data);
		FinishTransfer();
		return *this;
	}

}

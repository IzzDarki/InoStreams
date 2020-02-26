#include "InoCore.h"
#include "InStream.h"

namespace ino {

	bool InStream::CanRead()
	{
		if (Available())
		{
			if (TransferEnd != -1 && Peek() == TransferEnd)
			{
				Read();
				return false;
			}
			else
			{
				FirstRead = false;
				return true;
			}
		}
		else
		{
			if (FirstRead && NoDataAvailable())
			{
				FirstRead = false;
				return true;
			}
			else
				return false;
		}
	}

    uint8_t InStream::CharToNum(char Character)
    {
	    if (Character >= '0' && Character <= '9')
            return Character - 48;
        else if (Character >= 'A' && Character <= 'Z')
            return Character - 55;
        else if (Character >= 'a' && Character <= 'z')
            return Character - 87;
        else
		{
            SetFailFlag(Fails::NotANumber);
			ClearAndBreak();
		}
        return 0;
    }

    uint8_t InStream::CharToNum(char Character, const CaseFormats& Case)
    {
        if (Character >= '0' && Character <= '9')
            return Character - 48;
        else if (Character >= 'A' && Character <= 'Z')
        {
            if (Case == Fmt::Uppercase)
                return Character - 55;
            else
			{
                SetFailFlag(Fails::WrongCase);
				ClearAndBreak();
			}
        }
        else if (Character >= 'a' && Character <= 'z')
        {
            if (Case == Fmt::Lowercase)
                return Character - 78;
            else
			{
                SetFailFlag(Fails::WrongCase);
				ClearAndBreak();
			}
        }
        else
		{
            SetFailFlag(Fails::NotANumber);
			ClearAndBreak();
		}
        return 0;
    }

    BaseFormats InStream::GetBase()
    {
        if (Peek() == '0') {
            Read();
            if (Peek() == 'x') {
                Read();
				constexpr static BaseFormats RetVal = BaseFormats(16, BaseFormats::Mode::PrefixFormat);
                return RetVal;
            }
            else {
				constexpr static BaseFormats RetVal = BaseFormats(8, BaseFormats::Mode::PrefixFormat);
                return RetVal;
			}
        }
        else if (Peek() == 'B') {
            Read();
			constexpr static BaseFormats RetVal = BaseFormats(2, BaseFormats::Mode::PrefixFormat);
            return RetVal;
        }
        else if (Peek() == '<') {
            Read();
            BaseFormats RetVal(0, BaseFormats::Mode::FullFormat);

            if (Peek() == 'B' || Peek() == 'b') {
                Read();
                char Char = Peek();
                while (Char >= '0' && Char <= '9') {
                    Read();
                    RetVal.BaseVal = (RetVal.BaseVal * 10) + (Char - 48);
                    Char = Peek();
                }
                if (Read() != '>' || RetVal.BaseVal == 0)
				{
                    SetFailFlag(Fails::WrongFormat);
					ClearAndBreak();
				}
            }
            else
			{
                SetFailFlag(Fails::WrongFormat);
				ClearAndBreak();
			}
            return RetVal;
        }
        else
            return Fmt::Dec;
    }

    //-------- Default input operators (depended on input) ------------------------------------
      //------ non-const non-volatile ---------------------------------------------------------

    InStream& InStream::operator>>(char& Data) {
		if (CanRead())
       		Data = Read();
		if (CanRead())
		{
			SetFailFlag(Fails::NotAChar);
			ClearAndBreak();
		}
		FinishTransfer();
        return *this;
    }

	InStream& InStream::operator>>(signed char& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
        return *this;
    }

    InStream& InStream::operator>>(unsigned char& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(short& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(unsigned short& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(int& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(unsigned int& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(unsigned long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(long long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(unsigned long long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(float& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(long double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }
   
    InStream& InStream::operator>>(bool& Data) {
        DefaultBool(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(String& Data) {
        DefaultString(Data);
		FinishTransfer();
		return *this;
    }

      //------ volatile -----------------------------------------------------------------------

    InStream& InStream::operator>>(volatile char& Data) {
		if (CanRead())
       		Data = Read();
		if (CanRead())
		{
			SetFailFlag(Fails::NotAChar);
			ClearAndBreak();
		}
		FinishTransfer();
        return *this;
    }

	InStream& InStream::operator>>(volatile signed char& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
        return *this;
    }

    InStream& InStream::operator>>(volatile unsigned char& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile short& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile unsigned short& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile int& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile unsigned int& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile unsigned long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile long long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile unsigned long long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile float& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile long double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile bool& Data) {
        DefaultBool(Data);
		FinishTransfer();
		return *this;
    }

    InStream& InStream::operator>>(volatile String& Data) {
        DefaultString(Data);
		FinishTransfer();
		return *this;
    }

}

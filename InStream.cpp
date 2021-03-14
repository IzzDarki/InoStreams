#include "InoCore.h"
#include "InStream.h"

namespace ino {

	/**
	 * @brief Checks whether the stream can be read. The function differs from ino::InStream::Available() in that it checks whether the end of a transfer has been reached.
	 * @return Returns whether the stream can be read or not.
	 */
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

	/**
	 * @brief Converts a character to the number it represents.
	 * @details
	 * 	Ex. `(char)'3'` -> `(uint8_t)3`
	 * 	If the character is not a digit between `'0'` and `'9'` or a non-numeric digit between `'A'` and `'Z'` or `'a'` and `'z'` (intended for higher base numbers), the fail flag ino::InStream::Fails::NotANumber is set, any input operation is aborted and the stream is cleared.
	 */
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

	/**
	 * @brief Converts a character to the number it represents. Also checks the case format (uppercase and lowercase).
	 * @param Case The case format that the character must conform to.
	 * 
	 * @details
	 * 	Ex. `(char)'3'` -> `(uint8_t)3`
	 * 	If the character is not a digit between `'0'` and `'9'` or a non-numeric digit between `'A'` and `'Z'` or `'a'` and `'z'` (intended for higher base numbers), the fail flag ino::InStream::Fails::NotANumber is set, any input operation is aborted and the stream is cleared.
	 * 	If the character is a non-numeric digit that does not match the specified case format, the fail flag ino::InStream::Fails::WrongCase is set, any input operation is aborted and the stream is cleared.
	 */
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

	/**
	 * @brief Figures out the base of the data of the stream.
	 * 	It checks wheather the data of the stream contains
	 * 	- a base prefix (`"0x"` hexadecimal, `"B"` binary or `"0"` octal) (see ino::Stream::BaseFormats::Mode::PrefixFormat)
	 * 	- a fully specified base (ex. `"<B5>"` base 5) (see ino::Stream::BaseFormats::Mode::FullFormat)
	 * 
	 * @return Returns the base of the data of the stream. If no base specification could be found in the data of the stream, ino::Fmt::Dec is returned as default value.
	 * 
	 * @details If the base specification is ill-formed in any way, the fail flag ino::InStream::Fails::WrongFormat is set, any input operation is aborted and the stream is cleared.
	 */
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

	/**
	 * @brief Input operator for `char`. Reads the input as single character.
	 * @details
	 * 	If the stream contains more than one character, the fail flag ino::InStream::Fails::NotAChar is set, the input operation is aborted and the stream is cleared.
	 *	Use Read() instead to get a single character from the stream without clearing the stream afterwards.
	 */
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

	/**
	 * @brief Input operator for `signed char`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
	InStream& InStream::operator>>(signed char& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
        return *this;
    }

	/**
	 * @brief Input operator for `unsigned char`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(unsigned char& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `short`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(short& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `unsigned short`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(unsigned short& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `int`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(int& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `unsigned int`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(unsigned int& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `long`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `unsigned long`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(unsigned long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `long long`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(long long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `unsigned long long`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(unsigned long long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `float`. Reads the input as a floating point number.
	 * @details
	 * 	- Both decimal point and decimal comma are allowed
	 * 	- The input is expected to be decimal (base 10)
	 * 	- If the input contains characters that aren't decimal digits or if the input contains multiple decimal points or commas, the fail flag ino::InStream::Fails::NotANumber is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(float& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `double`. Reads the input as a floating point number.
	 * @details
	 * 	- Both decimal point and decimal comma are allowed
	 * 	- The input is expected to be decimal (base 10)
	 * 	- If the input contains characters that aren't decimal digits or if the input contains multiple decimal points or commas, the fail flag ino::InStream::Fails::NotANumber is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `long double`. Reads the input as a floating point number.
	 * @details
	 * 	- Both decimal point and decimal comma are allowed
	 * 	- The input is expected to be decimal (base 10)
	 * 	- If the input contains characters that aren't decimal digits or if the input contains multiple decimal points or commas, the fail flag ino::InStream::Fails::NotANumber is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(long double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }
   
	/**
	 * @brief Input operator for `bool`. Reads the input as a boolean.
	 * @details
	 * 	Expects either `0` and `1` (see ino::Fmt::BoolNum), `true` and `false` (see ino::Fmt::BoolWord), `True` and `False` (see ino::Fmt::BoolCapital) or `TRUE` and `FALSE` (see ino::Fmt::BoolCaps).
	 * 	Any other input leads to fail flag ino::InStream::Fails::NotABool being set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(bool& Data) {
        DefaultBool(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `String`. Reads the input as string.
	 */
    InStream& InStream::operator>>(String& Data) {
        DefaultString(Data);
		FinishTransfer();
		return *this;
    }

      //------ volatile -----------------------------------------------------------------------

	/**
	 * @brief Input operator for `volatile char`. Reads the input as single character.
	 * @details
	 * 	If the input is more than one character, the fail flag ino::InStream::Fails::NotAChar is set, the input operation is aborted and the stream is cleared.
	 */
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

	/**
	 * @brief Input operator for `volatile signed char`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
	InStream& InStream::operator>>(volatile signed char& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
        return *this;
    }

	/**
	 * @brief Input operator for `volatile unsigned char`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile unsigned char& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile short`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile short& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile unsigned short`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile unsigned short& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile int`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile int& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile unsigned int`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile unsigned int& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile long`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile unsigned long`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile unsigned long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile long long`. Reads the input as a signed integral.
	 * @details
	 * Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the FailFlag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile long long& Data) {
        DefaultSignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile unsigned lng long`. Reads the input as an unsigned integral.
	 * @details
	 * 	Input starting with a negative sign leads to the fail flag ino::InStream::Fails::NotANumber being set, the input operation is aborted and the stream is cleared.
	 * 
	 * 	Expects the input to either:
	 * 	- be decimal (see ino::Fmt::Dec)
	 * 	- specify it's base (see ino::BaseFormats). If the input and the specified base do not match, the fail flag ino::InStream::Fails::WrongBase is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile unsigned long long& Data) {
        DefaultUnsignedInt(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile float`. Reads the input as a floating point number.
	 * @details
	 * 	- Both decimal point and decimal comma are allowed
	 * 	- The input is expected to be decimal (base 10)
	 * 	- If the input contains characters that aren't decimal digits or if the input contains multiple decimal points or commas, the fail flag ino::InStream::Fails::NotANumber is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile float& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile double`. Reads the input as a floating point number.
	 * @details
	 * 	- Both decimal point and decimal comma are allowed
	 * 	- The input is expected to be decimal (base 10)
	 * 	- If the input contains characters that aren't decimal digits or if the input contains multiple decimal points or commas, the fail flag ino::InStream::Fails::NotANumber is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile long double`. Reads the input as a floating point number.
	 * @details
	 * 	- Both decimal point and decimal comma are allowed
	 * 	- The input is expected to be decimal (base 10)
	 * 	- If the input contains characters that aren't decimal digits or if the input contains multiple decimal points or commas, the fail flag ino::InStream::Fails::NotANumber is set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile long double& Data) {
        DefaultFloat(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile bool`. Reads the input as a boolean.
	 * @details
	 * 	Expects either `0` and `1` (see ino::Fmt::BoolNum), `true` and `false` (see ino::Fmt::BoolWord), `True` and `False` (see ino::Fmt::BoolCapital) or `TRUE` and `FALSE` (see ino::Fmt::BoolCaps).
	 * 	Any other input leads to fail flag ino::InStream::Fails::NotABool being set, the input operation is aborted and the stream is cleared.
	 */
    InStream& InStream::operator>>(volatile bool& Data) {
        DefaultBool(Data);
		FinishTransfer();
		return *this;
    }

	/**
	 * @brief Input operator for `volatile String`. Reads the input as string.
	 */
    InStream& InStream::operator>>(volatile String& Data) {
        DefaultString(Data);
		FinishTransfer();
		return *this;
    }

}

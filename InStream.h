#pragma once
#ifndef INO_INSTREAM_INCLUDED
#define INO_INSTREAM_INCLUDED

#include "InoCore.h"
#include "StreamBase.h"
#include "Utility.h"
#include "std/TypeTraits.h"

#include <Arduino.h>

#define INO_INSTREAM_BIT(x) (1 << x)

namespace ino {

	class InStream : virtual public StreamBase
	{
	public:
		enum class Fails {
			NoFail = 0,
			NotANumber = INO_INSTREAM_BIT(0),
			NotABool = INO_INSTREAM_BIT(2),
			NotAChar = INO_INSTREAM_BIT(3),
			WrongFormat = INO_INSTREAM_BIT(4),
			WrongBase = INO_INSTREAM_BIT(5),
			WrongCase = INO_INSTREAM_BIT(6),
			WrongDecimalPoint = INO_INSTREAM_BIT(7),
			WrongPrecision = INO_INSTREAM_BIT(8),
			WrongCString = INO_INSTREAM_BIT(10),
			NoData = INO_INSTREAM_BIT(11),
		};

	private:
		Fails FailFlags;
		bool FirstRead = true;
		
	protected:
		inline void SetFailFlag(Fails Flag) { FailFlags = static_cast<Fails>(static_cast<typename std::underlying_type<Fails>::type>(FailFlags) | static_cast<typename std::underlying_type<Fails>::type>(Flag)); }
		virtual inline bool NoDataAvailable() { SetFailFlag(Fails::NoData); return false; }

	public:
		inline bool Failed() const { return FailFlags != Fails::NoFail; }
		inline bool Failed(Fails TestFlag) const { return static_cast<typename std::underlying_type<Fails>::type>(FailFlags) & static_cast<typename std::underlying_type<Fails>::type>(TestFlag); }

		inline void ClearFails() { FailFlags = Fails::NoFail; }
		inline void ClearFailFlag(Fails Flag) { FailFlags = static_cast<Fails>(static_cast<typename std::underlying_type<Fails>::type>(FailFlags) & ~static_cast<typename std::underlying_type<Fails>::type>(Flag)); }

		virtual char Peek() const = 0;
		virtual unsigned int Available() const = 0;
		virtual char Read() = 0;

		inline void Clear() { while (CanRead()) Read(); FinishTransfer(); }

		// * ----- Default input operators (depended on input) --------------------------------------------------------------------
		  //------ non-const non-volatile -----------------------------------------------------------------------------------------

		InStream& operator>>(char& Data);
		InStream& operator>>(signed char& Data);
		InStream& operator>>(unsigned char& Data);
		InStream& operator>>(short& Data);
		InStream& operator>>(unsigned short& Data);
		InStream& operator>>(int& Data);
		InStream& operator>>(unsigned int& Data);
		InStream& operator>>(long& Data);
		InStream& operator>>(unsigned long& Data);
		InStream& operator>>(long long& Data);
		InStream& operator>>(unsigned long long& Data);

		InStream& operator>>(float& Data);
		InStream& operator>>(double& Data);
		InStream& operator>>(long double& Data);

		InStream& operator>>(bool& Data);
		InStream& operator>>(String& Data);

			 //------ volatile ------------------------------------------------------------------------------------------------------

		InStream& operator>>(volatile char& Data);
		InStream& operator>>(volatile signed char& Data);
		InStream& operator>>(volatile unsigned char& Data);
		InStream& operator>>(volatile short& Data);
		InStream& operator>>(volatile unsigned short& Data);
		InStream& operator>>(volatile int& Data);
		InStream& operator>>(volatile unsigned int& Data);
		InStream& operator>>(volatile long& Data);
		InStream& operator>>(volatile unsigned long& Data);
		InStream& operator>>(volatile long long& Data);
		InStream& operator>>(volatile unsigned long long& Data);

		InStream& operator>>(volatile float& Data);
		InStream& operator>>(volatile double& Data);
		InStream& operator>>(volatile long double& Data);

		InStream& operator>>(volatile bool& Data);
		InStream& operator>>(volatile String& Data);


		// * ----- Baseformat-specific input operators ----------------------------------------------------------------------------
		  //------ Signed integral output operator --------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<IsSigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const BaseFormat<T>& Data);

		  //------ Unsigned integral output operator ------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<IsUnsigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const BaseFormat<T>& Data);


		// * ----- Caseformat-specific input operators ----------------------------------------------------------------------------
		  //------ Char output operator -------------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<char, typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const CaseFormat<T>& Data);

		  //------ Char* and Char[] input operator --------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<(std::is_same<char*, typename ReduceTypeExceptConst<T>::type>::value || (std::is_array<typename std::remove_reference<T>::type>::value && std::is_same<char, typename ElementType<T>::type>::value)) && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const CaseFormat<T>& Data);

		  //------ String input operator ------------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<String, typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const CaseFormat<T>& Data);


		// * ----- Boolformat-specific input operators ----------------------------------------------------------------------------
		  //------ Bool input operator --------------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<bool, typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const BoolFormat<T>& Data);

		// * ----- Decimalpointformat-specific input operators --------------------------------------------------------------------
		  //------ Floating-point input operator ----------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const DecimalpointFormat<T>& Data);


		// * ----- Precisionformat-specific input operators -----------------------------------------------------------------------
		  //------ Floating-point input operator ----------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const PrecisionFormat<T>& Data);


		// * ----- Specialnumberformat-specific input operators -------------------------------------------------------------------
		  //------ Floating-point input operator ----------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const SpecialnumberFormat<T>& Data);


		// * ----- CStringformat-specific input operators -------------------------------------------------------------------------
		  //------ Char* and Char[] input operator --------------------------------------------------------------------------------
			
		template <typename T, typename std::enable_if<(std::is_same<char*, typename ReduceTypeExceptConst<T>::type>::value || IsElementTypeConsiderConst<char, T>::value) && std::is_lvalue_reference<T>::value, int>::type = 0>
		InStream& operator>>(const CStringFormat<T>& Data);


		// * ----- Multiple format-specific output operators ----------------------------------------------------------------------
		  //------ Signed integral output operator (base- and case-specific) ------------------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<IsSigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);

		  //------ Unsigned integral input operator (base- and case-specific) -----------------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<IsUnsigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point input operator (decimalpoint- and precision-specific) -------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point input operator (decimalpoint- and specialnumber-specific) ---------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point input operator (precision- and specialnumber-specific) ------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point input operator (decimalpoint-, precision- and specialnumber-specific) ---------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 3 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);

		  //------ Char* and Char[] input operator (CString- and Case-specific) ---------------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<(std::is_same<char*, typename ReduceTypeExceptConst<T>::type>::value || IsElementTypeConsiderConst<char, T>::value) && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<CStringFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type = 0>
		InStream& operator>>(const MultiFormat<T, FmtTs...>& Data);


	protected: // Helper functions
		inline bool GetSign()
		{
			if (Peek() == '-')
			{
				Read();
				return true;
			}
			return false;
		}
		uint8_t CharToNum(char Character);
		uint8_t CharToNum(char Character, const CaseFormats& Case);
		BaseFormats GetBase();

	protected: // Underlaying stream functions
		bool CanRead();

		inline void ClearAndBreak()
		{
			while(CanRead()) Read();
		}

		inline void FinishTransfer()
		{
			FirstRead = true;
		}

		template <typename T>
		inline void DefaultUnsignedInt(T&& Data)
		{
			if (CanRead())
			{
				Data = 0;
				auto Base = GetBase();
				while (CanRead())
				{
					auto Num = CharToNum(Read());
					if (Num >= Base.BaseVal)
					{
						SetFailFlag(Fails::WrongBase);
						ClearAndBreak();
						return;
					}
					Data = (Data * Base.BaseVal) + Num;
				}
			}
		}
		template <typename T>
		inline void DefaultUnsignedInt(T&& Data, const BaseFormats& Base)
		{
			if (CanRead())
			{
				Data = 0;
				auto DetcBase = GetBase();
				if (Base != DetcBase)
					SetFailFlag(Fails::WrongBase);
				while (CanRead())
				{
					auto Num = CharToNum(Read());
					if (Num >= Base.BaseVal)
					{
						SetFailFlag(Fails::WrongBase);
						ClearAndBreak();
						return;
					}
					Data = (Data * Base.BaseVal) + Num;
				}
				if (Data == 0)
					ClearFailFlag(Fails::WrongBase);
			}
		}
		template <typename T>
		inline void DefaultUnsignedInt(T&& Data, const CaseFormats& Case)
		{
			if (CanRead())
			{
				Data = 0;
				auto Base = GetBase();
				while (CanRead())
				{
					auto Num = CharToNum(Read(), Case);
					if (Num > Base.BaseVal)
					{
						SetFailFlag(Fails::WrongBase);
						ClearAndBreak();
						return;
					}
					Data = (Data * Base.BaseVal) + Num;
				}
			}
		}
		template <typename T>
		inline void DefaultUnsignedInt(T&& Data, const BaseFormats& Base, const CaseFormats& Case)
		{
			if (CanRead())
			{
				Data = 0;
				auto DetcBase = GetBase();
				if (Base != DetcBase)
					SetFailFlag(Fails::WrongBase);
				while (CanRead())
				{
					auto Num = CharToNum(Read(), Case);
					if (Num >= Base.BaseVal)
					{
						SetFailFlag(Fails::WrongBase);
						ClearAndBreak();
						return;
					}
					Data = (Data * Base.BaseVal) + Num;
				}
				if (Data == 0)
					ClearFailFlag(Fails::WrongBase);
			}
		}

		template <typename T>
		inline void DefaultSignedInt(T&& Data)
		{
			if (CanRead())
			{
				Data = 0;
				bool Negative = GetSign();
				DefaultUnsignedInt(Data);
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultSignedInt(T&& Data, const BaseFormats& Base)
		{
			if (CanRead())
			{
				Data = 0;
				bool Negative = GetSign();
				DefaultUnsignedInt(Data, Base);
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultSignedInt(T&& Data, const CaseFormats& Case)
		{
			if (CanRead())
			{
				Data = 0;
				bool Negative = GetSign();
				DefaultUnsignedInt(Data, Case);
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultSignedInt(T&& Data, const BaseFormats& Base, const CaseFormats& Case)
		{
			if (CanRead())
			{
				Data = 0;
				bool Negative = GetSign();
				DefaultUnsignedInt(Data, Base, Case);
				if (Negative)
					Data = -Data;
			}
		}

		template <typename T>
		inline void DefaultFloat(T&& Data)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Fmt::SpecialnumberShort.Nan)
						Data = NAN;
					else if (Str == Fmt::SpecialnumberShort.PosInf)
						Data = INFINITY;
					else if (Str == Fmt::SpecialnumberShort.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}

				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead())
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const DecimalpointFormats& Decimalpoint)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Fmt::SpecialnumberShort.Nan)
						Data = NAN;
					else if (Str == Fmt::SpecialnumberShort.PosInf)
						Data = INFINITY;
					else if (Str == Fmt::SpecialnumberShort.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}

				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead())
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if ((Decimalpoint == Fmt::DecimalDot && Char != '.') || (Decimalpoint == Fmt::DecimalComma && Char != ','))
						{
							SetFailFlag(Fails::WrongDecimalPoint);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const PrecisionFormats& Precision)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Fmt::SpecialnumberShort.Nan)
						Data = NAN;
					else if (Str == Fmt::SpecialnumberShort.PosInf)
						Data = INFINITY;
					else if (Str == Fmt::SpecialnumberShort.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}
				
				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead() && DecimalPlace <= Precision.PrecisionVal)
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (CanRead())
				{
					SetFailFlag(Fails::WrongPrecision);
					ClearAndBreak();
					return;
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const SpecialnumberFormats& Specialnum)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Specialnum.Nan)
						Data = NAN;
					else if (Str == Specialnum.PosInf)
						Data = INFINITY;
					else if (Str == Specialnum.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}

				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead())
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const DecimalpointFormats& Decimalpoint, const PrecisionFormats& Precision)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Fmt::SpecialnumberShort.Nan)
						Data = NAN;
					else if (Str == Fmt::SpecialnumberShort.PosInf)
						Data = INFINITY;
					else if (Str == Fmt::SpecialnumberShort.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}

				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead() && DecimalPlace <= Precision.PrecisionVal)
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if ((Decimalpoint == Fmt::DecimalDot && Char != '.') || (Decimalpoint == Fmt::DecimalComma && Char != ','))
						{
							SetFailFlag(Fails::WrongDecimalPoint);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (CanRead())
				{
					SetFailFlag(Fails::WrongPrecision);
					ClearAndBreak();
					return;
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const DecimalpointFormats& Decimalpoint, const SpecialnumberFormats& Specialnum)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Specialnum.Nan)
						Data = NAN;
					else if (Str == Specialnum.PosInf)
						Data = INFINITY;
					else if (Str == Specialnum.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}

				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead())
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if ((Decimalpoint == Fmt::DecimalDot && Char != '.') || (Decimalpoint == Fmt::DecimalComma && Char != ','))
						{
							SetFailFlag(Fails::WrongDecimalPoint);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const PrecisionFormats& Precision, const SpecialnumberFormats& Specialnum)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Specialnum.Nan)
						Data = NAN;
					else if (Str == Specialnum.PosInf)
						Data = INFINITY;
					else if (Str == Specialnum.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}
				
				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead() && DecimalPlace <= Precision.PrecisionVal)
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + CharToNum(Digit);
					}
				}
				if (CanRead())
				{
					SetFailFlag(Fails::WrongPrecision);
					ClearAndBreak();
					return;
				}
				if (Negative)
					Data = -Data;
			}
		}
		template <typename T>
		inline void DefaultFloat(T&& Data, const DecimalpointFormats& Decimalpoint, const PrecisionFormats& Precision, const SpecialnumberFormats& Specialnum)
		{
			if (CanRead())
			{
				if ((Peek() < '0' || Peek() > '9') && Peek() != '-')
				{
					String Str;
					DefaultString(Str);
					if (Str == Specialnum.Nan)
						Data = NAN;
					else if (Str == Specialnum.PosInf)
						Data = INFINITY;
					else if (Str == Specialnum.NegInf)
						Data = -INFINITY;
					else
					{
						SetFailFlag(Fails::NotANumber);
						ClearAndBreak();
						return;
					}
					return;
				}
				
				Data = 0;
				bool Negative = GetSign();
				uint8_t DecimalPlace = 0;
				while (CanRead() && DecimalPlace <= Precision.PrecisionVal)
				{
					auto Char = Read();
					if (Char == '.' || Char == ',')
					{
						if ((Decimalpoint == Fmt::DecimalDot && Char != '.') || (Decimalpoint == Fmt::DecimalComma && Char != ','))
						{
							SetFailFlag(Fails::WrongDecimalPoint);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						DecimalPlace = 1;
					}
					else
					{
						auto Digit = CharToNum(Char);
						if (Digit >= 10)
						{
							SetFailFlag(Fails::NotANumber);
							ClearAndBreak();
							return;
						}
						if (DecimalPlace)
							Data += static_cast<typename std::decay<T>::type>(Digit) / Power(10, DecimalPlace++);
						else
							Data = (Data * 10) + Digit;
					}
				}
				if (CanRead())
				{
					SetFailFlag(Fails::WrongPrecision);
					ClearAndBreak();
					return;
				}
				if (Negative)
					Data = -Data;
			}
		}

		template <typename T>
		inline void DefaultBool(T&& Data)
		{
			if (CanRead())
			{
				if (Peek() == '0')
				{
					Data = false;
					Read();
					if (CanRead())
					{
						SetFailFlag(Fails::NotABool);
						ClearAndBreak();
						return;
					}
				}
				else if (Peek() == '1')
				{
					Data = true;
					Read();
					if (CanRead())
					{
						SetFailFlag(Fails::NotABool);
						ClearAndBreak();
						return;
					}
				}
				else
				{
					String Bool;
					DefaultString(Bool);
					if (Bool == Fmt::BoolWord.BoolTrue || Bool == Fmt::BoolCapital.BoolTrue || Bool == Fmt::BoolCaps.BoolTrue)
						Data = true;
					else if (Bool == Fmt::BoolWord.BoolFalse || Bool == Fmt::BoolCapital.BoolFalse || Bool == Fmt::BoolCaps.BoolFalse)
						Data = false;
					else
						SetFailFlag(Fails::NotABool);
				}
			}
		}
		template <typename T>
		inline void DefaultBool(T&& Data, const BoolFormats& BoolWord)
		{
			String Bool;
			DefaultString(Bool);
			if (Bool == BoolWord.BoolTrue)
				Data = true;
			else if (Bool == BoolWord.BoolFalse)
				Data = false;
			else
				SetFailFlag(Fails::NotABool);
		}

		template <typename T>
		inline void DefaultChar(T&& Data, const CaseFormats& Case)
		{
			if (CanRead())
			{
				Data = Read();
				if (Case == Fmt::Uppercase && (Data >= 'a' && Data <= 'z'))
				{
					SetFailFlag(Fails::WrongCase);
					ClearAndBreak();
					return;
				}
				else if (Case == Fmt::Lowercase && (Data >= 'A' && Data <= 'Z'))
				{
					SetFailFlag(Fails::WrongCase);
					ClearAndBreak();
					return;
				}
			}
		}

		template <typename T>
		inline void DefaultString(T&& Data)
		{
			if (CanRead())
			{
				Data = "";
				while (CanRead())
					Data += String(Read());
			}
		}
		template <typename T>
		inline void DefaultString(T&& Data, const CaseFormats& Case)
		{
			if (CanRead())
			{
				Data = "";
				while (CanRead())
				{
					char Char;
					DefaultChar(Char, Case);
					Data += String(Char);
				}
			}
		}

		template <typename T>
		inline void DefaultCString(T* Data, const CStringFormats& CString)
		{
			if (CString.StringMode == CStringFormats::Mode::Exact)
			{
				int C = 0;
				for (; CanRead() && C < CString.StringSize - 1; C++)
					Data[C] = Read();
				Data[C] = '\0';
				if (C < CString.StringSize - 1)
				{
					SetFailFlag(Fails::WrongCString);
					ClearAndBreak();
					return;
				}
			}
			else
			{
				int C = 0;
				for (; CanRead() && C < CString.StringSize - 1; C++)
				{
					char Input = Read();
					if (Input == '\0')
						break;
					Data[C] = Input;
				}
				Data[C] = '\0';
			}
			if (CanRead())
			{
				SetFailFlag(Fails::WrongCString);
				ClearAndBreak();
				return;
			}
		}
		template <typename T>
		inline void DefaultCString(T* Data, const CStringFormats& CString, const CaseFormats& Case)
		{
			if (CString.StringMode == CStringFormats::Mode::Exact)
			{
				int C = 0;
				for (; CanRead() && C < CString.StringSize - 1; C++)
					DefaultChar(Data[C], Case);
				Data[C] = '\0';
				if (C < CString.StringSize - 1)
				{
					SetFailFlag(Fails::WrongCString);
					ClearAndBreak();
					return;
				}
			}
			else
			{
				int C = 0;
				for (; CanRead() && C < CString.StringSize - 1; C++)
				{
					if (Peek() == '\0')
						ClearAndBreak();
					DefaultChar(Data[C], Case);
				}
				Data[C] = '\0';
			}
			if (CanRead())
			{
				SetFailFlag(Fails::WrongCString);
				ClearAndBreak();
				return;
			}
		}

	};

	// * ----- Baseformat-specific input operators --------------------------------------------------------------------------------
	  //------ Signed integral output operator ------------------------------------------------------------------------------------

	/**
	 * TODO rewrite this (old)
	 * @brief Input operator for signed integrals with BaseFormat
	 * @details If the stream cannot be read with the specified basis, reading is aborted and the Fail Flag ::ino::InStream::Fails::WrongBase is set
	 */
	template <typename T, typename std::enable_if<IsSigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const BaseFormat<T>& Data) {
		DefaultSignedInt(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}

	  //------ Unsigned integral output operator ----------------------------------------------------------------------------------
	
	template <typename T, typename std::enable_if<IsUnsigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const BaseFormat<T>& Data) {
		DefaultUnsignedInt(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Caseformat-specific input operators --------------------------------------------------------------------------------
	  //------ Char output operator -----------------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<std::is_same<char, typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const CaseFormat<T>& Data) {
		DefaultChar(Data.Var, Data.Val);
		if (CanRead()) {
			SetFailFlag(Fails::NotAChar);
			ClearAndBreak();
		}
		FinishTransfer();
		return *this;
	}

	  //------ String input operator ----------------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<std::is_same<String, typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const CaseFormat<T>& Data) {
		DefaultString(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Boolformat-specific input operators --------------------------------------------------------------------------------
	  //------ Bool input operator ------------------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<std::is_same<bool, typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const BoolFormat<T>& Data) {
		DefaultBool(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Decimalpointformat-specific input operators ------------------------------------------------------------------------
	  //------ Floating-point input operator --------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const DecimalpointFormat<T>& Data) {
		DefaultFloat(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Precisionformat-specific input operators ---------------------------------------------------------------------------
	  //------ Floating-point input operator --------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const PrecisionFormat<T>& Data) {
		DefaultFloat(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Specialnumberformat-specific input operators -----------------------------------------------------------------------
	  //------ Floating-point input operator --------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const SpecialnumberFormat<T>& Data) {
		DefaultFloat(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}

	// * ----- CStringformat-specific input operators -----------------------------------------------------------------------------
	  //------ Char* and Char[] input operator ------------------------------------------------------------------------------------
		
	template <typename T, typename std::enable_if<(std::is_same<char*, typename ReduceTypeExceptConst<T>::type>::value || IsElementTypeConsiderConst<char, T>::value) && std::is_lvalue_reference<T>::value, int>::type>
	InStream& InStream::operator>>(const CStringFormat<T>& Data) {
		DefaultCString(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Multiple format-specific output operators --------------------------------------------------------------------------
	  //------ Signed integral output operator (base- and case-specific) ----------------------------------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<IsSigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultSignedInt(Data.Var, Data.template Get<BaseFormats>(), Data.template Get<CaseFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Unsigned integral input operator (base- and case-specific) --------------------------------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<IsUnsigned<typename ReduceTypeExceptConst<T>::type>::value && std::is_integral<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultUnsignedInt(Data.Var, Data.template Get<BaseFormats>(), Data.template Get<CaseFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point input operator (decimalpoint- and precision-specific) ----------------------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<DecimalpointFormats>(), Data.template Get<PrecisionFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point input operator (decimalpoint- and specialnumber-specific) ------------------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<DecimalpointFormats>(), Data.template Get<SpecialnumberFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point input operator (precision- and specialnumber-specific) ---------------------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<PrecisionFormats>(), Data.template Get<SpecialnumberFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point input operator (decimalpoint-, precision- and specialnumber-specific) ------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 3 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<DecimalpointFormats>(), Data.template Get<PrecisionFormats>(), Data.template Get<SpecialnumberFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Char* and Char[] input operator (CString- and Case-specific) ------------------------------------------------------
		
	template <typename T, typename... FmtTs, typename std::enable_if<(std::is_same<char*, typename ReduceTypeExceptConst<T>::type>::value || IsElementTypeConsiderConst<char, T>::value) && std::is_lvalue_reference<T>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<CStringFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type>
	InStream& InStream::operator>>(const MultiFormat<T, FmtTs...>& Data) {
		DefaultCString(Data.Var, Data.template Get<CStringFormats>(), Data.template Get<CaseFormats>());
		FinishTransfer();
		return *this;
	}

}

#undef INO_INSTREAM_BIT

#endif

#pragma once
#ifndef INO_OUTSTREAM_INCLUDED
#define INO_OUTSTREAM_INCLUDED

#include "InoCore.h"
#include "StreamBase.h"
#include "Utility.h"

#include <Arduino.h>

#ifndef INO_OUTSTREAM_NANFUNC
#include <math.h>
#define INO_OUTSTREAM_NANFUNC(Arg) isnan(Arg)
#endif

#ifndef INO_OUTSTREAM_INFFUNC
#include <math.h>
#define INO_OUTSTREAM_INFFUNC(Arg) (isinf(Arg) == 1)
#endif

#ifndef INO_OUTSTREAM_NINFFUNC
#include "IEEE754.h"
#define INO_OUTSTREAM_NINFFUNC(Arg) ::ino::IEEE754::isninf(Arg)
#endif

#ifdef INO_OUTSTREAM_CURSORTRACKER
#ifndef INO_OUTSTREAM_CURSORTRACKER_TABSIZE
#define INO_OUTSTREAM_CURSORTRACKER_TABSIZE 8
#endif
#endif

namespace ino {

	class OutStream : virtual public StreamBase
	{
	protected:
		virtual void Write(char Character) = 0;

	public:

		// * ----- Default output operators (always decimal, char as character) -------------------------------------------------------

		OutStream& operator<<(char Data);
		OutStream& operator<<(signed char Data);
		OutStream& operator<<(unsigned char Data);
		OutStream& operator<<(short Data);
		OutStream& operator<<(unsigned short Data);
		OutStream& operator<<(int Data);
		OutStream& operator<<(unsigned int Data);
		OutStream& operator<<(long Data);
		OutStream& operator<<(unsigned long Data);
		OutStream& operator<<(long long Data);
		OutStream& operator<<(unsigned long long Data);

		OutStream& operator<<(float Data);
		OutStream& operator<<(double Data);
		OutStream& operator<<(long double Data);

		OutStream& operator<<(bool Data);

		OutStream& operator<<(const String& Data);
		OutStream& operator<<(const volatile String& Data);
		OutStream& operator<<(const char* Data);
		OutStream& operator<<(const volatile char* Data);


		// * ----- Base-specific output operators -------------------------------------------------------------------------------------
		  //------ Signed integral output operator ------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<IsSigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const BaseFormat<T>& Data);

		  //------ Unsigned integral output operator ----------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<IsUnsigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const BaseFormat<T>& Data);


		// * ----- Caseformat-specific output operators -------------------------------------------------------------------------------
		  //------ Char output operator -----------------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<char, typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const CaseFormat<T>& Data);

		  //------ Char* and Char[] output operator -----------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<char*, typename ReduceType<T>::type>::value || IsElementType<char, T>::value, int>::type = 0>
		OutStream& operator<<(const CaseFormat<T>& Data);

		  //------ String output operator ---------------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<String, typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const CaseFormat<T>& Data);


		// * ----- Boolformat-specific output operators -------------------------------------------------------------------------------
		  //------ Bool output operator -----------------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<bool, typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const BoolFormat<T>& Data);


		// * ----- Decimalpointformat-specific output operators -----------------------------------------------------------------------
		  //------ Floating-point output operator -------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const DecimalpointFormat<T>& Data);


		// * ----- Precisionformat-specific output operators --------------------------------------------------------------------------
		  //------ Floating-point output operator -------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const PrecisionFormat<T>& Data);


		// * ----- Specialnumberformat-specific output operators ----------------------------------------------------------------------
		  //------ Floating-point output operator -------------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value, int>::type = 0>
		OutStream& operator<<(const SpecialnumberFormat<T>& Data);


		// * ----- CStringformat-specific output operators ----------------------------------------------------------------------------
		  //------ Char* and Char[] output operator -----------------------------------------------------------------------------------

		template <typename T, typename std::enable_if<std::is_same<char*, typename ReduceType<T>::type>::value || IsElementType<char, T>::value, int>::type = 0>
		OutStream& operator<<(const CStringFormat<T>& Data);


		// * ----- Multiple format-specific output operators --------------------------------------------------------------------------
		  //------ Signed integral output operator (base- and case-specific) ----------------------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<IsSigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);

		  //------ Unsigned integral output operator (base- and case-specific) --------------------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<IsUnsigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point output operator (decimalpoint- and precision-specific) ----------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point output operator (decimalpoint- and specialnumber-specific) ------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point output operator (precision- and specialnumber-specific) ---------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);

		  //------ Floating-point output operator (decimalpoint-, precision- and specialnumber-specific) ------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 3 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);

		  //------ Char* and Char[] output operator (cstring- and case-specific) -----------------------------------------------------

		template <typename T, typename... FmtTs, typename std::enable_if<(std::is_same<char*, typename ReduceType<T>::type>::value || IsElementType<char, T>::value) && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<CStringFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type = 0>
		OutStream& operator<<(const MultiFormat<T, FmtTs...>& Data);
	
#ifdef INO_OUTSTREAM_CURSORTRACKER
	public: 
		struct CursorPosition { unsigned short Num = 0, Line = 0; };

	private:
		CursorPosition CursorPos;
		void InternalWrite(char Character);

	public:
		inline const CursorPosition& GetCursorPos() const { return CursorPos; }
#endif

	private: // Private helper functions
		template <size_t size, typename std::enable_if<size == 1, int>::type = 1>
		constexpr static int GetMaxLengthDecHelper() { return 3; }
		template <size_t size, typename std::enable_if<size == 2, int>::type = 1>
		constexpr static int GetMaxLengthDecHelper() { return 5; }
		template <size_t size, typename std::enable_if<size == 4, int>::type = 1>
		constexpr static int GetMaxLengthDecHelper() { return 10; }
		template <size_t size, typename std::enable_if<size == 8, int>::type = 1>
		constexpr static int GetMaxLengthDecHelper() { return 20; }

		template <typename T, bool IsIntegral>
		struct IntegralTypeHelper
		{
			using type = T;
		};
		template <typename T>
		struct IntegralTypeHelper<T, false>
		{
			using type = int;
		};

#ifndef INO_OUTSTREAM_CURSORTRACKER
		inline void InternalWrite(char Character) { Write(Character); }
#endif

	protected: // Helper functions
		template <size_t size>
		struct GetMaxLengthDec : std::integral_constant<int, GetMaxLengthDecHelper<size>()> {};

		template <typename T>
		struct IntegralType
		{
			using type = typename IntegralTypeHelper<T, std::is_integral<T>::value>::type;
		};

		template <typename T>
		static inline T GetDecimalPart(T Num)
		{
			T DecimalPart;
			ModFunction(Num, DecimalPart);
			return DecimalPart;
		}

		struct DecimalsRound {
			uint8_t Decimals;
			bool Round;
		};

		template <typename T>
		static DecimalsRound GetTotalDecimals(T Num)
		{
			DecimalsRound ReturnVal = { 0, false };
			uint8_t Zeros = 0;
			uint8_t Nines = 0;
			Num = abs(Num) - GetDecimalPart(Num);
			while (Zeros < 2 && Nines < 2 && ReturnVal.Decimals < 6)
			{
				Num *= 10;
				if (static_cast<int>(Num) == 0)
				{
					Zeros++;
					ReturnVal.Decimals += Nines;
					Nines = 0;
				}
				else if (static_cast<int>(Num) == 9)
				{
					Nines++;
					ReturnVal.Decimals += Zeros;
					Zeros = 0;
					Num -= 9;
				}
				else {
					ReturnVal.Decimals++;
					ReturnVal.Decimals += (Zeros + Nines);
					Zeros = 0;
					Nines = 0;
					Num -= static_cast<int>(Num);
				}
			}
			if (Zeros && static_cast<int>(Num *= 10) == 9)
			{
				ReturnVal.Decimals += Zeros;
				ReturnVal.Round = true;
			}	
			if (Nines)
				ReturnVal.Round = true;

			return ReturnVal;
		}

		static inline String GetBaseFullFormat(uint8_t BaseVal)
		{
			return String("<B") + String(BaseVal) + String(">");
		}

		static inline String GetBasePrefix(uint8_t BaseVal)
		{
			constexpr const char* BinPrefix = "B";
			constexpr const char* OctPrefix = "0";
			constexpr const char* DecPrefix = "";
			constexpr const char* HexPrefix = "0x";

			if      (BaseVal == 10) return DecPrefix;
			else if (BaseVal == 16) return HexPrefix;
			else if (BaseVal == 2)  return BinPrefix;
			else if (BaseVal == 8)  return OctPrefix;
			else return GetBaseFullFormat(BaseVal);
		}

		static char NumToChar(uint8_t Num, const CaseFormats&);
		static char NumToChar(uint8_t Num);

	protected: // Underlaying stream functions
		template <typename T>
		inline void Negative(T&& Num)
		{
			//Serial.println("Custom note from .......Negative(T&&)");
			//Serial.println(Num < 0 ? "negative" : "positive");
			if (Num < 0)
			{
				InternalWrite('-');
				Num = -Num;
			}
		}

		inline void FinishTransfer()
		{
			if (TransferEnd != -1)
				InternalWrite(TransferEnd);
		}
		
		template <typename T>
		inline void DefaultCString(const T* Data)
		{
			for (int C = 0; Data[C] != '\0'; C++)
				InternalWrite(Data[C]);
		}
		template <typename T>
		inline void DefaultCString(const T* Data, const CaseFormats& Case)
		{
			for (int C = 0; Data[C] != '\0'; C++)
				DefaultChar(Data[C], Case);
		}
		template <typename T>
		inline void DefaultCString(const T* Data, const CStringFormats& CString)
		{
			if (CString.StringMode == CStringFormats::Mode::Exact)
			{
				for (int C = 0; C < CString.StringSize - 1; C++) {
					InternalWrite(Data[C]);
				}
			}
			else
			{
				for (int C = 0; C < CString.StringSize - 1; C++)
				{
					if (Data[C] == '\0') {
						break;
					}
					InternalWrite(Data[C]);
				}
			}
		}
		template <typename T>
		inline void DefaultCString(const T* Data, const CStringFormats& CString, const CaseFormats& Case)
		{
			if (CString.StringMode == CStringFormats::Mode::Exact)
			{
				for (int C = 0; C < CString.StringSize - 1; C++)
					DefaultChar(Data[C], Case);
			}
			else
			{
				for (int C = 0; C < CString.StringSize - 1; C++)
				{
					if (Data[C] == '\0')
						break;
					DefaultChar(Data[C], Case);
				}
			}
		}

		template <typename T>
		inline void DefaultUnsignedInt(T Num)
		{
			constexpr size_t MaxLength = GetMaxLengthDec<sizeof(typename std::remove_reference<T>::type)>::value + 1;
			char CString[MaxLength];

			int Pos = 0;
			do {
				CString[Pos++] = NumToChar(ModOperator(Num, 10));
				Num /= 10;
			} while (static_cast<typename IntegralType<T>::type>(Num));

			do {
				InternalWrite(CString[--Pos]);
			} while (Pos > 0);
		}
		template <typename T>
		inline void DefaultUnsignedInt(T Num, const BaseFormats& Base)
		{
			constexpr size_t MaxLength = 8 * sizeof(typename std::remove_reference<T>::type) + 1;
			char CString[MaxLength];

			if (Base.BaseMode == BaseFormats::Mode::PrefixFormat)
				DefaultString(GetBasePrefix(Base.BaseVal));
			else if (Base.BaseMode == BaseFormats::Mode::FullFormat)
				DefaultString(GetBaseFullFormat(Base.BaseVal));
#if INO_DEBUG
			if (Base.BaseVal < 2)
			{
				DefaultCString("Error: ino runtime debug error: Tried to print with base less than 2 (from ");
				DefaultCString(__FILE__);
				InternalWrite(':');
				DefaultUnsignedInt(__LINE__);
				DefaultCString(")\n");
			}
#endif
			int Pos = 0;
			do {
				CString[Pos++] = NumToChar(ModOperator(Num, Base.BaseVal));
				Num /= Base.BaseVal;
			} while (static_cast<typename IntegralType<T>::type>(Num));

			do {
				InternalWrite(CString[--Pos]);
			} while (Pos > 0);
		}
		template <typename T>
		inline void DefaultUnsignedInt(T Num, const BaseFormats& Base, const CaseFormats& Case)
		{
			constexpr size_t MaxLength = 8 * sizeof(typename std::remove_reference<T>::type) + 1;
			char CString[MaxLength];

			if (Base.BaseMode == BaseFormats::Mode::PrefixFormat)
				DefaultString(GetBasePrefix(Base.BaseVal));
			else if (Base.BaseMode == BaseFormats::Mode::FullFormat)
				DefaultString(GetBaseFullFormat(Base.BaseVal));
#if INO_DEBUG
			if (Base.BaseVal < 2)
			{
				DefaultCString("Error: ino runtime debug error: Tried to print with base less than 2 (from ");
				DefaultCString(__FILE__);
				InternalWrite(':');
				DefaultUnsignedInt(__LINE__);
				DefaultCString(")\n");
			}
#endif
			int Pos = 0;
			do {
				CString[Pos++] = NumToChar(ModOperator(Num, Base.BaseVal), Case);
				Num /= Base.BaseVal;
			} while (static_cast<typename IntegralType<T>::type>(Num));

			do {
				InternalWrite(CString[--Pos]);
			} while (Pos > 0);
		}

		template <typename T>
		inline void DefaultSignedInt(T Num)
		{
			Negative(Num);
			DefaultUnsignedInt(Num);
		}
		template <typename T>
		inline void DefaultSignedInt(T Num, const BaseFormats& Base)
		{
			Negative(Num);
			DefaultUnsignedInt(Num, Base);
		}
		template <typename T>
		inline void DefaultSignedInt(T Num, const BaseFormats& Base, const CaseFormats& Case)
		{
			Negative(Num);
			DefaultUnsignedInt(Num, Base, Case);
		}

		inline void DefaultChar(char Character, const CaseFormats& Case)
		{
			if (Case == Fmt::Uppercase)
			{
				if ((Character >= 'A' && Character <= 'Z') || !(Character >= 'a' && Character <= 'z'))
					InternalWrite(Character);
				else
					InternalWrite(Character - 32);
			}
			else
			{
				if ((Character >= 'a' && Character <= 'z') || !(Character >= 'A' && Character <= 'Z'))
					InternalWrite(Character);
				else
					InternalWrite(Character + 32);
			}
		}

		template <typename T>
		inline void DefaultString(const T& string)
		{
			for (char Character : string)
				InternalWrite(Character);
		}
		template <typename T>
		inline void DefaultString(const T& string, const CaseFormats& Case)
		{
			for (char Character : string)
				DefaultChar(Character, Case);
		}

		template <typename T>
		inline void DefaultFloat(T Num)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.NegInf);
				return;
			}

			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			auto Decimals = GetTotalDecimals(Fraction);
			if (Decimals.Decimals == 0 && Decimals.Round)
				DefaultSignedInt(Integer + 1);
			else
				DefaultSignedInt(Integer);

			if (Decimals.Decimals)
			{
				InternalWrite('.');
				for (int C = 0; C < Decimals.Decimals; C++)
				{
					Fraction *= 10;
					if (C == Decimals.Decimals - 1 && Decimals.Round)
						InternalWrite(static_cast<int>(Fraction) + 1 + 48);
					else
						InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const DecimalpointFormats& Decimalpoint)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.NegInf);
				return;
			}
			
			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			auto Decimals = GetTotalDecimals(Fraction);
			if (Decimals.Decimals == 0 && Decimals.Round)
				DefaultSignedInt(Integer + 1);
			else
				DefaultSignedInt(Integer);

			if (Decimals.Decimals)
			{
				if (Decimalpoint == Fmt::DecimalDot)
					InternalWrite('.');
				else 
					InternalWrite(',');
				for (int C = 0; C < Decimals.Decimals; C++)
				{
					Fraction *= 10;
					if (C == Decimals.Decimals - 1 && Decimals.Round)
						InternalWrite(static_cast<int>(Fraction) + 1 + 48);
					else
						InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const PrecisionFormats& Precision)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.NegInf);
				return;
			}
			
			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			DefaultSignedInt(Integer);
			if (Precision.PrecisionVal)
			{
				InternalWrite('.');
				for (int C = 0; C < Precision.PrecisionVal; C++)
				{
					Fraction *= 10;
					InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const SpecialnumberFormats& Specialnum)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Specialnum.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Specialnum.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Specialnum.NegInf);
				return;
			}		

			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			auto Decimals = GetTotalDecimals(Fraction);
			if (Decimals.Decimals == 0 && Decimals.Round)
				DefaultSignedInt(Integer + 1);
			else
				DefaultSignedInt(Integer);

			if (Decimals.Decimals)
			{
				InternalWrite('.');
				for (int C = 0; C < Decimals.Decimals; C++)
				{
					Fraction *= 10;
					if (C == Decimals.Decimals - 1 && Decimals.Round)
						InternalWrite(static_cast<int>(Fraction) + 1 + 48);
					else
						InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const DecimalpointFormats& Decimalpoint, const PrecisionFormats& Precision)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Fmt::SpecialnumberShort.NegInf);
				return;
			}
			
			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			DefaultSignedInt(Integer);
			if (Precision.PrecisionVal)
			{
				if (Decimalpoint == Fmt::DecimalDot)
					InternalWrite('.');
				else 
					InternalWrite(',');
				for (int C = 0; C < Precision.PrecisionVal; C++)
				{
					Fraction *= 10;
					InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const DecimalpointFormats& Decimalpoint, const SpecialnumberFormats& Specialnum)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Specialnum.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Specialnum.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Specialnum.NegInf);
				return;
			}	
			
			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			auto Decimals = GetTotalDecimals(Fraction);
			if (Decimals.Decimals == 0 && Decimals.Round)
				DefaultSignedInt(Integer + 1);
			else
				DefaultSignedInt(Integer);

			if (Decimals.Decimals)
			{
				if (Decimalpoint == Fmt::DecimalDot)
					InternalWrite('.');
				else 
					InternalWrite(',');
				for (int C = 0; C < Decimals.Decimals; C++)
				{
					Fraction *= 10;
					if (C == Decimals.Decimals - 1 && Decimals.Round)
						InternalWrite(static_cast<int>(Fraction) + 1 + 48);
					else
						InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const PrecisionFormats& Precision, const SpecialnumberFormats& Specialnum)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Specialnum.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Specialnum.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Specialnum.NegInf);
				return;
			}	

			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			DefaultSignedInt(Integer);
			if (Precision.PrecisionVal)
			{
				InternalWrite('.');
				for (int C = 0; C < Precision.PrecisionVal; C++)
				{
					Fraction *= 10;
					InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}
		template <typename T>
		inline void DefaultFloat(T Num, const DecimalpointFormats& Decimalpoint, const PrecisionFormats& Precision, const SpecialnumberFormats& Specialnum)
		{
			if (INO_OUTSTREAM_NANFUNC(Num)) {
				DefaultCString(Specialnum.Nan);
				return;
			}
			else if (INO_OUTSTREAM_INFFUNC(Num)) {
				DefaultCString(Specialnum.PosInf);
				return;
			}
			else if (INO_OUTSTREAM_NINFFUNC(Num)) {
				DefaultCString(Specialnum.NegInf);
				return;
			}	

			T Integer;
			T Fraction = ModFunction(Num, Integer);
			Integer = abs(Integer);
			Negative(Fraction);

			DefaultSignedInt(Integer);
			if (Precision.PrecisionVal)
			{
				if (Decimalpoint == Fmt::DecimalDot)
					InternalWrite('.');
				else 
					InternalWrite(',');
				for (int C = 0; C < Precision.PrecisionVal; C++)
				{
					Fraction *= 10;
					InternalWrite(static_cast<int>(Fraction) + 48);
					Fraction -= static_cast<int>(Fraction);
				}
			}
		}

		inline void DefaultBool(bool Val)
		{
#ifdef INO_OUTSTREAM_FASTBOOL
			if (Val)
				Write('1');
			else
				Write('0');
#else
			if (Val)
				DefaultCString("true");
			else
				DefaultCString("false");
#endif
		}
		inline void DefaultBool(bool Val, const BoolFormats& BoolFmt)
		{
			if (Val)
				DefaultCString(BoolFmt.BoolTrue);
			else
				DefaultCString(BoolFmt.BoolFalse);
		}

	};

	// * ----- Base-specific output operators -------------------------------------------------------------------------------------
	  //------ Signed integral output operator ------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<IsSigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const BaseFormat<T>& Data) {
		DefaultSignedInt(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}

	  //------ Unsigned integral output operator ----------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<IsUnsigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const BaseFormat<T>& Data) {
		DefaultUnsignedInt(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Caseformat-specific output operators -------------------------------------------------------------------------------
	  //------ Char output operator -----------------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_same<char, typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const CaseFormat<T>& Data) {
		DefaultChar(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}

	  //------ Char* and Char[] output operator -----------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_same<char*, typename ReduceType<T>::type>::value || IsElementType<char, T>::value, int>::type>
	OutStream& OutStream::operator<<(const CaseFormat<T>& Data) {
		DefaultCString(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}

	  //------ String output operator ---------------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_same<String, typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const CaseFormat<T>& Data) {
		DefaultString(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Boolformat-specific output operators -------------------------------------------------------------------------------
	  //------ Bool output operator -----------------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_same<bool, typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const BoolFormat<T>& Data) {
		DefaultBool(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Decimalpointformat-specific output operators -----------------------------------------------------------------------
	  //------ Floating-point output operator -------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const DecimalpointFormat<T>& Data) {
		DefaultFloat(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Precisionformat-specific output operators --------------------------------------------------------------------------
	  //------ Floating-point output operator -------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const PrecisionFormat<T>& Data) {
		DefaultFloat(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Specialnumberformat-specific output operators ----------------------------------------------------------------------
	  //------ Floating-point output operator -------------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value, int>::type>
	OutStream& OutStream::operator<<(const SpecialnumberFormat<T>& Data) {
		DefaultFloat(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- CStringformat-specific output operators ----------------------------------------------------------------------------
	  //------ Char* and Char[] output operator -----------------------------------------------------------------------------------

	template <typename T, typename std::enable_if<std::is_same<char*, typename ReduceType<T>::type>::value || IsElementType<char, T>::value, int>::type>
	OutStream& OutStream::operator<<(const CStringFormat<T>& Data) {
		DefaultCString(Data.Var, Data.Val);
		FinishTransfer();
		return *this;
	}


	// * ----- Multiple format-specific output operators --------------------------------------------------------------------------
	  //------ Signed integral output operators (base- and case-specific) ---------------------------------------------------------

	template <typename T, typename... FmtTs, typename std::enable_if<IsSigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultSignedInt(Data.Var, Data.template Get<BaseFormats>(), Data.template Get<CaseFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Unsigned integral output operators (base- and case-specific) -------------------------------------------------------

	template <typename T, typename... FmtTs, typename std::enable_if<IsUnsigned<typename ReduceType<T>::type>::value && std::is_integral<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<BaseFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultUnsignedInt(Data.Var, Data.template Get<BaseFormats>(), Data.template Get<CaseFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point output operators (decimalpoint- and precision-specific) ---------------------------------------------

	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<DecimalpointFormats>(), Data.template Get<PrecisionFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point output operators (decimalpoint- and specialnumber-specific) -----------------------------------------

	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<DecimalpointFormats>(), Data.template Get<SpecialnumberFormats>());
		Data.Var = 45;
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point output operators (precision- and specialnumber-specific) --------------------------------------------

	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<PrecisionFormats>(), Data.template Get<SpecialnumberFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Floating-point output operators (decimalpoint-, precision- and specialnumber-specific) -----------------------------

	template <typename T, typename... FmtTs, typename std::enable_if<std::is_floating_point<typename ReduceType<T>::type>::value && sizeof...(FmtTs) == 3 && MultiFormat<T, FmtTs...>::template Contains<DecimalpointFormats>::value && MultiFormat<T, FmtTs...>::template Contains<PrecisionFormats>::value && MultiFormat<T, FmtTs...>::template Contains<SpecialnumberFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultFloat(Data.Var, Data.template Get<DecimalpointFormats>(), Data.template Get<PrecisionFormats>(), Data.template Get<SpecialnumberFormats>());
		FinishTransfer();
		return *this;
	}

	  //------ Char* and Char[] output operator (cstring- and case-specific) ------------------------------------------------------
	  
	template <typename T, typename... FmtTs, typename std::enable_if<(std::is_same<char*, typename ReduceType<T>::type>::value || IsElementType<char, T>::value) && sizeof...(FmtTs) == 2 && MultiFormat<T, FmtTs...>::template Contains<CStringFormats>::value && MultiFormat<T, FmtTs...>::template Contains<CaseFormats>::value, int>::type>
	OutStream& OutStream::operator<<(const MultiFormat<T, FmtTs...>& Data) {
		DefaultCString(Data.Var, Data.template Get<CStringFormats>(), Data.template Get<CaseFormats>());
		FinishTransfer();
		return *this;
	}

}

#endif

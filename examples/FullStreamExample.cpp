#include <Arduino.h>

#include <IOStream.h>

void PrintFails()
{
	using Fails = ino::InStream::Fails;
	if (ino::in.Failed())
	{
		ino::out << "------- Fails: ";
		if (ino::in.Failed(Fails::NotANumber))
			ino::out << "not a number, ";
		if (ino::in.Failed(Fails::NotABool))
			ino::out<< "not a bool, ";
		if (ino::in.Failed(Fails::NotAChar))
			ino::out << "not a char, ";
		if (ino::in.Failed(Fails::WrongFormat))
			ino::out<< "wrong format, ";
		if (ino::in.Failed(Fails::WrongBase))
			ino::out<< "wrong base, ";
		if (ino::in.Failed(Fails::WrongCase))
			ino::out<< "wrong case, ";
		if (ino::in.Failed(Fails::WrongDecimalPoint))
			ino::out<< "wrong decimalpoint, ";
		if (ino::in.Failed(Fails::WrongPrecision))
			ino::out << "wrong precision, ";
		if (ino::in.Failed(Fails::WrongSpecialNum))
			ino::out<< "wrong specialnum, ";
		if (ino::in.Failed(Fails::WrongCString))
			ino::out<< "wrong CString, ";
		if (ino::in.Failed(Fails::NoData))
			ino::out<< "no data, ";
#ifdef INO_PLATFORMIOIN
		ino::out << "\b\b  "; // Arduino IDE console cant work with backspace character '\b'
#endif
		ino::out << ino::endl;
		ino::in.ClearFails();
	}
}

char Character = 'n';
int Integer = 42;
double Floatingpoint = 3.14159265;
bool Boolean = true;
String string = "This is a string";
char CString[] = "This is a c-style string";
struct Custom {
	int X;
	double Y;
	String Z;
} CustomData = {3, 3.5, "custom data"};

// Output operator for class 'Custom'
ino::OutStream& operator<<(ino::OutStream& Os, const Custom& custom)
{
	return Os << "X: " << custom.X << ino::tab << "Y: " << custom.Y << ino::tab << "Z: " << custom.Z;
}

// Output operator for class 'Custom' with CaseFormat
template <typename T, typename std::enable_if<std::is_same<Custom, typename ino::ReduceType<T>::type>::value, int>::type = 0>
ino::OutStream& operator<<(ino::OutStream& Os, const ino::CaseFormat<T>& custom)
{
	return Os << "X: " << custom.Var.X << ino::tab << "Y: " << custom.Var.Y << ino::tab << "Z: " << ino::Format(custom.Var.Z, custom.Val);	
}

// Input operator for class 'Custom'
ino::InStream& operator>>(ino::InStream& Is, Custom& custom)
{
	return Is >> custom.X >> custom.Y >> custom.Z;
}

// Input operator for class 'Custom' with CaseFormat
template <typename T, typename std::enable_if<std::is_same<Custom, typename ino::ReduceTypeExceptConst<T>::type>::value && std::is_lvalue_reference<T>::value, int>::type = 0>
ino::InStream& operator>>(ino::InStream& Is, const ino::CaseFormat<T>& custom)
{
	return Is >> custom.Var.X >> custom.Var.Y >> ino::Format(custom.Var.Z, custom.Val);
}

void setup() {
	// Preparing the streams is required
	ino::out.begin(9600);
	ino::in.begin(9600);

	ino::out << "Character: " << ino::endl
		<< ino::tab << "standard: " << Character << ino::endl
		<< ino::tab << "uppercase: " << ino::Uppercase(Character) << ino::endl
		<< ino::tab << "lowercase: " << ino::Format('n', ino::Fmt::Lowercase) << ino::endl;

	ino::out << "Integer: " << ino::endl
		<< ino::tab << "standard: "<< Integer << ino::endl
		<< ino::tab << "binary: " << ino::Bin(Integer) << ino::endl
		<< ino::tab << "octal with full base: " << ino::Base(Integer, { 8, ino::BaseFormats::Mode::FullFormat }) << ino::endl
		<< ino::tab << "hexadecimal with prefix base: " << ino::Base(91, { 16, ino::BaseFormats::Mode::PrefixFormat }) << ino::endl
		<< ino::tab << "hexadecimal and lowercase: " << ino::Format(Integer, ino::Fmt::Hex, ino::Fmt::Lowercase) << ino::endl;

	ino::out << "Floatinpoint: " << ino::endl
		<< ino::tab << "standard: " << Floatingpoint << ino::endl
		<< ino::tab << "with precision 3: " << ino::Precision(Floatingpoint, 3) << ino::endl
		<< ino::tab << "with decimal comma: " << ino::DecimalComma(3.14159265) << ino::endl
		<< ino::tab << "with decimal dot and precision 2: " << ino::Format(Floatingpoint, ino::Fmt::DecimalDot, ino::PrecisionFormats(2)) << ino::endl;

	ino::out << "nan: " << ino::endl
		<< ino::tab << "standard: " << NAN << ino::endl
		<< ino::tab << "represented as word: " << ino::SpecialnumLong(NAN) << ino::endl
		<< ino::tab << "represented as user defined word: " << ino::SpecialnumUserDef(NAN, { "This is not a number", "This is positive infinity", "This is negative infinity" }) << ino::endl;

	ino::out << "Boolean: " << ino::endl
		<< ino::tab << "standard: " << true << ino::endl
		<< ino::tab << "as numbers: " << ino::Format(Boolean, ino::Fmt::BoolNum) << ino::endl
		<< ino::tab << "as capslock word: " << ino::BoolCaps(Boolean) << ino::endl
		<< ino::tab << "as user defined word: " << ino::BoolUserDef(true, { "This is true", "This is false" }) << ino::endl;

	ino::out << "String: " << ino::endl
		<< ino::tab << "standard: " << string << ino::endl
		<< ino::tab << "uppercase: " << ino::Uppercase(string) << ino::endl;

	ino::out << "CString: " << ino::endl
		<< ino::tab << "standard: " << CString << ino::endl
		<< ino::tab << "print max. 30 characters: " << ino::CString(CString, 30) << ino::endl
		<< ino::tab << "print exactly 20 characters: " << ino::CString(CString, { 20, ino::CStringFormats::Mode::Exact }) << ino::endl
		<< ino::tab << "print max 20 characters as uppercase: " << ino::Format(CString, ino::Fmt::Uppercase, ino::CStringFormats(20)) << ino::endl;
	
	ino::out << "Custom: " << ino::endl
		<< ino::tab << "standard: " << CustomData << ino::endl
		<< ino::tab << "uppercase: " << ino::Uppercase(CustomData) << ino::endl;


	ino::out << "Enter Character: " << ino::endl
		<< ino::tab << "standard: ";
	ino::in >> Character;
	ino::out << Character << ino::endl;
	PrintFails();

	ino::out << ino::tab << "uppercase: ";
	ino::in >> ino::Uppercase(Character);
	ino::out << Character << ino::endl;
	PrintFails();
	
	ino::out << "Enter Integer: " << ino::endl
		<< ino::tab << "standard: ";
	ino::in >> Integer;
	ino::out << Integer << ino::endl;
	PrintFails();

	ino::out << ino::tab << "as hexadecimal: ";
	ino::in >> ino::Hex(Integer);
	ino::out << ino::Hex(Integer) << ino::endl;
	PrintFails();

	ino::out << ino::tab << "as octal with full base: ";
	ino::in >> ino::Base(Integer, { 8, ino::BaseFormats::Mode::FullFormat });
	ino::out << ino::Base(Integer, { 8, ino::BaseFormats::Mode::FullFormat }) << ino::endl;
	PrintFails();

	ino::out << "Enter Floatingpoint: " << ino::endl
		<< ino::tab << "standard: ";
	ino::in >> Floatingpoint;
	ino::out << Floatingpoint << ino::endl;
	PrintFails();

	ino::out << ino::tab << "with decimal comma: ";
	ino::in >> ino::Format(Floatingpoint, ino::Fmt::DecimalComma);
	ino::out << ino::DecimalComma(Floatingpoint) << ino::endl;
	PrintFails();

	ino::out << ino::tab << "with precision 2: ";
	ino::in >> ino::Precision(Floatingpoint, 2);
	ino::out << ino::Precision(Floatingpoint, 2) << ino::endl;
	PrintFails();

	ino::out << ino::tab << "with decimal comma, precision 2 and special numbers long: ";
	ino::in >> ino::Format(Floatingpoint, ino::Fmt::DecimalComma, ino::PrecisionFormats(2), ino::Fmt::SpecialnumberLong);
	ino::out << ino::Format(Floatingpoint, ino::Fmt::DecimalComma, ino::PrecisionFormats(2), ino::Fmt::SpecialnumberLong) << ino::endl;
	PrintFails();

	ino::out << "Enter Boolean: " << ino::endl
		<< ino::tab << "standard: ";
	ino::in >> Boolean;
	ino::out << Boolean << ino::endl;
	PrintFails();

	ino::out << ino::tab << "as 0 or 1: ";
	ino::in >> ino::BoolNum(Boolean);
	ino::out << ino::BoolNum(Boolean) << ino::endl;
	PrintFails();

	ino::out << ino::tab << "as ""true"" or ""false"": ";
	ino::in >> ino::BoolWord(Boolean);
	ino::out << ino::BoolWord(Boolean) << ino::endl;
	PrintFails();

	ino::out << ino::tab << "as ""This is true"" or ""This is false"": ";
	ino::in >> ino::BoolUserDef(Boolean, { "This is true", "This is false" });
	ino::out << ino::BoolUserDef(Boolean, { "This is true", "This is false" }) << ino::endl;
	PrintFails();
	
	ino::out << "Enter String: " << ino::endl
		<< ino::tab << "standard: ";
	ino::in >> string;
	ino::out << string << ino::endl;
	PrintFails();

	ino::out << ino::tab << "lowercase: ";
	ino::in >> ino::Lowercase(string);
	ino::out << string << ino::endl;
	PrintFails();
	
	ino::out << "Enter CString: " << ino::endl
		<< ino::tab << "maximum 25 characters: ";
	ino::in >> ino::CString(CString, 25);
	ino::out << CString << ino::endl;
	PrintFails();

	ino::out << ino::tab << "lowercase and exactly 15 characters: ";
	ino::in >> ino::Format(CString, ino::Fmt::Lowercase, ino::CStringFormats(15, ino::CStringFormats::Mode::Exact));
	ino::out << CString << ino::endl;
	PrintFails();

	ino::out << "Enter Custom: " << ino::endl
		<< ino::tab << "standard: ";
	ino::in >> CustomData;
	ino::out << CustomData << ino::endl;
	PrintFails();

	ino::out << ino::tab << "lowercase: ";
	ino::in >> ino::Lowercase(CustomData);
	ino::out << CustomData << ino::endl;
	PrintFails();

#ifdef INO_OUTSTREAM_CURSORTRACKER
	ino::out << "line: " << ino::out.GetCursorPos().Line << " num: " << ino::out.GetCursorPos().Num << ino::endl;
#endif

}

void loop() {

}

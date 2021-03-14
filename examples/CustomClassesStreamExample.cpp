#include <Arduino.h>

#include <IOStream.h>

struct IntWrapper
{
	int Integer;

	inline IntWrapper(int Integer) : Integer(Integer) {}

	// This functions enables printing IntWrappers
	friend ino::OutStream& operator<<(ino::OutStream& OStream, const IntWrapper& Int) {
		return OStream << Int.Integer;
	}

	// This function enables printing IntWrappers with BaseFormats
	template <typename T, typename std::enable_if<std::is_same<IntWrapper, typename ino::ReduceType<T>::type>::value, int>::type = 0> // std::enable_if requires T to be some kind of IntWrapper (including const, volatile, &, &&)
	friend ino::OutStream& operator<<(ino::OutStream& OStream, const ino::BaseFormat<T>& Int) {
		return OStream << ino::Base(Int.Var.Integer, Int.Val);
	}

	// This function enables reading IntWrappers
	friend ino::InStream& operator>>(ino::InStream& IStream, IntWrapper& Int) {
		return IStream >> Int.Integer;
	}

	//This function enables reading IntWrappers with BaseFormats
	template <typename T, typename std::enable_if<std::is_same<IntWrapper, typename ino::ReduceTypeExceptConst<T>::type>::value, int>::type = 0> // std::enable_if requires T to be some kind of non-const IntWrapper (including volatile, &, &&)
	friend ino::InStream& operator>>(ino::InStream& IStream, const ino::BaseFormat<T>& Int) {
		return IStream >> ino::Base(Int.Var.Integer, Int.Val);
	}
};

void setup() {
	ino::out.begin(9600);

	IntWrapper Int = 34;
	
	ino::out << "IntWrapper: " << Int << ino::endl
		<< "IntWrapper (hexadecimal): " << ino::Hex(Int) /* or equivalently ino::Format(Int, ino::Fmt::Hex) */ << ino::endl
		<< "IntWrapper (base 5): " << ino::Base(Int, 5) /* or equivalently ino::Format(Int, ino::BaseFormats(5)) */ << ino::endl;

	
	ino::out << "Enter Int: ";
	ino::in >> Int;
	ino::out << ino::endl << "Int: " << Int << ino::endl;

	ino::out << "Enter Int hexadecimal: ";
	ino::in >> ino::Hex(Int); // Interprets the input as hexadecimal and sets fail flag if the input is not hexadecimal
	ino::out << ino::endl << ino::Hex(Int) << ino::endl;
	if (ino::in.Failed(ino::InStream::Fails::WrongBase))
		ino::out << "Entered Int with wrong base" << ino::endl;

}

void loop() {

}
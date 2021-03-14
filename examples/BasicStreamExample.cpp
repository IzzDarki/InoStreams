#include <Arduino.h>

#include <IOStream.h>


void setup() {
	// Preparing the streams is required
	ino::out.begin(9600);
	ino::in.begin(9600);

	String Variable;
	
	// Output to serial monitor
	ino::out << "Hello world" << ino::endl;

	// Input from serial monitor
	ino::out << "Enter Variable: ";
	ino::in >> Variable;
	ino::out << ino::endl << "New variable: " << Variable << ino::endl;

}

void loop() {

}
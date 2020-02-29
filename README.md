---------------------------------------------------------------
#                         Ino Library                         #
#                           Streams                           #
---------------------------------------------------------------

This library provides standard c++ like streams for communication via serial interface, via pins or for strings.

---------------------------------------------------------------

This library is dependent on the Ino (core) library: https://github.com/IzzDarki/Ino.git
and the SoftwareSerial library from Arduino (which is installed with the Arduino IDE and other Arduino development environment)

---------------------------------------------------------------

Preprocessor definitions (users can modify)

INO_STANDARDIN
    - default: undefined when INO_PLATFORMIO is not defined, otherwise defined
    - when manually defined ino::in works the standard way, even if INO_PLATFORMIO is defined, INO_PLATFORMIOIN will not be defined then

INO_OUTSTREAM_CURSORTRACKER
    - default: undefined
    - when defined provides extra functionality in ino::OutStream for tracking the cursor postition

INO_OUTSTREAM_CURSORTRACKER_TABSIZE
    - default: undefined when INO_OUTSTREAM_CURSORTRACKER is not defined, otherwise defined as 8
    - specifies how many spaces make up a tab, just needed for cursortracker

INO_OUTSTREAM_FASTBOOL
    - default: undefined
    - when defined ino::OutStream::operator<<(bool) prints bool as single characters 0 and 1 instead of cstrings "true" and "false"
	
---------------------------------------------------------------

Preprocessor definitions (not to modify)

INO_PLATFORMIOIN    - defined when ino::in is optimized for use with PlatformIO environment

---------------------------------------------------------------

Preprocessor macros (users can modify)

INO_OUTSTREAM_NANFUNC(Arg)
    - default: defined as isnan(Arg)
    - used when printing floating point numbers to figure out if Arg is not-a-number

INO_OUTSTREAM_INFFUNC(Arg)
    - default: (isinf(Arg) == 1)
    - used when printing floating point numbers to figure out if Arg is positive infinty
    
INO_OUTSTREAM_NINFFUNC(Arg)
    - default: ::ino::IEEE754::isninf(Arg)
    - used when printing floating point numbers to figure out if Arg is negative infinty
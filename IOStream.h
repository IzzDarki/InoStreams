#pragma once
#ifndef INO_IOSTREAM_INCLUDED
#define INO_IOSTREAM_INCLUDED

#include "InoCore.h"
#include "SerialInStream.h"
#include "SerialOutStream.h"
#ifdef INO_PLATFORMIOIN
#include "PIOSerialInStream.h"
#endif

namespace ino {
	
	extern SerialOutStream out;
#ifdef INO_PLATFORMIOIN
	extern PIOSerialInStream in;
#else
	extern SerialInStream in;
#endif

}

#endif
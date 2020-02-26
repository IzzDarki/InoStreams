#include "InoCore.h"
#include "IOStream.h"

namespace ino {

	SerialOutStream out(Serial);
#ifdef INO_PLATFORMIOIN
	PIOSerialInStream in(Serial);
#else
	SerialInStream in(Serial);
#endif

}
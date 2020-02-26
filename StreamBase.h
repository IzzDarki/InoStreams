#pragma once
#ifndef INO_STREAMBASE_INCLUDED
#define INO_STREAMBASE_INCLUDED

#include "InoCore.h"
#include "StreamFormat.h"

namespace ino {

	constexpr char endl = '\n';
	constexpr char tab = '\t';
	constexpr char space = ' ';
	constexpr char cr = '\r';
    
    class StreamBase
    {
	protected:
		char TransferEnd = -1;

    public:
		inline void SetEnd(char NewTransferEnd) { TransferEnd = NewTransferEnd; }
		inline void NoEnd() { TransferEnd = -1; }
		inline char GetEnd() const { return TransferEnd; }
		
    };

}

#endif

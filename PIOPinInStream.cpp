#include "InoCore.h"
#include "PIOPinInStream.h"

#include "IOStream.h"

namespace ino {

	char PIOPinInStream::WaitAndRead() const
	{
		while (!SerialRef.available());
		char RetVal = SerialRef.read();
		return RetVal;
	}

	bool PIOPinInStream::Run() const
	{
		if (SerialRef.available())
		{
			auto BufferLength = Buffer.length();
			char End = out.GetEnd();
			out.NoEnd();
			
			/*	// Multiline PIO input
#ifdef INO_OUTSTREAM_CURSORTRACKER
			if (out.GetCursorPos().Num > 0)
				out << endl;
#endif
			out << "------------------------------------------------" << endl
				<< "PlatformIO input: ";
			*/
			
			out << "| PlatformIO input: ";

			char Character = WaitAndRead();
			while (Character != cr)
			{
				if (Character > 31 && Character < 127)
				{
					Buffer += Character;
					out << Character;
				}
				else if (Character == '\b' && Buffer.length())
				{
					Buffer.remove(Buffer.length() - 1);
					out << "\b \b";
				}
				else if (Character == 23 && Buffer.length()) // Ctrl + backspace
				{
					int C = Buffer.length() - 1;
					while (C >= 0 && Buffer[C] == ' ')
						C--;
					if ((C >= 0) && ((Buffer[C] >= '0' && Buffer[C] <= '9') || (Buffer[C] >= 'A' && Buffer[C] <= 'Z') || (Buffer[C] >= 'a' && Buffer[C] <= 'z')))
					{
						do
							C--;
						while ((C >= 0) && ((Buffer[C] >= '0' && Buffer[C] <= '9') || (Buffer[C] >= 'A' && Buffer[C] <= 'Z') || (Buffer[C] >= 'a' && Buffer[C] <= 'z')));
					}
					else if ((C >= 0) && ((Buffer[C] >= '!' && Buffer[C] <= '/') || (Buffer[C] >= ':' && Buffer[C] <= '@') || (Buffer[C] >= '[' && Buffer[C] <= '`') || (Buffer[C] >= '{' && Buffer[C] <= '~')))
					{
						do
							C--;
						while ((C >= 0) && ((Buffer[C] >= '!' && Buffer[C] <= '/') || (Buffer[C] >= ':' && Buffer[C] <= '@') || (Buffer[C] >= '[' && Buffer[C] <= '`') || (Buffer[C] >= '{' && Buffer[C] <= '~')));
					}
					for (unsigned int I = C + 1 ; I < Buffer.length(); I++)
						out << "\b \b";
					Buffer.remove(C + 1);
				}
				Character = WaitAndRead();
			}
			WaitAndRead(); // there will be a '\n'

			/* // Multiline PIO input
			out << endl
				<< "------------------------------------------------" << endl;
			*/

			for (unsigned int C = 0; C < Buffer.length() - BufferLength + 20; C++)
				out << "\b \b";

			out.SetEnd(End);
			return true;
		}
		return false;
	}
}
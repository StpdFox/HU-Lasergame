#include "MessageHandler.hpp"


void MessageHandler::printByteArray()	{
	unsigned int index = 0;
	//unsigned int testCounter = 0;
	while(index != bytesFilled && index < BUFFER_SIZE )	{
		hwlib::cout << bytearray[index++];
		//hwlib::cout.write(bytearray, 80);
		
		//testCounter++;
	}
	hwlib::cout.flush();
}

void MessageHandler::fillheader(MESSAGE_TYPES type)	{
	switch (type)	{
		case MESSAGE_TYPES::PD:
			bytearray[0] = 'P';
			bytearray[1] = 'D';
	}

	bytearray[2] = msgSize >> 8;
	bytearray[3] = msgSize;
	bytearray[4] = RESERVED;
	bytearray[5] = RESERVED;
	//unsigned int rsize = (bytearray[3]) + (unsigned int)(bytearray[2] << 8);
	//hwlib::cout << "returned size:\t" << rsize << hwlib::endl;
}

void MessageHandler::clearBuffer()	{
	while(bytesFilled > 0) {
		bytearray[bytesFilled--] = '\0';
	}
	bytearray[bytesFilled] = '\0';
	msgSize = 0;
}

void MessageHandler::fillBodyMsg(const char* str)	{
	// copy string into byte array
	while(*str != '\0')	{
		bytearray[bytesFilled++] = *str;
		// Safeguard because msgSize is size of msg data only (excluding: Header, Footer).
		++msgSize;
		++str;
	}
	bytearray[bytesFilled] = '\0';
}

void MessageHandler::fillMsgWithUint(unsigned int convert)	{
	int size = 0;
	int xtraconvert = convert;
	while(xtraconvert != 0)	{
		size++;
		xtraconvert = (xtraconvert / 10);
	}
	
	if(size <= 1)	{
		bytearray[bytesFilled++] = '0' + convert;
		bytearray[bytesFilled] = '\0';
		msgSize++;
	}
	else	{
		//char buffer[32]; // size of int?
		//hwlib::cout << "nextdigit:\t" << nextDigit;
		int nextDigit = convert;
		int counter = size;
		
		while(nextDigit != 0)	{
			bytearray[bytesFilled + --counter] = (nextDigit % 10) + '0';
			//buffer[counter--] = (nextDigit % 10) + '0';
			convert /= 10;
			
			nextDigit = convert;
		}
		bytesFilled += size + 1;
		msgSize += size;
		bytearray[bytesFilled] = '\0';
	}
	
	//buffer[size] = '\0';
	//return buffer;
}

void MessageHandler::writeKeyValuePair(const char* key, const char* value)	{
	bytesFilled = msgSize + MSG_BODY_OFFSET; // jump to offset of body
	if(msgSize > 0)	{
		fillBodyMsg("&");
	}
	fillBodyMsg(key);
	fillBodyMsg(":");
	fillBodyMsg(value);
}

void MessageHandler::writeKeyValuePair(const char* key, unsigned int value)	{
	bytesFilled = msgSize + MSG_BODY_OFFSET; // jump to offset of body
	if(msgSize > 0)	{
		fillBodyMsg("&");
	}
	fillBodyMsg(key);
	fillBodyMsg(":");
	fillMsgWithUint(value);
}

void MessageHandler::setEnd()	{
	bytesFilled = msgSize + MSG_BODY_OFFSET;
	bytearray[bytesFilled++] = ';';
	bytearray[bytesFilled++] = 'E';
	bytearray[bytesFilled++] = 'N';
	bytearray[bytesFilled++] = 'D';
	bytearray[bytesFilled] = '\0';
}
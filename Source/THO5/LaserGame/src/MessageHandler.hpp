#ifndef MESSAGE_HANDLER_HPP
#define MESSAGE_HANDLER_HPP

#include "hwlib.hpp"

#define BUFFER_SIZE 1024
constexpr char RESERVED = (char)0xff;
constexpr int MSG_BODY_OFFSET = 6;

enum class MESSAGE_TYPES{
	PD = 0x00,
};
class MessageHandler
{
private:
	//MessageHandler(const MessageHandler& rhs);
	//MessageHandler& operator=(const MessageHandler& rhs);
	
	byte bytearray[BUFFER_SIZE];
	unsigned int bytesFilled = 6;
	unsigned int msgSize = 0;
	
	MessageHandler() {};
	~MessageHandler()=delete;

public:
	void printByteArray();
	
	void fillheader(MESSAGE_TYPES type);
	
	void clearBuffer();
	
	// Should only be used to write into the body of the msg.
	void fillBodyMsg(const char* str);
	void fillMsgWithUint(unsigned int convert);
	
	void writeKeyValuePair(const char* key, const char* value);
	void writeKeyValuePair(const char* key, unsigned int value);
	void setEnd();
	
	static MessageHandler& getInstance()	{
		static MessageHandler mh;
		return mh;
	}

};

#endif // MESSAGE_HANDLER_HPP

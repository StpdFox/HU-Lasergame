///	\file RunGameController.hpp
/// The RunGameController file,
/// contains the RunGameController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef RUNGAME_HPP
#define RUNGAME_HPP
#include "gameParameters.hpp"
#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"
#include "ReceiveListener.hpp"
#include "GameParameters.hpp"
#include <array>
#include <utility>
#include "Player.hpp"
#define BUFFER_SIZE 1024
class OLEDBoundary;
class RunGameController;
constexpr char RESERVED = (char)0xff;
constexpr int MSG_BODY_OFFSET = 6;
enum class MESSAGE_TYPES{
	PD = 0x00,
};
class data_struct {
public:
	unsigned int size = 0;
	// should be only method to have access to byte buffer from data struct objects, can use friends?
	virtual void toByteBuffer() = 0;
	
};
class MessageHandler{
private:

	byte bytearray[BUFFER_SIZE];
	unsigned int bytesFilled = 6;
	unsigned int msgSize = 0;

	//friend void data_struct::toByteBuffer();

	MessageHandler() {}
	~MessageHandler()=delete;
	
public:
	void printByteArray()	{
		unsigned int index = 0;
		while(index != bytesFilled && index < BUFFER_SIZE )	{
			hwlib::cout << bytearray[index++];
		}
	}
	
	void fillheader(MESSAGE_TYPES type)	{
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

	void resetBuffer()	{
		while(bytesFilled > 0) {
			bytearray[bytesFilled--] = '\0';
		}
		bytearray[bytesFilled] = '\0';
		msgSize = 0;
	}
	// Should only be used to write into the body of the msg.
	void fillBodyMsg(const char* str)	{
		// copy string into byte array
		while(*str != '\0')	{
			bytearray[bytesFilled++] = *str;
			// Safeguard because msgSize is size of msg data only (excluding: Header, Footer).
/*			if(bytesFilled >= 6)	{
				++msgSize;
			}*/
			++msgSize;
			++str;
		}
		bytearray[bytesFilled] = '\0';
	}
	void fillMsgWithUint(unsigned int convert)	{
		int size = 0;
		int xtraconvert = convert;
		while(xtraconvert != 0)	{
			size++;
			xtraconvert = (xtraconvert / 10);
			hwlib::cout << xtraconvert << hwlib::endl;
		}
		
		if(size <= 1)	{
			bytearray[bytesFilled++] = '0' + convert;
			bytearray[bytesFilled] = '\0';
			msgSize++;
/*			char buffer[2];
			buffer[0] = '0' + convert;
			buffer[1] = '\0';
			return buffer;*/
		}
		else	{
			//char buffer[32]; // size of int?
			//hwlib::cout << "nextdigit:\t" << nextDigit;
			int nextDigit = convert;
			int counter = size;
			
			while(nextDigit != 0)	{
				bytearray[bytesFilled + counter--] = (nextDigit % 10) + '0';
				//buffer[counter--] = (nextDigit % 10) + '0';
				hwlib::cout << "convert:\t" << convert << hwlib::endl;
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
	void writeKeyValuePair(const char* key, const char* value)	{
		hwlib::cout << "I AM NOT SOO WISE" << hwlib::endl;
		bytesFilled = msgSize + MSG_BODY_OFFSET; // jump to offset of body
		if(msgSize > 0)	{
			fillBodyMsg("&");
		}
		fillBodyMsg(key);
		fillBodyMsg(":");
		fillBodyMsg(value);
	}
	void writeKeyValuePair(const char* key, unsigned int value)	{
		hwlib::cout << "I AM WISE" << hwlib::endl;
		bytesFilled = msgSize + MSG_BODY_OFFSET; // jump to offset of body
		if(msgSize > 0)	{
			fillBodyMsg("&");
		}
		fillBodyMsg(key);
		fillBodyMsg(":");
		fillMsgWithUint(value);
	}
	void setEnd()	{
		bytesFilled = msgSize + MSG_BODY_OFFSET;
		bytearray[bytesFilled++] = ';';
		bytearray[bytesFilled++] = 'E';
		bytearray[bytesFilled++] = 'N';
		bytearray[bytesFilled++] = 'D';
		bytearray[bytesFilled] = '\0';
	}
	static MessageHandler& getInstance()	{
		static MessageHandler mh;
		return mh;
	}
	/*void createMessage(MESSAGE_TYPES type, data_struct data) {
		switch (type) {
			case MESSAGE_TYPES::PD:
				fillMsgBody(data);
				void getMessageSize
				fillheader("PD", data.size);
				break;
			
		}
	}*/
};
class Player : data_struct	{
private:
	friend class FormattedGameStats; // They are bff's!
	
	void toByteBuffer() override {
		MessageHandler& mh = MessageHandler::getInstance();
		
		//ToDo make fillkeypair instead of fillbytearray and fillbufferwithuint each and every time... Done?
		const unsigned int pID =  0;//playerInfo.getPlayerID();
		mh.writeKeyValuePair("Player_ID", pID);
		mh.writeKeyValuePair("Player_Name", "zoro");
		mh.fillheader(MESSAGE_TYPES::PD);
		mh.setEnd();
		mh.printByteArray();
		
		mh.resetBuffer(); // Should be renamed to clear?
		/*mh.writeKeyValuePair("key", "value");
		mh.fillByteArray("Player_ID:")
		mh.fillBufferWithUInt(playerInfo.getPlayerID());
		mh.fillByteArray("&Player_Health:");
		mh.fillBufferWithUInt(healthPoints);
		mh.fillheader(MESSAGE_TYPES::PD, mh.bytesFilled - MSG_BODY_OFFSET);*/
	}
/*	unsigned int bytesFilled = 0;

	void printByteArray()	{
		unsigned int index = 0;
		while(index != bytesFilled && index < BUFFER_SIZE )	{
			hwlib::cout << bytearray[index++];
		}
	}
	void fillBufferWithUInt(unsigned int convert)	{
		int size = 0;
		int xtraconvert = convert;
		while(xtraconvert != 0)	{
			size++;
			xtraconvert = (xtraconvert / 10);
			hwlib::cout << xtraconvert << hwlib::endl;
		}

		if(size == 1)	{
			bytearray[bytesFilled++] = '0' + convert;
			bytearray[bytesFilled] = '\0';
			char buffer[2];
			buffer[0] = '0' + convert;
			buffer[1] = '\0';
			return buffer;
		}
		else	{
			//char buffer[32]; // size of int?
			//hwlib::cout << "nextdigit:\t" << nextDigit;
			int nextDigit = convert;
			int counter = size;
			while(nextDigit != 0)	{
				bytearray[bytesFilled + counter--] = (nextDigit % 10) + '0';
				//buffer[counter--] = (nextDigit % 10) + '0';
				hwlib::cout << "convert:\t" << convert << hwlib::endl;
				convert /= 10;
				
				nextDigit = convert;
			}
			bytesFilled += size + 1;
			bytearray[bytesFilled] = '\0';
		}*/
		
		//buffer[size] = '\0';
		//return buffer;
	//}
public:
bool playerisAlive = true;
	playerInformation& playerInfo;
	RunGameController& parentController;
	
	int healthPoints;
	// Array van 10 elementen (max 10 spelers), waar de key-pair is: hoeveelheid damage, aantal schoten
	std::array<std::pair<uint8_t, uint8_t>, 10> other_players { { 
		std::pair<uint8_t,uint8_t>(0x00, 0), 
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0),
		std::pair<uint8_t,uint8_t>(0x00, 0)} };
		
	Player(playerInformation& playerInfo, RunGameController& parentController);
	//Player(uint8_t playerId, RunGameController& parentController, int life = 100);
	
	void getResultsXml();

	void takeDamage(uint8_t, uint8_t);
	void doDamage();
	bool playerIsAlive();
};
typedef struct IREntity {
	//auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver
	hwlib::pin_in & button;
	hwlib::pin_out & led;
	transmitterController trans;
	messageLogic & logic;
	receiverController & receive;
	IREntity(auto & button,auto & led,auto & playerInformation,auto & logic,auto & receiver) :
	button{button},
	led{led},
	trans{playerInformation,1},
	logic{logic},
	receive{receiver} {}

} &irentity;

/// \author Matthijs Vos
/// \Author Ferdi Stoeltie
/// \brief Controller for the runnable game logic
/// \date 11-07-2017
class RunGameController : public rtos::task<>, public KeypadListener, public ReceiveListener, private KeyConsume {
private:
	friend class Player;
	// A reference to the keypad controller. This is required to register itself as a listener
   KeypadController& kpC;
   
   // A reference to a sound interface. ISound.setSound is required to set an active sound.
   ISound& sound;
   
   // The OLEDBoundary hardware interface object.
   OLEDBoundary& oledBoundary;
   
   // RTOS
   rtos::pool<char> keypadMsgPool;
   //std::array<char, 2> commandCode { {'0', '0' } };
   rtos::pool<std::array<char, 2>> irMsgPool;
   rtos::flag keypadFlag;
   rtos::flag irMsgFlag;
   irentity irE;
   rtos::clock gameTimeSecondsClock;
   
	//get and set gameParameters
	playerInformation playerInfo;
	
   // Primitive data types
   int startOfGameTimestamp;
   int gameDurationMin;
   
	//int life = 100;
	//Player player{0x00, *this};
	Player player;
	//bool damagePlayer(uint8_t, uint8_t);
	bool previousPressHashCode = true;
public:

	/// \author Matthijs Vos
   /// \author Peter Bonnema 
   /// \author Marianne Delmaar
   /// \author Ferdi Stoeltie
   /// \brief Constructor that needs to be called to initialize the \c RunGameController \c instance.
   /// \param[in] kpC A reference tot the KeypadController.
   /// \param[in] sound A handle to the ISound interface.
   /// \param[in] oledBoundary a reference to the oledBoundary object.
   /// \param Priority of this rtos::task.
   RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, playerInformation& playerInfo , unsigned int priority );
   ~RunGameController();
	
   rtos::channel<char16_t,10> receiverMessageChannel;
   void main() override;
   /// \author Matthijs Vos
   /// \author Ferdi Stoeltie
   /// \brief Method that is inherited from interface \c KeypadListener \c.
   /// 			In here the key that is being pressed by the KeypadController will be handled.
   void handleMessageKey(char c);
   
   	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
	
	void handleReceivedMessage(auto msg);
	
	void shutDownGame();
	void writeGameResults();
	void receivedMsgstd(std::array<char, 2> msg) {
		irMsgPool.write(msg);
		irMsgFlag.set();
	}
};

#endif //RUNGAMECONTROLLER_HPP

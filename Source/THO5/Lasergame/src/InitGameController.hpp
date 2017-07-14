///	\file InitgameController.hpp
/// The InitgameController file,
/// contains the InitgameController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef INITGAME_HPP
#define INITGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"
#include "gameParameters.hpp"
#include "RunGameController.hpp"
#include <array>

class OLEDBoundary;
class KeypadController;
class transmitterController;

/// \author Ferdi Stoeltie
/// \author Peter Bonnema
/// \brief Controller class that handles the initialize of the game settings
class InitGameController : public rtos::task<>, public KeypadListener, private KeyConsume  {
public:
//<<<<<<< HEAD
//	
//	/// \author Ferdi Stoeltie
//	/// \brief InitGameController Constructor that initializes this task.
//	/// \param KeypadController& kpC The KeypadController that is used to register the next listener to.
//	/// \param KeypadListener* nextListener The next KeypadListener to register to the KeypadController when this task is finished.
//	/// \param unsigned_int priority The priority of this RTOS task.
//    InitGameController(KeypadController& kpC, KeypadListener* nextListener, unsigned int priority);
//	
//    
//private:
//	KeypadController& keypadController; // The owner
//	rtos::mailbox<char> msg;
//	KeypadListener* nextListener;
//	void main();
//    bool incomingCommand = false;
//    bool commandoIsValid = false;
//    char commandCount = 0;
//    std::array<char, 2> commandCode { {'0', '0' } };
//	
//    int COMMANDSIZE = 2; 
//=======
    InitGameController(KeypadController& kpC, KeypadListener* nextListener, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority);

    void handleMessageKey(char c);
private:
	enum STATE
	{
		WAITING_FOR_C,
		INPUTTING_CMD,
		WAITING_FOR_HASHTAG,
		SENDING_CMD,
		SENDING_START_CMD,
	} state;
	
	irentity& irEntity;
	playerInformation& playerInfo;
	hwlib::font_default_8x8 font;
	OLEDBoundary& oledBoundary;
    KeypadController& keypadController; // The owner
    rtos::mailbox<char> msg;
    KeypadListener* nextListener;
	
    void main() override;

    char commandCount = 0;
    std::array<char, 2> commandCode { { '0', '0' } };
	
    void initNewCommand();
    bool validateCommand();
    void sendMessage();

    void sendStartMessage();
	
	// KeyConsumer methods
	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
};
#endif
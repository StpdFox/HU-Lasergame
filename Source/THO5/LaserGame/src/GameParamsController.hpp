///	\file GameParamsController.hpp
/// The GameParamsController file,
/// contains the GameParamsController class decleration only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 11-07-2017

#ifndef GameParams_HPP
#define GameParams_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadController.hpp"
#include "KeypadListener.hpp"
#include "gameParameters.hpp"

#include <array>

///\author Marianne Delmaar
/// \author Ferdi Stoeltie
/// \date 11-07-2017
/// \brief Controller task for game params like player id and weapon damage. Waits for 'start game' signal from IRReceiver.
class GameParamsController : public rtos::task<>, public KeypadListener, private KeyConsume{
private:
	void parseKeypad(char s);
	
	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
	
public:
	/// \author Marianne Delmaar
	/// \author Ferdi Stoeltie
	/// \brief Controller task for game params like player id and weapon damage.
	/// \param Priority of this rtos::task.
	
	GameParamsController(KeypadController& kpC, KeypadListener* initGameListener, KeypadListener* runGameListener,unsigned int priority);
	~GameParamsController();
	
	KeypadController& kpC; // The owner
    rtos::mailbox<char> msg;
    KeypadListener* initGameListener;
	KeypadListener* runGameListener;
	playerInformation playerInfo;
	
	char commandCount = 0;
    std::array<char, 1> commandCode { {'0'} };
    int COMMANDSIZE = 1; 
	bool id = true;
	
	uint8_t playerID;
	uint8_t weaponID;
	
	void validateCommand();
	void initNewCommand();
	void handleMessageKey(char c);
	void main();
};

#endif // GameParams_HPP

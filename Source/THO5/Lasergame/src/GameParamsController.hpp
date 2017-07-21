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
#include "RunGameController.hpp"
#include "InitGameController.hpp"

#include <array>

class OLEDBoundary;
class KeypadController;
class transmitterController;
class ReceiveListener;

/// \author Marianne Delmaar
/// \author Ferdi Stoeltie
/// \author Peter Bonnema
/// \date 11-07-2017
/// \brief Controller task for game params like player id and weapon damage. Waits for 'start game' signal from IRReceiver.
class GameParamsController : public rtos::task<>, public KeypadListener, public ReceiveListener, private KeyConsume{
private:
	void consumeChar(char c);
	void consumeHashTag();
	void consumeWildcard();
	void consumeDigits(char c);
	
public:
	/// \author Marianne Delmaar
	/// \author Ferdi Stoeltie
	/// \author Peter Bonnema
	/// \brief Controller task for game params like player id and weapon damage.
	/// \param Priority of this rtos::task.
	
	GameParamsController(KeypadController& kpC, InitGameController& initGameController, RunGameController& runGameController, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irEntity, unsigned int priority);
	~GameParamsController();
	
	void receivedMsgstd(const std::array<char, 2>& msg) override;
	
private:
	enum STATE
	{
		WAITING_FOR_A,
		INPUTTING_PLAYER_ID,
		WAITING_FOR_B,
		INPUTTING_WEAPON_ID,
		WAITING_FOR_HASHTAG,
		WAITING_FOR_COMMANDS,
	} state;
	
	irentity& irEntity;
	playerInformation& playerInfo;
	hwlib::font_default_8x8 font;
	OLEDBoundary& oledBoundary;
	hwlib::window_ostream oledStream, statusStream, playerIDStream, confirmStream, weaponIDStream;
	KeypadController& kpC; // The owner
    rtos::mailbox<char> msg;
    InitGameController& initGameController;
	RunGameController& runGameController;
	
	int playerID;
	int weaponID;
	
	typedef struct { byte receivedPlayerID; byte receivedWeaponID; } irMessage;
	rtos::channel<irMessage, 1> irMessageChannel;
	
	void handleMessageKey(char c);
	void waitForCommands();
	void main();
};

#endif // GameParams_HPP

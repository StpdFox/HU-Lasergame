///	\file RunGameController.hpp
/// The RunGameController file,
/// contains the RunGameController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "KeypadListener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"
#include "ReceiveListener.hpp"

class OLEDBoundary;

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
	// A reference to the keypad controller. This is required to register itself as a listener
	KeypadController& kpC;

	// A reference to a sound interface. ISound.setSound is required to set an active sound.
	ISound& sound;

	// The OLEDBoundary hardware interface object.
	OLEDBoundary& oledBoundary;
	hwlib::font_default_8x8 font;
	hwlib::window_ostream oledStream, gameTimeStream, statusMessageStream, playerHealthStream;
	// RTOS
	rtos::pool<char> keypadMsgPool;
	rtos::pool<std::array<char, 2>> irMsgPool;
	rtos::pool<byte> durationPool;
	rtos::flag startFlag, keypadFlag, irMsgFlag;
	irentity& irE;
	rtos::clock gameTimeSecondsClock;

	//get and set gameParameters
	playerInformation& playerInfo;
	
	void doCountDown(int seconds);
   
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
	RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, playerInformation& playerInfo, irentity& irE, unsigned int priority);
	~RunGameController();

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

	void handleReceivedMessage(const std::array<char, 2>& msg);
	void receivedMsgstd(const std::array<char, 2>& msg) override;
	void startGame(byte durationMin);
};

#endif //RUNGAMECONTROLLER_HPP

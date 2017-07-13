///	\file RunGameController.hpp
/// The RunGameController file,
/// contains the RunGameController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 11-07-2017

#ifndef RUNGAME_HPP
#define RUNGAME_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "keypadlistener.hpp"
#include "KeypadController.hpp"
#include "SpeakerController.hpp"
#include "transmitterController.hpp"
#include "receiverController.hpp"
#include "messageLogic.hpp"
#include "gameParameters.hpp"

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
class RunGameController : public rtos::task<>, public KeypadListener, private KeyConsume {
private:
	// A reference to the keypad controller. This is required to register itself as a listener
   KeypadController& kpC;
   
   // A reference to a sound interface. ISound.setSound is required to set an active sound.
   ISound& sound;
   
   // The OLEDBoundary hardware interface object.
   OLEDBoundary& oledBoundary;
   
   // RTOS
   rtos::pool<char> keypadMsgPool;
   rtos::flag keypadFlag;
   irentity irE;
   rtos::channel<char16_t,10> receiverMessageChannel;
   rtos::clock gameTimeSecondsClock;
	
	
   // Primitive data types
   int startOfGameTimestamp;
   int gameDurationMin;

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
   RunGameController(KeypadController& kpC, ISound& sound, OLEDBoundary& oledBoundary, irentity irE, unsigned int priority );
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
};

#endif //RUNGAMECONTROLLER_HPP

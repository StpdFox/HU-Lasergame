///	\file KeypadController.hpp
/// The KeypadController file,
/// contains the KeypadController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef KEYPAD_CONTROLLER_HPP
#define KEYPAD_CONTROLLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include "KeypadListener.hpp"

/// \author Marianne Delmaar
/// \author Ferdi Stoeltie
/// \brief Rtos task that handles the input given by a keypad. This is then passed in to a keypadlistener.
class KeypadController : public rtos::task<> {
public:
	/// \author Marianne Delmaar
	/// \brief KeypadController constructor that takes in an existing keypad device and a task priority.
	/// \param hwlib::keypad<16>& keypad The keypad device
	/// \param Priority of this rtos::task.
    KeypadController(hwlib::keypad< 16 >& keypad, unsigned int priority);
    
	/// \author Marianne Delmaar
	/// \author Ferdi Stoeltie
	/// \brief Use to register a new keypadlistener as the listener where the keypad input should be passed to.
	/// \param KeypadListener* nextListener Pointer to the KeypadListener to register as active KeypadListener.
    void registerNext(KeypadListener* nextListener);
	
private:
	// Time that the task activates at, to poll the keypad peripheral.
	int waitTime = 100; // 100 ms
	
	// Keypad hardware device
    hwlib::keypad<16> &keypad;
	
	// Pointer to the keypad listener that keypad input should be send to.
	KeypadListener* kp;
	
	// RTOS main implementation
	void main();
};

#endif //KEYPAD_HANDLER_HPP
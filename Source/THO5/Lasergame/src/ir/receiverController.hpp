///	\file receiverController.hpp
/// The receiverController file,
/// contains the receiverController class declaration only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 14-07-2017

#include "hwlib.hpp"
#include "rtos.hpp"
#include "messageLogic.hpp"
#include "ReceiveListener.hpp"

#ifndef RECEIVERCONTROLLER_HPP
#define RECEIVERCONTROLLER_HPP

/// \author Matthijs Vos
/// \author Ferdi Stoeltie
/// \brief Handles the receiving of IR messages by retrieving them, 
///			decoding and afterwards sends the decoded message to the active l ReceiveListener.
class receiverController : public rtos::task<>{
private:
	rtos::timer poolReceiverTimer;
	hwlib::pin_in & rPin;
	hwlib::pin_out & gnd;
	hwlib::pin_out & vcc;
	messageLogic & logic;
	
	void main(void) override;
	ReceiveListener* rL;
public:
	/// author Matthijs Vos
	/// \brief Constructor that requires the pins that the IR receiver is connected to. 
	///			This ensures correct setup of the receiver peripheral.
	///			Messagelogic, The protocol that the message should follow.
	/// \param hwlib::pin_in& rPin Read Pin.
	/// \param hwlib::pin_out & gnd Ground Pin.
	/// \param hwlib::pin_out & vcc Voltage pin.
	/// \param messageLogic& logic Message type that ensures correct message logic.
	/// \param unsigned_int priority Priority that is passed to the underlying RTOS task.
	receiverController(hwlib::pin_in & rPin, hwlib::pin_out & gnd, hwlib::pin_out & vcc, messageLogic & logic,unsigned int priority );

	/// author Matthijs Vos
	/// \brief Checks whether or not the IR message contains a start bit
	/// \return int Startbit of the message
//	int getStartBit();
	
	/// author Ferdi Stoeltie
	/// \brief Sets the ReceiveListener that the received messages should be passed to and handled in.
	/// \param ReceiveListener* newRL The new ReceiveListener that should be set as active listener.
	void setReceiveListener(ReceiveListener* newRL);
	
	/// \author Matthijs Vos
	/// \brief Gets the bit value that is determined by the pin being HIGH or LOW.
	/// \return int Bit within message byte.
//	int getBit();

	/// \author Matthijs Vos
	/// \brief Returns the entire message that has been retrieved by the ReceiverController.
	/// \return char16_t Message 
//    char16_t getMessage();
	
	uint16_t readMessage();
	uint16_t readBit(unsigned int maxDelayUs = 0, unsigned int intervalUs = 0);
	bool messageIsValid(uint16_t message);
};
#endif
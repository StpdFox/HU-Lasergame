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

/**
 * @author Peter Bonnema
 * @author Mattijs Vos
 * @author Ferdi Stoeltie
 * @date 25/07/2017
 * 
 * @brief Handles the receiving of IR messages by retrieving them, decoding and afterwards sends the decoded message to the active ReceiveListener.
 */
class receiverController : public rtos::task<> {
public:
	/// \author Mattijs Vos
	/// \brief Constructor that requires the pins that the IR receiver is connected to. 
	/// This ensures correct setup of the receiver peripheral.
	///	Messagelogic, The protocol that the message should follow.
	/// \param hwlib::pin_in& rPin Read Pin.
	/// \param hwlib::pin_out & gnd Ground Pin.
	/// \param hwlib::pin_out & vcc Voltage pin.
	/// \param messageLogic& logic Message type that ensures correct message logic.
	/// \param unsigned_int priority Priority that is passed to the underlying RTOS task.
	receiverController(hwlib::pin_in & rPin, hwlib::pin_out & gnd, hwlib::pin_out & vcc, messageLogic & logic,unsigned int priority );
	
	/// \author Ferdi Stoeltie
	/// \brief Sets the ReceiveListener that the received messages should be passed to and handled in.
	/// \param ReceiveListener* newRL The new ReceiveListener that should be set as active listener.
	void setReceiveListener(ReceiveListener* newRL);
	
	/// \author Peter Bonnema
	/// \brief Waits until a bit is read or until `maxDelayUs` us have passed. The method checks for a bit every `intervalUs` us.
	/// \param maxDelayUs The timeout in us for reading a bit. On timeout the max value of uint16_t is returned.
	/// \param intervalUs The interval in us between checks. The function will always wait an integer mulitple of intervalUs before timing out.
	/// \return int The value of a single bit (0 or 1) or max value of uint16_t upon timeout.
	uint16_t readBit(unsigned int maxDelayUs = 0, unsigned int intervalUs = 0);

	/// \author Peter Bonnema
	/// \brief Returns the entire message that has been retrieved by the ReceiverController.
	/// \return char16_t Message 
	uint16_t readMessage();
	
private:
	rtos::timer poolReceiverTimer;
	hwlib::pin_in & rPin;
	hwlib::pin_out & gnd;
	hwlib::pin_out & vcc;
	messageLogic & logic;
	
	void main(void) override;
	ReceiveListener* rL;
};
#endif
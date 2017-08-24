///	\file transmitterController.hpp
/// The transmitterController file,
/// contains the transmitterController class declaration only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef TRANSMIT_HPP
#define TRANSMIT_HPP

#include "hwlib.hpp"
#include "rtos.hpp"
#include "gameParameters.hpp"
#include "transmitter.hpp"

/// \author Matthijs Vos
/// \brief This class contains the required logic to transmit data over the IR using the required protocol.
/// 		Usage: 
class transmitterController : public rtos::task<>{
private:
	// Transmitter boundary object
 	transmitter irTransmit;
	
 	playerInformation & messageToSend;
	
	// Flag that is set when a message should be transmitted
 	rtos::flag sendMessageFlag;
	
	// implementation from rtos::task
 	void main();

	// Sending of the message
 	void sendMessage();

public:
	/// \author Matthijs Vos
	/// \brief Constructor that takes in a playerInformation object and priority for the rtos task.
	/// \param playerInformation& playerInformation The player information that is required as a source when sending.
	/// \param unsigned_int priority Passed to RTOS as a task priority.
	transmitterController(playerInformation & playerInformation,unsigned int priority);

	/// \author Matthijs Vos
	/// \brief Calling this method will set the flag that activates this task to send a message.
 	void enableFlag();
 };
 #endif
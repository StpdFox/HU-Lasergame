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
class transmitterController : public rtos::task<> {
public:
	/// \brief Constructor that takes in the priority for the rtos task.
	/// \param unsigned_int priority Passed to RTOS as a task priority.
	transmitterController(unsigned int priority);

	/**
	* @brief Sets the message to be send by the controller. This only writes the message to a pool and sets a flag.
	* @param message
	*/
 	void sendMessage(char16_t message);
	
private:
 	transmitter irTransmit;

	rtos::mutex messagePoolMutex;
	rtos::flag messagePoolFlag;
 	rtos::pool<char16_t> messagePool;

 	void main();
 };
 #endif

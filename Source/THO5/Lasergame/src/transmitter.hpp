#ifndef TRANSMITTER_HPP
#define TRANSMITTER_HPP

#include "hwlib.hpp"
#include "gameParameters.hpp"
#include "rtos.hpp"

class transmitter {
public:
	/**
	 * @brief Sends a one via an IR signal consisting of high for 1600 us and low for 800 us
	 */
	void sendOne();
	
	/**
	 * @brief Sends a zero via an IR signal consisting of high for 800 us and low for 1600 us
	 */
	void sendZero();
	
	/**
	 * @brief Sends the message specified by calling either sendOne() or sendZero() for the value of each bit of message. Per the protocol, the message is send twice with a 3000us pauze.
	 * @param message The message to send
	 */
	void sendMessage(char16_t message);
	
private:
	hwlib::target::d2_36kHz sendPin;
};
#endif
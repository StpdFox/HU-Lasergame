///@file
#ifndef RECEIVELISTENER_HPP
#define RECEIVELISTENER_HPP

#include <array>

/**
 * @author Matthijs Vos
 * @date 25/07/2017
 * 
 * @brief An interface for classes that want to react to received IR messages.
 */
class ReceiveListener {
public:
	/**
	 * @brief The method that is called by receiverController when it receives an IR message.
	 * Implementing classes should not keep references or pointers to the message referenced by msg because its lifetime ends after returning from this method.
	 * The first byte is the player id of the sender and the second byte is the data which is either the weapon id or a special command.
	 */
	virtual void receivedMsgstd(const std::array<char, 2>& msg) = 0;
};

#endif // RECEIVELISTENER_HPP

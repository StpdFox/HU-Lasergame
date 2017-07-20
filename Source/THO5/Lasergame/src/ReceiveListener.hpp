#ifndef RECEIVELISTENER_HPP
#define RECEIVELISTENER_HPP
#include <array>
class ReceiveListener {
public:
	virtual void receivedMsgstd(std::array<char, 2> msg) = 0;
};

#endif // RECEIVELISTENER_HPP

#ifndef KEYPADLISTENER_HPP
#define KEYPADLISTENER_HPP

/// \author Ferdi Stoeltie
/// \brief A simple interface that enables the keypadHandler to take in as a listener(observer)
class keypadListener  {
public:
    keypadListener() {};
    virtual void handleMessageKey(char c) = 0;
private:
};

#endif // KEYPADLISTENER_HPP
#ifndef KEYPADLISTENER_HPP
#define KEYPADLISTENER_HPP

/// \author Ferdi Stoeltie en Marianne Delmaar
/// \brief A simple interface that enables the keypadHandler to take in as a listener(observer)
class KeypadListener  {
public:
    KeypadListener() {};
    virtual void handleMessageKey(char c) = 0;
private:
};

#endif // KEYPADLISTENER_HPP
#ifndef KEYPADLISTENER_HPP
#define KEYPADLISTENER_HPP

/// \author Ferdi Stoeltie
/// \brief This class offers default methods that can be implemented to handle certain keypad presses.
class KeyConsume {
	friend class KeyConsumer;
private:
	virtual void consumeChar(char c) = 0;
	virtual void consumeHashTag() = 0;
	virtual void consumeWildcard() = 0;
	virtual void consumeDigits(char c) = 0;
};

/// \author Ferdi Stoeltie
/// \brief Can consume keys by giving a given char key to the correct method.
class KeyConsumer {
	public:
	static void handleMessageKey(KeyConsume& kC, char c)	{
	 hwlib::cout << c << " pressed, handled in KeyConsumer \n";
        switch(c)   {
			
            case 'A':
                kC.consumeChar(c);
            break;
			case 'B':
                kC.consumeChar(c);
            break;
			case 'C':
                kC.consumeChar(c);
            break;
			case 'D':
                kC.consumeChar(c);
            break;
			
            case '#':
				kC.consumeHashTag();
            break;
            case '*':
				kC.consumeWildcard();
            break;
			
            default:
				kC.consumeDigits(c);
            break;
        }
}
};

/// \author Ferdi Stoeltie en Marianne Delmaar
/// \brief A simple interface that enables the keypadHandler to take in as a listener(observer)
class KeypadListener  {
public:
    KeypadListener() {};
    virtual void handleMessageKey(char c) = 0;
private:
};

#endif // KEYPADLISTENER_HPP
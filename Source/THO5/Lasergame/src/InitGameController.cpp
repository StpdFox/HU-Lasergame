#include "InitGameController.hpp"

InitGameController::InitGameController(KeypadController& kpC, KeypadListener* nextListener, unsigned int priority) : 
    task(priority, "initGame task"), keypadController{kpC} , msg("keypad char"), nextListener{nextListener}  {
		//kpC.registerNext(kC);
}

void InitGameController::handleMessageKey(char c)  {
   msg.write(c);
}

void InitGameController::main()  {
        for(;;) {
            char c = msg.read();
			KeyConsumer::handleMessageKey(*this, c);
            //parseKeypad(s);
        }
}

    void InitGameController::initNewCommand()   {
        incomingCommand = true;
        commandCode[0] = 0;
        commandCode[1] = 0;
        commandCount = 0;

    }

        void InitGameController::validateCommand()  {
        if(commandCode[0] >= '0' && commandCode[0] <= '9' 
            && commandCode[1] >= '0' && commandCode[1] <= '9')  {
            incomingCommand = false;
            commandoIsValid = true;
        }
    }

        void InitGameController::sendMessage()  {
        // Send the message over the air with apple update system
    }

        void InitGameController::sendStartMessage() {
        // Send the start message + start timer over the air with apple update system
        // Deregisters self and registers other keypadlistener
		//hwlib::cout << "sending start message and registering next task for keypadcontroller\n";
        keypadController.registerNext(nextListener);
    }
	
	// Keypad Methods
	void InitGameController::consumeChar(char c) {
			initNewCommand();
	}
	void InitGameController::consumeHashTag() {
		// If the command has been validated, it can be executed
		if(commandoIsValid) {
			sendMessage();
		}else   {
			validateCommand();
		}
	}
	void InitGameController::consumeWildcard() {
		// Send a start message
		sendStartMessage();	
	}
	void InitGameController::consumeDigits(char c) {
		// Should only be run if keycode is a number
		if((int)commandCode.size() <= COMMANDSIZE - 1)    {
			commandCode[commandCount++] = c;
		}
	}
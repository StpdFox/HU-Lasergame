#include "InitGameController.hpp"

InitGameController::InitGameController(KeypadController& kpC, KeypadListener* nextListener) : 
    task("initGame task"), keypadController{kpC} , msg("keypad char"), nextListener{nextListener}  {
    }

void InitGameController::handleMessageKey(char c)  {
   msg.write(c);
}

void InitGameController::main()  {
        for(;;) {
            char s = msg.read();
            parseKeypad(s);
        }
}

void InitGameController::parseKeypad(char s)    {
        hwlib::cout << s << " pressed, handled in initgamecontroller \n";
        switch(s)   {
            case 'C':
                // Start commando
                initNewCommand();
            break;
            case '#':
                // If the command has been validated, it can be executed
                if(commandoIsValid) {
                    sendMessage();
                }else   {
                    validateCommand();
                }
            break;
            case '*':
                // Send a start message
                sendStartMessage();
            break;
            default:
            // Should only be run if keycode is a number
            if((int)commandCode.size() <= COMMANDSIZE - 1)    {
                commandCode[commandCount++] = s;
            }
            break;
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
        keypadController.registerNext(nextListener);
    }
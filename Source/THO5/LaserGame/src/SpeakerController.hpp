/// \author Marianne Delmaar
/// \brief Rtos task that handles the output for speaker. Speaker commands come from RunGameController.

#ifndef SPEAKER_CONTROLLER_HPP
#define SPEAKER_CONTROLLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include <array>

/// \author Marianne Delmaar en Ferdi Stoeltie
/// \brief Enum for sound types
enum class Sounds {HIT = 0x01, END_GAME = 0x02, START_GAME = 0x04, SHOOT = 0x08};

/// \author Marianne Delmaar en Ferdi Stoeltie
/// \brief Interface to provide the setSound method
class ISound  {
public:
    ISound() {};
    virtual void setSound(Sounds s) = 0;
private:
};

/// \author Marianne Delmaar en Ferdi Stoeltie
/// \brief SpeakerController that handles the playing of sounds
class SpeakerController : public rtos::task<>, public ISound{
private:
	
	rtos::pool<Sounds> soundPool;
	rtos::flag playSoundFlag;
	
	hwlib::pin_out &lsp;
	
	int frequency = 0; 
	int duration = 0;
	
public:	
	SpeakerController(hwlib::pin_out& lsp, unsigned int priority);
	void main();
	void setSound(Sounds soundType); // inherit from ISound
	void playSound(Sounds soundType);

};

#endif //SPEAKER_CONTROLLER_HPP

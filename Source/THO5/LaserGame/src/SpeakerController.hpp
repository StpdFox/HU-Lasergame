/// \author Marianne Delmaar
/// \brief Rtos task that handles the output for speaker. Speaker commands come from RunGameController.

#ifndef SPEAKER_CONTROLLER_HPP
#define SPEAKER_CONTROLLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include <array>

enum class Sounds {HIT = 0x01, END_GAME = 0x02, START_GAME = 0x04, SHOOT = 0x08};

class SpeakerController : public rtos::task<>{
private:
	
	rtos::pool<Sounds> soundPool;
	rtos::flag playSoundFlag;
	
	hwlib::pin_out &lsp;
	
	int frequency = 0; 
	int duration = 0;
	
public:	
	SpeakerController(hwlib::pin_out& lsp);
	void main();
	void play(int frequency, int duration);
	void setSound(Sounds sounds);

};

#endif //SPEAKER_CONTROLLER_HPP

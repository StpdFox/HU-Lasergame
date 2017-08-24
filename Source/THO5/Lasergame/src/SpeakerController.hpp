///	\file SpeakerController.hpp
/// The SpeakerController file,
/// contains the SpeakerController class declaration only. 
/// Date file created:
/// \date 07-07-2017
/// Date Last Update:
/// \date 14-07-2017

#ifndef SPEAKER_CONTROLLER_HPP
#define SPEAKER_CONTROLLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"
#include <array>

/// \author Marianne Delmaar
/// \author Ferdi Stoeltie
/// \brief Enum for sound types
enum class Sounds {HIT = 0x01, END_GAME = 0x02, START_GAME = 0x04, SHOOT = 0x08, WAIT = 0x10};

/// \author Marianne Delmaar en Ferdi Stoeltie
/// \brief Interface to provide the setSound method
class ISound {
public:
    virtual void setSound(Sounds s) = 0;
};

/// \author Marianne Delmaar
/// \author Ferdi Stoeltie
/// \brief SpeakerController that handles the playing of sounds
class SpeakerController : public rtos::task<>, public ISound {
public:	
	SpeakerController(hwlib::pin_out& lsp, unsigned int priority);
	void main();
	
	/**
	 * @brief Specify which sound is to be played next by this controller. This method only sets a flag and writes the argument to a pool.
	 * @param soundType
	 */
	void setSound(Sounds soundType); // inherit from ISound
	
private:
	void playSound(Sounds soundType);
	
	rtos::pool<Sounds> soundPool;
	rtos::flag playSoundFlag;
	
	hwlib::pin_out &lsp;
	
	int frequency = 0; 
	int duration = 0;
};

#endif //SPEAKER_CONTROLLER_HPP

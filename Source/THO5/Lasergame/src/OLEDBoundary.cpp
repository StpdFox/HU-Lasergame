#include "OLEDBoundary.hpp"
#include "hwlib.hpp"

OLEDBoundary::OLEDBoundary(unsigned int priority) :
	rtos::task<>{ priority, "OLEDBoundary" },
	scl{ hwlib::target::pins::scl },
	sda{ hwlib::target::pins::sda },
	i2c_bus{ scl, sda },
	//bufferedLCD{ i2c_bus, 0x3c }, //wont work because of the necessary 200 ms init time for the OLED and there is no default constructor
	bufferedLCD{
			[this](){
				hwlib::wait_ms(200);
				// use the buffered version
				return hwlib::glcd_oled_buffered{ i2c_bus, 0x3c };
			}()
		},
	flushFlag{ this, "flushFlag" },
	flushPartsFlag{ this, "flushPartsFlag" },
	playerNumberInput{ i2c_bus, 0x3c },
	firePowerInput{ i2c_bus, 0x3c },
	gameDurationInput{ i2c_bus, 0x3c },
	gameTime{ i2c_bus, 0x3c },
	score{ i2c_bus, 0x3c }
{
}

OLEDBoundary::~OLEDBoundary()
{
}

void OLEDBoundary::flush()
{
	flushFlag.set();
}

void OLEDBoundary::flushParts()
{
	flushPartsFlag.set();
}

hwlib::glcd_oled_buffered& OLEDBoundary::getBufferedLCD()
{
	return bufferedLCD;
}

glcd_oled_part_buffered<PLAYERNUMBERINPUT_WIDTH * 8, PLAYERNUMBERINPUT_HEIGHT>& OLEDBoundary::getPlayerNumberInput()
{
	return playerNumberInput;
}

glcd_oled_part_buffered<FIREPOWERINPUT_WIDTH * 8, FIREPOWERINPUT_HEIGHT>& OLEDBoundary::getFirePowerInput()
{
	return firePowerInput;
}

glcd_oled_part_buffered<GAMEDURATIONINPUT_WIDTH * 8, GAMEDURATIONINPUT_HEIGHT>& OLEDBoundary::getGameDurationInput()
{
	return gameDurationInput;
}

glcd_oled_part_buffered<GAMETIME_WIDTH * 8, GAMETIME_HEIGHT>& OLEDBoundary::getGameTime()
{
	return gameTime;
}

glcd_oled_part_buffered<SCORE_WIDTH * 8, SCORE_HEIGHT>& OLEDBoundary::getScore()
{
	return score;
}

void OLEDBoundary::main()
{
	while(true)
	{
		const rtos::event& event = wait();
		if(event == flushFlag)
		{
			bufferedLCD.flush();
		}
		else if(event == flushPartsFlag)
		{
			playerNumberInput.flush();
			firePowerInput.flush();
			gameDurationInput.flush();
			gameTime.flush();
			score.flush();
		}
	}
}
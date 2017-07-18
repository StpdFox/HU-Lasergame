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
	statusMessageField{ i2c_bus, 0x3c},
	playerNumberInputField{ i2c_bus, 0x3c },
	firePowerInputField{ i2c_bus, 0x3c },
	gameDurationInputField{ i2c_bus, 0x3c },
	gameTimeField{ i2c_bus, 0x3c },
	scoreField{ i2c_bus, 0x3c }
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
glcd_oled_part_buffered<STATUSMESSAGEFIELD_WIDTH * 8, STATUSMESSAGEFIELD_HEIGHT * 8>& OLEDBoundary::getStatusMessageField()
{
	return statusMessageField;
}
glcd_oled_part_buffered<PLAYERNUMBERINPUT_WIDTH * 8, PLAYERNUMBERINPUT_HEIGHT * 8>& OLEDBoundary::getPlayerNumberInputField()
{
	return playerNumberInputField;
}

glcd_oled_part_buffered<FIREPOWERINPUT_WIDTH * 8, FIREPOWERINPUT_HEIGHT * 8>& OLEDBoundary::getFirePowerInputField()
{
	return firePowerInputField;
}

glcd_oled_part_buffered<GAMEDURATIONINPUT_WIDTH * 8, GAMEDURATIONINPUT_HEIGHT * 8>& OLEDBoundary::getGameDurationInputField()
{
	return gameDurationInputField;
}

glcd_oled_part_buffered<GAMETIME_WIDTH * 8, GAMETIME_HEIGHT * 8>& OLEDBoundary::getGameTimeField()
{
	return gameTimeField;
}

glcd_oled_part_buffered<SCORE_WIDTH * 8, SCORE_HEIGHT * 8>& OLEDBoundary::getScoreField()
{
	return scoreField;
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
			gameTimeField.flush();
			playerNumberInputField.flush();
			firePowerInputField.flush();
			gameDurationInputField.flush();
			statusMessageField.flush();
			scoreField.flush();
		}
	}
}
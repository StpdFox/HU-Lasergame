#define GAMETIME_WIDTH 5
#define GAMETIME_HEIGHT 1

#include "RunGameController.hpp"
#include "OLEDBoundary.hpp"

RunGameController::RunGameController(unsigned int priority, OLEDBoundary& oledBoundary) :
	rtos::task<>{ priority, "RunGameController" },
	oledBoundary{ oledBoundary },
	gameTimeTimer{ this, "gameTimeTimer" },
	gameTimeSecondsClock{ this, 1000, "gameTimeSecondsClock" }
	
	//gameTimeStream{ oledBoundary.getGameTimeField(), hwlib::font_default_8x8() }
{
	oledBoundary.getGameTimeField().setLocation({ 7 * 8, 6 * 8 });
}

RunGameController::~RunGameController()
{
}

void RunGameController::main()
{
	hwlib::glcd_oled_buffered& lcd = oledBoundary.getBufferedLCD();
	auto f = hwlib::font_default_8x8();
	auto stream = hwlib::window_ostream{ lcd, f };
	stream << "\f";
	oledBoundary.flush();
	sleep(1);
	
	startOfGameTimestampMin = hwlib::now_us() / 60'000'000;
	gameDurationMin = 1;
	gameTimeTimer.set(10 * 100'000'000ul);
	const rtos::event& event = wait(); //TODO for some reason the timer gets set immediately. Fix it
	(void) event;
//	if(event == gameTimeTimer)
//	{
//		HWLIB_TRACE << "gameTimeTimer";
//	}
	
	//auto f = hwlib::font_default_8x8();
	hwlib::window_ostream gameTimeStream{ oledBoundary.getGameTimeField(), f };
//	gameTimeStream.width(2);
//	gameTimeStream.fill('0');
	
	while(true)
	{
		const rtos::event& event = wait();
		if(event == gameTimeTimer)
		{
			//TODO fix bug dat de timer te vroeg wordt getriggered. Misschien kan de timer wel helemaal weg door bij de clock gewoon op timeout te checken.
			//TODO Game over
			HWLIB_TRACE << "Game over!"; 
			while(true) sleep(1);
		}
		else if(event == gameTimeSecondsClock)
		{
			int remainingTimeSec = gameDurationMin * 60 - (hwlib::now_us() / 1'000'000 - startOfGameTimestampMin * 60);
			gameTimeStream << "\f" << remainingTimeSec / 60 << ":" << remainingTimeSec % 60;
			oledBoundary.flushParts();
		}
	}
}

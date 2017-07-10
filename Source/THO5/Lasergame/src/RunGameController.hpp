#ifndef RUNGAMECONTROLLER_HPP
#define RUNGAMECONTROLLER_HPP

#include "rtos.hpp"
#include "hwlib.hpp"

class OLEDBoundary;

class RunGameController : public rtos::task<>
{
public:
	RunGameController(unsigned int priority, OLEDBoundary& oledBoundary);
	~RunGameController();

private:
	void main() override;
	
	OLEDBoundary& oledBoundary;
	
	rtos::clock gameTimeSecondsClock;
	int startOfGameTimestamp;
	int gameDurationMin;
	//hwlib::window_ostream gameTimeStream;
};

#endif // RUNGAMECONTROLLER_HPP

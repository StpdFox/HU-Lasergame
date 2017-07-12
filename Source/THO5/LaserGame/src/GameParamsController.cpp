///	\file GameParamsController.cpp
/// The GameParamsController file,
/// contains the GameParamsController class implementation only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 11-07-2017

#include "GameParamsController.hpp"

GameParamsController::GameParamsController(unsigned int priority) : 
rtos::task<>{ priority, "GameParamsController" }
{

}

GameParamsController::~GameParamsController()
{
}

void GameParamsController::main()	{
	// ToDo
}


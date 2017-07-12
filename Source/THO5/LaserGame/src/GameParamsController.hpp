///	\file GameParamsController.cpp
/// The GameParamsController file,
/// contains the GameParamsController class decleration only. 
/// Date file created:
/// \date 11-07-2017
/// Date Last Update:
/// \date 11-07-2017

#ifndef GameParams_HPP
#define GameParams_HPP

#include "hwlib.hpp"
#include "rtos.hpp"

/// \author Ferdi Stoeltie
/// \date 11-07-2017
/// \brief i havent got a clue
class GameParamsController : public rtos::task<>
{
private:
	//GameParamsController(const GameParamsController& rhs);
	//GameParamsController& operator=(const GameParamsController& rhs);
	
public:
   /// \author Ferdi Stoeltie
   /// \brief i dont know :/.
   /// \param Priority of this rtos::task.
	GameParamsController(unsigned int priority);
	~GameParamsController();
	void main();
};

#endif // GameParams_HPP

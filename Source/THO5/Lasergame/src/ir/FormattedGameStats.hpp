#ifndef FORMATTEDGAMESTATS_HPP
#define FORMATTEDGAMESTATS_HPP
#include <string>
#include <array>
#include "rtos.hpp"
#include "RunGameController.hpp"
class FormattedGameStats
{
private:
	//std::string
	//std::array<std::string, N> messages;
	//FormattedGameStats(const FormattedGameStats& rhs);
	//FormattedGameStats& operator=(const FormattedGameStats& rhs);
	
public:
	/// \deprecated
	FormattedGameStats();
	//~FormattedGameStats();
	/// \deprecated
	void getResultsXml(Player& player);
};

#endif // FORMATTEDGAMESTATS_HPP

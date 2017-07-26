#include "FormattedGameStats.hpp"

FormattedGameStats::FormattedGameStats()
{
}

void FormattedGameStats::getResultsXml(Player& player)	{
	hwlib::cout << "<player_game_info>\n";
	hwlib::cout << "\t<player_id>" << player.playerInfo.playerID << "</player_id>\n" << "\t<player_healthpoints>";
	hwlib::cout << "\t<player_healthpoints>" << player.healthPoints << "</player_healthpoints>\n";
	hwlib::cout << "\t<other_players amount=\"" << player.other_players.size() << "\">\n";
	for(unsigned int i = 0; i < player.other_players.size() - 1; i++)	{
		hwlib::cout << "\t\t<player>\n";
		hwlib::cout << "\t\t\t<player_id>" << i << "</player_id>\n";
		hwlib::cout << "\t\t\t<weapon_id>" << player.other_players[i].first << "</weapon_id>\n";
		hwlib::cout << "\t\t\t<player_damage>" << player.other_players[i].second << "</player_damage>\n";
		hwlib::cout << "\t\t</player>\n";
	}
	hwlib::cout << "\t</other_players>\n";
	hwlib::cout << "</player_game_info>\n" << hwlib::endl;
}


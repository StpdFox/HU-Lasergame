/*#include "Player.hpp"

Player::Player(playerInformation& playerInfo, RunGameController& parentController) : playerInfo{playerInfo}, parentController{parentController}{
	//playerInfo.getPlayerHealth();
	healthPoints = playerInfo.playerHealth;
}
//Player::Player(uint8_t playerId, RunGameController& parentController, int healthPoints) : parentController{parentController}, healthPoints{healthPoints}	{}
void Player::doDamage()	{
		// send over ir to do damage
		//parentController.irE.trans.sendMessage(playerInfo.getPlayerID(), playerInfo.getWeaponID());
}
void Player::takeDamage(byte player_id, byte damage_id)	{
	other_players[(uint8_t)player_id].first = (uint8_t)damage_id;
	other_players[(uint8_t)player_id].second += 1; // got hit another time
	if((healthPoints -= damage_id) < 0)	{
		healthPoints = 0; // game over
		parentController.sound.setSound(Sounds::END_GAME);
		parentController.shutDownGame();
		// shutdown game
	}
	else{
		parentController.sound.setSound(Sounds::HIT);
		// stop game
	}
}

bool Player::playerIsAlive()	{
		return playerisAlive;
}
*/

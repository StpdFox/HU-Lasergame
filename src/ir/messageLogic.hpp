class messageLogic{
public:
	messageLogic()
	{}

	char16_t encode(uint_fast8_t playerID, uint_fast8_t weaponID){
		char16_t compiledMessage = 1;
		compiledMessage = compiledMessage <<5;
		compiledMessage = (compiledMessage | playerID);
		compiledMessage = compiledMessage <<5;
		compiledMessage = (compiledMessage | weaponID);
		compiledMessage = compiledMessage <<5;
		compiledMessage = (compiledMessage | (playerID ^weaponID));
		return compiledMessage;
	}

	bool decode(char16_t receivingBits, byte & receivedPlayerID , byte & receivedWeaponID){
		byte exor = (receivingBits & 0x1F);
		byte identifierWeapon = ((receivingBits >> 0x05) & 0x1F);
		byte identifierPlayer = ((receivingBits >> 0x0A) & 0x1F);

		if((identifierPlayer ^identifierWeapon) == exor){
			receivedPlayerID = identifierPlayer;
			receivedWeaponID = identifierWeapon;
			return 1;
		}
		else{
			return 0;
		}
	}
};
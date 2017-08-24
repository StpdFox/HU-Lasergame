///@file
#ifndef OLEDBOUNDARY_HPP
#define OLEDBOUNDARY_HPP

#include "rtos.hpp"
#include "defines.h"
#include "glcd_oled_part_buffered.hpp"

/**
 * @class OLEDBoundary
 * @author Peter Bonnema
 * @date 24/07/2017
 * 
 * @brief This task simply reacts to two flags that are set by flush() and flushParts() and in response
 * either flushes the whole screen (but no window parts) or flushes all window parts, respectively.
 * 
 * Note that flushing glcd_oled_part_buffered only does something when the part is marked dirty by some write or clear action on it.
 * You can retrieve the interal glcd_oled_buffered object to base a window_ostream upon but it is
 * higly discouraged to call glcd_oled_buffered::flush() on this object to prevent it from interfering with other tasks as
 * flushing an lcd takes a considerable amount of time. Flushing it should always be done through this class.
 * The same goes for the window parts.
 * 
 * The defines used in the template parameters of the glcd_oled_part_buffered objects are defined in defines.h and specify the size of the parts.
 */
class OLEDBoundary : public rtos::task<>
{
public:
    /**
     * @brief Constructs a new OLEDBoundary object with the given priority.
     * @param priority The priority of this task.
     */
    OLEDBoundary(unsigned int priority);
	
    /**
     * @brief Sets a flag that will cause the task to call flush() on the internal glcd_oled_buffered object.
     */
    void flush();
	
    /**
     * @brief Sets a flag that will cause the task to call flush() on the internal glcd_oled_part_buffered objects.
     */
    void flushParts();
	
	/**
	 * @brief Returns a reference to the internal glcd_oled_buffered object.
	 */
	hwlib::glcd_oled_buffered& getBufferedLCD();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the player health field.
	*/
	glcd_oled_part_buffered<PLAYERHEALTHFIELD_WIDTH * 8, PLAYERHEALTHFIELD_HEIGHT * 8>& getPlayerHealthField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the hit notification field.
	*/
	glcd_oled_part_buffered<HITNOTIFICATION_WIDTH * 8, HITNOTIFICATION_HEIGHT *8>& getHitNotificationField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the status message field.
	*/
	glcd_oled_part_buffered<STATUSMESSAGEFIELD_WIDTH * 8, STATUSMESSAGEFIELD_HEIGHT * 8>& getStatusMessageField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the confirm message field.
	*/
	glcd_oled_part_buffered<CONFIRMMESSAGEFIELD_WIDTH * 8, CONFIRMMESSAGEFIELD_HEIGHT * 8>& getConfirmMessageField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the player number input field.
	*/
	glcd_oled_part_buffered<PLAYERNUMBERINPUT_WIDTH * 8, PLAYERNUMBERINPUT_HEIGHT * 8>& getPlayerNumberInputField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the fire power input field.
	*/
	glcd_oled_part_buffered<FIREPOWERINPUT_WIDTH * 8, FIREPOWERINPUT_HEIGHT * 8>& getFirePowerInputField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the game duration input field.
	*/
	glcd_oled_part_buffered<GAMEDURATIONINPUT_WIDTH * 8, GAMEDURATIONINPUT_HEIGHT * 8>& getGameDurationInputField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the game time field.
	*/
	glcd_oled_part_buffered<GAMETIME_WIDTH * 8, GAMETIME_HEIGHT * 8>& getGameTimeField();
	
	/**
	* @brief Returns a reference to the internal glcd_oled_part_buffered object that represents the score field.
	*/
	glcd_oled_part_buffered<SCORE_WIDTH * 8, SCORE_HEIGHT * 8>& getScoreField();

private:
    void main() override;

	//needed to store these objects somewhere because they only hold references to oneanother instead of copying then in their ctors
	hwlib::target::pin_oc scl, sda;
	hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
    hwlib::glcd_oled_buffered bufferedLCD;
    rtos::flag flushFlag, flushPartsFlag;
	
	//these will be default initialized to nullptr's
	glcd_oled_part_buffered<PLAYERHEALTHFIELD_WIDTH * 8, PLAYERHEALTHFIELD_HEIGHT *8> playerHealthField;
	glcd_oled_part_buffered<HITNOTIFICATION_WIDTH * 8, HITNOTIFICATION_HEIGHT *8> hitNotificationField;
	glcd_oled_part_buffered<STATUSMESSAGEFIELD_WIDTH * 8, STATUSMESSAGEFIELD_HEIGHT * 8> statusMessageField;
	glcd_oled_part_buffered<CONFIRMMESSAGEFIELD_WIDTH * 8, CONFIRMMESSAGEFIELD_HEIGHT * 8> confirmMessageField;
	glcd_oled_part_buffered<PLAYERNUMBERINPUT_WIDTH * 8, PLAYERNUMBERINPUT_HEIGHT * 8> playerNumberInputField;
	glcd_oled_part_buffered<FIREPOWERINPUT_WIDTH * 8, FIREPOWERINPUT_HEIGHT * 8> firePowerInputField;
	glcd_oled_part_buffered<GAMEDURATIONINPUT_WIDTH * 8, GAMEDURATIONINPUT_HEIGHT * 8> gameDurationInputField;
	glcd_oled_part_buffered<GAMETIME_WIDTH * 8, GAMETIME_HEIGHT * 8> gameTimeField;
	glcd_oled_part_buffered<SCORE_WIDTH * 8, SCORE_HEIGHT * 8> scoreField;
};

#endif // OLEDBOUNDARY_HPP

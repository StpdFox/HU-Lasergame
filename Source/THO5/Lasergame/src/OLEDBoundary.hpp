#ifndef OLEDBOUNDARY_HPP
#define OLEDBOUNDARY_HPP

#include "rtos.hpp"

class OLEDBoundary;

/// Oled B/W graphics LCD, buffered
//
/// This class implements a *buffered* interface to an 128 x 64 pixel
/// monochrome (on/off) OLED display. Buffering means that all writes
/// are buffered in memory until flush() is called.
///
/// These displays are available
/// in various colcors (green, red, white, etc.).
/// The interface is I2C.
/// The driver chip is an SSD1306.
///
/// When the PCB has regulator (3-legged component) the power can be 3 - 5V. 
/// If it hasn't, you can use only 3.3V.
///
/// There are variations of this display with more pins, which 
/// have more interface options (SPI as alternative interface).
///
/// This type of display is reasonably priced 
/// and available from lots of sources.
///
/// \image html lcdoled-empty.jpg
///
template<unsigned int width, unsigned int height>
class glcd_oled_part_buffered : public hwlib::window {
private:
	bool dirty;
	hwlib::i2c_bus& bus;

	// the 7-bit i2c address of the controller
	fast_byte address;

	// current cursor setting in the controller;
	// used to avoid explicit cursor updates when such are not needed
	fast_byte cursor_x, cursor_y;

	// SSD1306 commands
	static constexpr const uint8_t COLUMNADDR	= 0x21;
	static constexpr const uint8_t PAGEADDR		= 0x22;

	hwlib::location start;
	uint8_t buffer[width * height / 8];   
   
	void command(byte d){
		byte data[] = { 0x80, d };
		bus.write( 
			address, 
			data, 
			sizeof(data) / sizeof(byte) 
		);      
	} 	

	void command(byte d0, byte d1){
		byte data[] = { 0x80, d0, 0x80, d1 };
		bus.write( 
			address, 
			data, 
			sizeof(data) / sizeof(byte) 
		);    
	} 	

	void command(byte d0, byte d1, byte d2){
		byte data[] = { 0x80, d0, 0x80, d1, 0x80, d2 };
		bus.write( 
			address, 
			data, 
			sizeof(data) / sizeof(byte) 
		);   
	}

   void write_implementation(hwlib::location pos, hwlib::color col) override {
		unsigned int a = pos.x + ( pos.y / 8 ) * width;

		if(col == foreground){ 
			buffer[a] |=  (0x01 << (pos.y % 8));
		} else {
			buffer[a] &= ~(0x01 << (pos.y % 8));
		}
		dirty = true;
   }
   
	/// write the pixel buffer to the oled
	//
	/// All write (and clear) calls change only the in-memory pixel
	/// buffer. This call writes this pixel buffer to the oled.
	void flush() {
		if(dirty)
		{
			command( COLUMNADDR,  start.x,  127 );
			command( PAGEADDR,    start.y / 8,    7 );
			for(unsigned int y = 0; y < height / 8; y++)
			{
				for(unsigned int x = 0; x < width; x++)
				{
					byte d = buffer[ x + width * y ];
					byte data[] = { 0x40, d };
					bus.write( 
						address, 
						data, 
						sizeof(data) / sizeof(byte) 
					);
					rtos::current_task()->release();
				}
			}
			dirty = false;
		}
	}
	
	friend OLEDBoundary;
public:
	glcd_oled_part_buffered(hwlib::i2c_bus & bus, fast_byte address = 0x3C, hwlib::location start = { 0, 0 }):
		hwlib::window{ { width, height }, hwlib::black, hwlib::white },
		dirty{ false },
		bus(bus),
		address(address),
		cursor_x(255), cursor_y(255),
		start(start)
	{ }
   
	virtual void clear() {
		for(unsigned int i = 0; i < sizeof(buffer) / sizeof(uint8_t); i++) {
			buffer[i] = 0x00;
		}
	}
	
	void setLocation(const hwlib::location& loc)
	{
		start = loc;
	}
}; // class glcd_oled_part_buffered

class OLEDBoundary : public rtos::task<>
{
public:
    OLEDBoundary(unsigned int priority);
	~OLEDBoundary();
	
    void flush();
    void flushParts();
	hwlib::glcd_oled_buffered& getBufferedLCD();
	glcd_oled_part_buffered<PLAYERNUMBERINPUT_WIDTH * 8, PLAYERNUMBERINPUT_HEIGHT>& getPlayerNumberInput();
	glcd_oled_part_buffered<FIREPOWERINPUT_WIDTH * 8, FIREPOWERINPUT_HEIGHT>& getFirePowerInput();
	glcd_oled_part_buffered<GAMEDURATIONINPUT_WIDTH * 8, GAMEDURATIONINPUT_HEIGHT>& getGameDurationInput();
	glcd_oled_part_buffered<GAMETIME_WIDTH * 8, GAMETIME_HEIGHT>& getGameTime();
	glcd_oled_part_buffered<SCORE_WIDTH * 8, SCORE_HEIGHT>& getScore();

private:
    void main() override;

//needed to store these objects somewhere because they only hold references to oneanother instead of copying then in their ctors
	hwlib::target::pin_oc scl, sda;
	hwlib::i2c_bus_bit_banged_scl_sda i2c_bus;
    hwlib::glcd_oled_buffered bufferedLCD;
    rtos::flag flushFlag, flushPartsFlag;
	
	//these will be default initialized to nullptr's
	glcd_oled_part_buffered<PLAYERNUMBERINPUT_WIDTH * 8, PLAYERNUMBERINPUT_HEIGHT> playerNumberInput;
	glcd_oled_part_buffered<FIREPOWERINPUT_WIDTH * 8, FIREPOWERINPUT_HEIGHT> firePowerInput;
	glcd_oled_part_buffered<GAMEDURATIONINPUT_WIDTH * 8, GAMEDURATIONINPUT_HEIGHT> gameDurationInput;
	glcd_oled_part_buffered<GAMETIME_WIDTH * 8, GAMETIME_HEIGHT> gameTime;
	glcd_oled_part_buffered<SCORE_WIDTH * 8, SCORE_HEIGHT> score;
};

#endif // OLEDBOUNDARY_HPP

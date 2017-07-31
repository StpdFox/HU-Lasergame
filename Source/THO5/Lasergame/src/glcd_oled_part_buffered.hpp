///@file
#ifndef GLCD_OLED_PART_BUFFERED__HPP
#define GLCD_OLED_PART_BUFFERED__HPP

#include "hwlib.hpp"
#include "rtos.hpp"

class OLEDBoundary;

/// \brief Oled B/W graphics LCD, buffered
/// \author	Peter Bonnema
///
/// \detailed This class implements a *buffered* interface to an 128 x 64 pixel
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
/// This class is designed to write to a user-definable part of the screen and
/// it only maintains a buffer for the pixels in that part.
/// This is in contrast to how a window_part on top of a glcd_oled_buffered
/// would work which would still maintain a buffer for the whole screen and would also flush the whole screen.
///
/// The user can specify the part of the screen that will be written to by calling setLocation(const hwlib::location& loc)
/// to specify the top-left corner of the window part and by specifying the width and height template parameters.
/// These parameters have to be known at compile time because the size of the internal buffer is based upon them and there
/// is no dynamic memory allocation in rtos.
///
/// This class only flushes its designated part which saves time and only does so when it's marked dirty. This happens by
/// writing to it or by calling clear().
/// It also calls release() of the current task in between flushing of individual characters and after issuing commands
/// in order to not claim a large amount of time but use multiple smaller blocks of time instead.
///
template<unsigned int width, unsigned int height>
class glcd_oled_part_buffered : public hwlib::window {
private:
	bool dirty; ///<If set to true, this window will be flushed during the next call to flush()
	hwlib::i2c_bus& bus;

	// the 7-bit i2c address of the controller
	fast_byte address;

	// SSD1306 commands
	static constexpr const uint8_t COLUMNADDR	= 0x21;
	static constexpr const uint8_t PAGEADDR		= 0x22;

	hwlib::location start;
	uint8_t buffer[width * height / 8];   
   
	void command(byte d);
	void command(byte d0, byte d1);	
	void command(byte d0, byte d1, byte d2);
	void write_implementation(hwlib::location pos, hwlib::color col) override;
   
	/// write the pixel buffer to the oled
	//
	/// All write (and clear) calls change only the in-memory pixel
	/// buffer. This call writes this pixel buffer to the oled.
	void flush();
	
	friend OLEDBoundary; ///<OLEDBoundary will be calling flush and we want to enforce that this is the only class who can call flush
public:
	/**
	 * @brief Creates a glcd_oled_part_buffered object with the specified i2c bus, start position of top-left corner and i2c address.
	 * 
	 * Creates a glcd_oled_part_buffered object with `bus` as the i2c bus over which it will communicate with the screen over `address`.
	 * It's top-left corner will be at `start`. The window starts out as clean.
	 * 
	 * @param bus The i2c bus over which it will communicate with the screen.
	 * @param start The location of the top-left corner of this window part in pixels.
	 * @param address The address of the i2c bus.
	 * @see setLocation(const hwlib::location& loc)
	 */
	glcd_oled_part_buffered(hwlib::i2c_bus& bus, hwlib::location start = { 0, 0 }, fast_byte address = 0x3C);

	/**
	 * @brief clears in internal buffer and marks the window dirty. This has no effect on the screen until the next call to flush().
	 */
	virtual void clear() override;
	
	/**
	 * @brief Sets the location of the top-left corner of the window part this object represents.
	 * 
	 * The x-coordinate can be anything (anything drawn off-screen will be ignored).
	 * The y-coordinate can be anything except for that it has to be a multiple of 8 (so { 5, 17 } is not allowed.
	 * This is not checked for so you'll be at the mercy of the behaviour of the lcd).
	 * 
	 * @param loc the location of the top-left corner of the window part in pixels.
	 */
	void setLocation(const hwlib::location& loc);
}; // class glcd_oled_part_buffered

template<unsigned int width, unsigned int height>
inline glcd_oled_part_buffered<width, height>::glcd_oled_part_buffered(hwlib::i2c_bus& bus, hwlib::location start, fast_byte address) :
	hwlib::window{ { width, height }, hwlib::black, hwlib::white },
	dirty{ false },
	bus(bus),
	address(address),
	start(start)
{ }

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::command(byte d)
{
	byte data[] = { 0x80, d };
	bus.write( 
		address, 
		data, 
		sizeof(data) / sizeof(byte) 
	);
	rtos::current_task()->release();
}

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::command(byte d0, byte d1)
{
	byte data[] = { 0x80, d0, 0x80, d1 };
	bus.write( 
		address, 
		data, 
		sizeof(data) / sizeof(byte) 
	);
	rtos::current_task()->release();
}

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::command(byte d0, byte d1, byte d2)
{
	byte data[] = { 0x80, d0, 0x80, d1, 0x80, d2 };
	bus.write( 
		address, 
		data, 
		sizeof(data) / sizeof(byte) 
	);
	rtos::current_task()->release();
}

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::write_implementation(hwlib::location pos, hwlib::color col)
{
	unsigned int a = pos.x + ( pos.y / 8 ) * width;

	if(col == foreground){ 
		buffer[a] |=  (0x01 << (pos.y % 8));
	} else {
		buffer[a] &= ~(0x01 << (pos.y % 8));
	}
	dirty = true;
}

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::flush()
{
	if(dirty)
	{
		command( COLUMNADDR,  start.x,		width + start.x - 1);
		command( PAGEADDR,    start.y / 8,	(height + start.y) / 8 - 1 );			
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

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::clear()
{
	for(unsigned int i = 0; i < sizeof(buffer) / sizeof(uint8_t); i++) {
		buffer[i] = 0x00;
	}
	dirty = true;
}

template<unsigned int width, unsigned int height>
inline void glcd_oled_part_buffered<width, height>::setLocation(const hwlib::location& loc)
{
	start = loc;
}

#endif // GLCD_OLED_PART_BUFFERED__HPP

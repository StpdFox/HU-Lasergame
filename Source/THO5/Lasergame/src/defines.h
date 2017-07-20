#define block_wait(ticks) { auto t = hwlib::now_ticks() + (ticks); \
      while( hwlib::now_ticks() < t ); }
	  
#define sleep_non_block(ticks) { auto t = hwlib::now_ticks() + (ticks), now = t; \
	while((now = hwlib::now_ticks()) < t) rtos::current_task()->sleep(t - now); }

#ifndef PLAYERNUMBERINPUT_WIDTH
#define PLAYERNUMBERINPUT_WIDTH 0
#endif
#ifndef PLAYERNUMBERINPUT_HEIGHT
#define PLAYERNUMBERINPUT_HEIGHT 0
#endif
#ifndef FIREPOWERINPUT_WIDTH
#define FIREPOWERINPUT_WIDTH 0
#endif
#ifndef FIREPOWERINPUT_HEIGHT
#define FIREPOWERINPUT_HEIGHT 0
#endif
#ifndef GAMEDURATIONINPUT_WIDTH
#define GAMEDURATIONINPUT_WIDTH 0
#endif
#ifndef GAMEDURATIONINPUT_HEIGHT
#define GAMEDURATIONINPUT_HEIGHT 0
#endif
#ifndef GAMETIME_WIDTH
#define GAMETIME_WIDTH 5
#endif
#ifndef GAMETIME_HEIGHT
#define GAMETIME_HEIGHT 1
#endif
#ifndef SCORE_WIDTH
#define SCORE_WIDTH 0
#endif
#ifndef SCORE_HEIGHT
#define SCORE_HEIGHT 0
#endif
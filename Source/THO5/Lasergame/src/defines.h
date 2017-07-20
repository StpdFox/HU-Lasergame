#define block_wait(ticks) { auto t = hwlib::now_ticks() + (ticks); \
      while( hwlib::now_ticks() < t ); }
	  
#define sleep_non_block(ticks) { auto t = hwlib::now_ticks() + (ticks), now = t; \
	while((now = hwlib::now_ticks()) < t) rtos::current_task()->sleep(t - now); }

#define STATUSMESSAGEFIELD_WIDTH 10

#define STATUSMESSAGEFIELD_HEIGHT 2

#define CONFIRMMESSAGEFIELD_WIDTH 12

#define CONFIRMMESSAGEFIELD_HEIGHT 1

#define PLAYERNUMBERINPUT_WIDTH 1

#define PLAYERNUMBERINPUT_HEIGHT 1

#define FIREPOWERINPUT_WIDTH 5

#define FIREPOWERINPUT_HEIGHT 1

#define GAMEDURATIONINPUT_WIDTH 5

#define GAMEDURATIONINPUT_HEIGHT 1

#define GAMETIME_WIDTH 5

#define GAMETIME_HEIGHT 1

#define SCORE_WIDTH 0

#define SCORE_HEIGHT 0

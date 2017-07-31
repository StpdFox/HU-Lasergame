///@file
/**
 * @brief Waits until AT LEAST the specified number of ticks has passed. This macro replaces the hwlib::wait_xx() functions because they sleep() the current task instead of actually blocking execution.
 */
#define block_wait(ticks) { auto t = hwlib::now_ticks() + (ticks); \
      while( hwlib::now_ticks() < t ); }

/**
 * @brief **This is a work-around for a suspected rtos bug and causes more context switches than strictly needed** Waits until AT LEAST the specified number of ticks has passed but does so by sleeping the current task.
 */
#define sleep_non_block(ticks) { auto t = hwlib::now_ticks() + (ticks), now = t; \
	while((now = hwlib::now_ticks()) < t) rtos::current_task()->sleep(t - now); }

#define STATUSMESSAGEFIELD_WIDTH 10

#define STATUSMESSAGEFIELD_HEIGHT 2

#define CONFIRMMESSAGEFIELD_WIDTH 12

#define CONFIRMMESSAGEFIELD_HEIGHT 2

#define PLAYERNUMBERINPUT_WIDTH 1

#define PLAYERNUMBERINPUT_HEIGHT 1

#define FIREPOWERINPUT_WIDTH 5

#define FIREPOWERINPUT_HEIGHT 1

#define GAMEDURATIONINPUT_WIDTH 5

#define GAMEDURATIONINPUT_HEIGHT 1

#define GAMETIME_WIDTH 5

#define GAMETIME_HEIGHT 1

#define SCORE_WIDTH 2

#define SCORE_HEIGHT 1

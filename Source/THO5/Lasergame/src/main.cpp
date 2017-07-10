// simple IR signal detector
#include "hwlib.hpp"
#include "rtos.hpp"
#include "RunGameController.hpp"
#include "OLEDBoundary.hpp"
#include "TestTask.hpp"

int main( void ){
   
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   
   OLEDBoundary oledBoundary{ 1 };
//   TestTask tt{ 2 };
//   tt.setOledBoundary(&oledBoundary);
   RunGameController runGameController{ 2, oledBoundary };

   rtos::run();
}


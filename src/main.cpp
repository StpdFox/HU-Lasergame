// simple IR signal detector

#include "hwlib.hpp"
#include "ir/transmitterController.hpp"
#include "ir/transmitter.hpp"
#include "ir/gameParameters.hpp"
#include "ir/messageLogic.hpp"
#include "ir/receiverController.hpp"
#include "rtos.hpp"

int main( void ){
   
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);



   namespace target = hwlib::target;

    hwlib::target::pin_out vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    hwlib::target::pin_out gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    hwlib::target::pin_in data = hwlib::target::pin_in(hwlib::target::pins::d8);

    auto receiver = receiverController(data,gnd,vcc);
   transmitterController transmitter;
   
   messageLogic messageLogic;
   playerInformation playerInformation;
  
   transmitter.enableFlag();

   rtos::run();
}


// simple IR signal detector

#include "hwlib.hpp"
#include "ir/transmitter.hpp"
#include "ir/gameParameters.hpp"
#include "ir/messageLogic.hpp"
#include "ir/receiverController.hpp"
#include "game.hpp"
#include "rtos.hpp"

int main( void ){
   
   // kill the watchdog
   WDT->WDT_MR = WDT_MR_WDDIS;
   hwlib::wait_ms(1000);
   hwlib::target::pin_in button = hwlib::target::pin_in(hwlib::target::pins::d22);
   hwlib::target::pin_out led = hwlib::target::pin_out(hwlib::target::pins::d24);



   namespace target = hwlib::target;

    hwlib::target::pin_out vcc = hwlib::target::pin_out(hwlib::target::pins::d10);
    hwlib::target::pin_out gnd = hwlib::target::pin_out(hwlib::target::pins::d9);
    hwlib::target::pin_in data = hwlib::target::pin_in(hwlib::target::pins::d8);


    messageLogic messageLogic;
    playerInformation playerInformation;
    char16_t compiledMessage = messageLogic.encode(1,1);
    playerInformation.setCompiledBits(compiledMessage);
    auto receiver = receiverController(data,gnd,vcc,messageLogic);
    

    auto game = gameController(button,led,playerInformation,messageLogic,receiver);

   rtos::run();
}


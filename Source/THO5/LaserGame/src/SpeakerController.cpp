#include "SpeakerController.hpp"

SpeakerController::SpeakerController(hwlib::pin_out &lsp) : task("SpeakerController task"), playSoundFlag(this, "flag"), lsp{lsp} {
}


void await (long long int t){
	while(t > hwlib::now_us()){}
}

void beep( hwlib::pin_out & lsp, int f, int d, int fd = 1000 ){
   auto t = hwlib::now_us();    
   auto end = t + d;
   while( end > hwlib::now_us() ){
       auto p = 500'000 / f;
       f = f * fd / 1000;
       lsp.set( 1 );
       await( t += p );
       lsp.set( 0 );
       await( t += p );
   }   
}

void peew( hwlib::pin_out & lsp ){
   beep( lsp, 20'000, 200'000, 990 );
}

void beep3( hwlib::pin_out & lsp ){
   for( int i = 0; i < 3; i++ ){
       beep( lsp, 1'000, 50'000 );
       hwlib::wait_us( 20'000 );
   }
}

void ohhh( hwlib::pin_out & lsp ) {
   for (int i=1000; i<2000; i=i*1.02) { 
      beep(lsp,i,10'000); 
   } 
   for (int i=2000; i>1000; i=i*.98) {
      beep(lsp,i,10'000);
   }
}


void uhoh( hwlib::pin_out & lsp ) {
   for (int i=1000; i<1244; i=i*1.01) { 
      beep(lsp,i,10'000); 
   } 
   hwlib::wait_us( 30'000 );
   for (int i=1244; i>1108; i=i*.99) {
      beep(lsp,i,10'000);
   }
}

void noise( hwlib::pin_out & lsp ) {
   for (int i = 0; i < 200; i++ ){
      lsp.set( ( hwlib::rand() & 0x01 ) == 0);
      hwlib::wait_us( 1'000 ); 
   }     
}    

void noise2( hwlib::pin_out & lsp ){
   for (int i = 0; i < 200; i++ ){
      lsp.set( 1 );
      hwlib::wait_us( hwlib::random_in_range( 500, 2000 )); 
      lsp.set( 0 );
      hwlib::wait_us( hwlib::random_in_range( 500, 2000 )); 
   }     
}    

void click( hwlib::pin_out & lsp ){
   for (int i = 0; i < 200; i++ ){
      lsp.set( 1 );
      hwlib::wait_us( hwlib::random_in_range( 500, 600 )); 
      lsp.set( 0 );
      hwlib::wait_us( hwlib::random_in_range( 500, 600 )); 
   }     
} 

void SpeakerController::main(){
	for(;;) {
        //wait(playSoundFlag);
		setSound(Sounds::SHOOT);
    }
}

void SpeakerController::setSound(Sounds sounds){
	switch(sounds){
		case Sounds::HIT : ohhh(lsp);   break;
		case Sounds::SHOOT : peew(lsp); break;
		case Sounds::START_GAME : click(lsp);  break;
		case Sounds::END_GAME : noise2(lsp); break;
		default : break;
	}
	
}
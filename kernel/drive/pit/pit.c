#include <os.h>

#define FREQ 100

uint32_t timer_ticks = 0;
uint32_t  eticks = 0;

void set_frequencia(uint32_t freq)
{

	uint32_t divisor = 1193180/freq;

    	// Control uint16_t register
	// bits 7-6 = 0 - Set counter 0 (counter divisor),bit 5-4=11 LSB/MSB 16-bit
	// bit 3-1=x11 Square wave generator, bit 0 =0 Binary counter 16-bit
	outportb(0x43,0x36);
	outportb(0x40,(uint8_t)(divisor & 0xFF));		
	outportb(0x40,(uint8_t)(divisor >> 8) & 0xFF);

}


static void timer_wait(uint32_t  ticks)
{


	
	eticks = ticks;

	while(eticks){
	
	}



}


void sleep(uint32_t segundos)
{
	timer_wait(FREQ * segundos);

}

void delay(uint32_t  milesegundo)
{

	timer_wait(FREQ * milesegundo/1000);

}


void timer_handler(void)
{

	//++timer_ticks;
	if(eticks)--eticks;

}

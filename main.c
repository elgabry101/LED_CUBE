#include <avr/delay.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#define latch 2
#define clock 13
#define data 11


const long level[] = { 0x20000000, 0x40000000, 0x80000000 };
volatile char red[3][9];  //value for each red led in each level
volatile char green[3][9];
volatile char blue[3][9];
volatile char bam_counter = 0;
volatile char l = 0;

void light(char x, char y, char z, char r, char g, char b);
void transfer(uint16_t a);

int main() {
	TCCR0B = 0b00001010;
	TIMSK0 = 0b00000010;
	OCR0A = 30;
	DDRD|=(1<<2);
	DDRB|=(1<<2)|(1<<3)|(1<<5);
	PORTB|=(1<<2);
	SPCR=(1<<SPE)|(1<<MSTR);
	SPSR=1;
	sei();
	while(1)
	{
	show();
	}
}

void light(char x, char y, char z, char r, char g, char b) {
	if (x < 0)
	x = 0;
	else if (x > 2)
	x = 2;
	if (y < 0)
	y = 0;
	else if (y > 2)
	y = 2;
	if (z < 0)
	z = 0;
	else if (z > 2)
	z = 2;
	if (r < 0)
	r = 0;
	else if (r > 15)
	r = 15;
	if (g < 0)
	g = 0;
	else if (g > 15)
	g = 15;
	if (b < 0)
	b = 0;
	else if (b > 15)
	b = 15;
	int w = x + (y * 3);
	red[z][w] = r;
	green[z][w] = g;
	blue[z][w] = b;
}

ISR(TIMER0_COMPA_vect) {
	unsigned long send = 0;

	if (bam_counter < 3) {
		for (int i = 8; i >= 0; i--) {
			send |= blue[l][i] & 1;
			send <<= 1;
		}
		for (int i = 8; i >= 0; i--) {
			send |= green[l][i] & 1;
			send <<= 1;
		}
		for (int i = 8; i > 0; i--) {
			send |= red[l][i] & 1;
			send <<= 1;
		}
		send |= red[l][0] & 1;
	}
	
	else if (bam_counter < 9) {
		for (int i = 8; i >= 0; i--) {
			send |= (blue[l][i] >> 1) & 1;
			send <<= 1;
		}
		for (int i = 8; i >= 0; i--) {
			send |= (green[l][i] >> 1) & 1;
			send <<= 1;
		}
		for (int i = 8; i > 0; i--) {
			send |= (red[l][i] >> 1) & 1;
			send <<= 1;
		}
		send |= (red[l][0] >> 1) & 1;
	}

	else if (bam_counter < 21) {
		for (int i = 8; i >= 0; i--) {
			send |= (blue[l][i] >> 2) & 1;
			send <<= 1;
		}
		for (int i = 8; i >= 0; i--) {
			send |= (green[l][i] >> 2) & 1;
			send <<= 1;
		}
		for (int i = 8; i > 0; i--) {
			send |= (red[l][i] >> 2) & 1;
			send <<= 1;
		}
		send |= (red[l][0] >> 2) & 1;
	}
	
	else if (bam_counter < 45) {
		for (int i = 8; i >= 0; i--) {
			send |= (blue[l][i] >> 3) & 1;
			send <<= 1;
		}
		for (int i = 8; i >= 0; i--) {
			send |= (green[l][i] >> 3) & 1;
			send <<= 1;
		}
		for (int i = 8; i > 0; i--) {
			send |= (red[l][i] >> 3) & 1;
			send <<= 1;
		}
		send |= (red[l][0] >> 3) & 1;
	}
	send |= level[l];
	PORTD&=!(1<<2);
	transfer(send >> 16);
	transfer(send);
	PORTD|=(1<<2);
	l++;
	bam_counter++;
	if (l == 3)
	l = 0;
	if (bam_counter == 45)
	bam_counter = 0;
}

void transfer(uint16_t a)
{
	SPDR=(a>>8);
	while(!(SPSR&(1<<SPIF)));
	SPDR=a;
	while(!(SPSR&(1<<SPIF)));

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void aroundtheworld(){

	for(int i=0;i<3;i++)
	{
		light(i,0,0,15,0,0);
		light(i,0,1,15,0,0);
		light(i,0,2,15,0,0);
		_delay_ms(520);
		light(i,0,0,0,0,0);
		light(i,0,1,0,0,0);
		light(i,0,2,0,0,0);
	}
	for(int i=0;i<3;i++)
	{
		light(2,i,0,15,0,0);
		light(2,i,1,15,0,0);
		light(2,i,2,15,0,0);
		_delay_ms(520);
		light(2,i,0,0,0,0);
		light(2,i,1,0,0,0);
		light(2,i,2,0,0,0);

	}
	for(int i=2;i>=0;i--)
	{
		light(i,2,0,7,3,0);
		light(i,2,1,7,3,0);
		light(i,2,2,7,3,0);
		_delay_ms(520);
		light(i,2,0,0,0,0);
		light(i,2,1,0,0,0);
		light(i,2,2,0,0,0);

	}
	for(int i=2;i>=0;i--)
	{
		light(0,i,0,5,10,0);
		light(0,i,1,5,10,0);
		light(0,i,2,5,10,0);
		_delay_ms(520);
		light(0,i,0,0,0,0);
		light(0,i,1,0,0,0);
		light(0,i,2,0,0,0);
	}

	light(0,0,3,0,15,0);
	light(1,0,1,0,15,0);
	light(2,0,0,0,15,0);
	_delay_ms(1040);
	light(0,0,3,0,0,0);
	light(1,0,1,0,0,0);
	light(2,0,0,0,0,0);
	light(0,0,2,0,15,0);
	light(1,0,2,0,15,0);
	light(2,0,2,0,15,0);
	_delay_ms(1040);
	for(int i=0;i<3;i++)
	{
		light(0,i,2,0,15,0);
		light(1,i,2,0,15,0);
		light(2,i,2,0,15,0);
		_delay_ms(520);
		light(0,i,2,0,0,0);
		light(1,i,2,0,0,0);
		light(2,i,2,0,0,0);
	}
	for(int i=2;i>-1;i--)
	{
		light(0,2,i,0,15,0);
		light(1,2,i,0,15,0);
		light(2,2,i,0,15,0);
		_delay_ms(520);
		light(0,2,i,0,0,0);
		light(1,2,i,0,0,0);
		light(2,2,i,0,0,0);
	}
	for(int i=2;i>=0;i--)
	{
		light(0,i,0,0,15,0);
		light(1,i,0,0,15,0);
		light(2,i,0,0,15,0);
		_delay_ms(520);
		light(0,i,0,0,0,0);
		light(1,i,0,0,0,0);
		light(2,i,0,0,0,0);

	}
	for(int i=0;i<3;i++)
	{
		light(0,0,i,0,10,3);
		light(1,0,i,0,10,3);
		light(2,0,i,0,10,3);
		_delay_ms(520);
		light(0,0,i,0,0,0);
		light(1,0,i,0,0,0);
		light(2,0,i,0,0,0);
	}

	light(2,0,2,0,7,7);
	light(1,0,1,0,7,7);
	light(0,0,0,0,7,7);
	_delay_ms(1040);
	light(2,0,2,0,0,0);
	light(1,0,1,0,0,0);
	light(0,0,0,0,0,0);
	for(int i=2;i>=0;i--)
	{
		light(i,0,0,0,3,12);
		light(i,0,1,0,3,12);
		light(i,0,2,0,3,12);
		_delay_ms(520);
		light(i,0,0,0,0,0);
		light(i,0,1,0,0,0);
		light(i,0,2,0,0,0);
	}
	for(int i=0;i<3;i++)
	{
		light(0,i,0,0,0,15);
		light(0,i,1,0,0,15);
		light(0,i,2,0,0,15);
		_delay_ms(520);
		light(0,i,0,0,0,0);
		light(0,i,1,0,0,0);
		light(0,i,2,0,0,0);

	}
	for(int i=0;i<3;i++)
	{
		light(i,2,0,0,0,15);
		light(i,2,1,0,0,15);
		light(i,2,2,0,0,15);
		_delay_ms(520);
		light(i,2,0,0,0,0);
		light(i,2,1,0,0,0);
		light(i,2,2,0,0,0);

	}
	for(int i=2;i>=0;i--)
	{
		light(2,i,0,0,0,15);
		light(2,i,1,0,0,15);
		light(2,i,2,0,0,15);
		_delay_ms(520);
		light(2,i,0,0,0,0);
		light(2,i,1,0,0,0);
		light(2,i,2,0,0,0);
	}

	light(0,0,2,15,15,15);
	light(1,0,1,15,15,15);
	light(2,0,0,15,15,15);
	_delay_ms(1040);
	light(0,0,2,0,0,0);
	light(1,0,1,0,0,0);
	light(2,0,0,0,0,0);
	light(0,0,0,15,15,15);
	light(1,0,0,15,15,15);
	light(2,0,0,15,15,15);
	_delay_ms(1040);
	for(int i=0;i<3;i++)
	{
		light(0,i,0,15,15,15);
		light(1,i,0,15,15,15);
		light(2,i,0,15,15,15);
		_delay_ms(520);
		light(0,i,0,0,0,0);
		light(1,i,0,0,0,0);
		light(2,i,0,0,0,0);
	}
	for(int i=0;i<3;i++)
	{
		light(0,2,i,15,15,15);
		light(1,2,i,15,15,15);
		light(2,2,i,15,15,15);
		_delay_ms(520);
		light(0,2,i,0,0,0);
		light(1,2,i,0,0,0);
		light(2,2,i,0,0,0);
	}
	for(int i=2;i>=0;i--)
	{
		light(0,i,3,15,10,10);
		light(1,i,3,15,10,10);
		light(2,i,3,15,10,10);
		_delay_ms(520);
		light(0,i,3,0,0,0);
		light(1,i,3,0,0,0);
		light(2,i,3,0,0,0);

	}
	for(int i=2;i>=0;i--)
	{
		light(0,0,i,15,7,7);
		light(1,0,i,15,7,7);
		light(2,0,i,15,7,7);
		_delay_ms(520);
		light(0,0,i,0,0,0);
		light(1,0,i,0,0,0);
		light(2,0,i,0,0,0);
	}
	light(2,0,2,15,4,4);
	light(1,0,1,15,4,4);
	light(0,0,0,15,4,4);
	_delay_ms(1040);
	light(2,0,2,0,0,0);
	light(1,0,1,0,0,0);
	light(0,0,0,0,0,0);
}

void radioactive()
{
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,1,1,1);
				
			}
		}
	}

	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,2,2,2);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,3,3,3);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,4,4,4);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,5,5,5);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,6,6,6);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,7,7,7);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,8,8,8);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,9,9,9);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,10,10,10);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,12,12,12);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,13,13,13);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,14,14,14);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,15,15,15);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,14,14,14);
				
			}
		}
	}

	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,13,13,13);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,12,12,12);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,11,11,11);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,10,10,10);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,9,9,9);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,8,8,8);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,7,7,7);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,6,6,6);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,5,5,5);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,4,4,4);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,3,3,3);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,2,2,2);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,1,1,1);
				
			}
		}
	}
	_delay_ms(480);
	for(int i=0;i<3;i++)
	{
		for(int s=0;s<3;s++)
		{
			for(int y=0;y<3;y++)
			{
				light(i,s,y,0,0,0);
				
			}
		}
	}
	_delay_ms(480);

}


void dot()
{
	for(int i=0;i<3;i++)
	{
		light(i,0,2,15,0,0);
		_delay_ms(960);
		light(i,0,2,0,0,0);
	}

	for(int i=2;i>=0;i--)
	{
		light(i,1,2,12,7,0);
		_delay_ms(960);
		light(i,1,2,0,0,0);
	}
	for(int i=0;i<3;i++)
	{
		light(i,2,2,7,11,0);
		_delay_ms(960);
		light(i,2,2,0,0,0);
	}

	for(int i=2;i>=0;i--)
	{
		light(i,2,1,0,15,0);
		_delay_ms(960);
		light(i,2,1,0,0,0);
	}

	for(int i=0;i<3;i++)
	{
		light(i,1,1,0,12,7);
		_delay_ms(960);
		light(i,1,1,0,0,0);
	}

	for(int i=2;i>=0;i--)
	{
		light(i,0,1,0,7,11);
		_delay_ms(960);
		light(i,0,1,0,0,0);
	}


	for(int i=0;i<3;i++)
	{
		light(i,0,0,0,0,15);
		_delay_ms(960);
		light(i,0,0,0,0,0);
	}

	for(int i=2;i>=0;i--)
	{
		light(i,1,0,7,0,12);
		_delay_ms(960);
		light(i,1,0,0,0,0);
	}
	for(int i=0;i<3;i++)
	{
		light(i,2,0,11,0,7);
		_delay_ms(960);
		light(i,2,0,0,0,0);
	}
	light(1,1,1,15,0,3);
	_delay_ms(960);
	light(1,1,1,0,0,0);
	_delay_ms(960);

}

void rain(int n){

	char x,y,r,g,b;
	for(int i=0;i<n;i++)
	{
		x=random(i)%4;
		y=random(i)%4;
		r=random(i+18)%16;
		g=random(i+90)%16;
		b=random(i+800)%16;
		if(r==15&&g==15&&b==15)
		{
			light(x,y,0,r,g,b);
			_delay_ms(1200);
			light(x,y,0,0,0,0);
			light(x,y,1,r,g,b);
			_delay_ms(1200);
			light(x,y,1,0,0,0);
			light(x,y,2,r,g,b);
			_delay_ms(1200);
			light(x,y,2,0,0,0);
			delay(9600-(random(i+900)%9601));
		}
		else
		{
			light(x,y,2,r,g,b);
			_delay_ms(1200);
			light(x,y,2,0,0,0);
			light(x,y,1,r,g,b);
			_delay_ms(1200);
			light(x,y,1,0,0,0);
			light(x,y,0,r,g,b);
			_delay_ms(1200);
			light(x,y,0,0,0,0);
			delay(9600-(random(i+500)%9601));
		}

	}


}

void show(){

	radioactive();
	radioactive();
	radioactive();
	_delay_ms(1600);
	for(int i=0;i<3;i++)
	{
		for(int p=0;p<3;p++)
		{
			light(i,0,p,15,0,0);
		}
	}
	_delay_ms(4800);
	for(int i=0;i<3;i++)
	{
		for(int p=0;p<3;p++)
		{
			light(i,1,p,0,15,0);
		}
	}
	_delay_ms(4800);
	for(int i=0;i<3;i++)
	{
		for(int p=0;p<3;p++)
		{
			light(i,2,p,0,0,15);
		}
	}
	_delay_ms(4800);
	for(int i=0;i<3;i++)
	{
		for(int p=0;p<3;p++)
		{
			light(i,p,0,0,0,0);
		}
	}
	_delay_ms(4800);
	for(int i=0;i<3;i++)
	{
		for(int p=0;p<3;p++)
		{
			light(i,p,1,0,0,0);
		}
	}
	_delay_ms(4800);
	for(int i=0;i<3;i++)
	{
		for(int p=0;p<3;p++)
		{
			light(i,p,2,0,0,0);
		}
	}
	_delay_ms(4800);
	dot();
	aroundtheworld();
	rain(300);
}


void delay(int s)
{
	for(int i=0;i<s;i++)
	{
		_delay_ms(1);
	}
	
}
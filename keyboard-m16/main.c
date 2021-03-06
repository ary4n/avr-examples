#include <mega16.h>
#include <alcd.h>
#include <delay.h>
#include <stdio.h>



unsigned char buffer[16];

unsigned char page = 0;
unsigned char keyboard = 0;
bit update = 1;

unsigned char map[] = {10,0,11,12,1,2,3,13,4,5,6,14,7,8,9,15};

#define GOTO(p) page=p; update=1;
#define UPDATE() update=1;


char read(){

	unsigned char r, c;
	PORTD |= 0x0f;

	for(c=0;c<4;c++){
		DDRD = 0x00;
		DDRD |= 0x80 >> c;
		for(r=0;r<4;r++){
			if(!(PIND & (0x08 >> r))){
				return map[r*4+c];
			}
		}
	}

	return 0xff;

}


void main(void)
{



	unsigned char last_key=0xff;
	unsigned int weight = 0;

    lcd_init(16);
    while (1)
    {

		if(page == 0 && update){
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_puts("LCD Test! Hello ");
			lcd_gotoxy(0,1);
			lcd_puts("Press ON.. ");
			update = 0;
		}else if(page == 1 && update){
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_puts("Enter your weight:");
			lcd_gotoxy(0,1);
			sprintf(buffer, "%i ", weight);
			lcd_puts(buffer);
			update = 0;
		} else if(page == 2 && update){
			lcd_clear();
			lcd_gotoxy(0,0);
			lcd_puts("This is page 3");
			lcd_gotoxy(0,1);
			lcd_puts("haha.. ");
			update = 0;
		}

		switch(page){
			case 0:
				keyboard = read();
				if(keyboard != 0xff && keyboard != last_key){
					if(keyboard == 10)
						GOTO(1);
					last_key = keyboard;
				}
				break;
			case 1:

        		keyboard = read();
				if(keyboard != 0xff && keyboard != last_key){
					if(keyboard >= 0 && keyboard <= 9){
						weight = weight * 10 + keyboard;
						UPDATE();
					}
					if(keyboard == 15){
						GOTO(2);
					}
					last_key = keyboard;
				}
				break;

		}

    }
}

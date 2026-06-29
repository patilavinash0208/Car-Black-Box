/*
 * File:   main.c
 * Author: avinash
 *
 * Created on June 18, 2026, 10:44 AM
 */


#include <xc.h>
#include<string.h>
#include "adc.h"
#include "clcd.h"
#include "ds1307.h"
#include "i2c.h"
#include "matrix_keypad.h"
#include "uart.h"
#include "timer0.h"
#include "external_eeprom.h"

unsigned char clock_reg[3];
unsigned char pos=0 , read_pos=0;
static void get_time(void);
//unsigned char time[9];
char *menu[4]={"Download Log","Set Log","Clear Log","View Log"};
char flag=0, blink = 1;
struct details{
    char speed[4];
    char gear[2];
    char time[9];
}; 
struct details Info[10];
struct details read_Info[10];

void __interrupt() isr(void)
{
	static unsigned short count;
     
	if (TMR0IF)
	{
		TMR0 = TMR0 + 8;

		if (count++ == 10000)
		{
			count = 0;
			blink = !blink;
		}
		TMR0IF = 0;
	}
}

void read_data(){
    unsigned int add=0;
    for(int k=0 ; k <= read_pos; k++)
    {
        for(int l=0; l<3 ; l++){
            read_Info[k].speed[l] = read_external_eeprom(add++);
        }
        add++;
        read_Info[k].speed[3] = 0;
        for(int l=0; l<1 ; l++){
            read_Info[k].gear[l] = read_external_eeprom(add++);
        }
        add++;
        read_Info[k].gear[1] = 0;
        for(int l=0; l<8 ; l++){
            read_Info[k].time[l] = read_external_eeprom(add++);
        }
        read_Info[k].time[8] = 0;
    }
    
}

void store_data()
{
    unsigned int add=0;
    for(int k=0 ; k <= pos; k++)
    {
        for(int l=0; Info[k].speed[l] != 0 ; l++){
            write_external_eeprom(add++,Info[k].speed[l]);
        }
            write_external_eeprom(add++,' ');
        for(int l=0; Info[k].gear[l] != 0 ; l++){
            write_external_eeprom(add++,Info[k].gear[l]);
        }
            write_external_eeprom(add++,' ');
        for(int l=0; Info[k].time[l] != 0 ; l++){
            write_external_eeprom(add++,Info[k].time[l]);
        }
    }
}

void download_log()
{
    read_data();
    puts("SPD GEAR   RTC\n\r");
    for(int i=0 ; i<=pos ; i++)
    {
        puts(read_Info[i].speed);
        puts("  ");
        puts(read_Info[i].gear);
        puts("   ");
        puts(read_Info[i].time);
        puts("\n\r");
    }
    
    CLEAR_DISP_SCREEN;
    do{
       clcd_print("Download Log",LINE1(0));
       clcd_print("Success",LINE2(0)); 
    }while(read_switches(STATE_CHANGE) != MK_SW12);
    CLEAR_DISP_SCREEN;
}

void set_log()
{
    CLEAR_DISP_SCREEN;
    unsigned char dummy;
    char mode=0;
    char key, hr, min, sec;
    clcd_print("RTC",LINE1(2));
    dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy | 0x80);
    get_time();
    do{
        hr = (Info[pos].time[0] - '0') * 10 + (Info[pos].time[1] - '0');
        min = (Info[pos].time[3] - '0') * 10 + (Info[pos].time[4] - '0');
        sec = (Info[pos].time[6] - '0') * 10 + (Info[pos].time[7] - '0');
        
            clcd_print(Info[pos].time,LINE2(0));
        if(blink)
        {
            if(mode == 0)
                clcd_print("__",LINE2(0));
            else if(mode == 1)
                clcd_print("__",LINE2(3));
            else if(mode == 2)
                clcd_print("__",LINE2(6));
        }
        key = read_switches(STATE_CHANGE);
        if(key == MK_SW5)
        {
            if(mode < 2)
                mode++;
        }
        else if(key == MK_SW6)
        {
            if(mode > 0)
                mode--;
        }
        else if(key ==  MK_SW1)
        {
            if(mode == 0){
                if(++hr == 24) 
                    hr = 0;
            }
            else if(mode == 1){
                if(++min == 60) 
                    min = 0;
            }
            else if(mode == 2){
                if(++sec == 60) 
                    sec = 0;
            }
        }
        else if(key ==  MK_SW2)
        {
            if(mode == 0){
                if(hr == 0)
                    hr = 23;
                else
                    hr--;
            }
            else if(mode == 1){
                if(min == 0)
                    min = 59;
                else
                    min--;
            }
            else if(mode == 2){
                if(sec == 0)
                    sec = 59;
                else
                    sec--;
            }
        }
        
        write_ds1307(SEC_ADDR,0x80 | ((sec / 10) << 4) | (sec % 10));
        write_ds1307(MIN_ADDR, (min/10)<<4 | (min%10));
        write_ds1307(HOUR_ADDR, (hr/10)<<4 | (hr%10));
        
    }while(key != MK_SW12);
    CLEAR_DISP_SCREEN;
    dummy = read_ds1307(SEC_ADDR);
	write_ds1307(SEC_ADDR, dummy & 0x7F); 
}

void clear_log()
{
    pos = 0;
    for(int i=0; i<150 ;i++)
       write_external_eeprom(i, 0); 
        CLEAR_DISP_SCREEN;
    do{
        clcd_print("Clear Log",LINE1(0));
        clcd_print("Success",LINE2(0));
    }while(read_switches(STATE_CHANGE) != MK_SW12);
    CLEAR_DISP_SCREEN;
}

void view_log()
{
    read_data();
    char i=0;
    char key;
    CLEAR_DISP_SCREEN;
    do{
        if(i==pos)
        clcd_print("                ",LINE2(0));
        
        key = read_switches(STATE_CHANGE);
        clcd_print(read_Info[i].speed,LINE1(0));
        clcd_print(read_Info[i].gear,LINE1(5));
        clcd_print(read_Info[i].time,LINE1(8));
        clcd_print(read_Info[i+1].speed,LINE2(0));
        clcd_print(read_Info[i+1].gear,LINE2(5));
        clcd_print(read_Info[i+1].time,LINE2(8));
        if(key == MK_SW1){
            if(i<8){
                i++;
            }
        }
        else if(key==MK_SW2){
            if(i>0){
                i--;
            }
        }   
    }while(key != MK_SW12);
    CLEAR_DISP_SCREEN;
}

void enter_menu(){
    char i=0;
    char arr=0;
    char key;
    do{
        key = read_switches(STATE_CHANGE);
        if(arr==0){
            clcd_print("->",LINE1(0));
            clcd_print("  ",LINE2(0));
        }
        else{
            clcd_print("->",LINE2(0));
            clcd_print("  ",LINE1(0));
        }
        
        clcd_print(menu[i],LINE1(3));
        clcd_print(menu[i+1],LINE2(3));
        if(key==MK_SW1){
            if(i<2 && arr==1){
                i++;
                CLEAR_DISP_SCREEN;
            }
            arr=1;
        }
        else if(key==MK_SW2){
            if(i>0 && arr==0){
                i--;
                CLEAR_DISP_SCREEN;
            }
            arr=0;
        }
        else if(key==MK_SW11){
            char j;
            if(arr==0)
                j = i;
            else 
                j = i+1;
            
            if(!strcmp(menu[j], "Download Log"))
                download_log();
            else if(!strcmp(menu[j], "Set Log"))
                set_log();
            else if(!strcmp(menu[j], "Clear Log"))
                clear_log();
            else if(!strcmp(menu[j], "View Log"))
                view_log();
        }   
    }while(key != MK_SW12);
    CLEAR_DISP_SCREEN;
}
unsigned int get_speed()
{
    // Implement the speed function
    return read_adc(CHANNEL4)/10.23;
}

unsigned char get_gear_pos()
{
    // Implement the gear function
    unsigned char gear[] = {'N','1','2','3','4','5','R','C'};
    static char idx = 0;
    char key = read_switches(STATE_CHANGE);
    
    if(key == MK_SW1)
    {
        if(idx < 6)
        {
            flag=1;
            idx++;
            if(pos < 9)
                pos++;
        }
    }
    else if(key == MK_SW2)
    {
        if(idx > 0 && idx != 7)
        {
            flag=1;
            idx--;
            if(pos < 9)
                pos++;
        }
    }
    else if(key == MK_SW3)
    {
        flag=1;
        idx = 7;
        if(pos < 9)
            pos++;
    }
    else{
        flag=0;
    }
    read_pos = pos;
    return gear[idx];
}

static void get_time(void)
{
	clock_reg[0] = read_ds1307(HOUR_ADDR);
	clock_reg[1] = read_ds1307(MIN_ADDR);
	clock_reg[2] = read_ds1307(SEC_ADDR);

	if (clock_reg[0] & 0x40)
	{
		Info[pos].time[0] = '0' + ((clock_reg[0] >> 4) & 0x01);
		Info[pos].time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	else
	{
		Info[pos].time[0] = '0' + ((clock_reg[0] >> 4) & 0x03);
		Info[pos].time[1] = '0' + (clock_reg[0] & 0x0F);
	}
	Info[pos].time[2] = ':';
	Info[pos].time[3] = '0' + ((clock_reg[1] >> 4) & 0x0F);
	Info[pos].time[4] = '0' + (clock_reg[1] & 0x0F);
	Info[pos].time[5] = ':';
	Info[pos].time[6] = '0' + ((clock_reg[2] >> 4) & 0x0F);
	Info[pos].time[7] = '0' + (clock_reg[2] & 0x0F);
	Info[pos].time[8] = '\0';
}

void main(void) {
    init_adc();
    init_clcd();
    init_i2c();
    init_ds1307();
    init_timer0();
    init_matrix_keypad();
    init_uart();
   //char speed[4];
    //char gear[2];
    
    
    while(1)
    {
        clcd_print("SPD",LINE1(0));
        clcd_print("GR",LINE1(5));
        clcd_print("RTC",LINE1(11)); 
        int speed_v = get_speed();
        
        
        unsigned char gear_v = get_gear_pos();
        Info[pos].gear[0] = gear_v;
        Info[pos].gear[1] = 0;
        
        Info[pos].speed[0] = (speed_v/100)%10 + '0';
        Info[pos].speed[1] = (speed_v/10)%10 + '0';
        Info[pos].speed[2] = (speed_v)%10 + '0';
        Info[pos].speed[3] = 0;
        clcd_print(Info[pos].speed,LINE2(0));
        clcd_print(Info[pos].gear,LINE2(5));
        
        get_time();
        clcd_print(Info[pos].time,LINE2(8));
        if(flag==1){   
            if(pos == 9)
            {
                for(int i=0; i<9 ; i++)
                    Info[i] = Info[i+1];
            }
            store_data();
        }
        char key = read_switches(STATE_CHANGE);
        if(key == MK_SW11){
            CLEAR_DISP_SCREEN;
            enter_menu();
        }
        
        
    }
    
    return;
}

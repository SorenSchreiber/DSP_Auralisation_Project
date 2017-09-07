/*******  user\apps\Audio\Audio_Auraljisation\src\Peripheral_control.c
*
* Summary:
*		Peripheral Control function for different programm states
*
* Dependences:
*		ex_timer.h
*		Peripheral_control.h
*
* Inputs:
*		State of the programm		int
*		Type						int
*
* Outputs:
*		LEDs output
*
* Functions:
*		int displayState(int state);
*		void readyState();
*		void errorState();
*		void analysingState(int type);
*		void playbackState();
*		void turnOnAll();
*		void turnOffAll();
*
* Notes:
*		none
*
* Originator: 
*		Sören Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* Version:
*		1.0		19/04/2016
*
*
*/
#include <board\h\sask.h>
#include <peripherals\timers\inc\ex_timer.h>

#include "..\inc\Peripheral_control.h"

int currentState; //Variable containing the programs current state -> returned to calling program code for further use

float instruction_clock_frequency, instruction_cycle_time;	//variables needed for timer functions -> types taken from the needs of the timer functions


int displayState(int state)	//hub function for state selection of the program
{
	switch(state)
	{
		case 0:
				currentState=0;	//set currentState to 0
				readyState();	//display ready state on the LEDs
				break;
		case 1:
				currentState=1;	//set current state to 1
				break;
		case 3:
				currentState=3; //set current state to 3
				analysingState(0);
				break;
		case 4:
				currentState=4;
				playbackState();
				break;
		default:
				currentState=2;	//set current state to 2
				errorState();	//display error state on the LEDs
	}

	return currentState;	//return current state to the calling program code
}

void readyState()	//function to display ready state using the LEDs
{
	while(currentState==0)	//while program is in ready state
	{
		if((SWITCH_S1==0) && (SWITCH_S2==1))	//checking if Switch 1 is pressed and switch 2 is unpressed (direct read of the buttons to bypass a debouncer problem)
		{
			currentState=1;	//set current state to 1
		}
		else
		{
			GREEN_LED=SASK_LED_ON;
			YELLOW_LED=SASK_LED_OFF;
			RED_LED=SASK_LED_OFF;
		}
	}
}

void analysingState(int type) //display analysing state on leds
{
	turnOffAll();

	if(type==0)
	{
		while(1)
		{
			if((SWITCH_S1==1) && (SWITCH_S2==0))	//check if switch 2 is pressed
			{
				currentState=4;			//set current state to play back
				break;
			}
			
			RED_LED=SASK_LED_OFF;
			YELLOW_LED=SASK_LED_ON;
			GREEN_LED=SASK_LED_OFF;
		}
	}
	else if(type==1)
	{
		RED_LED=SASK_LED_ON;
		YELLOW_LED=SASK_LED_OFF;
		GREEN_LED=SASK_LED_OFF;
	}
	else if(type==2)
	{
		RED_LED=SASK_LED_ON;
		YELLOW_LED=SASK_LED_ON;
		GREEN_LED=SASK_LED_OFF;
	}
	else if(type==3)
	{
		RED_LED=SASK_LED_ON;
		YELLOW_LED=SASK_LED_ON;
		GREEN_LED=SASK_LED_ON;
	}
	else
	{
		errorState();
	}
}

void playbackState() //display playback state on leds
{
	instruction_clock_frequency = clock_frequency;				//set up variable needed for fo the timer function
	instruction_cycle_time = 1.0 / instruction_clock_frequency;	//set up variable needed for fo the timer function

	ex_timer_init( instruction_cycle_time );			//init timer 

	int cycling=TRUE;
	int cycle=0;

	turnOffAll();

	while(cycling)
	{
		if(cycle==0)
		{
			RED_LED=SASK_LED_ON;
			YELLOW_LED=SASK_LED_OFF;
			GREEN_LED=SASK_LED_OFF;
			
			cycle=cycle+1;
		}
		else if(cycle==1)
		{
			RED_LED=SASK_LED_OFF;
			YELLOW_LED=SASK_LED_ON;
			GREEN_LED=SASK_LED_OFF;
			
			cycle=cycle+1;
		}
		else if(cycle==2)
		{
			RED_LED=SASK_LED_OFF;
			YELLOW_LED=SASK_LED_OFF;
			GREEN_LED=SASK_LED_ON;
		
			cycling=FALSE;
		}
		else
		{
			errorState();
			break;
		}
		
		ex_timer_wait(timeout);	//wait for the time[in seconds] set in timeout(defined in header file)
	}
}

void errorState()		//display error state on the LEDs
{
	instruction_clock_frequency = clock_frequency;					//set up variable needed for fo the timer function
	instruction_cycle_time = 1.0 / instruction_clock_frequency;		//set up variable needed for fo the timer function

	ex_timer_init( instruction_cycle_time );		//init timer

	int current_led_state=FALSE; 	//set LED status to OFF

	while(1)
	{
		if((SWITCH_S1==0) && (SWITCH_S2==0))	//check if switch 1 and 2 are pressed
		{
			current_led_state=FALSE;	//set LED status to OFF
			currentState=0;			//set current state to ready
			break;
		}
		else if(current_led_state==FALSE) //if LEDs are off turn them on
		{
			RED_LED=SASK_LED_ON;
			YELLOW_LED=SASK_LED_ON;
			GREEN_LED=SASK_LED_ON;

			current_led_state=TRUE;		//invert current LED status
		}
		else if(current_led_state==TRUE) //if LEDs are on turn them off
		{

			RED_LED=SASK_LED_OFF;
			YELLOW_LED=SASK_LED_OFF;
			GREEN_LED=SASK_LED_OFF;

			current_led_state=FALSE;	//invert current LED status
		}

		ex_timer_wait(timeout);	//wait for the time[in seconds] set in timeout(defined in header file)
	}
}

void turnOffAll()
{
	RED_LED=SASK_LED_OFF;
	YELLOW_LED=SASK_LED_OFF;
	GREEN_LED=SASK_LED_OFF;
}

void turnOnAll()
{
	RED_LED=SASK_LED_ON;
	YELLOW_LED=SASK_LED_ON;
	GREEN_LED=SASK_LED_ON;
}

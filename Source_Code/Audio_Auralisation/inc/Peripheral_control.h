/*******  user\apps\Audio_Auralisation\inc\Peripheral_control.h
*
* Summary:
*		Peripheral control function for different programm states
*
* Dependences:
*		ex_timer.h
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
*/

#ifndef __LED_CONTROL__
#define __LED_CONTROL__		//define __LED_CONTROL__

#define STATE_READY 0 //ready state
#define STATE_RUN 1 //run state
#define STATE_ERROR 2 //error state
#define STATE_ANALYSE 3 //analysing state
#define STATE_PLAYBACK 4 //playback state

#define LED_ON 1 //led on
#define LED_OFF 0 //led off

#define TRUE 1 //bool true
#define FALSE 0 //bool false

#define clock_frequency 40e6 // standart clock frequency
#define timeout 0.3 //wait time for the timer 

int displayState(int state); //switch between different running states[0=READY, 1=RUN, 2=ERROR, 3=ANALYSING, 4=PLAYBACK]

void readyState(); //display ready state on leds

void errorState(); //display error state on leds

void analysingState(int type); //display analysing state on leds

void playbackState(); //display playback state on leds

void turnOffAll(); //turn off all LEDs

void turnOnAll(); //turn on all LEDs

#endif

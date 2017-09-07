/*******        user\apps\audio\Audio_Auralisation\Audio_Auralisation.c
* Usage:
*  		Tested for use on the DsPIC starer kit 1
* Summary:
*         Automatic auralisation of a input signal sampled at 8kHz
*
* Inputs:
*       Audio codec input
* Outputs:
*       LED output
*		Audio codec output
*
* Dependencies:
*      Sask_Init()
*
* Notes:
*		This auralisation can identify a pitch set up to 4kHz.
*
* ToDo:
*     none
*
* Originator:
*		Sören Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* History:
*		Version 1.0     19/04/2016
*		Version 1.1		20/04/2016	--	added audio output
*		Version 1.2		25/04/2016	--	removed FRAME_SIZE, takenout of the Signal_processing.h now
*		Version 1.3		26/04/2016	--	working base(prog states input output), no confirmed accuracy of result
*		Version 1.4		29/04/2016	--	added handler stop for output
*
*****/

#include <p33FJ256GP506.h>
#include <dsp\h\dsp.h>
#include <board\h\sask.h>
#include <peripherals\adc\h\ADCChannelDrv.h>
#include <peripherals\pwm\h\OCPWMDrv.h>

#include <board\inc\ex_sask_generic.h>
#include <board\inc\ex_sask_led.h>
#include <peripherals\timers\inc\ex_timer.h>

#include "..\inc\ex_audio_process.h"
#include "..\inc\Peripheral_control.h"
#include "..\inc\Signal_processing.h"

int		adcBuffer		[ADC_CHANNEL_DMA_BUFSIZE] 	__attribute__((space(dma)));	//buffer for the audio input, stored in the direct access memory
int		ocPWMBuffer		[OCPWM_DMA_BUFSIZE]		__attribute__((space(dma)));

fractional 		AudioIn[FFT_FRAME_SIZE], AudioOut[FFT_FRAME_SIZE][FFT_FRAME_SIZE];	//variable storing the audio input signal

int 		state;		//program state

fractcomplex FFTcompResults[FFT_FRAME_SIZE]		__attribute__ ((space(ymemory),far)); //Fractional complex variable containing the results generated from the fft function (fractcomplex datatype is defined by the FFT function requirements)
fractcomplex AuralisationWorkSpace[FFT_FRAME_SIZE][FFT_FRAME_SIZE];	
ADCChannelHandle adcChannelHandle;	//handler for the audio input
OCPWMHandle 	ocPWMHandle;

ADCChannelHandle *pADCChannelHandle 	= &adcChannelHandle;
OCPWMHandle 	*pOCPWMHandle 		= &ocPWMHandle;

int main(void)		//main program
{
	ex_sask_init( );	//init sask

	ADCChannelInit	(pADCChannelHandle,adcBuffer);	//init audio input handler
	OCPWMInit		(pOCPWMHandle,ocPWMBuffer);

	ADCChannelStart	(pADCChannelHandle);	//start audio input handler
	OCPWMStart		(pOCPWMHandle);	

	while(1)
	{
		if(state==0)	//Program is in READY state
		{
			state=0; //set state to 0[READY]
			state=displayState(STATE_READY); //call ready state function and read new state back
			turnOffAll();	//turn off all LEDs when leaving ready state
		}
		else if(state==1)		//Program is in READ state
		{
			while(ADCChannelIsBusy(pADCChannelHandle)); //read audio input
				ADCChannelRead	(pADCChannelHandle,AudioIn,FFT_FRAME_SIZE);
			
			state=3;
		}
		else if(state==3)		//Program is in ANALZYE state
		{
			int i=0;

				analysingState(1);
			FFT(&AudioIn, &FFTcompResults); //FFT function used on audio input, using FFT_FRAME_SIZE and returning the results in FFTcompResults
			
				analysingState(2);
			generateAuralisation(&AuralisationWorkSpace, &FFTcompResults);
			
				analysingState(3);
			for(i=0;i<FFT_FRAME_SIZE;i++)
			{
				inverseFFT(AudioOut[i], AuralisationWorkSpace[i]);
			}

			state=displayState(STATE_ANALYSE);	//analising finished
		}
		else if(state==4)		//Program is in PLAY BACK state
		{
			int x;
			for(x=0;x<FFT_FRAME_SIZE;x++)
			{
				while(OCPWMIsBusy(pOCPWMHandle));	
					OCPWMWrite (pOCPWMHandle,AudioOut[x],FFT_FRAME_SIZE);

				playbackState();
			}

			OCPWMStop (pOCPWMHandle); //stop audio output
			state=0;
		}
		else		//Program is in ERROR state
		{
			state=displayState(STATE_ERROR);	//show error state on LEDs and read new state
		}
	}
}

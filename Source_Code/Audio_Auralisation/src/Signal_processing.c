/*******  user\apps\Audio\Audio_Pitch_Detection\src\Signal_processing.c
*
* Summary:
*		FFT/Auralisation functions
*
* Dependences:
*		FFT_Processing.h
*		dsp.h
*		
Inputs:
*		Audio input (fractional)
*		FFT result array (Complex)
*
* Outputs:
*		FFT Results (fractcomplex)
*		inverse FFT results(fractional)
*		auralisation (fractcomplex)
*
* Functions:
*		void FFT(int framsize, fractional *audioIN, fractcomplex *compX)		
*		void inverseFFT(int framesize,fractional *frctAudioWorkSpace,fractcomplex *compX);
*		void generateAuralisation(fractsomplex *auralisation[FFT_FRAME_SIZE][FFT_FRAME_SIZE], fractcomplex *fftResults); 
*
* Notes:
*		Pitch detector can detect pitches between 10 and 4000 HZ. 
*		Lower then 10 is treated as 4000 HZ due to insuficient resulution on the FFT spectrum, higher then 4000 HZ is treated as 4000 HZ due to the highest freq-pin being taken as the detected pitch if all values are the same in this case 0
*		The error state would only occure in case of a calculation error which returns a value higher then 4000 HZ
*
* Originator: 
*		Sören Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* Version:
*		1.0		19/04/2016
*		1.1		21/04/2016	--	removed old function pitch detection
*		1.2		23/04/2016	--	changed generateAuralisation function to support 2d array reference, utilising FRAME_SIZE definition in functions, removed input for framesize from functions
*		1.3		25/04/2016	--	changed generateAuralisation input variables to solve array issue, reduced frame size to accomodate memory restrictions
*
*/
#include "..\inc\Signal_processing.h"
#include <dsp.h>

fractcomplex FFTcompTwidFactors[FFT_FRAME_SIZE]__attribute__ ((space(xmemory),far)); //Twid factors with size "FFT_FRAME_SIZE" stored in the X-memory
fractcomplex FFTcompWorkSpace[FFT_FRAME_SIZE]__attribute__ ((space(ymemory),far)); //Complex workspace for the FFT function with size "FFT_FRAME_SIZE" stored in the Y-memory

/*
*	This function runs the FFT on the given input values and returns the results into the address given with "FFTcombResult"
*/
void FFT(fractional *audioIN, fractcomplex *FFTcompResult)
{
	int i; //loop counter 
	
	//copy fractional audio signal into real part of complex fractional data type
	for(i=0;i<FFT_FRAME_SIZE;i++)
	{
		FFTcompWorkSpace[i].real = audioIN[i];
		FFTcompWorkSpace[i].imag = 0;
	}	
	
	//generate the first half of the set of twiddle factors required by the DFT
	TwidFactorInit (LOG2N,FFTcompTwidFactors,0);

	//generate the DFT of the audio signal
	FFTComplex(LOG2N,FFTcompResult,FFTcompWorkSpace,FFTcompTwidFactors,0xFF00);

}

/*
*	This function runs an inverse FFT on the given input values and returns the results
*/
void inverseFFT(fractional *frctAudioWorkSpace,fractcomplex *compX)
{
	int i;
	
	//generate the first half of the set of twiddle factors required by the DFT
	TwidFactorInit (LOG2N,FFTcompTwidFactors,1);//1 for inverse fourier transform

	//generate the inverse DFT of the audio signals frequency spectrum
	IFFTComplex(LOG2N,FFTcompWorkSpace,compX,FFTcompTwidFactors,0xFF00);

	for(i=0;i<FFT_FRAME_SIZE;i++)
	{
		frctAudioWorkSpace[i] = FFTcompWorkSpace[i].real;
	}		
}

/**
*		This function will generate an array which contains the seperated pins from teh frequency spectrum
**/
void generateAuralisation(fractcomplex auralisation[FFT_FRAME_SIZE][FFT_FRAME_SIZE], fractcomplex fftResults[FFT_FRAME_SIZE])
{
	int i;

	for(i=0; i<FFT_FRAME_SIZE; i++)
	{
		auralisation[i][i]=fftResults[i];
	}
}

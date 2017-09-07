/*******  user\apps\Audio\Audio_Auralisation\inc\Signal_processing.h
*
* Summary:
*		FFT function 
*
* Dependences:
*		dsp.h
*		
*
* Inputs:
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
*		void inverseFFT(int framesize,fractional *frctAudioWorkSpace,fractcomplex *compX)
*		void generateAuralisation(fractsomplex *auralisation[FFT_FRAME_SIZE][FFT_FRAME_SIZE], fractcomplex *fftResults); 
*
* Notes:
*		none
*
* Originator: 
*		Sören Schreiber, Student Kingston University, DSP Course, soeren.schreiber@arcor.de
*
* Version:
*		1.0		19/04/2016
*		1.1		21/04/2016	--	removed old function pitch detection, added function for auralisation
*		1.2		23/04/2016	--	changed generateAuralisation function to support 2d array reference, utilising FRAME_SIZE definition in functions, removed input for framesize from functions
*
*/
#ifndef FFT_INIT
#define FFT_INIT				//define FFT_INIT

#include <dsp.h>				

#define FFT_FRAME_SIZE 16		// define frame size used in the FFT functions
#define SAMPLING_RATE 8000		// define sampling rate for the FFT functions
#define COEFFS_IN_DATA 0xFF00	// Coefficiants for the Twid factors
#define LOG2N 4					// define log of frame size for fft functions

void FFT(fractional *audioIN, fractcomplex *compX);
void inverseFFT(fractional *frctAudioWorkSpace,fractcomplex *compX);
void generateAuralisation(fractcomplex auralisation[FFT_FRAME_SIZE][FFT_FRAME_SIZE], fractcomplex fftResults[FFT_FRAME_SIZE]);

#endif

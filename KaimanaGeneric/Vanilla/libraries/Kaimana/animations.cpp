//  kaimana.cpp
//
//  Copyright 2013 Paradise Arcade Shop, ParadiseArcadeShop.com
//  All rights reserved.  Use is subject to license terms.
//
//  Code is provided for entertainment purposes and use with the Kaimana controller.
//  Code may be copied, modified, resused with this Copyright notice.
//  No commercial use without written permission from Paradise Arcade Shop.
//
//  Paradise Arcade Shop Kaimana LED Driver Board
//  Initial Release October 15, 2013
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//
//  Kaimana animations based on original source released by ParadiseArcadeShop.com October 15, 2013
//
//  Created:  October 24, 2013    zonbipanda // gmail.com  -- Arduino 1.0.5 Support
//  Revised:  October 29, 2013    zonbipanda // gmail.com
//  Revised:  April   11, 2015    zonbipanda // gmail.com  -- Arduino 1.6.3 Support
//	Revised:  August 23, 2016		info // mightyjrmods.com

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "Arduino.h"
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"

int trackled[]= {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140};

void turnOn(int i,int iR,int iG, int iB)
{
	kaimana.setLED(i, iR,iG,iB);
	kaimana.updateALL();
	delay( MAIN_LOOP_DELAY );
}
void blink(int i,int iR,int iG, int iB)
{
  kaimana.setLED(i,iR,iG,iB);
  kaimana.updateALL();
  delay( MAIN_LOOP_DELAY );	
  kaimana.setLED(i, BLACK);
}
// blink LED to a color selected at random
void blinkMulti(int index)
{
	switch(random(1,9))    // pick a random color between 1 and 9
	{
		case 1:
		blink(index, COLOR_RANDOM_1);
		break;
		case 2:
		blink(index, COLOR_RANDOM_2);  
		break;
		case 3:
		blink(index, COLOR_RANDOM_3);    
		break;
		case 4:
		blink(index, COLOR_RANDOM_4);    
		break;
		case 5:
		blink(index, COLOR_RANDOM_5);    
		break;
		case 6:
		blink(index, COLOR_RANDOM_6);    
		break;
		case 7:
		blink(index, COLOR_RANDOM_7);    
		break;
		case 8:
		blink(index, COLOR_RANDOM_8);    
		break;
		case 9:
		blink(index, COLOR_RANDOM_9);    
		break;
		default:   // any undefined value so discard data and set led to BLACK
		kaimana.setLED(index, BLACK);    
		break;
	} 
}	
// set LED to one of 8 predefined colors selected at random
void setLEDRandomColor(int index)
{
  switch(random(1,7))    // pick a random color between 1 and 7
  {
    case 1:
      kaimana.setLED(index, COLOR_RANDOM_1);
      break;
    case 2:
      kaimana.setLED(index, COLOR_RANDOM_2);
      break;
    case 3:
      kaimana.setLED(index, COLOR_RANDOM_3);
      break;
    case 4:
      kaimana.setLED(index, COLOR_RANDOM_4);
      break;
    case 5:
      kaimana.setLED(index, COLOR_RANDOM_5);
      break;
    case 6:
      kaimana.setLED(index, COLOR_RANDOM_6);
      break;
    case 7:
      kaimana.setLED(index, COLOR_RANDOM_7);
      break;
   
    default:   // any undefined value so discard data and set led to BLACK
      kaimana.setLED(index, BLACK);    
      break;
  }  
}
// Color Fade Animation when Idle
int animation_idle(void)
{
  int  index;
  int  i;

  // set initial color to BLACK
  kaimana.setALL(BLACK);
  
  while(true)
  {
    for(index=0;index<IDLE_SIZE;++index)
    {
      // update strip with new color2
      for(i=0;i<LED_COUNT;++i)
      {
        kaimana.setLED(
          i,
          pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_2+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)]),
          pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_1+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)]),
          pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_0+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)])
        );
      }

      // update the leds with new/current colors in the array
      kaimana.updateALL();

      // test all switches and exit idle animation if active switch found
	for(i=0;i<SWITCH_COUNT;++i)
	{
		if( !digitalRead(switchPins[i]) )
		return(false);
	} 

      // place test for switches here and use calculated timer not delay
      //
      delay( IDLE_ANIMATION_DELAY );
    }
  }
}
int animation_idle2(void)
{
  int  index;
  int  i;

  // set initial color to BLACK
  kaimana.setALL(BLACK);
  
  while(true)
  {
    for(index=0;index<IDLE_SIZE;++index)
    {
      // update strip with new color2
      for(i=0;i<LED_COUNT;++i)
      {
        kaimana.setALL(        
          pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_2+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)]),
          pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_1+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)]),
          pgm_read_byte_near(&colorCycleData[((index+IDLE_OFFSET_0+((LED_COUNT-i)*IDLE_OFFSET))%IDLE_SIZE)])
        );
      }

      // test all switches and exit idle animation if active switch found
      for(i=0;i<SWITCH_COUNT;++i)
      {
        if( !digitalRead(switchPins[i]) )
          return(false);
      }

      // place test for switches here and use calculated timer not delay
      //
      delay( IDLE_ANIMATION_DELAY );
    }
  }
}
//light walks thru all 8 buttons
void walkyStartup(int iR,int iG, int iB)	
{
  	for (int i = 1; i <= LED_COUNT; i++) 
	{
		turnOn(i, iR,iG,iB);
		      // test all switches and exit idle animation if active switch found
	}
	kaimana.setALL(BLACK);
}
void walkyIdle(int iR,int iG, int iB)	
{
  	for (int i = 0; i <= LED_COUNT; i++) 
	{
		blink(i, iR,iG,iB);
		      // test all switches and exit idle animation if active switch found
	for(i=0;i<SWITCH_COUNT;++i)
	{
		if( !digitalRead(switchPins[i]) )
		return(false);
	} 
	}
	kaimana.setALL(BLACK);
}
void defaultStartup(void)
{
  kaimana.setALL( BLACK );
  delay( BOOT_COLOR_DELAY );
  kaimana.setALL( RED );
  delay( BOOT_COLOR_DELAY );
  kaimana.setALL( BLACK );
  delay( BOOT_COLOR_DELAY );
  kaimana.setALL( RED );
  delay( BOOT_COLOR_DELAY );
  kaimana.setALL( BLACK );
  delay( BOOT_COMPLETE_DELAY );  
} 
int breatheSine(int iR, int iG, int iB)
{
	int index;
	int i;	
	while(true)
	{
	int breatheSpeed= 1;
	float factor = millis()/1000.0;

		int alpha = 129.0 + 127 * sin((factor*.50)* PI );
		//STROOOBE int alpha = 127.0 + 127 * sin((factor*3)* PI );
		// set all leds in the array to the RGB color passed to this function
		if (alpha != 0 ){
			for(index=0;index<LED_COUNT;++index)
			{
				kaimana.setLEDBrightness( index, iR, iG, iB,alpha );
			}
			
			// update the leds with new/current colors in the array
			kaimana.updateALL();
			// test all switches and exit idle animation if active switch found
			for(i=0;i<SWITCH_COUNT;++i)
			{
				if( !digitalRead(switchPins[i]) )
				return(false);
			}
	
			// place test for switches here and use calculated timer not delay
			delay( IDLE_ANIMATION_DELAY );

		}			
	}
  } 
int breatheApple(int iR, int iG, int iB)
{
	int index;
	int i;
	
	while(true)
	{
	float factor = millis()/1000.0;

		//int alpha = 127.0 + 127 * sin((factor*.50)* PI );
		int alpha = (exp(sin(factor*PI)) - 0.36787944)*108.0;
		// set all leds in the array to the RGB color passed to this function
		
		if (alpha != 0 ){
			for(index=0;index<LED_COUNT;++index)
			{
				kaimana.setLEDBrightness( index, iR, iG, iB,alpha );
			}
			// update the leds with new/current colors in the array
			kaimana.updateALL();
			if (alpha != 0 ){
			// test all switches and exit idle animation if active switch found
			for(i=0;i<SWITCH_COUNT;++i)
			{
				if( !digitalRead(switchPins[i]) )
				return(false);
			}
			// place test for switches here and use calculated timer not delay
			delay( FAST_COLOR_DELAY );	
		}
		//delay(100);
		
	}
  } 
}  
// LEDS blink on randomly
void starryStartup(int iR,int iG, int iB)	
{
	static int i;
	
	
	kaimana.setALL( BLACK ); //set everything to OFF | this is for when you are calling from a button combination the buttons pressed do not remain on
	
	for (i = 0; i < LED_COUNT; ++i) //randomizing the array
    {     
      int rand = random(1,LED_COUNT);
	  int temp = trackled[i];
	  trackled[i] = trackled[rand];
	  trackled[rand] = temp;

    }
		
	for (i = 0; i < LED_COUNT; ++i) 
    {  
		turnOn(trackled[i], iR,iG,iB);	
	}
	
    	
}
// LEDS blink on/off randomly
int starryIdle(int iR,int iG, int iB)	
{
	static int i;
	
	
	kaimana.setALL( BLACK ); //set everything to OFF | this is for when you are calling from a button combination the buttons pressed do not remain on
	for (i = 0; i < LED_COUNT; ++i) //randomizing the array
    {     
      int rand = random(1,LED_COUNT);
	  int temp = trackled[i];
	  trackled[i] = trackled[rand];
	  trackled[rand] = temp;
    }	
	
	for (i = 0; i < LED_COUNT; ++i) 
    {      
		blink(trackled[i], iR,iG,iB);
 		for(i=0;i<SWITCH_COUNT;++i)
		{
			if( !digitalRead(switchPins[i]) )
			{return(false);}
		}  
	}
}
// LEDS blink on/off randomly with multiple colors
int starryIdleMulti()	
{
	static int i;
	int delay_val;
	
	kaimana.setALL( BLACK ); //set everything to OFF | this is for when you are calling from a button combination the buttons pressed do not remain on
	for (i = 0; i < LED_COUNT; ++i) //randomizing the array
    {     
      int rand = random(1,LED_COUNT);
	  int temp = trackled[i];
	  trackled[i] = trackled[rand];
	  trackled[rand] = temp;
    }	

	for (i = 0; i < LED_COUNT; ++i) 
    {  
		blinkMulti(trackled[i]);		
	}
}
//Tournament mode animations
void tourneyModeActivate(void)
{
	
	kaimana.setALL(RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K4, BLACK);
	kaimana.setLED(LED_P4, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );	
	
	kaimana.setLED(LED_K3, BLACK);
	kaimana.setLED(LED_P3, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K2, BLACK);
	kaimana.setLED(LED_P2, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K1, BLACK);
	kaimana.setLED(LED_P1, BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
}
void tourneyModeDeactivate(void)
{
	
	kaimana.setALL(BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K1, RED);
	kaimana.setLED(LED_P1, RED);
	kaimana.updateALL();
	delay( T_DELAY );
		
	kaimana.setLED(LED_K2, RED);
	kaimana.setLED(LED_P2, RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K3, RED);
	kaimana.setLED(LED_P3, RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setLED(LED_K4, RED );
	kaimana.setLED(LED_P4, RED);
	kaimana.updateALL();
	delay( T_DELAY );	

	kaimana.setALL(BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setALL(RED);
	kaimana.updateALL();
	delay( T_DELAY );
	
	kaimana.setALL(BLACK);
	kaimana.updateALL();
	delay( T_DELAY );
}

//  Master.ino
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
//  Kaimana example based on original source released by ParadiseArcadeShop.com October 15, 2013
//
//  Created:  October 24, 2013    zonbipanda // gmail.com  -- Arduino 1.0.5 Support
//  Revised:  October 29, 2013    zonbipanda // gmail.com
//  Revised:  April   11, 2015    zonbipanda // gmail.com  -- Arduino 1.6.3 Support
//	Revised:  March	   7, 2016	  info // mightyjrmods.com -- Added startup animations
//

#define __PROG_TYPES_COMPAT__
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "kaimana.h"
#include "kaimana_custom.h"
#include "animations.h"

struct Button
{
	int pin;
	int led;
};
// local function declarations
int  pollSwitches(void);
void showStartup(void);
void setLEDRandomColor(int index);
void buttonDetect(Button button);
int tourneypollSwitches(void);
boolean tournamentMode = false;
int holdTimeout = 0;
int selection = 0;


Button K1 = {PIN_K1,LED_K1};

// ParadiseArcadeShop.com Kaimana features initialzied when Kaimana class instantiated
Kaimana kaimana;


// the setup routine runs first and once each time power is applied to the Kaimana board
void setup() 
{                
  // light up all leds at boot to demonstrate everything is functional
  walkyStartup(RED);
}

// the loop routine repeats indefinitely and executes immediately following the setup() function
void loop() 
{
  unsigned long  ulTimeout;
  
    
  // initialize timeout value to now + some seconds
  ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
  
 // infinite loop of read switches, update LEDs and idle animation when necessary
  while( true)
  {
    // active -- poll switches and update leds
	
	if (tournamentMode != true)
	{
		if( pollSwitches() != 0 )	
		{
			// some switches were active so reset idle timeout to now + some seconds
			ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
		}
		else
		{
			// no switches active so test for start of idle timeout  
			if( millis() > ulTimeout )
			{				
			  animation_idle();
			}  
		}
	}
	else
	{
		if( tourneypollSwitches() != 0 )	
		{
			// some switches were active so reset idle timeout to now + some seconds
			ulTimeout = millis() + ( (unsigned long)IDLE_TIMEOUT_SECONDS * 1000 );
		}
		
	}
  }
    // delay a little to avoid flickering (yea, updates happens really, really fast!)
    delay( MAIN_LOOP_DELAY );
 
}


// ==============================================================
//
//  local functions start here
//
// ==============================================================





int pollSwitches(void)
{
  static int  iLED[LED_COUNT];
  static int  iActiveSwitchCount;
  static int  i;  
  static int  j;  
  static int  firsttime;
  static uint16_t  joystickDirection;
  static uint16_t  switchActivity;

  // reset LED status
  if (firsttime == 1)
  {
    for(i=0;i<LED_COUNT;++i)
    {
      iLED[i] = false;
      firsttime = 0;
    }
  }

  // read arduino pins and save results in the mapped LED if button is pressed (pin grounded)

  // complex special case for joystick but it's worth the effort
  joystickDirection = ATTACK_NONE;

  if(!digitalRead(PIN_RIGHT))    
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_RIGHT;
  if(!digitalRead(PIN_LEFT))
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_LEFT;
  if(!digitalRead(PIN_DOWN))
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_DOWN;
  if(!digitalRead(PIN_UP))
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_UP;

  switch(joystickDirection)
  {
    case ATTACK_RIGHT:    // right
      kaimana.setLED(LED_JOY, 127, 220, 000); 
      iLED[LED_JOY] = true;
      break;
    case ATTACK_LEFT:    // left
      kaimana.setLED(LED_JOY, 127, 000, 220); 
      iLED[LED_JOY] = true;
      break;
    case ATTACK_DOWN:    // down
      kaimana.setLED(LED_JOY, 000, 220, 220);
	  selection++;
      iLED[LED_JOY] = true;
      break;
    case ATTACK_DOWN + ATTACK_RIGHT:    // down + right
      kaimana.setLED(LED_JOY, 000, 255, 127); 
      iLED[LED_JOY] = true;
      break;
    case ATTACK_DOWN + ATTACK_LEFT:    // down + left
      kaimana.setLED(LED_JOY, 000, 127, 255); 
      iLED[LED_JOY] = true;
      break;
    case ATTACK_UP:    // up
      kaimana.setLED(LED_JOY, 255, 000, 000); 
      iLED[LED_JOY] = true;
      break;
    case ATTACK_UP + ATTACK_RIGHT:    // up + right
      kaimana.setLED(LED_JOY, 220, 127, 000); 
      iLED[LED_JOY] = true;
      break;
    case ATTACK_UP + ATTACK_LEFT:   // up + left
      kaimana.setLED(LED_JOY, 220, 000, 127); 
      iLED[LED_JOY] = true;
      break;
    default:   // zero or any undefined value on an 8 way stick like UP+DOWN which is not happening on my watch
      kaimana.setLED(LED_JOY, BLACK);    
      iLED[LED_JOY] = false;
      break;
  }  
  
  

  // clear results for switch activity
  switchActivity = ATTACK_NONE;
  
  // test switch and set LED based on result
  if(!digitalRead(PIN_P1))
  {
    switchActivity |= ATTACK_P1;digitalWrite(PIN_START, HIGH);
    // switch is active
    if(iLED[LED_P1] == true)
    {
		//maintain color while switch is active
		iLED[LED_P1] = true;
		
    }
    else
    {
      // select new color when switch is first activated
	  setLEDRandomColor(LED_P1);
      iLED[LED_P1] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_P1, BLACK);          
	  iLED[LED_P1] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_P2))
  {
    switchActivity |= ATTACK_P2;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P2] == true)
    {
      //maintain color while switch is active
      iLED[LED_P2] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_P2);
	  iLED[LED_P2] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_P2, BLACK);    
      iLED[LED_P2] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_P3))
  {
    switchActivity |= ATTACK_P3;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P3] == true)
    {
      //maintain color while switch is active
      iLED[LED_P3] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_P3);
      iLED[LED_P3] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_P3, BLACK);    
      iLED[LED_P3] = false;
  }
  

  // test switch and set LED based on result
  if(!digitalRead(PIN_P4))
  {
    switchActivity |= ATTACK_P4;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P4] == true)
    {
      //maintain color while switch is active
      iLED[LED_P4] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_P4);	  
      iLED[LED_P4] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_P4, BLACK);    
      iLED[LED_P4] = false;
  }


  // test switch and set LED based on result
   if(!digitalRead(PIN_K1))
  {
    switchActivity |= ATTACK_K1;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_K1] == true)
    {
      //maintain color while switch is active
      iLED[LED_K1] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_K1);
	  digitalWrite(PIN_START, LOW);
      iLED[LED_K1] = true;
    }
  }
  else
  {
      // switch is inactive
	  digitalWrite(PIN_START, HIGH);
      kaimana.setLED(LED_K1, BLACK);    
      iLED[LED_K1] = false;
  } 


  // test switch and set LED based on result
  if(!digitalRead(PIN_K2))
  {
    switchActivity |= ATTACK_K2;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_K2] == true)
    {
      //maintain color while switch is active
      iLED[LED_K2] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_K2);
      iLED[LED_K2] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_K2, BLACK);    
      iLED[LED_K2] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_K3))
  {
    switchActivity |= ATTACK_K3;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_K3] == true)
    {
      //maintain color while switch is active
      iLED[LED_K3] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_K3);
      iLED[LED_K3] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_K3, BLACK);    
      iLED[LED_K3] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_K4))
  {
    switchActivity |= ATTACK_K4;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_K4] == true)
    {
      //maintain color while switch is active
      iLED[LED_K4] = true;
    }
    else
    {
      // select new color when switch is first activated
      setLEDRandomColor(LED_K4);	  
      iLED[LED_K4] = true;
    }
  }
  else
  {
      // switch is inactive
      kaimana.setLED(LED_K4, BLACK);    
      iLED[LED_K4] = false;
	  holdTimeout = 0;
  }



  // convert joystick, P1-P4, K1-K4 into a single unsigned int
  switchActivity = joystickDirection + switchActivity;
  kaimana.switchHistorySet(switchActivity);
  

  // test for combinations from most complext to least complex
  // test for switches in reverse order (newest to oldest)


  // combo #6
  // test for: Ultra 2 (Metsu Shoryuken)
  // combo is: DOWN, DOWN+RIGHT, RIGHT, DOWN, DOWN+RIGHT, RIGHT, RIGHT+K3
  if( kaimana.switchHistoryTest( COMBO_PATTERN_6A ) )
      animation_combo_6();

  // combo #5
  // test for: Ultra 1 (Metsu Hadouken)
  // combo is: DOWN, DOWN+RIGHT, RIGHT, <NONE>, DOWN, DOWN+RIGHT, RIGHT, RIGHT+P3
  if( kaimana.switchHistoryTest( COMBO_PATTERN_5A ) )
      animation_combo_5();

  // combo #4
  // test for: Super (Shinkuu Hadouken)
  // combo is: DOWN, DOWN+RIGHT, RIGHT, <NONE>, DOWN, DOWN+RIGHT, RIGHT, RIGHT+P1
  if( kaimana.switchHistoryTest( COMBO_PATTERN_4A ) )
      animation_combo_4();

  // combo #3
  // test for: Tatsumaki Senpukyaku (Hurricane Kick)
  // combo is: DOWN, DOWN+LEFT, LEFT, LEFT+K1 or LEFT+K2
  if( kaimana.switchHistoryTest( COMBO_PATTERN_3A ) )
      animation_combo_3();
  if( kaimana.switchHistoryTest( COMBO_PATTERN_3B ) )
      animation_combo_3();

  // combo #2
  // test for: Ryu Shoryuken (Dragon Punch)
  // combo is: RIGHT, <NONE>, DOWN, DOWN+RIGHT, DOWN+RIGHT+P1 or DOWN+RIGHT+P2
  if( kaimana.switchHistoryTest( COMBO_PATTERN_2A ) )
      animation_combo_2();
  if( kaimana.switchHistoryTest( COMBO_PATTERN_2B ) )
      animation_combo_2();

  // combo #1
  // test for: Ryu Hadouken (Fireball) 
  // combo is: DOWN, DOWN+RIGHT, RIGHT, RIGHT+P1 or RIGHT+P2  
  if( kaimana.switchHistoryTest( COMBO_PATTERN_1A ) )
      animation_combo_1();
  if( kaimana.switchHistoryTest( COMBO_PATTERN_1B ) )
      animation_combo_1();
	

  // zero active switch counter (note: 4 way joystick counts as 1)
  iActiveSwitchCount = 0;
  
  // set LED color based on switch
  for(i=0;i<LED_COUNT;++i)
  {
    if(iLED[i] == true)
      ++iActiveSwitchCount;
  }  

  // update the leds with new/current colors in the array
  kaimana.updateALL();
  
  // return number of active switches
  return(iActiveSwitchCount);
}  

int tourneypollSwitches(void)
{
  static int  iLED[LED_COUNT];
  static int  iActiveSwitchCount;
  static int  i;  
  static int  j;  
  static int  firsttime;
  static uint16_t  joystickDirection;
  static uint16_t  switchActivity;

  // reset LED status
  if (firsttime == 1)
  {
    for(i=0;i<LED_COUNT;++i)
    {
      iLED[i] = false;
      firsttime = 0;
    }
  }

  // read arduino pins and save results in the mapped LED if button is pressed (pin grounded)
  // complex special case for joystick but it's worth the effort
  joystickDirection = ATTACK_NONE;

  if(!digitalRead(PIN_RIGHT))    
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_RIGHT;
  if(!digitalRead(PIN_LEFT))
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_LEFT;
  if(!digitalRead(PIN_DOWN))
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_DOWN;
  if(!digitalRead(PIN_UP))
    digitalWrite(PIN_START, HIGH);joystickDirection |= ATTACK_UP;

  switch(joystickDirection)
  {
    case ATTACK_RIGHT:    // right
      iLED[LED_JOY] = true;
      break;
    case ATTACK_LEFT:    // left
      iLED[LED_JOY] = true;
      break;
    case ATTACK_DOWN:    // down
      iLED[LED_JOY] = true;
      break;
    case ATTACK_DOWN + ATTACK_RIGHT:    // down + right
      iLED[LED_JOY] = true;
      break;
    case ATTACK_DOWN + ATTACK_LEFT:    // down + left
      iLED[LED_JOY] = true;
      break;
    case ATTACK_UP:    // up
      iLED[LED_JOY] = true;
      break;
    case ATTACK_UP + ATTACK_RIGHT:    // up + right
      iLED[LED_JOY] = true;
      break;
    case ATTACK_UP + ATTACK_LEFT:   // up + left
      iLED[LED_JOY] = true;
      break;
    default:   // zero or any undefined value on an 8 way stick like UP+DOWN which is not happening on my watch
      iLED[LED_JOY] = false;
      break;
  }  
  
  

  // clear results for switch activity
  switchActivity = ATTACK_NONE;

  // test switch and set LED based on result
  if(!digitalRead(PIN_P1))
  {
    switchActivity |= ATTACK_P1;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P1] == true)
    {		
		
	  iLED[LED_P1] = true;
    }
    else
    {    
	iLED[LED_P1] = true;
    
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_P1] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_P2))
  {
    switchActivity |= ATTACK_P2;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P2] == true)
    {
      //maintain color while switch is active
      iLED[LED_P2] = true;
    }
    else
    {
      // select new color when switch is first activated
      iLED[LED_P2] = true;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_P2] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_P3))
  {
    switchActivity |= ATTACK_P3;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P3] == true)
    {
      //maintain color while switch is active
      iLED[LED_P3] = true;
    }
    else
    {
      // select new color when switch is first activated
      iLED[LED_P3] = true;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_P3] = false;
  }
  

  // test switch and set LED based on result
  if(!digitalRead(PIN_P4))
  {
    switchActivity |= ATTACK_P4;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_P4] == true)
    {
      //maintain color while switch is active
      iLED[LED_P4] = true;
    }
    else
    {
      // select new color when switch is first activated
	  
      iLED[LED_P4] = true;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_P4] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_K1))
  {
    switchActivity |= ATTACK_K1;digitalWrite(PIN_START, HIGH);

    // switch is active
    if(iLED[LED_K1] == true)
    {
      //maintain color while switch is active
      iLED[LED_K1] = true;
    }
    else
    {
      // select new color when switch is first activated
      iLED[LED_K1] = true;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_K1] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_K2))
  {
    switchActivity |= ATTACK_K2;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_K2] == true)
    {
      //maintain color while switch is active
      iLED[LED_K2] = true;
    }
    else
    {
      // select new color when switch is first activated
      iLED[LED_K2] = true;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_K2] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_K3))
  {
    switchActivity |= ATTACK_K3;digitalWrite(PIN_START, HIGH);
    
    // switch is active
    if(iLED[LED_K3] == true)
    {
      //maintain color while switch is active
      iLED[LED_K3] = true;
    }
    else
    {
      // select new color when switch is first activated
      iLED[LED_K3] = true;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_K3] = false;
  }


  // test switch and set LED based on result
  if(!digitalRead(PIN_K4))
  {
    switchActivity |= ATTACK_K4;digitalWrite(PIN_START, HIGH);
   
    // switch is active
    if(iLED[LED_K4] == true)
    {
      //maintain color while switch is active
      iLED[LED_K4] = true;
    }
    else
    {
      // select new color when switch is first activated
      iLED[LED_K4] = true;
	  holdTimeout = 0;
    }
  }
  else
  {
      // switch is inactive
      iLED[LED_K4] = false;
  }



  // convert joystick, P1-P4, K1-K4 into a single unsigned int
  switchActivity = joystickDirection + switchActivity;
  kaimana.switchHistorySet(switchActivity);
  

  // test for combinations from most complext to least complex
  // test for switches in reverse order (newest to oldest)


  // combo #6
  // test for: Ultra 2 (Metsu Shoryuken)
  // combo is: DOWN, DOWN+RIGHT, RIGHT, DOWN, DOWN+RIGHT, RIGHT, RIGHT+K3
  if( kaimana.switchHistoryTest( COMBO_PATTERN_6A ) )
      animation_combo_6();

  // combo #5
  // test for: Ultra 1 (Metsu Hadouken)
  // combo is: DOWN, DOWN+RIGHT, RIGHT, <NONE>, DOWN, DOWN+RIGHT, RIGHT, RIGHT+P3
  if( kaimana.switchHistoryTest( COMBO_PATTERN_5A ) )
      animation_combo_5();

  // combo #4
  // test for: Super (Shinkuu Hadouken)
  // combo is: DOWN, DOWN+RIGHT, RIGHT, <NONE>, DOWN, DOWN+RIGHT, RIGHT, RIGHT+P1
  if( kaimana.switchHistoryTest( COMBO_PATTERN_4A ) )
      animation_combo_4();

  // combo #3
  // test for: Tatsumaki Senpukyaku (Hurricane Kick)
  // combo is: DOWN, DOWN+LEFT, LEFT, LEFT+K1 or LEFT+K2
  if( kaimana.switchHistoryTest( COMBO_PATTERN_3A ) )
      animation_combo_3();
  if( kaimana.switchHistoryTest( COMBO_PATTERN_3B ) )
      animation_combo_3();

  // combo #2
  // test for: Ryu Shoryuken (Dragon Punch)
  // combo is: RIGHT, <NONE>, DOWN, DOWN+RIGHT, DOWN+RIGHT+P1 or DOWN+RIGHT+P2
  if( kaimana.switchHistoryTest( COMBO_PATTERN_2A ) )
      animation_combo_2();
  if( kaimana.switchHistoryTest( COMBO_PATTERN_2B ) )
      animation_combo_2();

  // combo #1
  // test for: Ryu Hadouken (Fireball) 
  // combo is: DOWN, DOWN+RIGHT, RIGHT, RIGHT+P1 or RIGHT+P2  
  if( kaimana.switchHistoryTest( COMBO_PATTERN_1A ) )
      animation_combo_1();
  if( kaimana.switchHistoryTest( COMBO_PATTERN_1B ) )
      animation_combo_1();
	
  // zero active switch counter (note: 4 way joystick counts as 1)
  iActiveSwitchCount = 0;
  
  // set LED color based on switch
  for(i=0;i<LED_COUNT;++i)
  {
    if(iLED[i] == true)
      ++iActiveSwitchCount;
  }  

  // update the leds with new/current colors in the array
  kaimana.updateALL();
  
  // return number of active switches
  return(iActiveSwitchCount);
}  


void buttonDetect(Button button)
{
    switchActivity |= ATTACK_K1;digitalWrite(PIN_START, HIGH);
    // switch is active
    if(iLED[button.led] == true)
    {
		//maintain color while switch is active
		iLED[button.led] = true;		
    }
    else
    {
      // select new color when switch is first activated
	  setLEDRandomColor(button.led);
      iLED[button.led] = true;
    }
 	
}

/*	if(!digitalRead(K1.pin))
	{
		buttonDetect(K1);
	}		
	  else
	{
      // switch is inactive
	  digitalWrite(PIN_START, HIGH);
      kaimana.setLED(K1.led, BLACK);          
	  iLED[K1.led] = false;
	}*/
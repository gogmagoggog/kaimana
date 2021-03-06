Tested on Arduino 1.6.11

This Kaimana codebase is fully compatible with the Kaimana J2 LEDs and expects the following Wiring Setup:

KAIMANA->LED-K1->LED_K2->LED_K3->LED_P3->LED-P2->LED_P1

Every LED except for the Joystick is a J2, meaning there are two LEDs per button. The joystick is a single RGB LED which can be ordered from PAS.

- Tournament Mode functionality
	- Hold K1 for about 2 seconds and you should see the activation/deactivation animation play
- New Startup Animations
	- See animations.cpp, to select a different one remove the double slash "//" from the front of the line and add them to the line you dont want to use.
- New Idle Animations
	- Hold P3 for a second to cycle through them, you should see the LEDs flash white.


/////////////////////////////////////////////////

Paradise Arcade Shop Kaimana LED Driver Board
Initial Release October 15, 2013

Copyright 2013 Paradise Arcade Shop, ParadiseArcadeShop.com  
All rights reserved.  Use is subject to license terms.

Code is provided for entertainment purposes and use with the Kaimana controller.
Code may be copied, modified, resused with this Copyright notice.
No commercial use without written permission from Paradise Arcade Shop.

Paradise Arcade Shop Kaimana LED Driver Board
Initial Release October 15, 2013

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. 

The Kaimana class library is based on original source released by ParadiseArcadeShop.com
with feedback from the community.

Created:  October 24, 2013    zonbipanda  -- Arduino 1.0.5 Support

Revised:  October 29, 2013    zonbipanda

Revised:  April   11, 2015    zonbipanda  -- Arduino 1.6.3 Support

Revised:  December 5,2015     mightyjrmods.com -- 6 button jwyder support

Revised:  August  23, 2016    info //mightyjrmods.com  -- Added _LED_ORDER_MIGHTYJR_

Revised:  August  28, 2016    info //mightyjrmods.com  -- Updated uint8_t value type for Arduino 1.6.11 support

Revised:  June	   6, 2017	  info //mightyjrmods.com -- Made code J2 compatible and added info section
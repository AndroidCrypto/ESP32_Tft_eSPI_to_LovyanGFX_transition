C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino: In function 'void setup()':
C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino:100:19: error: no matching function for call to 'lgfx::v1::LGFX_Sprite::setTextColor(const int&, uint16_t&, bool)'
  100 |   spr.setTextColor(TFT_WHITE, bg_color, true);
      |   ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1_init.hpp:25,
                 from c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/LovyanGFX.hpp:25,
                 from C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino:43:
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:682:10: note: candidate: 'template<class T> void lgfx::v1::LGFXBase::setTextColor(T)'
  682 |     void setTextColor(T color) {
      |          ^~~~~~~~~~~~
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:682:10: note:   candidate expects 1 argument, 3 provided
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:686:10: note: candidate: 'template<class T1, class T2> void lgfx::v1::LGFXBase::setTextColor(T1, T2)'
  686 |     void setTextColor(T1 fgcolor, T2 bgcolor) {
      |          ^~~~~~~~~~~~
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:686:10: note:   candidate expects 2 arguments, 3 provided
C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino: In function 'void createNeedle()':
C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino:164:10: error: 'using LGFX_Sprite = class lgfx::v1::LGFX_Sprite' {aka 'class lgfx::v1::LGFX_Sprite'} has no member named 'getRotatedBounds'; did you mean 'getRotation'?
  164 |   needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);
      |          ^~~~~~~~~~~~~~~~
      |          getRotation
C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino: In function 'void plotNeedle(int16_t, uint16_t)':
C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino:202:19: error: 'using LGFX_Sprite = class lgfx::v1::LGFX_Sprite' {aka 'class lgfx::v1::LGFX_Sprite'} has no member named 'getRotatedBounds'; did you mean 'getRotation'?
  202 |       if ( needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y) )
      |                   ^~~~~~~~~~~~~~~~
      |                   getRotation
C:\Users\Arduino3\Documents\Arduino\Esp32_LovyanGFX_Ili9341_Animated_dial\Esp32_LovyanGFX_Ili9341_Animated_dial.ino:217:21: error: no matching function for call to 'lgfx::v1::LGFX_Sprite::setTextColor(const int&, uint16_t&, bool)'
  217 |     spr.setTextColor(TFT_WHITE, bg_color, true);
      |     ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:682:10: note: candidate: 'template<class T> void lgfx::v1::LGFXBase::setTextColor(T)'
  682 |     void setTextColor(T color) {
      |          ^~~~~~~~~~~~
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:682:10: note:   candidate expects 1 argument, 3 provided
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:686:10: note: candidate: 'template<class T1, class T2> void lgfx::v1::LGFXBase::setTextColor(T1, T2)'
  686 |     void setTextColor(T1 fgcolor, T2 bgcolor) {
      |          ^~~~~~~~~~~~
c:\Users\Arduino3\Documents\Arduino\libraries\LovyanGFX\src/lgfx/v1/LGFXBase.hpp:686:10: note:   candidate expects 2 arguments, 3 provided
Multiple libraries were found for "SD.h"
  Used: C:\Users\Arduino3\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.2.0\libraries\SD
  Not used: C:\Users\Arduino3\AppData\Local\Arduino15\libraries\SD
  Not used: C:\Users\Arduino3\Documents\Arduino\libraries\SD
exit status 1

Compilation error: no matching function for call to 'lgfx::v1::LGFX_Sprite::setTextColor(const int&, uint16_t&, bool)'
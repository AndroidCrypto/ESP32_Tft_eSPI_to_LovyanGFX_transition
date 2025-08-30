Code: spr.setTextColor(TFT_WHITE, bg_color, true);
Error: no matching function for call to 'lgfx::v1::LGFX_Sprite::setTextColor(const int&, uint16_t&, bool)'
TFT_eSPI constructor: setTextColor(uint16_t fgcolor, uint16_t bgcolor, bool bgfill = false),  // Set character (glyph) foreground and background colour, optional background fill for smooth fonts
Resolution: there is no 'Fill Background' option available, so I removed the third parameter (bgfill):
New Code: spr.setTextColor(TFT_WHITE, bg_color);

Code: needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);
Error: 'using LGFX_Sprite = class lgfx::v1::LGFX_Sprite' {aka 'class lgfx::v1::LGFX_Sprite'} has no member named 'getRotatedBounds'; did you mean 'getRotation'?
TFT_eSPI constructor: // Get the TFT bounding box for a rotated copy of this Sprite
                      bool getRotatedBounds(int16_t angle, int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y);
Resolution: there is no 'getRotatedBounds' method available
Where is the method needed ?
Pos 1: Lines 164...169:
  // Work out the worst case area that must be grabbed from the TFT,
  // this is at a 45 degree rotation
  needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);

  // Calculate the size and allocate the buffer for the grabbed TFT area
  tft_buffer =  (uint16_t*) malloc( ((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2 );

Pos 2: Lines 204...209
      if ( needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y) )
      {
        // Grab a copy of the area before needle is drawn
        tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
        buffer_loaded = true;
      }

What does the code: the method retrieves the screen rectangle where the needle was located. Before drawing the needle at the the
                    new position the 'old' screen information is deleted by drawing the old buffer.

Not optimal solution: Read the complete display that is covered by the animated dial (x = 0, y = 0, w = 240, h = 240)
                      This takes much more memory and is time consuming, let's see if this gives still a smooth transisition
      min_x = 0;
      min_y = 0;
      max_x = 240;
      max_y = 240;       
Result: the code is compiling and 'running', but after displaying the dial is crashes with: 
        Guru Meditation Error: Core  1 panic'ed (StoreProhibited). Exception was unhandled.


Slightly better solution: get the minimum coordinates the needle is operating in by drawing a green rectangle
           
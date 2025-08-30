// This example draws an animated dial with a rotating needle.

// The dial is a jpeg image, the needle is created using a rotated
// Sprite. The example operates by reading blocks of pixels from the
// TFT, thus the TFT setup must support reading from the TFT CGRAM.

// The sketch operates by creating a copy of the screen block where
// the needle will be drawn, the needle is then drawn on the screen.
// When the needle moves, the original copy of the screen area is
// pushed to the screen to over-write the needle graphic. A copy
// of the screen where the new position will be drawn is then made
// before drawing the needle in the new position. This technique
// allows the needle to move over other screen graphics.

// The sketch calculates the size of the buffer memory required and
// reserves the memory for the TFT block copy.

// Created by Bodmer 17/3/20 as an example to the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

/*
// ### lgfx need to change the variable as #define is int and not uint16_t
#define NEEDLE_LENGTH 35          // Visible length
#define NEEDLE_WIDTH 5            // Width of needle - make it an odd number
#define NEEDLE_RADIUS 90          // Radius at tip
*/
const uint16_t NEEDLE_LENGTH  = 35; // Visible length
const uint16_t NEEDLE_WIDTH = 5;    // Width of needle - make it an odd number
const uint16_t NEEDLE_RADIUS = 90;  // Radius at tip
#define NEEDLE_COLOR1 TFT_MAROON    // Needle periphery colour
#define NEEDLE_COLOR2 TFT_RED       // Needle centre colour
#define DIAL_CENTRE_X 120
#define DIAL_CENTRE_Y 120

// Font attached to this sketch
#include "NotoSansBold36.h"
#define AA_FONT_LARGE NotoSansBold36

/* ###
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite needle = TFT_eSprite(&tft); // Sprite object for needle
TFT_eSprite spr    = TFT_eSprite(&tft); // Sprite for meter reading
*/

// --------------------------------------------------------------
// LovyanGFX library

#include <LovyanGFX.hpp>
#include "LovyanGFX_ILI9341_Settings.h"
LGFX tft;
LGFX_Sprite needle(&tft);  // Sprite object for needle
LGFX_Sprite spr(&tft);     // Sprite for meter reading

// Jpeg image array attached to this sketch
#include "dial.h"

// Include the jpeg decoder library
#include <TJpg_Decoder.h>  // https://github.com/Bodmer/TJpg_Decoder

uint16_t *tft_buffer;
bool buffer_loaded = false;
uint16_t spr_width = 0;
uint16_t bg_color = 0;

// =======================================================================================
// This function will be called during decoding of the jpeg file
// =======================================================================================
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  // Stop further decoding as image is running off bottom of screen
  if (y >= tft.height()) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

// =======================================================================================
// Setup
// =======================================================================================
void setup() {
  Serial.begin(115200);  // Debug only

  tft.setBrightness(255);

  // The byte order can be swapped (set true for TFT_eSPI)
  TJpgDec.setSwapBytes(true);

  // The jpeg decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  tft.begin();
  // ### lgfx change the rotation from (old) 2 to (new) 0
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  // Draw the dial
  TJpgDec.drawJpg(0, 0, dial, sizeof(dial));
  tft.drawCircle(DIAL_CENTRE_X, DIAL_CENTRE_Y, NEEDLE_RADIUS - NEEDLE_LENGTH, TFT_DARKGREY);

  // Load the font and create the Sprite for reporting the value
  spr.loadFont(AA_FONT_LARGE);
  spr_width = spr.textWidth("777");  // 7 is widest numeral in this font
  spr.createSprite(spr_width, spr.fontHeight());
  bg_color = tft.readPixel(120, 120);  // Get colour from dial centre
  spr.fillSprite(bg_color);
  // ### lgfx
  //spr.setTextColor(TFT_WHITE, bg_color, true);
  spr.setTextColor(TFT_WHITE, bg_color);
  spr.setTextDatum(MC_DATUM);
  spr.setTextPadding(spr_width);
  spr.drawNumber(0, spr_width / 2, spr.fontHeight() / 2);
  spr.pushSprite(DIAL_CENTRE_X - spr_width / 2, DIAL_CENTRE_Y - spr.fontHeight() / 2);

  // Plot the label text
  tft.setTextColor(TFT_WHITE, bg_color);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("(degrees)", DIAL_CENTRE_X, DIAL_CENTRE_Y + 48, 2);

  // Define where the needle pivot point is on the TFT before
  // creating the needle so boundary calculation is correct
  tft.setPivot(DIAL_CENTRE_X, DIAL_CENTRE_Y);

  // Create the needle Sprite
  createNeedle();

  // Reset needle position to 0
  plotNeedle(0, 0);

  delay(2000);
}

// =======================================================================================
// Loop
// =======================================================================================
void loop() {
  uint16_t angle = random(241);  // random speed in range 0 to 240
  //Serial.printf("loop angle: %d\n", angle);
  // Plot needle at random angle in range 0 to 240, speed 40ms per increment
  plotNeedle(angle, 30);

  // Pause at new position
  delay(1000);
}

// ### lgfx This is new
// taken from https://github.com/Bodmer/TFT_eSPI/blob/master/Extensions/Sprite.cpp

// xp & yp are the pivot points of the sprite
// note: this is not checking any viewport or other parameters
/***************************************************************************************
** Function name:           rotatedBounds
** Description:             Get bounding box of a rotated Sprite wrt pivot
***************************************************************************************/
void getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp, int16_t yp,
                      int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y) {
  // Trig values for the rotation
  float radAngle = -angle * 0.0174532925;  // Convert degrees to radians
  float sina = sin(radAngle);
  float cosa = cos(radAngle);

  w -= xp;  // w is now right edge coordinate relative to xp
  h -= yp;  // h is now bottom edge coordinate relative to yp

  // Calculate new corner coordinates
  int16_t x0 = -xp * cosa - yp * sina;
  int16_t y0 = xp * sina - yp * cosa;

  int16_t x1 = w * cosa - yp * sina;
  int16_t y1 = -w * sina - yp * cosa;

  int16_t x2 = h * sina + w * cosa;
  int16_t y2 = h * cosa - w * sina;

  int16_t x3 = h * sina - xp * cosa;
  int16_t y3 = h * cosa + xp * sina;

  // Find bounding box extremes, enlarge box to accomodate rounding errors
  *min_x = x0 - 2;
  if (x1 < *min_x) *min_x = x1 - 2;
  if (x2 < *min_x) *min_x = x2 - 2;
  if (x3 < *min_x) *min_x = x3 - 2;

  *max_x = x0 + 2;
  if (x1 > *max_x) *max_x = x1 + 2;
  if (x2 > *max_x) *max_x = x2 + 2;
  if (x3 > *max_x) *max_x = x3 + 2;

  *min_y = y0 - 2;
  if (y1 < *min_y) *min_y = y1 - 2;
  if (y2 < *min_y) *min_y = y2 - 2;
  if (y3 < *min_y) *min_y = y3 - 2;

  *max_y = y0 + 2;
  if (y1 > *max_y) *max_y = y1 + 2;
  if (y2 > *max_y) *max_y = y2 + 2;
  if (y3 > *max_y) *max_y = y3 + 2;

  //_sinra = round(sina * (1<<FP_SCALE));
  //_cosra = round(cosa * (1<<FP_SCALE));
}

// vpW is viewport
/***************************************************************************************
** Function name:           getRotatedBounds
** Description:             Get TFT bounding box of a rotated Sprite wrt pivot
***************************************************************************************/
bool getRotatedBounds(int16_t angle, int16_t *min_x, int16_t *min_y,
                      int16_t *max_x, int16_t *max_y) {
  // Get the bounding box of this rotated source Sprite relative to Sprite pivot
  getRotatedBounds(angle, NEEDLE_WIDTH, NEEDLE_LENGTH, NEEDLE_WIDTH / 2, NEEDLE_RADIUS, min_x, min_y, max_x, max_y);

  // Move bounding box so source Sprite pivot coincides with TFT pivot
  *min_x += (int16_t) tft.getPivotX();
  *max_x += (int16_t) tft.getPivotX();
  *min_y += tft.getPivotY();
  *max_y += tft.getPivotY();

  // LovyanGFX has no Viewport, so this is the full screen
  uint16_t vpW = tft.width();
  uint16_t vpH = tft.height();
  uint16_t vpX = 0;
  uint16_t vpY = 0;

  // Return if bounding box is outside of TFT viewport
  if (*min_x > vpW) return false;
  if (*min_y > vpH) return false;
  if (*max_x < vpX) return false;
  if (*max_y < vpY) return false;

  // Clip bounding box to be within TFT viewport
  if (*min_x < vpX) *min_x = vpX;
  if (*min_y < vpY) *min_y = vpY;
  if (*max_x > vpW) *max_x = vpW;
  if (*max_y > vpH) *max_y = vpH;

  return true;
}

// =======================================================================================
// Create the needle Sprite
// =======================================================================================
void createNeedle(void) {
  needle.setColorDepth(16);
  needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH);  // create the needle Sprite

  needle.fillSprite(TFT_BLACK);  // Fill with black

  // Define needle pivot point relative to top left corner of Sprite
  uint16_t piv_x = NEEDLE_WIDTH / 2;  // pivot x in Sprite (middle)
  uint16_t piv_y = NEEDLE_RADIUS;     // pivot y in Sprite
  needle.setPivot(piv_x, piv_y);      // Set pivot point in this Sprite

  // Draw the red needle in the Sprite
  needle.fillRect(0, 0, NEEDLE_WIDTH, NEEDLE_LENGTH, TFT_MAROON);
  needle.fillRect(1, 1, NEEDLE_WIDTH - 2, NEEDLE_LENGTH - 2, TFT_RED);

  // Bounding box parameters to be populated
  int16_t min_x;
  int16_t min_y;
  int16_t max_x;
  int16_t max_y;

  // Work out the worst case area that must be grabbed from the TFT,
  // this is at a 45 degree rotation
  // ### lgfx
  //needle.getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);
  // not optimal solution as is will take much more memory
  /*
  min_x = 0;
  min_y = 0;
  max_x = 239;
  max_y = 239;
  */
  // better: use the new getRotatedBounds method
  // important: this area does not contain the needle !
  //getRotatedBounds(45, NEEDLE_WIDTH, NEEDLE_LENGTH, piv_x, piv_y, &min_x, &min_y, &max_x, &max_y);
  // better use this:
  getRotatedBounds(45, &min_x, &min_y, &max_x, &max_y);

  // Calculate the size and allocate the buffer for the grabbed TFT area
  //tft_buffer = (uint16_t*)malloc(((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2);
  // hard wired
  //tft_buffer = (uint16_t*)malloc(2380); // data is taken from the original sketch
  tft_buffer = (uint16_t *)malloc(((max_x - min_x) + 2) * ((max_y - min_y) + 2) * 2);
}

// =======================================================================================
// Move the needle to a new position
// =======================================================================================
void plotNeedle(int16_t angle, uint16_t ms_delay) {
  static int16_t old_angle = -120;  // Starts at -120 degrees

  // ### get the pivot of the needle
  //uint16_t piv_x = NEEDLE_WIDTH / 2;  // pivot x in Sprite (middle) [is 2]
  //uint16_t piv_y = NEEDLE_RADIUS;     // pivot y in Sprite [is 90]
  // the width and height are already stored
  // needle.createSprite(NEEDLE_WIDTH, NEEDLE_LENGTH);  // create the needle Sprite

  // Bounding box parameters
  static int16_t min_x;
  static int16_t min_y;
  static int16_t max_x;
  static int16_t max_y;

  if (angle < 0) angle = 0;  // Limit angle to emulate needle end stops
  if (angle > 240) angle = 240;

  angle -= 120;  // Starts at -120 degrees

  // Move the needle until new angle reached
  while (angle != old_angle || !buffer_loaded) {
    if (old_angle < angle) old_angle++;
    else old_angle--;

    // Only plot needle at even values to improve plotting performance
    if ((old_angle & 1) == 0) {
      if (buffer_loaded) {
        // Paste back the original needle free image area
        tft.pushRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
      }

      // ### lgfx
      /*
      if ( needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y) )
      {
        // Grab a copy of the area before needle is drawn
        tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
        buffer_loaded = true;
      }
      */
      /*
      // not optimal solution as is takes much more memory and time for loading and redrawing
      min_x = 0;
      min_y = 0;
      max_x = 239;
      max_y = 239;
      // Grab a copy of the area before needle is drawn
      Serial.println("Memory before tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);");
      ShowMemoryInfo();
      Serial.printf("min_x: %d min_y: %d 1 + max_x - min_x: %d 1 + max_y - min_y: %d tft_buffer size: %d \n", min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, sizeof(tft_buffer));
      delay(100);
*/
      /*
      getRotatedBounds(int16_t angle, int16_t w, int16_t h, int16_t xp, int16_t yp,
                      int16_t *min_x, int16_t *min_y, int16_t *max_x, int16_t *max_y)
      needle.getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y)                     
*/
      /* 
// This leaves the painted needle on the screen      
      getRotatedBounds(old_angle, NEEDLE_WIDTH, NEEDLE_LENGTH, piv_x, piv_y, &min_x, &min_y, &max_x, &max_y);
      tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
      buffer_loaded = true;
*/
      if (getRotatedBounds(old_angle, &min_x, &min_y, &max_x, &max_y)) {
        // Grab a copy of the area before needle is drawn
        tft.readRect(min_x, min_y, 1 + max_x - min_x, 1 + max_y - min_y, tft_buffer);
        buffer_loaded = true;
      }

      // Draw the needle in the new position, black in needle image is transparent
      needle.pushRotated(old_angle, TFT_BLACK);

      // Wait before next update
      delay(ms_delay);
    }

    // Update the number at the centre of the dial
    // ### lgfx
    //spr.setTextColor(TFT_WHITE, bg_color, true);
    spr.setTextColor(TFT_WHITE, bg_color);
    spr.drawNumber(old_angle + 120, spr_width / 2, spr.fontHeight() / 2);
    // debug
    //Serial.printf("plotNeedle angle: %d\n", angle);
    spr.pushSprite(120 - spr_width / 2, 120 - spr.fontHeight() / 2);

    // Slow needle down slightly as it approaches the new position
    if (abs(old_angle - angle) < 10) ms_delay += ms_delay / 5;
  }  // while end
}

// =======================================================================================

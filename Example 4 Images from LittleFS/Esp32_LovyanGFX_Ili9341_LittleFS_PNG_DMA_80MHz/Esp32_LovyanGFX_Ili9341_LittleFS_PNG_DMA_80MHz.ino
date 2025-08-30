// This example if for processors with LittleFS capability (e.g. RP2040,
// ESP32, ESP8266). It renders a png file that is stored in LittleFS
// using the PNGdec library (available via library manager).

// Note: The PNGDEC required lots of RAM to work (~40kbytes) so
// this sketch is will not run on smaller memory processors (e.g.
// ESP8266, STM32F103 etc.)

// It uses DMA to send image data to the TFT while the decoding takes
// place. The processor and display combination must support DMA to
// run this sketch! The decode time dominates so DMA is mainly an advantage
// for slow display interface speeds.

// The test image is in the sketch "data" folder (press Ctrl+K to see it).
// You must upload the image to LittleFS using the Arduino IDE Tools Data
// Upload menu option (you may need to install extra tools for that).

// Don't forget to use the Arduino IDE Tools menu to allocate a LittleFS
// memory partition before uploading the sketch and data!

// Note: This sketch is NOT USING DMA drawing !

/*
  Notes: the former used setting in 'Setup704_ESP32_DEFAULT_ILI9341_240x320.h'
         #define SPI_FREQUENCY  80000000
         caused display errors on the large image, so I reduced the speed to
         #define SPI_FREQUENCY  55000000
  Data is for 'tft.pushImageDMA' that gives display errors with cfg.freq_write = 55000000;
  Even with 40 MHz there are stripes on the display for the two larger images
  Time to open /EagleEye.png                     image specs: (120 x 120), 8 bpp, pixel type: 6  36ms
  Time to open /panda.png                        image specs: (240 x 320), 8 bpp, pixel type: 6 135ms
  Time to open /GrandPrismaticSpring_240_320.png image specs: (240 x 320), 8 bpp, pixel type: 2 159ms

  Data is for 'tft.pushImage' that runs perfectly with cfg.freq_write = 55000000;
  Time to open /EagleEye.png                     image specs: (120 x 120), 8 bpp, pixel type: 6  36ms
  Time to open /panda.png                        image specs: (240 x 320), 8 bpp, pixel type: 6 135ms
  Time to open /GrandPrismaticSpring_240_320.png image specs: (240 x 320), 8 bpp, pixel type: 2 159ms

*/

#include <LittleFS.h>
#define FileSys LittleFS

// Include the PNG decoder library
#include <PNGdec.h> // https://github.com/bitbank2/PNGdec Important: version 1.0.2 !

PNG png;
#define MAX_IMAGE_WIDTH 240 // Adjust for your images

int16_t xpos = 0;
int16_t ypos = 0;

/* ###
// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include <SPI.h>
#include <TFT_eSPI.h>      // Hardware-specific library
TFT_eSPI tft = TFT_eSPI(); // Invoke custom library
*/

// --------------------------------------------------------------
// LovyanGFX library

#include <LovyanGFX.hpp>
#include "LovyanGFX_ILI9341_Settings.h"
LGFX tft;

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(115200);
  Serial.println("\n\n Using the PNGdec library");

  // Initialise FS
  if (!FileSys.begin()) {
    Serial.println("LittleFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }

  // Initialise the TFT
  tft.begin();
  // ### lgfx change the rotation from (old) 2 to (new) 0
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);

  //tft.initDMA();

  Serial.println("\r\nInitialisation done.");
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  // Scan LittleFS and load any *.png files
  File root = LittleFS.open("/", "r");
  while (File file = root.openNextFile()) {
    String strname = file.name();
    strname = "/" + strname;
    Serial.println(file.name());
    // If it is not a directory and filename ends in .png then load it
    if (!file.isDirectory() && strname.endsWith(".png")) {
      // Pass support callback function names to library
      int16_t rc = png.open(strname.c_str(), pngOpen, pngClose, pngRead, pngSeek, pngDraw);
      if (rc == PNG_SUCCESS) {
        tft.startWrite();
        Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        uint32_t dt = millis();
        if (png.getWidth() > MAX_IMAGE_WIDTH) {
          Serial.println("Image too wide for allocated lin buffer!");
        }
        else {
          rc = png.decode(NULL, 0);
          png.close();
        }
        tft.endWrite();
        // How long did rendering take...
        Serial.print(millis()-dt); Serial.println("ms");
      }
    }
    delay(3000);
    tft.fillScreen(random(0x10000));
  }
}

//=========================================v==========================================
//                                      pngDraw
//====================================================================================
// This next function will be called during decoding of the png file to
// render each image line to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
// Callback function to draw pixels to the display
void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  // ### lgfx
  //static uint16_t dmaBuffer[MAX_IMAGE_WIDTH]; // static so buffer persists after fn exit

  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
/* ### lgfx
                                                         tft.pushImageDMA(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer, dmaBuffer);
  Compilation error: no matching function for call to 'LGFX::pushImageDMA(int16_t&, int, int&, int, uint16_t [240], uint16_t [240])'
  Available constructors:
                                                        void pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, const T* data)
                                                        note:   candidate expects 5 arguments, 6 provided
                                                        void pushImageDMA(int32_t x, int32_t y, int32_t w, int32_t h, const void* data, color_depth_t depth, const T* palette)
                                                        candidate expects 7 arguments, 6 provided
  Solution: skip the dmaBuffer: 
  tft.pushImageDMA(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
*/  
  // ### lgfx
  //tft.pushImageDMA(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer, dmaBuffer);
  // ### lgfx pushImageDMA give display errors
  // so I'm using the draw method without DMA support:
  //tft.pushImageDMA(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

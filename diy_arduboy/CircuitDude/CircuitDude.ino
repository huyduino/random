//Circuit Dude
//  Version 2.1, January 9th, 2016
//  By Jonathan Holmes (crait)
//
//  Website: http://www.crait.net/
//  Twitter: @crait
//
//Help Circuit Dude build an awesome invention by leading him
//through 50 unique levels, completing circuits and plugging
//in microchips. After all chips are inserted into their sockets,
//the exit will activate and you can advance to the next puzzle!
//What kind of secret invention is Circuit Dude building?
//Complete all 50 levels to find out!
//
//2.1 - Feb 26, 2017
//  Minimized boot code to acommedate for library change
//
//2.0 - Jan 9, 2017
//  Switched over to the Arduboy2 library! (Shout out to MLXXXp!)
//  Busted out the audio to the Playtunes library
//  Added the ability to quit the level with A+B
//  Expanded the help section to include all of the tiles
//  Compressed all images to save some space (Shout out to Ignacio!)
//  Sound on/off is now saved in the global EEPROM location
//  Updated some screens to include better graphics
//  Packed everything into a .arduboy package
//
//1.11 - Dec 5, 2016
//  Level 6 and Level 14 were too hard! I moved them down past level 20.
//
//1.0 - November 17, 2016
//  Initial release
//
//License:
//  Code is supplied for 2 purposes: 1) Ease of loading onto an Arduboy device for personal
//  play, and 2) Educational value in the case of studying the code and modifying for personal,
//  educational use. Even though the source code is available to the public, this software is
//  not 'open-source'. Re-releasing my code/game, re-distributing my code/game, or publicly
//  sharing a modified variation or derivative of my code/game is not allowed. The code has no
//  guarantee of support in the future. The code is also free of charge in order to play. This
//  code must never be sold. This game must never be sold. Distribution of my game/code for
//  commercial or financial gain is explicitly NOT allowed. Finally, someone creating a new
//  version of the game for a different platform with imagery, gameplay elements, or game
//  design element derived from this game must have explicit permission from me.

#include <Arduboy2.h>
Arduboy2 arduboy;
#include <ArduboyPlaytune.h>
ArduboyPlaytune tunes(arduboy.audio.enabled);
#define ARDBITMAP_SBUF arduboy.getBuffer()
#include <ArdBitmap.h>
ArdBitmap<WIDTH, HEIGHT> ardbitmap;

#define SAVELOCATION (EEPROM_STORAGE_SPACE_START + 410)

boolean menubuttonfree = true;
char menubuttoncounter = 0;
boolean gamebuttonfree = true;
boolean casting = false;
int frame = 0;
char animation = 0;
char transitionanimation = 0;
char deathcounter = 0;
char gamestate = 0;
char level = 0;
boolean levelcomplete = false;
char levelcompletecounter = 0;
char playerx = 0;
char playery = 0;
char playertox = 0;
char playertoy = 0;
boolean canswitch = true;
char temperature = 0;
char maxlevel = 0;
char levelchecker = 0;

const unsigned char bubble[] PROGMEM = {
  0x00, 0x00, 0xf0, 0xf8, 0x1c, 0xe, 0x6, 0x6, 0x00, 0x00, 0xf, 0x1f, 0x38, 0x70, 0x60, 0x60, 
};
const unsigned char bubbleoutline[] PROGMEM = {
  0x00, 0xf8, 0xc, 0x6, 0xe3, 0xf1, 0xf9, 0xf9, 0x00, 0x1f, 0x30, 0x60, 0xc7, 0x8f, 0x9f, 0x9f, 
};

const unsigned char arrowup[] PROGMEM = {
  0x00, 0x20, 0x30, 0x38, 0x3c, 0x38, 0x30, 0x20, 0x00, 
};
const unsigned char arrowupoutline[] PROGMEM = {
  0x30, 0x78, 0x7c, 0x7e, 0x7f, 0x7e, 0x7c, 0x78, 0x30, 
};
const unsigned char arrowdown[] PROGMEM = {
  0x00, 0x4, 0xc, 0x1c, 0x3c, 0x1c, 0xc, 0x4, 0x00, 
};
const unsigned char arrowdownoutline[] PROGMEM = {
  0xc, 0x1e, 0x3e, 0x7e, 0xfe, 0x7e, 0x3e, 0x1e, 0xc, 
};

const unsigned char lock[] PROGMEM = {
  0x7e, 0x79, 0x59, 0x79, 0x7e
};

const unsigned char me[] PROGMEM = {  //Compressed
  0x2f, 0x6f, 0x10, 0x30, 0x40, 0x01, 0x04, 0x80, 0x02, 0x08, 0x00, 0x08, 0x00, 0x08, 0x10, 0xa1,
  0xc0, 0xa0, 0xa8, 0x34, 0x50, 0x60, 0x38, 0x9d, 0x2e, 0x56, 0x16, 0x21, 0xc2, 0x10, 0x16, 0xc3,
  0xcd, 0x62, 0x01, 0x9b, 0xc1, 0x66, 0xb1, 0xd9, 0x1c, 0x88, 0x09, 0x03, 0x21, 0x08, 0x00, 0x42,
  0x10, 0x88, 0x00, 0x0b, 0xc0, 0x50, 0x82, 0x40, 0x10, 0x88, 0x00, 0x20, 0x00, 0x02, 0x50, 0x20,
  0x0e, 0x16, 0x1c, 0x1e, 0x30, 0x9c, 0x9c, 0x99, 0x19, 0x6d, 0xdb, 0xb6, 0x59, 0x2e, 0x97, 0x4b,
  0x48, 0x90, 0x8a, 0xc9, 0x18, 0x59, 0x16, 0x22, 0xa4, 0x2c, 0x48, 0x51, 0xa9, 0x00, 0x40, 0x43,
  0x0c, 0x05, 0x40, 0x02, 0x48, 0x00, 0x01, 0x58, 0xa0, 0x88, 0x04, 0x84, 0x20, 0x43, 0x58, 0x00,
  0x20, 0x00, 0x08, 0x00, 0x02, 0x80, 0x00, 0x20, 0x87, 0xcf, 0xd4, 0xc9, 0xbb, 0x84, 0x62, 0xc5,
  0xc8, 0x64, 0xb2, 0x48, 0x8f, 0x6e, 0x86, 0x90, 0xc8, 0x44, 0x42, 0x9a, 0x2d, 0x4b, 0x00, 0x88,
  0x00, 0x06, 0x84, 0x20, 0x10, 0x01, 0x0a, 0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0x14, 0xc0, 0x00,
  0x12, 0x40, 0x82, 0x40, 0x84, 0x20, 0x00, 0x02, 0x0b, 0x00, 0x36, 0x36, 0x34, 0xaa, 0xdc, 0x13,
  0x19, 0x6c, 0x1e, 0x3b, 0x80, 0x01, 0x0c, 0x26, 0x3b, 0x8b, 0x27, 0x43, 0x84, 0x04, 0x29, 0x9b,
  0xd3, 0x60, 0xe8, 0x9e, 0x1b, 0x82, 0x00, 0x20, 0x04, 0x81, 0x20, 0xb0, 0x11, 0x08, 0x02, 0x11,
  0xa0, 0x00, 0x08, 0x00, 0x94, 0x08, 0xa1, 0x00, 0x42, 0xc0, 0x00, 0x08, 0x20, 0x01, 0x30, 0xc2,
  0x00, 0x01, 0x81, 0xda, 0x26, 0x82, 0x8a, 0x90, 0x88, 0x42, 0x4d, 0x33, 0x0c, 0x12, 0x87, 0x12,
  0x45, 0x01, 0x14, 0x10, 0x81, 0x88, 0x13, 0x69, 0x86, 0x92, 0x36, 0x21, 0xc2, 0x58, 0x13, 0x00,
  0x00, 0x01, 0x24, 0x00, 0x2c, 0x17, 0x01, 0x30, 0x00, 0x02, 0x00, 0x02, 0x41, 0x20, 0x48, 0x04,
  0x52, 0x62, 0xa8, 0x9b, 0xcd, 0x08, 0x0d, 0x1f, 0x5a, 0x30, 0x19, 0x40, 0x09, 0x22, 0x00, 0x05,
  0x87, 0x03, 0x69, 0x60, 0x02, 0x45, 0x83, 0x00, 0x8a, 0x02, 0x06, 0x09, 0x00, 0x98, 0xc0, 0x64,
  0xae, 0xc9, 0xe4, 0x7d, 0x07, 0x22, 0x10, 0x17, 0x33, 0x75, 0x71, 0x0f,
};

const unsigned char title[] PROGMEM = { //Compressed
  0xff, 0xbf, 0xff, 0x44, 0x4f, 0x5d, 0xb5, 0x6e, 0xeb, 0x76, 0xf8, 0x09, 0xb5, 0x5a, 0xfd, 0x4e,
  0x6f, 0x39, 0xf6, 0xd6, 0xd7, 0x1c, 0x7b, 0xeb, 0xad, 0xdf, 0xee, 0xcd, 0xd7, 0xbd, 0xf9, 0x52,
  0xab, 0x55, 0xdf, 0x7b, 0x9b, 0xeb, 0x72, 0xe8, 0xbf, 0xea, 0x87, 0x57, 0xaf, 0x1e, 0x7d, 0xff,
  0xee, 0xfd, 0xbb, 0xe9, 0xd5, 0x2b, 0xef, 0xde, 0x4d, 0xaf, 0x19, 0x0e, 0x3e, 0xf4, 0xf0, 0xea,
  0x7c, 0xf7, 0xee, 0xdd, 0xf7, 0xde, 0x4d, 0x7f, 0xbd, 0x57, 0xaf, 0xb9, 0xf5, 0x36, 0x2e, 0xeb,
  0xb2, 0x2e, 0xeb, 0xb2, 0x2e, 0xeb, 0xb2, 0x2e, 0xeb, 0xb0, 0xf7, 0xde, 0x7b, 0xef, 0xbd, 0xe7,
  0x65, 0x5d, 0xd6, 0x65, 0x96, 0x1c, 0xd4, 0x53, 0x87, 0x8b, 0xe9, 0xd5, 0x2b, 0x4b, 0xbc, 0x50,
  0x2e, 0xc4, 0x5a, 0x40, 0xca, 0x10, 0xe0, 0x04, 0x4c, 0xae, 0x3e, 0xea, 0xad, 0xb7, 0xde, 0xba,
  0x58, 0x4c, 0xaf, 0x5e, 0xf9, 0xbc, 0x57, 0xaf, 0x5e, 0x7d, 0xc6, 0x64, 0xb9, 0xfc, 0x3c, 0x57,
  0xaf, 0xfc, 0x88, 0x43, 0x66, 0x4a, 0x4e, 0x5d, 0xa9, 0x39, 0x28, 0x23, 0x05, 0x33, 0x63, 0xfa,
  0xf7, 0xff, 0x4c, 0xc7, 0x7c, 0x97, 0x55, 0x31, 0xe5, 0x94, 0xc1, 0xa2, 0x63, 0xb1, 0x59, 0x23,
  0x74, 0xbf, 0x7f, 0xf7, 0xee, 0xdd, 0xfb, 0xd5, 0xdb, 0xcb, 0xd5, 0xdb, 0xcb, 0x31, 0x6e, 0x75,
  0x5b, 0xfb, 0xee, 0x13, 0xa7, 0x6e, 0xad, 0x56, 0x7b, 0x1b, 0xe3, 0xbb, 0xef, 0xde, 0xbf, 0xfb,
  0xd4, 0xbf, 0x80, 0x4f, 0xed, 0x63, 0xe1, 0x42, 0x3c, 0xa1, 0xb3, 0x43, 0x40, 0x2d, 0xe9, 0x6d,
  0x04, 0xe6, 0xe6, 0xfd, 0x58, 0x97, 0xe5, 0x72, 0x99, 0xb1, 0xf6, 0x87, 0x0e, 0x6b, 0xc4, 0xe5,
  0x72, 0x5d, 0xd6, 0x78, 0x7b, 0x58, 0x23, 0x2e, 0x97, 0xeb, 0xb2, 0xc6, 0xfb, 0xce, 0xcb, 0x72,
  0xb9, 0x5c, 0xae, 0xd4, 0x6f, 0x37, 0x8f, 0x4e, 0xbe, 0x78, 0xd4, 0x7e, 0xef, 0x73, 0x1e, 0x7b,
  0xe4, 0xb9, 0x57, 0x8f, 0xbd, 0xf5, 0xb4, 0x2e, 0xeb, 0xb0, 0xf7, 0x9e, 0x97, 0x7c, 0x45, 0x8e,
  0x1d, 0x8e, 0xb6, 0x6d, 0xdb, 0x0e, 0xd6, 0xb8, 0xcc, 0x79, 0xe3, 0xdf, 0xc9, 0x8a, 0x17, 0xf5,
  0xa2, 0x73, 0xa8, 0x17, 0xf5, 0x02, 0xc2, 0x2d, 0xb7, 0xde, 0x7a, 0x93, 0x45, 0xc7, 0xe4, 0x3d,
  0xa7, 0x75, 0xb8, 0xbd, 0xfb, 0x6a, 0x6f, 0x79, 0xef, 0xc1, 0xe1, 0x70, 0x03, 0x20, 0x1b, 0x01,
  0xd8, 0x14, 0xd8, 0x14, 0xd8, 0x80, 0xc8, 0x42, 0x02, 0x2c, 0xb3, 0xca, 0x89, 0x87, 0xab, 0x36,
  0xaa, 0xae, 0xa4, 0x73, 0xef, 0xbd, 0xf7, 0xde, 0x25, 0x3b, 0x74, 0xd6, 0xae, 0xda, 0x55, 0xbb,
  0xd2, 0xac, 0x34, 0x2b, 0xe9, 0x28, 0xd8, 0x0e, 0x30, 0xcd, 0x68, 0x03, 0x60, 0x9a, 0x95, 0x66,
  0xd5, 0xec, 0x98, 0x1d, 0xb3, 0x63, 0x66, 0x15, 0x06, 0x4a, 0x44, 0xb3, 0xe2, 0x58, 0x97, 0x35,
  0x57, 0x52, 0x37, 0x83, 0xb1, 0xca, 0x85, 0x2c, 0x25, 0x8e, 0xc5, 0x62, 0x2c, 0x83, 0x19, 0x83,
  0xac, 0x06, 0x19, 0x33, 0x77, 0x4f, 0xeb, 0xb0, 0x77, 0xcc, 0x2c, 0x63, 0x5d, 0x32, 0x73, 0x5a,
  0x01, 0xb3, 0x5c, 0xae, 0xcb, 0x4f, 0xe2, 0x9a, 0x1b, 0x5a, 0x4b, 0x89, 0x10, 0x42, 0xd4, 0x00,
  0xa5, 0xee, 0x30, 0xd6, 0x29, 0x3a, 0xea, 0xd9, 0x2b, 0xbf, 0x8f, 0x23, 0x71, 0x72, 0x20, 0xfc,
  0x06, 0xe5, 0x40, 0x38, 0x12, 0x8e, 0x1c, 0x09, 0x5f, 0xe3, 0xa4, 0xc0, 0x32, 0x06, 0x08, 0xf5,
  0x23, 0xc8, 0x89, 0x72, 0x60, 0xb1, 0x99, 0x2c, 0x0e, 0x94, 0xc1, 0x15, 0xb8, 0x11, 0x2e, 0x94,
  0x03, 0x8b, 0xc9, 0x66, 0x1a, 0x16,
};

const unsigned char final1[] PROGMEM = {  //Compressed
  0xff, 0xff, 0xff, 0x2d, 0xb3, 0x6e, 0xeb, 0x8e, 0xd6, 0xaa, 0x69, 0x9c, 0xbe, 0x79, 0x02, 0xc0,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x7f, 0xa6, 0xdc, 0xc6, 0x45, 0xb3, 0xad,
  0x5b, 0xc1, 0x19, 0xd4, 0x25, 0xc5, 0x65, 0xc1, 0x85, 0x95, 0x85, 0x95, 0xa1, 0x89, 0x15, 0x22,
  0x56, 0x54, 0xac, 0xa8, 0x58, 0x51, 0x55, 0x49, 0x44, 0xc5, 0x8a, 0x8e, 0x0e, 0xc7, 0xa8, 0x71,
  0xb8, 0xa8, 0xb2, 0x70, 0xc0, 0x3a, 0xc4, 0x6c, 0xeb, 0x74, 0xc5, 0x5e, 0xfc, 0xfb, 0x70, 0xf5,
  0x87, 0xb8, 0xad, 0xdb, 0xba, 0xad, 0x31, 0xd6, 0x18, 0x6b, 0x78, 0xf7, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb8, 0xff, 0x5d, 0x96, 0x2f, 0xde, 0x1f, 0xd9,
  0x9d, 0xa7, 0x5c, 0x9d, 0x63, 0x5d, 0xd6, 0x30, 0x46, 0x16, 0x2c, 0xb4, 0xeb, 0xd6, 0x5b, 0x6f,
  0x3d, 0x7a, 0xf5, 0xea, 0x51, 0xb8, 0x01, 0x3a, 0xda, 0x0e, 0x9f, 0x7c, 0x6f, 0x7f, 0xd2, 0x81,
  0x84, 0x50, 0x4a, 0x99, 0x86, 0x9b, 0x72, 0x60, 0x39, 0x2c, 0x15, 0x43, 0x9d, 0x92, 0xa2, 0xbf,
  0xc6, 0x5b, 0xa3, 0xa6, 0x26, 0x46, 0x33, 0x54, 0x0d, 0xba, 0xd4, 0xe9, 0xe1, 0x05, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xee, 0x7f, 0x97, 0x77, 0xef, 0xdf, 0x7d, 0xf2, 0x23, 0x5c,
  0xf9, 0x3b, 0x7e, 0xe7, 0x74, 0xea, 0xad, 0xb7, 0x9e, 0x76, 0x92, 0x94, 0xc1, 0xe2, 0x22, 0xec,
  0x4c, 0x55, 0x40, 0x00, 0x44, 0x54, 0x40, 0x03, 0x46, 0x64, 0x7c, 0xc1, 0x33, 0x57, 0x8f, 0xbd,
  0x75, 0x6a, 0x50, 0x89, 0x6a, 0x1a, 0x8d, 0x89, 0x35, 0xd4, 0x3a, 0x33, 0x2e, 0xab, 0x8c, 0x19,
  0xba, 0xfb, 0x77, 0xf0, 0x34, 0xf3, 0x80, 0xd3, 0x9e, 0xc3, 0xf5, 0xec, 0x83, 0xd3, 0x6b, 0x4e,
  0x73, 0xd7, 0xac, 0xd2, 0xc3, 0xba, 0xcc, 0xbd, 0xf7, 0x61, 0x5d, 0xd6, 0xa9, 0xb7, 0x0e, 0x97,
  0x83, 0xed, 0x2b, 0x8f, 0x3d, 0x99, 0xa4, 0x87, 0x75, 0x59, 0x97, 0x75, 0x19, 0xb7, 0x1e, 0x73,
  0x03, 0x8f, 0x8e, 0x51, 0x73, 0x18, 0x9b, 0x24, 0xd6, 0x6d, 0x2a, 0xa9, 0x5d, 0xb5, 0xab, 0x76,
  0xd5, 0xae, 0x9a, 0x1d, 0x53, 0x12, 0x1a, 0xb3, 0x63, 0x76, 0xcc, 0x8e, 0xd9, 0x31, 0x3b, 0x66,
  0xc7, 0xec, 0x98, 0x1d, 0xb3, 0x63, 0x76, 0xcc, 0xac, 0x5d, 0x4d, 0x56, 0x9a, 0x95, 0x66, 0xa5,
  0x59, 0x69, 0x72, 0xe7, 0x25, 0x33, 0x6d, 0x07, 0xeb, 0xe5, 0xca, 0xcf, 0xe4, 0x28, 0x1e, 0x00,
  0x91, 0xa1, 0x08, 0x40, 0x0c, 0x02, 0x82, 0x45, 0x20, 0x06, 0x00, 0x09, 0x20, 0x0a, 0x21, 0x64,
  0x3e, 0xe1, 0x9a, 0x35, 0x37, 0xef, 0xb8, 0x7a, 0xf5, 0x9a, 0x5b, 0x6f, 0xbd, 0xf5, 0xb4, 0x2e,
  0x6b, 0x3c, 0xb8, 0x7a, 0xf5, 0xea, 0xd5, 0x63, 0x6f, 0x3d, 0xad, 0xcb, 0xba, 0xac, 0xcb, 0xb8,
  0xf5, 0xd6, 0x5b, 0x6f, 0xbd, 0xf5, 0xb4, 0x2e, 0xeb, 0xa2, 0x5e, 0xd4, 0x8b, 0x7a, 0x88, 0xd9,
  0x31, 0x3b, 0x66, 0xc7, 0x6c, 0xeb, 0xb6, 0x6e, 0x6b, 0x49, 0xa7, 0x57, 0x27, 0xcb, 0xe5, 0xe8,
  0x48, 0xdb, 0xde, 0x9e, 0x0a, 0xeb, 0xd4, 0xf5, 0xe2, 0xe8, 0xb1, 0xb7, 0x9e, 0x82, 0x17, 0xcd,
  0xb6, 0x4e, 0x97, 0xcb, 0xc9, 0x70, 0x5b, 0x77, 0x1c, 0x7e, 0x66, 0x74, 0x7b, 0xd9, 0x7b, 0x5e,
  0xd6, 0x65, 0x5d, 0xc6, 0xad, 0xb7, 0xde, 0x7a, 0x74, 0x0e, 0x0f, 0xed, 0xbf, 0xdd, 0x79, 0x59,
  0x97, 0x75, 0x59, 0xa7, 0xde, 0x84, 0xc7, 0xb9, 0xf5, 0xd6, 0x5b, 0xfb, 0xfd, 0x5f, 0xfc, 0xd9,
  0x3f, 0xcc, 0xfd, 0xdf, 0x2f,
};

const unsigned char final2[] PROGMEM = {  //Compressed
  0xff, 0xbf, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x30, 0x00, 0x00, 0x98,
  0x00, 0x00, 0x1c, 0x18, 0x7c, 0xe3, 0xfe, 0xb1, 0x3f, 0x25, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x30, 0xf9, 0x1c, 0x61, 0x70, 0x04, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x08, 0x30, 0xf9, 0x43, 0x67,
  0x7c, 0xe7, 0xfe, 0x09, 0x0e, 0x04, 0x98, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
  0x01, 0x00, 0x00, 0x20, 0xc0, 0x01, 0x80, 0x13, 0x00, 0x47, 0x3e, 0xcb, 0xbb, 0xfb, 0xf7, 0x9c,
  0x7f, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x00, 0x30, 0x00, 0x00, 0x00, 0x80, 0x10, 0x02, 0x04, 0x80, 0x03, 0x00, 0x30, 0x00, 0x00, 0x80,
  0x01, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x60, 0x02, 0x00, 0x07, 0xfe, 0x84, 0xee, 0xaf, 0xde, 0x9f, 0xf3, 0x59, 0xe0, 0x04, 0xc0, 0x09,
  0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00,
  0x07, 0x00, 0x8e, 0x3c, 0x63, 0xdf, 0xbf, 0xdf, 0xf1, 0x47, 0x04, 0xe0, 0x00, 0x00, 0xc0, 0x04,
  0x00, 0x60, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x60, 0x02, 0xf0, 0x9c, 0xc7, 0x04,
  0x18, 0xc0, 0x04, 0x00, 0x4e, 0xc0, 0x9f, 0x91, 0xd3, 0x6f, 0xb8, 0x9f, 0x79, 0x06, 0x9c, 0x00,
  0x80, 0x09, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x10, 0x06, 0x30,
  0x18, 0x00, 0x9c, 0x00, 0x80, 0x09, 0x00, 0xc0, 0x04, 0x00, 0x00, 0x02, 0xc0, 0x00, 0x00, 0x8e,
  0xdc, 0xef, 0xcf, 0x72, 0x32, 0x4e, 0x3f, 0x34, 0x0e, 0x7b, 0x9e, 0x7a, 0xcb, 0xd5, 0xab, 0x57,
  0xaf, 0x5e, 0xfd, 0xe7, 0xc1, 0xd5, 0xab, 0x57, 0xaf, 0x1e, 0x7b, 0x1b, 0x87, 0x3d, 0x4f, 0x7e,
  0xc2, 0xe1, 0x41, 0x3c, 0x72, 0xbf, 0x3f, 0x03, 0x0e, 0x00, 0x00, 0x40, 0x00, 0x18, 0x00, 0x00,
  0x4c, 0x00, 0x00, 0x0e, 0x00, 0x9c, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x04, 0x00, 0x80, 0x03, 0xf0, 0xd9, 0x8e, 0xd9, 0x41, 0xf6, 0xde, 0xd6, 0x6d, 0xdd,
  0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6,
  0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x12, 0x6b, 0xa2, 0xd5, 0x6a, 0xb5, 0x5a,
  0x4d, 0xac, 0xa1, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6,
  0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0x6d, 0xdd, 0xd6, 0xad,
  0xdd, 0x25, 0xf3, 0x46, 0x6e, 0x7c, 0x14, 0x00, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x04, 0xe0, 0xc8, 0x33, 0xe0, 0x04, 0x00, 0x4c, 0x80, 0x00, 0x04, 0x86,
  0x97, 0x8b, 0x7f, 0x07, 0x8e, 0xe3, 0x90, 0x64, 0x64, 0x86, 0x5c, 0xc6, 0x6d, 0xec, 0xe6, 0xf4,
  0xdd, 0x3d, 0xb2, 0xc7, 0xd1, 0xaf, 0x7b, 0x1c, 0xbb, 0xdd, 0xff, 0x84, 0xd6, 0x5a, 0xc0, 0x04,
  0x18, 0x00, 0x00, 0x1c, 0x00, 0x38, 0xf2, 0x0c, 0x38, 0x01, 0x01, 0x00, 0x00, 0x06, 0x00, 0x00,
  0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x10, 0x06, 0x6b, 0xe7, 0xbb, 0x1c, 0x7b,
  0x0a, 0x1e, 0x6c, 0x96, 0xae, 0x75, 0x6a, 0x57, 0xc8, 0x65, 0x7c, 0x85, 0xe3, 0x38, 0x24, 0x19,
  0x99, 0xf3, 0xe4, 0xf7, 0xee, 0x0e, 0x72, 0xca, 0x95, 0xef, 0x71, 0xcd, 0x29, 0x34, 0xf7, 0x7f,
  0x4f, 0xdc, 0x76, 0x07, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06,
  0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x02, 0x30, 0xd6, 0x7a,
  0xe5, 0x29, 0x64, 0x26, 0x23, 0xdd, 0xe3, 0xb6, 0x57, 0x5f, 0x0e, 0x7a, 0xe8, 0xb7, 0x8d, 0xd3,
  0x0e, 0x63, 0x3b, 0x6e, 0xfe, 0xca, 0xb5, 0x1d, 0x8e, 0x68, 0x8c, 0x69, 0xd4, 0x34, 0x6a, 0x6a,
  0x62, 0x8c, 0xc3, 0x91, 0xb9, 0xfe, 0xf5, 0x79, 0x29, 0xc3, 0x02, 0x00, 0x10, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00,
};

const unsigned char final3[] PROGMEM = {  //Compressed
  0x3f, 0x9f, 0x13, 0x38, 0xc1, 0x98, 0xc3, 0x41, 0x6e, 0xfc, 0xbb, 0xe2, 0xea, 0xd5, 0x2b, 0x1f,
  0x86, 0xc1, 0x60, 0x5c, 0xfc, 0x57, 0xb4, 0x66, 0x88, 0x51, 0x43, 0x2c, 0x69, 0x54, 0x97, 0x6a,
  0x6c, 0xec, 0x68, 0x4f, 0x9a, 0xd5, 0xb5, 0x16, 0xff, 0x2a, 0x72, 0xeb, 0x6d, 0x5c, 0x42, 0x66,
  0x5a, 0xc7, 0x1e, 0x47, 0x3e, 0x93, 0x53, 0x68, 0xd6, 0x18, 0x9b, 0xe3, 0xea, 0xec, 0xf8, 0xdb,
  0xb2, 0xd8, 0x26, 0x3b, 0xe4, 0x94, 0x2b, 0xba, 0xf7, 0xa1, 0x3f, 0xc5, 0xe4, 0xb0, 0x2e, 0xc1,
  0x8b, 0x7a, 0xea, 0xd1, 0xe7,
};

const unsigned char final4[] PROGMEM = {  //Compressed
  0x7f, 0xbf, 0x5f, 0xe2, 0xef, 0xc8, 0x9f, 0xc0, 0xe3, 0x38, 0x80, 0x00, 0x47, 0xe4, 0x00, 0x39,
  0xec, 0x79, 0xea, 0xad, 0xb7, 0xde, 0x3a, 0x1c, 0x9d, 0xb4, 0x65, 0x8e, 0x0d, 0x8b, 0xc9, 0xb6,
  0x1e, 0x08, 0x57, 0x7e, 0x0a, 0x7f, 0x1b, 0xfe, 0x11, 0xe7, 0x34, 0xe7, 0x29, 0x6f, 0x89, 0x23,
  0x7e, 0x82, 0x63, 0x8f, 0x4e, 0x26, 0x8b, 0xc9, 0x70, 0x51, 0xa7, 0x73, 0x1d, 0x99, 0x59, 0xe5,
  0xca, 0xf4, 0x92, 0xd3, 0x6c, 0x50, 0x46, 0xd1, 0x87, 0x5c, 0x85, 0x93, 0x7f, 0x05, 0x7e, 0x2f,
  0x3f, 0x93, 0xdf, 0xc5, 0xaf, 0xe3, 0xbb, 0x99, 0xbf, 0xc2, 0xcb, 0x21, 0xbf, 0x87, 0x01, 0x8c,
  0x71, 0xe8, 0x9d, 0xab, 0x03, 0x75, 0x55, 0x7f, 0x30, 0xae, 0xd1, 0xbd, 0xe6, 0xa9, 0x17, 0x27,
  0xff, 0x28, 0xf9, 0x9b, 0xfc, 0x32, 0x72, 0xe0, 0x7b, 0x5c, 0x39, 0x33, 0xd9, 0x1c, 0xc7, 0x3e,
  0xe5, 0xcf, 0x85, 0xdf, 0xe6, 0xef, 0xc0, 0x3f, 0xcb, 0xdf, 0x9b, 0xc3, 0x61, 0x04, 0x6b, 0x3c,
  0x2c, 0x3c, 0x64, 0x0a, 0x53, 0x4c, 0xea, 0x81, 0xf2, 0x17, 0x9c, 0xf3, 0x94, 0x6b, 0xfe, 0x2d,
  0xf0, 0x6b, 0xf8, 0x33, 0xf0, 0x73, 0x0e, 0xf9, 0xab, 0xe4, 0x14, 0xcc, 0x90, 0x1e, 0xbd, 0x7a,
  0xec, 0xad, 0xa7, 0x75, 0x59, 0x97, 0x39, 0x4f, 0xbd, 0xf5, 0x96, 0x5f, 0x93, 0xc3, 0xbf, 0x4a,
  0xfe, 0x86, 0xe3, 0x70, 0x18, 0x47, 0x7e, 0x27, 0xc7, 0xb1, 0x2f, 0xe3, 0xc7, 0x8f, 0xf5, 0x8a,
  0xe3, 0x38, 0x1c, 0xc6, 0x1f, 0x98, 0x7f, 0x60, 0xfc, 0x52, 0xae, 0x39, 0xcd, 0xbd, 0xe7, 0x65,
  0x9d, 0x7a, 0xeb, 0xad, 0xb7, 0x46, 0xea, 0xe7, 0x28, 0x77, 0x36, 0x93, 0xe1, 0x0e, 0x39, 0xe5,
  0xca, 0x87, 0xf9, 0x57, 0x03,
};
const unsigned char final5[] PROGMEM = {  //Compressed
  0x7f, 0xff, 0x1f, 0xe3, 0xaf, 0xc8, 0x5f, 0x87, 0x0d, 0xd0, 0x39, 0xe8, 0x6a, 0xb2, 0x6a, 0x6c,
  0x90, 0x3c, 0x61, 0x66, 0x2d, 0x19, 0x1d, 0xac, 0x16, 0x69, 0x4f, 0x0b, 0x52, 0x26, 0xc3, 0x6d,
  0x39, 0x71, 0xe7, 0x27, 0xf0, 0x17, 0xe2, 0xdf, 0x51, 0xf7, 0xa9, 0x99, 0xc6, 0x8b, 0x72, 0x52,
  0x1b, 0x1d, 0x22, 0xd9, 0x49, 0x46, 0x0c, 0x89, 0x99, 0xe2, 0xd1, 0x31, 0xfd, 0x19, 0x90, 0xb9,
  0x93, 0xc9, 0xf4, 0x46, 0xdc, 0xcb, 0xc3, 0x25, 0x93, 0xe3, 0xe0, 0xce, 0x32, 0x1c, 0xc7, 0xa1,
  0xca, 0x18, 0x2a, 0x1f, 0x70, 0x10, 0x81, 0xe0, 0x72, 0x64, 0x4b, 0xb7, 0x10, 0x36, 0xc3, 0x03,
  0xe6, 0xc6, 0x87, 0xf8, 0xb5, 0xfc, 0x4c, 0x7e, 0x25, 0x1f, 0xc8, 0xbc, 0x20, 0x07, 0x8d, 0x4d,
  0x99, 0xc2, 0xeb, 0x95, 0x9d, 0xa5, 0xa9, 0x78, 0xe4, 0xde, 0x75, 0xe4, 0x99, 0xdf, 0xe3, 0xc2,
  0x60, 0x30, 0xc4, 0xc4, 0xac, 0xbe, 0x5b, 0x66, 0x31, 0xaa, 0xd0, 0xed, 0x87, 0x79, 0xc8, 0x23,
  0x64, 0x0e, 0xab, 0x97, 0xc3, 0xb8, 0xa9, 0x8b, 0xc1, 0xa5, 0xe8, 0x58, 0x1e, 0x7b, 0x32, 0xae,
  0xcc, 0xf5, 0x2f, 0x87, 0x6f, 0xcd, 0xac, 0xed, 0xcd, 0x87, 0x47, 0x9e, 0x8c, 0x5a, 0x2e, 0xd4,
  0xcd, 0x62, 0x83, 0x00, 0x1b, 0x30, 0x3b, 0x02, 0x03, 0x6a, 0x29, 0x60, 0x42, 0x0c, 0x02, 0x22,
  0x08, 0x82, 0x54, 0x40, 0x22, 0x36, 0x03, 0x33, 0x35, 0x9a, 0x70, 0xe1, 0x1d, 0x57, 0xbf, 0xc1,
  0x91, 0xa7, 0x39, 0x15, 0x57, 0x10, 0x89, 0xaa, 0x12, 0x55, 0x55, 0x0d, 0x44, 0xa3, 0x09, 0x06,
  0x97, 0x08, 0x5a, 0x9c, 0x6e, 0x19, 0x6f, 0x5c, 0x5d, 0xf3, 0xb2, 0x2e, 0xeb, 0xb2, 0x2e, 0x21,
  0x6b, 0xe4, 0x34, 0xd7, 0xbf, 0x02, 0x8e, 0xe2, 0x85, 0xc1, 0x26, 0x84, 0xe5, 0xc8, 0x3c, 0x04,
  0xd6, 0xfe, 0x14, 0xdf, 0xe0, 0xca, 0xa3, 0x59, 0x9c, 0x04, 0xe5, 0xcc, 0xf4, 0x09, 0x81, 0xa2,
  0xea, 0x44, 0xbe, 0xc3, 0x72, 0x79, 0xec, 0x6d, 0x5c, 0xd6, 0x65, 0x9d, 0xc0, 0xc1, 0xf4, 0x79,
  0xc7, 0xe4, 0x9a, 0x93, 0xb1, 0xc3, 0xae, 0x81, 0x43, 0x18, 0x9c, 0x38, 0x50, 0x2e, 0xc2, 0xd7,
  0x3d, 0x8e, 0xf1, 0x6a, 0x8c, 0x1f, 0xc2, 0xef, 0xe1, 0xcf, 0xc0, 0x77, 0xca, 0x8a, 0x11, 0x23,
  0x04, 0x11, 0x61, 0x50, 0x8c, 0x6d, 0xa6, 0xf5, 0x20, 0x39, 0xf9, 0x35, 0xe0, 0x38, 0x0e, 0xa5,
  0xcb, 0x89, 0x63, 0x5e, 0xd6, 0x30, 0xb2, 0xe6, 0x58, 0x5d, 0x5e, 0xc6, 0x65, 0x5d, 0xd6, 0xe1,
  0x30, 0x6e, 0x9d, 0xb4, 0x83, 0x0b, 0xa7, 0x7c, 0x25, 0x8b, 0xae, 0x48, 0x81, 0x39, 0x1d, 0x4f,
  0xb8, 0xf6, 0x72, 0xca, 0x18, 0xea, 0x65, 0x5d, 0xd6, 0x65, 0x9d, 0xf2, 0x47, 0xe7, 0x0f, 0xe1,
  0xc9, 0x70, 0x09, 0xd9, 0x44, 0xd9, 0x73, 0x9e, 0xf2, 0x80, 0x23, 0x57, 0xe0, 0x93, 0x73, 0xde,
  0x46, 0x2b, 0x70, 0x18, 0x8c, 0x98, 0x85, 0x0b, 0x91, 0x13, 0xf0, 0x79, 0x4e, 0x1b, 0x7c, 0x96,
  0x1e, 0xd2, 0xa1, 0x51, 0x93, 0x42, 0x10, 0xa0, 0xc1, 0xb2, 0x88, 0x64, 0x8f, 0x5b, 0x8f, 0xfc,
  0x76, 0xfe, 0x71, 0xf1, 0x9b, 0xb9, 0x42, 0x0e, 0x1b, 0x82, 0x07, 0xca, 0x25, 0x70, 0x03, 0x6f,
  0xc8, 0x41, 0x10, 0x03, 0x42, 0x91, 0x18, 0x0d, 0x32, 0x06, 0x14, 0x10, 0x04, 0x40, 0x6c, 0x94,
  0x10, 0x04, 0x17, 0xa2, 0x98, 0xa0, 0x3a, 0x74, 0xa8, 0x78, 0xe0, 0x01, 0x5f, 0xe1, 0x1f, 0x0c,
};

const unsigned char eyes1[] PROGMEM = { //Compressed
  0x9f, 0xcf, 0x8a, 0x3a, 0x2a, 0x42, 0x86, 0x20, 0xb8, 0xa4, 0xd4, 0xd8, 0x83, 0x14, 0x97, 0x14,
  0x97, 0x44, 0x57, 0x10, 0x63, 0x44, 0x8c, 0xd9, 0x31, 0x3b, 0x8e, 0x10, 0xb5, 0x9a, 0x46, 0x40,
  0x8b, 0xb4, 0x1e, 0xae, 0x5e, 0x8d, 0xa9, 0xa4, 0xf1, 0x0f, 0x22, 0xdd, 0xc5, 0x47,
};

const unsigned char eyes2[] PROGMEM = { //Compressed
  0x9f, 0xcf, 0x8a, 0x0a, 0x49, 0x10, 0x13, 0x45, 0xb4, 0x5a, 0x67, 0x0a, 0x53, 0x8a, 0x4b, 0x8a,
  0x4b, 0xa2, 0x2b, 0x88, 0x31, 0x22, 0xc6, 0xec, 0x98, 0x1d, 0x3b, 0x08, 0x52, 0xc4, 0x44, 0x09,
  0x2a, 0x32, 0x06, 0x87, 0x33, 0x57, 0x4b, 0x12, 0x63, 0x0d, 0x7f, 0x0c, 0xe2, 0xc5, 0xf0, 0x08,
};
const unsigned char eyes3[] PROGMEM = { //Compressed
  0x9f, 0xcf, 0x8a, 0x0a, 0x11, 0x41, 0x80, 0x20, 0x80, 0x00, 0x2a, 0x50, 0x60, 0x20, 0xc5, 0x25,
  0xc5, 0x25, 0xc5, 0x25, 0xd1, 0x15, 0xc4, 0x18, 0x11, 0x63, 0x76, 0xcc, 0x8e, 0x99, 0xc4, 0x52,
  0x0c, 0xca, 0xc0, 0xe8, 0xb8, 0x5f, 0x3d, 0x7a, 0x05, 0x6e, 0xc0, 0x9f, 0x0d,
};

const unsigned char bl[] PROGMEM = {
  0x00, 0xaa, 0xaa, 0xff, 0x1, 0x5, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x5, 0x1, 0xff, 0xa, 0xea, 0xba, 0xe2, 0x2, 0xe, 0x8, 0x7e, 0x22, 0x62, 0x22, 0x7e, 0x00, 0x7e, 0x22, 0x62, 0x22, 0x7e, 0x00, 0x7e, 0x62, 0x7e, 0x00, 0x7e, 0x62, 0x7e
};
const unsigned char br[] PROGMEM = {
  0xaa, 0xff, 0x1, 0x1, 0xff, 0x00, 0xff, 0x1, 0xd5, 0xc1, 0xd5, 0x1, 0xfd, 0x5, 0x5, 0xfd, 0x1, 0xff, 0x54, 0x00, 0x54, 0x54, 0x54, 0x00, 0x54, 0xff, 0x1, 0x85, 0x1, 
};
const unsigned char tl[] PROGMEM = {
  0x00, 0x3b, 0x3a, 0x3a, 0x12, 0x1e, 0x2, 0x7e, 0x42, 0xee, 0xea, 0xea, 0xa, 0xa, 0xea, 0xfa, 0xe3, 0x40, 0xef, 0xe8, 0xf8, 0xe8, 0xef, 0x00, 
};
const unsigned char tr[] PROGMEM = {
  0xaa, 0xff, 0xc0, 0xc0, 0xff, 0xff, 0x80, 0xba, 0xa8, 0xba, 0x80, 0x9e, 0xb3, 0x92, 0xb3, 0x92, 0xb3, 0x9e, 0x80, 
};

const unsigned char signals[4][16] PROGMEM = {
  {
    0xff, 0x81, 0xff, 0x91, 0x89, 0x89, 0x89, 0x91,
    0x91, 0x91, 0x91, 0x89, 0x91, 0xff, 0x81, 0xff
  }, {
    0xff, 0x81, 0xff, 0x91, 0xa1, 0xa1, 0x91, 0x91,
    0x89, 0x85, 0x85, 0x89, 0x91, 0xff, 0x81, 0xff
  }, {
    0xff, 0x81, 0xff, 0x89, 0x85, 0x89, 0x91, 0xa1,
    0xc1, 0xa1, 0x91, 0x91, 0x91, 0xff, 0x81, 0xff
  }, {
    0xff, 0x81, 0xff, 0x89, 0x85, 0x85, 0x89, 0x91,
    0x91, 0xa1, 0xc1, 0xc1, 0xa1, 0xff, 0x81, 0xff
  }
};

const unsigned char temptop[] PROGMEM = {
  0xff, 0x01, 0x05, 0x01, 0x01, 0x01, 0xf1, 0x89,
  0x09, 0xf1, 0x01, 0x01, 0x01, 0x05, 0x01, 0xff
};

const unsigned char tempmiddle[] PROGMEM = {
  0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x88,
  0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff
};

const unsigned char tempbottom[] PROGMEM = {
  0xff, 0x00, 0x00, 0x00, 0x00, 0x80, 0xff, 0x08,
  0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0xff,
  0xff, 0x80, 0xa0, 0x80, 0x8f, 0x9f, 0xbf, 0xb0,
  0xb0, 0xbf, 0x9f, 0x8f, 0x80, 0xa0, 0x80, 0xff
};

const unsigned char side[] PROGMEM = {
  0xff, 0x80, 0xa0, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x80, 0x80, 0x80, 0x80, 0x80, 0xa0, 0x80, 0xff,
  0xee, 0xfe, 0xee, 0xe4, 0x07, 0x80, 0x9f, 0x90,
  0xbb, 0xfa, 0xba, 0x82, 0x82, 0xba, 0xbe, 0xb8,
  0xf7, 0x9f, 0x97, 0xf7, 0x01, 0xf7, 0x97, 0x97,
  0x97, 0xf7, 0x07, 0xf7, 0x97, 0x9f, 0x97, 0xf7,
  0x01, 0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01,
  0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xfe, 0x02, 0xaa, 0x82, 0xaa, 0x02, 0xe2, 0x36,
  0x24, 0x34, 0x24, 0x36, 0xe2, 0x02, 0xf2, 0x9e,
  0x0f, 0x98, 0xfb, 0x0a, 0xfb, 0x98, 0x09, 0x0b,
  0x09, 0x0b, 0x99, 0xfb, 0x09, 0xf8, 0x98, 0x0f,
  0xff, 0x81, 0xff, 0x81, 0x81, 0x81, 0x81, 0x81,
  0x81, 0x81, 0x81, 0x81, 0x81, 0xff, 0x81, 0xff
};


const unsigned char character[2][8] PROGMEM = {
  {
    0x30, 0x9f, 0xe1, 0x4d, 0x4d, 0xe1, 0x9f, 0x30  //0 - Outline
  }, {
    0x00, 0x00, 0x1e, 0x32, 0x32, 0x1e, 0x00, 0x00  //1 - Fill
  }
};

const unsigned char deathsheet[4][24] PROGMEM = {
  {
    0x00, 0x00, 0xc0, 0x7c, 0x04, 0x30, 0x34, 0x8c,
    0x58, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x03, 0x00, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00,
  }, {
    0x00, 0x40, 0xd6, 0x02, 0x00, 0x18, 0x10, 0x8a,
    0x46, 0x14, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x02,
    0x03, 0x00, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00
  }, {
    0x04, 0xea, 0x11, 0x0a, 0x82, 0x21, 0x81, 0x12,
    0x01, 0x81, 0x8a, 0x74, 0x00, 0x00, 0x01, 0x03,
    0x03, 0x01, 0x02, 0x03, 0x03, 0x00, 0x00, 0x00
  }, {
    0xc5, 0x00, 0x14, 0x01, 0x02, 0x80, 0x00, 0x10,
    0x01, 0x00, 0x80, 0x15, 0x01, 0x00, 0x06, 0x07,
    0x02, 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x01
  }
};

#define BLOCK 0
#define TLCOR 1
#define TRCOR 2
#define BLCOR 3
#define BRCOR 4
#define HWALL 5
#define VWALL 6
#define NUBUP 7
#define NUBDO 8
#define NUBLE 9
#define NUBRI 10
#define CHIPI 11
#define LEDOF 12
#define LEDON 13
#define CPUIN 14
#define HEATI 15
#define VTROT 19
#define HTROT 20
#define CONUP 21
#define CONDO 22
#define CONLE 23
#define CONRI 24
#define CHIPO 37
#define EXITS 38
#define CPUOU 42
#define HEATO 43
#define HEATB 44
#define ROTAB 45
#define TLROT 46
#define TRROT 47
#define BRROT 48
#define BLROT 49
#define BLANK 50
#define START 51
#define HOLD 99
const unsigned char spritesheet[][8] PROGMEM = {
  {
    0x00, 0x7e, 0x7e, 0x66, 0x66, 0x7e, 0x7e, 0x00  //0 Block Wall
  }, {
    0x00, 0xfe, 0xfe, 0x06, 0x06, 0xe6, 0xe6, 0x66  //1 Top-Left Corner Wall
  }, {
    0x66, 0xe6, 0xe6, 0x06, 0x06, 0xfe, 0xfe, 0x00  //2 Top-Right Corner Wall
  }, {
    0x00, 0x7f, 0x7f, 0x60, 0x60, 0x67, 0x67, 0x66  //3 Bottom-Left Corner Wall
  }, {
    0x66, 0x67, 0x67, 0x60, 0x60, 0x7f, 0x7f, 0x00  //4 Bottom-Right Corner Wall
  }, {
    0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66  //5 Horizontal Wall
  }, {
    0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00  //6 Vertical Wall
  }, {
    0x00, 0xfe, 0xfe, 0x06, 0x06, 0xfe, 0xfe, 0x00  //7 Top Nub Wall
  }, {
    0x00, 0x7f, 0x7f, 0x60, 0x60, 0x7f, 0x7f, 0x00  //8 Bottom Nub Wall
  }, {
    0x00, 0x7e, 0x7e, 0x66, 0x66, 0x66, 0x66, 0x66  //9 Left Nub Wall
  }, {
    0x66, 0x66, 0x66, 0x66, 0x66, 0x7e, 0x7e, 0x00  //10 Right Nub Wall
  }, {
    0xda, 0x7f, 0x61, 0x61, 0x61, 0x61, 0x7f, 0xda  //11 Chip Inserted
  }, {
    0x00, 0x7e, 0xd1, 0x51, 0x41, 0xc1, 0x7e, 0x00, //12 LED Off
  }, {
    0x00, 0x7e, 0xdf, 0x5f, 0x7f, 0xff, 0x7e, 0x00, //13 LED On
  }, {
    0xe7, 0xbd, 0xff, 0x7e, 0x7e, 0xff, 0xbd, 0xe7  //14 CPU In
  }, {
    0xff, 0xab, 0xd5, 0xab, 0xd5, 0xab, 0xd5, 0xff  //15 Heatsink In
  }, {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //16 ??? Wall
  }, {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //17 ??? Wall
  }, {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  //18 ??? Wall
  }, {
    0xff, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0xff  //19 Rotating Vertical Tunnel
  }, {
    0x81, 0x81, 0x81, 0x91, 0x81, 0x81, 0x81, 0x81  //20 Rotating Horizontal Tunnel
  }, {
    0xff, 0xe7, 0xb3, 0x99, 0x99, 0xb3, 0xe7, 0xff  //21 Conveyer Belt Up
  }, {
    0xff, 0xe7, 0xcd, 0x99, 0x99, 0xcd, 0xe7, 0xff  //22 Conveyer Belt Down
  }, {
    0xff, 0xe7, 0xc3, 0x99, 0xbd, 0xe7, 0xc3, 0xff  //23 Conveyer Belt Left
  }, {
    0xff, 0xc3, 0xe7, 0xbd, 0x99, 0xc3, 0xe7, 0xff  //24 Conveyer Belt Right
  }, {
    0xff, 0xb3, 0x99, 0x8d, 0x8d, 0x99, 0xb3, 0xff  //   Conveyer Belt Up 2
  }, {
    0xff, 0xcd, 0x99, 0xb1, 0xb1, 0x99, 0xcd, 0xff  //   Conveyer Belt Down 2
  }, {
    0xff, 0xc3, 0x99, 0xbd, 0xe7, 0xc3, 0x81, 0xff  //   Conveyer Belt Left 2
  }, {
    0xff, 0x81, 0xc3, 0xe7, 0xbd, 0x99, 0xc3, 0xff  //   Conveyer Belt Right 2
  }, {
    0xff, 0x99, 0xcd, 0xe7, 0xe7, 0xcd, 0x99, 0xff  //   Conveyer Belt Up 3
  }, {
    0xff, 0x99, 0xb3, 0xe7, 0xe7, 0xb3, 0x99, 0xff  //   Conveyer Belt Down 3
  }, {
    0xff, 0x99, 0xbd, 0xe7, 0xc3, 0x99, 0xbd, 0xff  //   Conveyer Belt Left 3
  }, {
    0xff, 0xbd, 0x99, 0xc3, 0xe7, 0xbd, 0x99, 0xff  //   Conveyer Belt Right 3
  }, {
    0xff, 0xcd, 0xe7, 0xb3, 0xb3, 0xe7, 0xcd, 0xff  //   Conveyer Belt Up 4
  }, {
    0xff, 0xb3, 0xe7, 0xcd, 0xcd, 0xe7, 0xb3, 0xff  //   Conveyer Belt Down 4
  }, {
    0xff, 0xbd, 0xe7, 0xc3, 0x99, 0xbd, 0xe7, 0xff  //   Conveyer Belt Left 4
  }, {
    0xff, 0xe7, 0xbd, 0x99, 0xc3, 0xe7, 0xbd, 0xff  //   Conveyer Belt Right 4
  }, {
    0x00, 0x54, 0x54, 0x00, 0x00, 0x54, 0x54, 0x00  //37 Chip Out
  }, {
    0xc3, 0x81, 0x00, 0x00, 0x00, 0x00, 0x81, 0xc3, //38 Level Exit
  }, {
    0x00, 0x66, 0x42, 0x00, 0x00, 0x42, 0x66, 0x00, //   Level Exit 2
  }, {
    0x00, 0x00, 0x3c, 0x24, 0x24, 0x3c, 0x00, 0x00, //   Level Exit 3
  }, {
    0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, //   Level Exit 4
  }, {
    0x00, 0x42, 0x00, 0x00, 0x00, 0x00, 0x42, 0x00  //42 CPU Out
  }, {
    0x55, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xaa  //43 Heatsink Out
  }, {
    0x00, 0x2a, 0x40, 0x1a, 0x58, 0x02, 0x54, 0x00  //44 Heatsink Button
  }, {
    0x00, 0x4a, 0xe0, 0x02, 0x40, 0x07, 0x52, 0x00  //45 Rotation Button
  }, {
    0xf8, 0x04, 0x02, 0x01, 0x11, 0x01, 0x01, 0x01  //46 Top-Left Rotating Corner
  }, {
    0x01, 0x01, 0x01, 0x11, 0x01, 0x02, 0x04, 0xf8  //47 Top-Right Rotating Corner
  }, {
    0x80, 0x80, 0x80, 0x88, 0x80, 0x40, 0x20, 0x1f  //48 Bottom-Right Rotating Corner
  }, {
    0x1f, 0x20, 0x40, 0x80, 0x88, 0x80, 0x80, 0x80  //49 Bottom-Left Rotating Corner
  }
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//              LEVELS START HERE

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

const unsigned char levelmap[50][112] PROGMEM = {
  {
    // Level 1
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, START, BLANK, BLANK, CHIPO, BLANK, CHIPO, BLANK, LEDOF, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, EXITS, BLANK, BLANK, CHIPO, BLANK, CHIPO, BLANK, LEDOF, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR
  }, {
    // Level 2
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR, BLANK, TLCOR, HWALL, HWALL, HWALL, TRCOR,
    VWALL, START, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL, BLANK, VWALL, LEDOF, BLANK, LEDOF, VWALL,
    VWALL, BLANK, BLANK, CHIPO, CHIPO, BLANK, BLANK, VWALL, BLANK, VWALL, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, BLANK, CHIPO, CHIPO, BLANK, BLANK, BLCOR, HWALL, BRCOR, BLANK, EXITS, BLANK, VWALL,
    VWALL, BLANK, BLANK, CHIPO, CHIPO, BLANK, BLANK, CHIPO, CHIPO, CHIPO, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, BLANK, CHIPO, CHIPO, BLANK, BLANK, CHIPO, CHIPO, CHIPO, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CHIPO, CHIPO, CHIPO, BLANK, BLANK, BLANK, VWALL,
    BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR,
  }, {
    // Level 3
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
    VWALL, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CHIPO, BLANK, CHIPO, VWALL,
    VWALL, BLANK, TLCOR, HWALL, HWALL, HWALL, HWALL, NUBRI, BLANK, NUBLE, HWALL, NUBRI, BLANK, VWALL,
    VWALL, BLANK, NUBDO, LEDOF, LEDOF, BLANK, BLANK, BLANK, BLANK, BLANK, LEDOF, LEDOF, BLANK, VWALL,
    VWALL, BLANK, CHIPO, CHIPO, CHIPO, BLANK, START, BLANK, EXITS, BLANK, TLCOR, NUBRI, BLANK, VWALL,
    VWALL, BLANK, BLOCK, BLANK, NUBLE, HWALL, NUBRI, BLANK, NUBLE, HWALL, BRCOR, LEDOF, BLANK, VWALL,
    VWALL, CHIPO, BLANK, BLANK, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, BLANK, CHIPO, CHIPO, VWALL,
    BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR, BLANK, CHIPO, CHIPO, VWALL,
  }, {
    // Level 4
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
    VWALL, BLANK, BLANK, BLANK, CHIPO, BLANK, CHIPO, BLANK, CHIPO, BLANK, BLANK, BLANK, LEDOF, VWALL,
    VWALL, BLANK, BLANK, BLANK, TLCOR, HWALL, HWALL, HWALL, TRCOR, BLANK, BLANK, TLCOR, HWALL, BRCOR,
    VWALL, BLANK, START, BLANK, VWALL, BLANK, BLANK, BLANK, VWALL, BLANK, BLANK, VWALL, BLANK, BLANK,
    VWALL, BLANK, BLANK, BLANK, NUBDO, BLANK, BLANK, BLANK, NUBDO, BLANK, BLANK, VWALL, BLANK, BLANK,
    VWALL, BLANK, EXITS, BLANK, CONLE, CONLE, CONLE, CONLE, CONLE, BLANK, BLANK, VWALL, BLANK, BLANK,
    VWALL, BLANK, BLANK, BLANK, NUBUP, BLANK, BLANK, BLANK, NUBUP, BLANK, BLANK, VWALL, BLANK, BLANK,
    BLCOR, HWALL, HWALL, HWALL, BRCOR, BLANK, BLANK, BLANK, BLCOR, HWALL, HWALL, BRCOR, LEDOF, LEDOF,
  }, {
    // Level 5
    CHIPO, CHIPO, CHIPO, BLCOR, HWALL, BRCOR, CHIPO, VWALL, BLANK, VWALL, LEDOF, EXITS, LEDOF, VWALL,
    CHIPO, CHIPO, BLANK, CONLE, CONLE, CONLE, BLANK, VWALL, BLANK, VWALL, BLANK, BLANK, BLANK, VWALL,
    CHIPO, CHIPO, CHIPO, TLCOR, HWALL, TRCOR, CONUP, VWALL, BLANK, BLCOR, TRCOR, CONUP, TLCOR, BRCOR,
    HWALL, TRCOR, CONDO, VWALL, BLANK, VWALL, CONUP, VWALL, BLANK, BLANK, VWALL, CONUP, VWALL, BLANK,
    BLANK, VWALL, CONDO, VWALL, BLANK, VWALL, CONUP, VWALL, BLANK, TLCOR, BRCOR, CONUP, BLCOR, TRCOR,
    HWALL, BRCOR, CONDO, BLCOR, HWALL, BRCOR, BLANK, BLCOR, HWALL, BRCOR, LEDOF, BLANK, LEDOF, VWALL,
    CHIPO, CHIPO, BLANK, CONRI, CONRI, CONRI, START, CONRI, CONRI, CONRI, BLANK, BLANK, BLANK, VWALL,
    CHIPO, CHIPO, BLANK, TLCOR, HWALL, TRCOR, CHIPO, TLCOR, HWALL, TRCOR, CHIPO, CHIPO, CHIPO, VWALL,
  }, {
    // Level 6
    CHIPO, CHIPO, CHIPO, CHIPO, VWALL, BLANK, CHIPO, BLANK, VWALL, BLANK, BLANK, BLANK, BLANK, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, VWALL, BLANK, CHIPO, BLANK, VWALL, BLANK, CHIPO, CHIPO, CHIPO, BLANK,
    HWALL, NUBRI, BLANK, BLANK, NUBDO, BLANK, CHIPO, BLANK, VWALL, BLANK, CHIPO, CHIPO, CHIPO, BLANK,
    BLANK, BLANK, BLANK, BLANK, CPUOU, BLANK, CHIPO, BLANK, VWALL, BLANK, CHIPO, CHIPO, CHIPO, BLANK,
    HWALL, NUBRI, CPUOU, NUBLE, TRCOR, BLANK, CHIPO, BLANK, NUBDO, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLANK, BLANK, BLANK, VWALL, BLANK, CHIPO, BLANK, CPUOU, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, START, EXITS, BLANK, BLCOR, HWALL, HWALL, HWALL, HWALL, NUBRI, CPUOU, NUBLE, HWALL, TRCOR,
    BLANK, BLANK, BLANK, BLANK, CONLE, BLANK, CPUOU, BLANK, CONLE, BLANK, BLANK, BLANK, BLANK, VWALL,
  }, {
    // Level 7
    LEDOF, CPUOU, LEDOF, TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR, LEDOF, CPUOU, CPUOU, LEDOF,
    BLANK, BLANK, BLANK, VWALL, CONRI, BLANK, CHIPO, BLANK, BLANK, VWALL, BLANK, BLANK, BLANK, BLANK,
    NUBRI, CPUOU, NUBLE, BRCOR, BLANK, BLOCK, BLANK, BLOCK, BLANK, BLCOR, NUBRI, CPUOU, NUBLE, TRCOR,
    BLANK, BLANK, BLANK, BLANK, CHIPO, BLANK, CPUOU, BLANK, BLANK, CPUOU, BLANK, BLANK, BLANK, VWALL,
    NUBRI, CPUOU, NUBLE, TRCOR, BLANK, BLOCK, BLANK, BLOCK, BLANK, TLCOR, NUBRI, CPUOU, NUBLE, BRCOR,
    BLANK, BLANK, BLANK, VWALL, CONUP, BLANK, CHIPO, BLANK, CONLE, VWALL, BLANK, BLANK, BLANK, BLANK,
    BLANK, EXITS, BLANK, BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR, BLANK, START, BLANK, BLANK,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CPUOU, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK
  }, {
    // Level 8
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
    VWALL, CHIPO, CHIPO, CHIPO, BLANK, CONLE, BLANK, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, VWALL,
    VWALL, CHIPO, CHIPO, CHIPO, CPUOU, BLOCK, BLANK, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, BLCOR,
    NUBDO, CHIPO, NUBLE, TRCOR, BLANK, CONRI, CONUP, NUBLE, HWALL, HWALL, NUBRI, BLANK, BLANK, START,
    CONDO, CONLE, CONLE, NUBDO, BLANK, NUBUP, TLCOR, HWALL, HWALL, HWALL, NUBRI, BLANK, BLANK, EXITS,
    CONDO, CHIPO, CHIPO, CHIPO, CPUOU, NUBDO, NUBDO, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, TLCOR,
    CONDO, CHIPO, CHIPO, CHIPO, BLANK, CONLE, BLANK, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, VWALL,
    CONRI, CONRI, CONRI, CONRI, CONUP, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR,
  }, {
    // Level 9
    CHIPO, CHIPO, CHIPO, BLANK, BLANK, BLANK, CHIPO, CHIPO, CHIPO, VWALL, BLANK, BLANK, BLANK, BLANK,
    CHIPO, LEDOF, CHIPO, BLANK, BLANK, BLANK, CHIPO, LEDOF, CHIPO, VWALL, BLANK, CHIPO, CHIPO, BLANK,
    CHIPO, CHIPO, CHIPO, BLANK, BLANK, BLANK, CHIPO, CHIPO, CHIPO, VWALL, BLANK, BLANK, BLANK, BLANK,
    TRCOR, CONDO, NUBLE, NUBRI, CONUP, NUBLE, NUBRI, CONDO, NUBLE, BRCOR, BLANK, BLANK, BLANK, BLANK,
    BRCOR, BLANK, CHIPO, CHIPO, START, CHIPO, CHIPO, BLANK, BLANK, CONRI, BLANK, BLANK, EXITS, BLANK,
    BLANK, CONRI, BLANK, CHIPO, CONDO, CHIPO, BLANK, CONLE, BLANK, NUBUP, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLANK, HEATI, CHIPO, BLANK, CHIPO, HEATI, BLANK, BLANK, VWALL, BLANK, CHIPO, CHIPO, BLANK,
    LEDOF, BLANK, CONLE, CHIPO, CONUP, CHIPO, CONRI, BLANK, LEDOF, VWALL, BLANK, BLANK, BLANK, BLANK,
  }, {
    // Level 10
    LEDOF, LEDOF, VWALL, BLANK, BLANK, BLANK, BLANK, HEATO, EXITS, VWALL, CHIPO, CHIPO, CHIPO, VWALL,
    HWALL, HWALL, BRCOR, BLANK, BLANK, BLANK, BLANK, HEATO, NUBLE, BRCOR, CHIPO, CHIPO, CHIPO, VWALL,
    BLANK, BLANK, HEATB, BLANK, CONRI, CONRI, CONDO, HEATO, HEATO, HEATB, BLANK, BLANK, BLANK, VWALL,
    BLANK, BLANK, NUBLE, HWALL, NUBRI, BLANK, CONDO, BLANK, NUBLE, TRCOR, HEATI, HEATI, HEATI, VWALL,
    CHIPO, CHIPO, CHIPO, NUBUP, CHIPO, CHIPO, CONDO, CHIPO, CHIPO, VWALL, BLANK, BLANK, BLANK, VWALL,
    CHIPO, CHIPO, CHIPO, NUBDO, CHIPO, CHIPO, HEATB, CHIPO, CHIPO, VWALL, BLANK, START, BLANK, VWALL,
    BLANK, BLANK, BLANK, HEATO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, BLANK, BLANK, BLANK, VWALL,
    BLANK, BLANK, BLANK, NUBUP, CHIPO, CHIPO, HEATB, CHIPO, CHIPO, VWALL, BLANK, HEATB, BLANK, VWALL,
  }, {
    // Level 11
    LEDOF, BLANK, BLANK, BLANK, CONRI, HEATB, BLANK, HEATO, CHIPO, HEATB, HEATI, HEATI, HEATB, CHIPO,
    BLANK, BLANK, EXITS, BLANK, NUBUP, CHIPO, CHIPO, NUBUP, CHIPO, CHIPO, HEATI, HEATI, CHIPO, CHIPO,
    LEDOF, BLANK, BLANK, BLANK, NUBDO, CHIPO, CHIPO, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO,
    BLANK, BLANK, HEATB, BLANK, HEATI, HEATI, CHIPO, BLCOR, NUBRI, CONDO, NUBLE, TRCOR, HEATI, HEATI,
    BLANK, START, BLANK, BLANK, NUBUP, CHIPO, CHIPO, CHIPO, CHIPO, HEATB, CHIPO, VWALL, CHIPO, CHIPO,
    LEDOF, BLANK, HEATO, HEATO, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, NUBDO, BLANK, NUBUP,
    BLANK, BLANK, HEATO, BLANK, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, BLANK, VWALL,
    LEDOF, BLANK, HEATO, HEATO, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR,
  }, {
    // Level 12
    TLCOR, HWALL, TRCOR, BLANK, LEDOF, BLANK, NUBUP, EXITS, TLCOR, HWALL, HWALL, HWALL, HWALL, TRCOR,
    VWALL, CHIPI, BLCOR, HWALL, HWALL, HWALL, BRCOR, BLANK, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL,
    VWALL, BLANK, LEDOF, CHIPO, CONRI, CONRI, CONRI, BLANK, CONRI, CONRI, BLANK, BLANK, CHIPO, VWALL,
    VWALL, BLANK, LEDOF, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, BLOCK, CHIPO, CHIPO, CHIPO, CHIPO, VWALL,
    VWALL, BLANK, TLCOR, NUBRI, CHIPO, CONRI, CONRI, CONUP, CONLE, CONLE, BLANK, BLANK, CHIPO, VWALL,
    VWALL, BLANK, VWALL, CHIPO, CHIPO, CHIPO, START, CONUP, BLOCK, CHIPO, CHIPO, CHIPO, CHIPO, VWALL,
    VWALL, CHIPI, VWALL, CHIPO, CHIPO, CHIPO, CONRI, CONUP, CONLE, CONLE, BLANK, BLANK, CHIPO, VWALL,
    BLCOR, NUBRI, BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, NUBRI, CHIPO, CHIPO, CHIPO, CHIPO, NUBDO
  }, {
    // Level 13
    LEDOF, LEDOF, LEDOF, VWALL, BLANK, BLANK, BLANK, VWALL, BLANK, CHIPO, BLANK, CHIPO, BLANK, LEDOF,
    BLANK, START, BLANK, VWALL, BLANK, EXITS, BLANK, VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLANK, BLANK, VWALL, BLANK, BLANK, BLANK, BLCOR, NUBRI, VTROT, NUBLE, HWALL, NUBRI, CONDO,
    NUBRI, VTROT, NUBLE, BRCOR, BLANK, BLANK, BLANK, BLANK, BLANK, VTROT, BLANK, BLANK, BLANK, BLANK,
    BLANK, VTROT, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VTROT, BLANK, BLANK, BLANK, BLANK,
    NUBRI, VTROT, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, NUBRI, VTROT, NUBLE, HWALL, NUBRI, CONUP,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLANK, CHIPO, BLANK, CHIPO, BLANK, CHIPO, BLANK, CHIPO, BLANK, CHIPO, BLANK, CHIPO, BLANK, LEDOF,
  }, {
    // Level 14
    CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, EXITS,
    CONRI, CONRI, CONRI, CONRI, CONRI, CONRI, BLANK, CONRI, CONRI, CONDO, CONLE, CONLE, CONLE, CONLE,
    CONUP, NUBLE, TRCOR, CONUP, NUBLE, NUBRI, VTROT, NUBLE, NUBRI, CONDO, NUBLE, HWALL, NUBRI, CONUP,
    BLANK, ROTAB, NUBDO, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, HTROT, BLANK, BLANK, BLANK,
    BLANK, BLANK, HTROT, BLANK, BLANK, BLANK, START, BLANK, BLANK, CHIPO, NUBUP, BLANK, BLANK, BLANK,
    CONDO, NUBLE, NUBRI, CONUP, NUBLE, NUBRI, VTROT, NUBLE, NUBRI, CONDO, BLCOR, HWALL, NUBRI, CONDO,
    CONRI, CONRI, CONRI, CONUP, CONLE, CONLE, BLANK, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE,
    CHIPO, CHIPO, BLANK, CONLE, CONLE, CONLE, BLANK, CONRI, CONRI, CONRI, BLANK, CHIPO, CHIPO, CHIPO,
  }, {
    // Level 15
    LEDOF, LEDOF, VWALL, LEDOF, LEDOF, LEDOF, VWALL, LEDOF, VWALL, LEDOF, LEDOF, NUBLE, HWALL, TRCOR,
    BLANK, BLANK, VWALL, BLANK, BLANK, BLANK, NUBDO, BLANK, VWALL, BLANK, BLANK, CHIPO, CHIPO, VWALL,
    BLANK, BLANK, VWALL, BLROT, HTROT, HTROT, HTROT, BLANK, VWALL, BLANK, BLANK, CHIPO, CHIPO, VWALL,
    BLANK, EXITS, VWALL, BLANK, BRROT, BLANK, NUBUP, BLANK, VWALL, BLANK, BLANK, CHIPO, CHIPO, VWALL,
    BLANK, BLANK, VWALL, BLROT, BRROT, TLCOR, BRCOR, BLANK, NUBDO, BRROT, BLANK, BLANK, BLANK, VWALL,
    BLANK, BLANK, VWALL, BLROT, BRROT, VWALL, BLANK, TLROT, BLANK, BLANK, TLCOR, NUBRI, CHIPO, NUBDO,
    BLANK, BLANK, NUBDO, BLANK, BLANK, BLCOR, TRCOR, BLANK, BLANK, BLANK, VWALL, BLANK, BLANK, BLANK,
    BLANK, BLANK, BLANK, BLANK, BLANK, LEDOF, VWALL, LEDOF, LEDOF, LEDOF, VWALL, BLANK, START, BLANK
  }, {
    // Level 16
    NUBUP, BLANK, START, NUBUP, LEDOF, LEDOF, LEDOF, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
    VWALL, BLROT, TRROT, VWALL, BLANK, EXITS, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    VWALL, BLROT, BRROT, BLCOR, HWALL, HWALL, HWALL, HWALL, TRCOR, CHIPO, CHIPO, NUBUP, CHIPO, VWALL,
    VWALL, CHIPO, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, CHIPO, CHIPO, NUBDO, CHIPO, VWALL,
    VWALL, BLANK, BLANK, CHIPO, CONRI, CHIPO, BLANK, TRROT, VWALL, CHIPO, BLANK, CONLE, CONLE, VWALL,
    VWALL, BLROT, TRROT, BLANK, NUBLE, HWALL, HWALL, HWALL, BRCOR, CHIPO, CHIPO, BLOCK, CHIPO, VWALL,
    VWALL, BLROT, BRROT, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR,
  }, {
    // Level 17
    NUBUP, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, START, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, NUBUP,
    VWALL, BLANK, BLOCK, CONDO, NUBLE, NUBRI, BLANK, NUBLE, NUBRI, CONDO, NUBLE, NUBRI, BLANK, VWALL,
    VWALL, BLANK, CHIPO, CONDO, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, CONDO, CHIPO, CHIPO, BLANK, VWALL,
    NUBDO, BLANK, BLOCK, CONDO, NUBLE, NUBRI, BLANK, NUBLE, NUBRI, CONDO, NUBLE, NUBRI, BLANK, NUBDO,
    TLROT, BLANK, CHIPO, CONDO, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, CONDO, CHIPO, CHIPO, BLANK, TRROT,
    VTROT, CONUP, BLOCK, CONDO, NUBLE, NUBRI, EXITS, NUBLE, NUBRI, CONDO, NUBLE, NUBRI, CONUP, VTROT,
    VTROT, CONUP, CONLE, CONLE, CONLE, CONLE, BLANK, CONRI, CONRI, CONRI, CONRI, CONRI, CONUP, VTROT,
    BLROT, HTROT, HTROT, HTROT, HTROT, HTROT, BLANK, HTROT, HTROT, HTROT, HTROT, HTROT, HTROT, BRROT,
  }, {
    // Level 18
    VWALL, LEDOF, START, LEDOF, VWALL, CHIPO, CHIPO, VWALL, CHIPO, NUBDO, BLANK, NUBDO, LEDOF, LEDOF,
    BLCOR, NUBRI, BLANK, NUBLE, BRCOR, TLROT, HTROT, VWALL, BLANK, VTROT, BLANK, HEATO, BLANK, BLANK,
    BLANK, BLANK, BLANK, CHIPO, CHIPO, BLANK, BLANK, VWALL, HEATB, NUBUP, BLANK, NUBUP, BLANK, BLANK,
    BLANK, BLANK, BLANK, CHIPO, CHIPO, BLANK, BLANK, NUBDO, HEATI, NUBDO, BLANK, VWALL, CHIPO, CHIPO,
    BLANK, TRROT, BLANK, CHIPO, CHIPO, BLANK, BLANK, HEATB, BLANK, HEATI, BLANK, VWALL, CHIPO, CHIPO,
    NUBRI, VTROT, NUBLE, HWALL, HWALL, HWALL, HWALL, TRCOR, HEATI, BLANK, BLANK, VWALL, CHIPO, CHIPO,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, ROTAB, VWALL, CHIPO, VTROT, BLANK, VWALL, BLANK, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, CHIPO, CHIPO, NUBLE, BRCOR, EXITS, BLANK,
  }, {
    // Level 19
    BLANK, BLANK, BLANK, EXITS, VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, START, BLANK, BLANK,
    NUBRI, HTROT, NUBLE, HWALL, BRCOR, BLANK, BLANK, TLCOR, HWALL, TRCOR, BLANK, BLANK, BLANK, BRROT,
    BLANK, VTROT, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL, LEDOF, VWALL, BLANK, NUBLE, NUBRI, BLANK,
    NUBRI, HTROT, BLOCK, CONUP, NUBUP, BLANK, BLANK, VWALL, BLANK, VWALL, CHIPO, CHIPO, CHIPO, CONUP,
    CHIPO, CHIPO, CHIPO, CHIPO, NUBDO, CHIPO, BLANK, VWALL, BLANK, VWALL, CHIPO, CHIPO, CONRI, CONUP,
    CHIPO, CHIPO, CHIPO, CHIPO, CONLE, CHIPO, BLANK, VWALL, LEDOF, VWALL, CHIPO, CHIPO, CHIPO, ROTAB,
    CHIPO, CHIPO, CHIPO, CHIPO, NUBUP, CHIPO, BLANK, BLCOR, HWALL, BRCOR, CHIPO, CHIPO, CHIPO, CONUP,
    CHIPO, CHIPO, CHIPO, CHIPO, VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, CHIPO, CHIPO, CONRI, CONUP,
  }, {
    // Level 20
    BLANK, BLANK, BLANK, VWALL, LEDOF, LEDOF, VWALL, START, BLANK, BLANK, BLANK, CHIPO, CHIPO, CHIPO,
    BLANK, LEDOF, BLANK, VWALL, LEDOF, LEDOF, VWALL, BLANK, BLANK, NUBUP, BLANK, CHIPO, BLOCK, CHIPO,
    BLANK, BLANK, BLANK, VWALL, LEDOF, LEDOF, VWALL, BLANK, BLANK, NUBDO, BLANK, TRROT, BLANK, CHIPO,
    BLANK, CHIPI, BLANK, BLCOR, HWALL, HWALL, BRCOR, BLANK, CHIPO, CHIPO, BLANK, VTROT, NUBLE, TRCOR,
    BLANK, BLANK, BLANK, NUBUP, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VTROT, CPUOU, VWALL,
    BLANK, CHIPI, BLANK, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, BRROT, CHIPO, VWALL,
    BLANK, BLANK, BLANK, VWALL, CHIPO, CHIPO, BLOCK, CHIPO, CHIPO, TLCOR, HWALL, HWALL, HWALL, BRCOR,
    BLANK, CHIPI, BLANK, VWALL, BLANK, CHIPO, CHIPO, CHIPO, EXITS, VWALL, BLANK, BLANK, BLANK, BLANK,
  }, {
    // Level 21
    TLCOR, HWALL, HWALL, HWALL, HWALL, TRCOR, BLANK, CHIPO, CHIPO, CHIPO, VWALL, CHIPO, CHIPO, CHIPO,
    VWALL, ROTAB, CHIPO, CHIPO, BLANK, NUBDO, BLANK, CHIPO, CHIPO, CHIPO, VWALL, CHIPO, HEATB, CHIPO,
    VWALL, BLANK, TLCOR, TRCOR, BLANK, VTROT, BLANK, CHIPO, CHIPO, BLANK, VWALL, CHIPO, CHIPO, NUBUP,
    VWALL, BLANK, VWALL, VWALL, BLANK, NUBLE, HWALL, NUBRI, CONDO, CONUP, NUBDO, CHIPO, CHIPO, VWALL,
    BRCOR, BLANK, BLCOR, BRCOR, BLANK, HTROT, BLANK, BLANK, BLANK, BLANK, HTROT, BLANK, BLANK, VWALL,
    BLANK, BLANK, CONRI, CONRI, BLANK, TLCOR, HWALL, NUBRI, HTROT, NUBLE, HWALL, NUBRI, HEATI, VWALL,
    START, BLANK, NUBLE, HWALL, HWALL, BRCOR, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    EXITS, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, LEDOF, LEDOF, LEDOF, LEDOF, BLANK, BLANK, VWALL,
  }, {
    // Level 22
    TLCOR, HWALL, NUBRI, CONDO, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, TRCOR,
    VWALL, START, CONLE, BLANK, NUBLE, HWALL, TRCOR, CHIPO, BLOCK, CHIPO, BLOCK, CHIPO, CHIPO, VWALL,
    VWALL, BLANK, CONRI, BLANK, CONRI, BLANK, VWALL, BLANK, CONRI, BLANK, CONRI, BLANK, BLANK, VWALL,
    VWALL, CHIPO, NUBUP, CHIPO, NUBUP, CHIPO, NUBDO, CHIPO, NUBUP, CHIPO, BLOCK, CHIPO, BLANK, VWALL,
    VWALL, EXITS, VWALL, CHIPO, VWALL, CHIPO, CONRI, BLANK, VWALL, BLANK, CONLE, CONLE, CONDO, VWALL,
    VWALL, CHIPO, NUBDO, BLANK, NUBDO, CHIPO, NUBUP, CHIPO, VWALL, CHIPO, NUBUP, CHIPO, BLANK, VWALL,
    VWALL, BLANK, CONLE, BLANK, CONRI, BLANK, NUBDO, BLANK, VWALL, BLANK, NUBDO, BLANK, BLANK, VWALL,
    BLCOR, HWALL, NUBRI, CONUP, CONLE, CONLE, CONLE, CONLE, NUBDO, CONRI, CONRI, CONUP, NUBLE, BRCOR,
  }, {
    // Level 23
    CHIPO, BLANK, BLANK, BLANK, CHIPO, CHIPO, HEATB, CHIPO, CHIPO, BLANK, BLANK, BLANK, BLANK, CHIPO,
    CONDO, NUBLE, NUBRI, CONUP, NUBLE, NUBRI, CONDO, NUBLE, NUBRI, CONUP, NUBLE, HWALL, NUBRI, CONDO,
    BLANK, BLANK, BLANK, BLANK, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK,
    HEATO, NUBLE, NUBRI, HEATI, TLCOR, NUBRI, CHIPO, NUBLE, TRCOR, HEATI, NUBLE, HWALL, NUBRI, HEATO,
    CHIPO, CHIPO, CHIPO, CHIPO, NUBDO, CHIPO, CHIPO, CHIPO, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO,
    HEATB, CHIPO, CHIPO, HEATB, CONLE, CHIPO, CHIPO, CHIPO, CONRI, HEATB, CHIPO, CHIPO, CHIPO, HEATB,
    HEATO, NUBLE, NUBRI, HEATI, BLOCK, CHIPO, CHIPO, CHIPO, BLOCK, HEATO, NUBLE, HWALL, NUBRI, HEATI,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, EXITS, BLANK, BLANK, BLANK, BLANK, START, BLANK, BLANK,
  }, {
    // Level 24
    CHIPO, CHIPO, CHIPO, CHIPO, CONRI, BLANK, NUBUP, CHIPO, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, CHIPO,
    HEATB, CHIPO, CHIPO, HEATB, NUBUP, HEATO, NUBDO, CHIPO, NUBLE, NUBRI, CONUP, CHIPO, LEDOF, CHIPO,
    CONUP, NUBLE, NUBRI, CONUP, VWALL, START, CHIPO, CONRI, CONRI, CONRI, BLANK, CHIPO, CHIPO, CHIPO,
    BLANK, CHIPO, CHIPO, BLANK, VWALL, BLANK, TLCOR, HWALL, HWALL, NUBRI, CONDO, NUBLE, NUBRI, CONDO,
    CONUP, NUBLE, NUBRI, CONUP, NUBDO, BLANK, NUBDO, CHIPO, CHIPO, BLANK, BLANK, CHIPO, CHIPO, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, BLANK, BLROT, BLANK, CHIPO, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, CHIPO,
    BLANK, BLANK, BLANK, BLANK, BLOCK, CONDO, NUBLE, HWALL, HWALL, NUBRI, BLANK, BLOCK, CHIPO, CHIPO,
    LEDOF, EXITS, LEDOF, BLANK, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CHIPO, BLANK,
  }, {
    // Level 25
    CHIPO, CONDO, CHIPO, BLANK, CONLE, BLANK, BLANK, BLANK, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, BLANK,
    CHIPO, BLANK, CHIPO, CHIPO, HTROT, HTROT, HTROT, TRROT, TLCOR, NUBRI, CONUP, NUBLE, NUBRI, BLANK,
    HWALL, NUBRI, VTROT, NUBLE, NUBRI, BLANK, NUBUP, CHIPO, VWALL, BLANK, BLANK, BLANK, BLANK, BLANK,
    EXITS, BLANK, VTROT, BLANK, BLANK, BLANK, VWALL, CHIPO, VWALL, CHIPO, BLANK, TLROT, BLANK, BLANK,
    HWALL, NUBRI, VTROT, NUBLE, TRCOR, BLANK, VWALL, CHIPO, VWALL, CHIPO, NUBUP, BLANK, NUBUP, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, VWALL, BLANK, VWALL, CHIPO, NUBDO, CHIPO, NUBDO, BLANK, NUBDO, BLANK,
    CHIPO, BLANK, CONLE, CHIPO, NUBDO, BLANK, NUBDO, BLANK, START, BLANK, CONLE, BLANK, CONRI, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, CONRI, BLANK, CONLE, BLANK, BLANK, BLANK, BLOCK, ROTAB, BLOCK, BLANK,
  }, {
    // Level 26
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR, LEDOF, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BRROT, BLROT, BRROT, BRROT, BRROT, BLANK,
    VWALL, BLANK, BLANK, TLROT, TLROT, TLROT, BLANK, BLANK, BRROT, HTROT, HTROT, HTROT, TRROT, BLANK,
    VWALL, BLANK, TRROT, VTROT, CHIPO, VTROT, TRROT, BLANK, BLROT, CHIPO, CHIPO, CHIPO, BRROT, BLANK,
    VWALL, BLANK, TRROT, VTROT, CHIPO, VTROT, TRROT, BLANK, BRROT, HTROT, HTROT, HTROT, BLROT, BLANK,
    VWALL, BLANK, BLANK, TLROT, TLROT, TLROT, BLANK, BLANK, TLROT, BRROT, BLROT, BRROT, BRROT, BLANK,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR, LEDOF, BLANK, ROTAB, START, EXITS, BLANK, BLANK,
  }, {
    // Level 27
    BLANK, BLANK, BLANK, CHIPO, BLANK, BLANK, HEATI, EXITS, BLANK, BLANK, BLANK, CHIPO, BLANK, BLANK,
    BLANK, NUBUP, CONUP, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR, BLANK,
    CHIPO, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, ROTAB, CHIPO, VWALL, CHIPO,
    HTROT, VWALL, HEATB, CHIPO, CHIPO, CHIPO, CHIPO, START, CHIPO, CHIPO, CHIPO, HEATB, VWALL, BLANK,
    BLANK, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, HEATB, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, HTROT,
    CHIPO, VWALL, ROTAB, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, ROTAB, CHIPO, CHIPO, CHIPO, VWALL, CHIPO,
    BLANK, BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, NUBRI, CONDO, NUBDO, BLANK,
    BLANK, BLANK, CHIPO, BLANK, BLANK, BLANK, HEATO, BLANK, BLANK, BLANK, CHIPO, BLANK, BLANK, BLANK,
  }, {
    // Level 28
    CHIPO, CONRI, CHIPO, CHIPO, BLANK, BLANK, TRROT, EXITS, TLROT, BLANK, CHIPO, BLANK, CONLE, CHIPO,
    CHIPO, NUBLE, HWALL, NUBRI, HEATO, BLOCK, BLANK, NUBUP, BLANK, TLCOR, TRCOR, CHIPO, NUBUP, CHIPO,
    BLANK, CONLE, BLANK, BLANK, BLANK, CHIPO, CHIPO, NUBDO, BLANK, VWALL, NUBDO, BLANK, VWALL, CHIPO,
    CHIPO, NUBUP, HEATI, NUBLE, TRCOR, CHIPO, CHIPO, BLANK, BLANK, VWALL, HEATB, BLANK, VWALL, CHIPO,
    CHIPO, NUBDO, BLANK, CHIPO, BLCOR, HWALL, HWALL, NUBRI, BLANK, NUBDO, HEATI, NUBLE, BRCOR, CHIPO,
    BLANK, CONRI, BLANK, CHIPO, BLANK, CHIPO, BLANK, BLANK, BLANK, CHIPO, BLANK, BLANK, CONLE, BLANK,
    CHIPO, BLOCK, CONUP, NUBLE, HWALL, NUBRI, CONDO, NUBLE, HWALL, HWALL, HWALL, HWALL, NUBRI, BLANK,
    CHIPO, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK, START, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
  }, {
    // Level 29
    BLANK, BLANK, BLANK, CONRI, START, HTROT, HTROT, HTROT, HTROT, HTROT, BLANK, CHIPO, CHIPO, ROTAB,
    CHIPO, CHIPO, CHIPO, BLOCK, HEATO, NUBLE, HWALL, HWALL, HWALL, NUBRI, BLANK, CHIPO, CHIPO, CHIPO,
    HEATB, BLANK, BLANK, CONLE, BLANK, CONLE, CHIPO, CHIPO, BLANK, HEATI, BLANK, BLANK, BLANK, BLANK,
    CHIPO, CHIPO, CHIPO, BLOCK, BLANK, NUBUP, CONDO, NUBUP, CONDO, NUBLE, NUBRI, VTROT, BLOCK, CONUP,
    HWALL, HWALL, HWALL, TRCOR, CONUP, NUBDO, CONDO, NUBDO, CONRI, CONRI, BLANK, BLANK, BLANK, BLANK,
    LEDOF, BLANK, BLANK, NUBDO, CONUP, BLANK, BLANK, BLANK, TRROT, TLROT, BLANK, CHIPO, CHIPO, CHIPO,
    EXITS, BLANK, BLANK, CONLE, CONLE, CHIPO, CHIPO, CHIPO, BLROT, BRROT, BLANK, CHIPO, CHIPO, CHIPO,
    LEDOF, BLANK, BLANK, NUBLE, NUBRI, CHIPO, CHIPO, CHIPO, CONRI, CONRI, BLANK, CHIPO, CHIPO, CHIPO,
  }, {
    // Level 30
    BLANK, BLANK, BLANK, HEATI, BLANK, EXITS, BLANK, START, BLANK, BLANK, NUBLE, HWALL, HWALL, TRCOR,
    CHIPO, CHIPO, CHIPO, NUBUP, TLCOR, HWALL, HWALL, NUBRI, BLANK, HTROT, HTROT, HTROT, CHIPO, VWALL,
    HEATB, ROTAB, CHIPO, NUBDO, VWALL, CHIPO, BLANK, BLANK, BLANK, BLROT, HTROT, HTROT, BLANK, VWALL,
    CHIPO, CHIPO, CHIPO, BLANK, BLCOR, HWALL, HWALL, NUBRI, CONDO, NUBLE, HWALL, TRCOR, HEATO, VWALL,
    BLANK, TLCOR, TRCOR, BLANK, CONLE, CONLE, HEATB, CONLE, CONLE, LEDOF, LEDOF, VWALL, BLANK, VWALL,
    BLANK, VWALL, VWALL, BLANK, TLCOR, HWALL, HWALL, NUBRI, CONUP, NUBLE, HWALL, BRCOR, BLANK, VWALL,
    BLANK, BLCOR, BRCOR, CONUP, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO, BLANK, CONRI, BLANK, CHIPO, VWALL,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR,
  }, {
    // Level 31
    CHIPO, CONRI, CHIPO, CHIPO, BLANK, BLANK, TRROT, EXITS, TLROT, BLANK, CHIPO, BLANK, CONLE, CHIPO,
    CHIPO, NUBLE, HWALL, NUBRI, HEATO, BLOCK, BLANK, NUBUP, BLANK, TLCOR, TRCOR, CHIPO, NUBUP, CHIPO,
    BLANK, CONLE, BLANK, BLANK, BLANK, CHIPO, CHIPO, NUBDO, BLANK, VWALL, NUBDO, BLANK, VWALL, CHIPO,
    CHIPO, NUBUP, HEATI, NUBLE, TRCOR, CHIPO, CHIPO, BLANK, BLANK, VWALL, HEATB, BLANK, VWALL, CHIPO,
    CHIPO, NUBDO, BLANK, CHIPO, BLCOR, HWALL, HWALL, NUBRI, BLANK, NUBDO, HEATI, NUBLE, BRCOR, CHIPO,
    BLANK, CONRI, BLANK, CHIPO, BLANK, CHIPO, BLANK, BLANK, BLANK, CHIPO, BLANK, BLANK, CONLE, BLANK,
    CHIPO, BLOCK, CONUP, NUBLE, HWALL, NUBRI, CONDO, NUBLE, HWALL, HWALL, HWALL, HWALL, NUBRI, BLANK,
    CHIPO, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK, START, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
  }, {
    // Level 32
    HEATB, CHIPO, HEATO, HEATO, HEATO, HEATI, HEATI, BLANK, BLANK, HTROT, HTROT, HTROT, BLANK, CHIPO,
    HEATI, NUBLE, NUBRI, CHIPO, CHIPO, CHIPO, NUBUP, CHIPO, BLANK, CONRI, HEATB, CONRI, BLANK, NUBUP,
    HEATI, CHIPO, CHIPO, BLANK, BLANK, BLANK, BLCOR, NUBRI, HEATI, NUBLE, HWALL, NUBRI, VTROT, NUBDO,
    HEATO, CHIPO, CHIPO, BLANK, BLANK, BLANK, CHIPO, CHIPO, HEATI, BLANK, EXITS, BLANK, VTROT, CHIPO,
    HEATO, CHIPO, CHIPO, BLANK, START, BLANK, CHIPO, CHIPO, HEATI, BLANK, BLANK, BLANK, VTROT, CHIPO,
    HEATO, NUBLE, TRCOR, BLANK, BLANK, BLANK, TLCOR, NUBRI, HEATI, NUBLE, HWALL, NUBRI, VTROT, NUBUP,
    BLANK, CHIPO, NUBDO, CHIPO, CHIPO, CHIPO, NUBDO, CHIPO, BLANK, HTROT, HTROT, HTROT, BLANK, NUBDO,
    HEATB, BLANK, HEATI, HEATI, HEATI, HEATO, HEATO, BLANK, BLANK, CONLE, ROTAB, CONLE, BLANK, CHIPO,
  }, {
    // Level 33
    LEDOF, EXITS, LEDOF, BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
    BLANK, START, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, HEATI, BLANK, HEATO, BLANK, CHIPO, VWALL,
    BLANK, BLANK, BLANK, BLANK, CONUP, NUBUP, CONUP, BLOCK, BLOCK, CPUOU, NUBUP, BLANK, CHIPO, VWALL,
    NUBUP, CONDO, TLCOR, TRCOR, HEATB, VWALL, BLANK, CONRI, CONRI, HEATB, VWALL, BLANK, CHIPO, VWALL,
    VWALL, CONDO, BLCOR, BRCOR, CONUP, VWALL, CHIPO, NUBUP, NUBUP, CPUOU, VWALL, CHIPO, CHIPO, VWALL,
    VWALL, BLANK, CHIPO, CHIPO, BLANK, NUBDO, VTROT, NUBDO, NUBDO, BLANK, NUBDO, CHIPO, CHIPO, VWALL,
    VWALL, CHIPO, CHIPO, CHIPO, BLANK, BLANK, HTROT, BLANK, HEATI, BLANK, HEATO, BLANK, BLANK, VWALL,
    BLCOR, HWALL, HWALL, NUBRI, BLROT, HTROT, BLANK, HTROT, HTROT, HTROT, HTROT, HTROT, BRROT, BLCOR,
  }, {
    // Level 34
    CHIPO, BLANK, VTROT, VTROT, HTROT, VTROT, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    CHIPO, ROTAB, NUBUP, BLANK, BLANK, BLANK, BLANK, TLCOR, HWALL, HWALL, TRCOR, CONUP, CHIPO, CHIPO,
    HWALL, NUBRI, VWALL, BLANK, BLANK, START, NUBLE, BRCOR, CHIPO, CHIPO, NUBDO, CONUP, CHIPO, CHIPO,
    CHIPO, CHIPO, BLCOR, HWALL, NUBRI, BLANK, BLANK, CHIPO, CHIPO, CHIPO, CHIPO, CONUP, CHIPO, CHIPO,
    CHIPO, CHIPO, CHIPO, CHIPO, BLANK, BLANK, NUBUP, CHIPO, CHIPO, HTROT, CHIPO, CONUP, CHIPO, CHIPO,
    CHIPO, CHIPO, TRROT, CHIPO, NUBUP, BLANK, VWALL, CHIPO, BLOCK, CHIPO, CHIPO, CONUP, CHIPO, CHIPO,
    CHIPO, CHIPO, CHIPO, CHIPO, NUBDO, BLANK, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO, CONUP, ROTAB, CHIPO,
    CHIPO, CHIPO, CHIPO, CHIPO, BLANK, BLANK, BLANK, HTROT, HTROT, HTROT, HTROT, EXITS, LEDOF, LEDOF,
  }, {
    // Level 35
    CONRI, CONRI, CONRI, CONRI, CONRI, EXITS, NUBLE, HWALL, HWALL, HWALL, TRCOR, LEDOF, LEDOF, LEDOF,
    CONUP, TLCOR, HWALL, TRCOR, CHIPO, CHIPO, CHIPO, CHIPO, TLCOR, TRCOR, VWALL, BLANK, START, BLANK,
    CONUP, BLCOR, HWALL, BRCOR, CHIPO, CHIPO, ROTAB, CHIPO, BLCOR, BRCOR, NUBDO, BLANK, BLANK, BLANK,
    CONUP, BLOCK, CHIPO, CHIPO, VTROT, CHIPO, CHIPO, VTROT, CHIPO, CHIPO, BLOCK, BLANK, BLANK, BLANK,
    CONUP, BLANK, CHIPO, ROTAB, CHIPO, TLCOR, TRCOR, CHIPO, ROTAB, CHIPO, BLANK, BLANK, BLANK, BLANK,
    CONUP, NUBUP, CHIPO, CHIPO, CHIPO, BLCOR, BRCOR, CHIPO, CHIPO, CHIPO, NUBUP, CHIPO, CHIPO, CHIPO,
    CONUP, VWALL, ROTAB, CHIPO, HTROT, CHIPO, CHIPO, HTROT, CHIPO, CHIPO, VWALL, CHIPO, ROTAB, CHIPO,
    CONUP, BLCOR, HWALL, NUBRI, CHIPO, CONRI, CONRI, CHIPO, NUBLE, HWALL, BRCOR, CHIPO, CHIPO, CHIPO,
  }, {
    // Level 36
    BLANK, BLANK, BLANK, CONRI, BLANK, CHIPO, CHIPO, BLANK, CONRI, BLANK, CHIPO, CHIPO, CHIPO, BLANK,
    VTROT, TLCOR, HWALL, TRCOR, CONDO, NUBLE, NUBRI, CONUP, TLCOR, HWALL, HWALL, HWALL, TRCOR, VTROT,
    VTROT, VWALL, CPUIN, VWALL, BLANK, CHIPO, CHIPO, CHIPO, VWALL, CPUIN, CPUIN, CPUIN, VWALL, VTROT,
    VTROT, VWALL, CPUIN, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO, NUBDO, CPUIN, CPUIN, CPUIN, VWALL, VTROT,
    VTROT, BLCOR, HWALL, HWALL, NUBRI, CHIPO, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR, VTROT,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    HTROT, NUBLE, HWALL, NUBRI, BLANK, BLANK, BLANK, BLANK, NUBLE, HWALL, HWALL, HWALL, NUBRI, HTROT,
    ROTAB, CHIPO, BLANK, BLANK, BLANK, START, EXITS, BLANK, BLANK, BLANK, BLANK, BLANK, CHIPO, ROTAB
  }, {
    // Level 37
    LEDOF, LEDOF, EXITS, LEDOF, LEDOF, VWALL, CONDO, BLOCK, CHIPO, HTROT, CHIPO, CHIPO, BLOCK, BLANK,
    CHIPO, BLANK, BLANK, BLANK, CHIPO, VWALL, CONDO, CONLE, CHIPO, CHIPO, ROTAB, CHIPO, CONLE, BLANK,
    HWALL, NUBRI, BLANK, NUBLE, HWALL, CPUIN, CONDO, NUBUP, CHIPO, CHIPO, CHIPO, CHIPO, BLOCK, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, CONDO, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CONLE, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, CONDO, VWALL, CHIPO, TRROT, CHIPO, CHIPO, NUBUP, BLANK,
    CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, VWALL, CONDO, NUBDO, CHIPO, CHIPO, VTROT, CHIPO, NUBDO, BLANK,
    HWALL, NUBRI, BLANK, NUBLE, HWALL, BRCOR, CONDO, CONLE, CHIPO, CHIPO, CHIPO, ROTAB, CONLE, BLANK,
    CONRI, CONRI, BLANK, CONLE, CONLE, CONLE, CONLE, BLOCK, CHIPO, CHIPO, CHIPO, CHIPO, BLOCK, START,
  }, {
    // Level 38
    CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, HEATO, CONDO, NUBDO, CHIPO, TLCOR, HWALL, TRCOR, CHIPO, CHIPO,
    CHIPO, HEATI, CHIPO, BLOCK, CHIPO, NUBUP, CONDO, BLANK, BLANK, VWALL, BLANK, VWALL, HEATB, CHIPO,
    CHIPO, BLANK, BLROT, BLANK, CHIPO, NUBDO, CONDO, NUBUP, CHIPO, BLCOR, HWALL, BRCOR, CHIPO, CHIPO,
    CHIPO, BLOCK, CHIPO, BLANK, TRROT, BLANK, CONDO, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO,
    CHIPO, HTROT, CHIPO, BLOCK, CHIPO, NUBUP, CONDO, NUBDO, CHIPO, BLOCK, HEATI, BLOCK, CHIPO, CHIPO,
    CHIPO, HTROT, CHIPO, HTROT, CHIPO, VWALL, CONDO, BLANK, BLANK, HEATO, CHIPO, HEATO, CHIPO, CHIPO,
    CHIPO, NUBLE, HWALL, NUBRI, CHIPO, NUBDO, CONDO, BLOCK, CHIPO, BLOCK, HEATI, BLOCK, CHIPO, CHIPO,
    BLANK, BLANK, BLANK, BLANK, BLANK, START, EXITS, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
  }, {
    // Level 39
    TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR, LEDOF,
    VWALL, BLANK, CHIPO, CHIPO, BLANK, BLANK, BLANK, START, BLANK, BLANK, CHIPO, CHIPO, VWALL, BLANK,
    VWALL, CONDO, TLCOR, HWALL, HWALL, NUBRI, BLANK, BLANK, NUBLE, HWALL, TRCOR, CHIPO, VWALL, BLANK,
    VWALL, CONDO, BLCOR, HWALL, NUBRI, TLROT, BRROT, BRROT, TRROT, NUBLE, BRCOR, BLANK, NUBDO, BLANK,
    VWALL, BLANK, BLANK, BLANK, BLANK, TRROT, BRROT, TRROT, BLROT, BLANK, BLANK, BLANK, BLANK, EXITS,
    VWALL, CONUP, TLCOR, HWALL, TRCOR, TRROT, TLROT, TRROT, BRROT, NUBLE, TRCOR, BLANK, NUBUP, BLANK,
    VWALL, CONUP, NUBDO, ROTAB, NUBDO, BLROT, TRROT, TRROT, BRROT, NUBLE, BRCOR, CHIPO, VWALL, BLANK,
    VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CHIPO, CHIPO, VWALL, LEDOF,
  }, {
    // Level 40
    BLANK, START, BLANK, BLANK, BLANK, BLANK, CHIPO, CHIPO, HEATB, HEATB, HEATB, TLCOR, HWALL, TRCOR,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CPUOU, BLOCK, CONDO, BLOCK, CONUP, NUBDO, ROTAB, VWALL,
    HEATI, HEATI, TLCOR, NUBRI, VTROT, BLOCK, CONRI, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    CHIPO, CHIPO, VWALL, BLROT, BLANK, BLANK, CPUOU, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
    CHIPO, CHIPO, VWALL, HEATO, TLCOR, HWALL, HWALL, NUBRI, CONUP, NUBUP, CHIPO, CHIPO, CHIPO, VWALL,
    HEATO, HEATO, NUBDO, BLANK, VWALL, CHIPO, BLANK, BLANK, BLROT, NUBDO, CHIPO, CHIPO, CHIPO, VWALL,
    BLANK, BLANK, BLANK, BLANK, VWALL, ROTAB, NUBLE, NUBRI, BLANK, CONLE, CHIPO, CHIPO, CHIPO, VWALL,
    BLANK, BLANK, EXITS, BLANK, VWALL, CHIPO, BLANK, BLANK, BLANK, NUBLE, HWALL, HWALL, HWALL, BRCOR
  }, {
    // Level 41
    CHIPO, CONRI, BLANK, CHIPO, BLANK, TLCOR, HWALL, HWALL, TRCOR, EXITS, TLCOR, HWALL, HWALL, NUBRI,
    BLANK, NUBLE, NUBRI, CONDO, HEATI, NUBDO, BLANK, START, VWALL, BLANK, NUBDO, BLANK, BLANK, BLANK,
    ROTAB, HEATB, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL, BLANK, BLANK, TRROT, NUBUP, BLANK,
    CONRI, CONUP, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL, BLANK, TLROT, BLANK, VWALL, BLANK,
    NUBUP, CONUP, NUBLE, HWALL, HWALL, NUBRI, TRROT, BLANK, BLCOR, TRCOR, BLANK, BLANK, VWALL, BLANK,
    VWALL, CONUP, BLANK, BLANK, TLROT, BLANK, BLANK, TLCOR, HWALL, BRCOR, HEATO, NUBLE, BRCOR, BLANK,
    VWALL, CONUP, BLANK, CHIPO, HTROT, BRROT, NUBLE, BRCOR, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, BLANK,
    VWALL, CONUP, BLANK, CHIPO, BLROT, BLANK, BLANK, BLANK, CHIPO, CHIPO, BLANK, CHIPO, CHIPO, BLANK,
  }, {
    // Level 42
    CHIPO, CHIPO, BLANK, CHIPO, HEATI, BLANK, CONLE, CHIPO, BLANK, HEATB, BLANK, EXITS, BLANK, BLANK,
    CHIPO, CHIPO, BLANK, CHIPO, NUBUP, CHIPO, NUBUP, CHIPO, BLANK, NUBLE, HWALL, HWALL, TRCOR, CONUP,
    CHIPO, CHIPO, BLANK, CHIPO, VWALL, CHIPO, VWALL, CHIPO, CHIPO, CONRI, BLANK, CHIPO, VWALL, CONUP,
    CHIPO, CHIPO, BLANK, CHIPO, VWALL, CHIPO, VWALL, CHIPO, CHIPO, NUBUP, CHIPO, CHIPO, VWALL, CONUP,
    CONDO, VTROT, NUBLE, HWALL, BRCOR, CHIPO, BLCOR, NUBRI, CHIPO, BLCOR, NUBRI, CONDO, NUBDO, CONUP,
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, HEATO, BLANK, BLANK, BLANK,
    NUBRI, VTROT, NUBLE, HWALL, NUBRI, CHIPO, NUBLE, HWALL, HWALL, HWALL, NUBRI, CONUP, NUBLE, HWALL,
    START, BLANK, CHIPO, CHIPO, HEATI, BLANK, CONRI, BLANK, CHIPO, CHIPO, BLANK, BLANK, BLANK, CHIPO,
  }, {
    // Level 43
    CHIPO, CHIPO, CHIPO, CONRI, BLANK, CONLE, BLANK, CONRI, BLANK, CHIPO, CHIPO, HEATO, CHIPO, CHIPO,
    CHIPO, CHIPO, CHIPO, HEATO, BLANK, NUBUP, CHIPO, NUBUP, BLANK, CHIPO, CHIPO, NUBUP, CHIPO, CHIPO,
    CHIPO, HEATB, CHIPO, NUBUP, BLANK, NUBDO, CHIPO, NUBDO, CONDO, CHIPO, CHIPO, NUBDO, CHIPO, CHIPO,
    CHIPO, CHIPO, CHIPO, VWALL, BLANK, CONRI, BLANK, CONLE, BLANK, CHIPO, HEATB, CHIPO, HEATI, BLANK,
    CONDO, HEATI, NUBLE, BRCOR, CONUP, TLCOR, HWALL, TRCOR, CHIPO, CHIPO, CHIPO, CHIPO, NUBUP, CHIPO,
    CHIPO, CHIPO, CHIPO, BLANK, BLANK, NUBDO, EXITS, NUBDO, BLANK, TLCOR, NUBRI, HEATI, NUBDO, CHIPO,
    CHIPO, CHIPO, CHIPO, CHIPO, BLANK, HTROT, BLANK, HTROT, BLANK, NUBDO, CHIPO, CHIPO, CHIPO, CHIPO,
    CHIPO, CHIPO, CHIPO, BLANK, BLANK, NUBUP, START, NUBUP, BLANK, HEATO, BLANK, CHIPO, CHIPO, HEATB,
  }, {
    // Level 44
    LEDOF, LEDOF, VWALL, BLANK, CONRI, ROTAB, HEATI, CONRI, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    LEDOF, LEDOF, VWALL, BLANK, NUBLE, HWALL, HWALL, NUBRI, BLANK, NUBUP, CONDO, CHIPO, NUBUP, HEATI,
    HWALL, HWALL, BRCOR, BLANK, CONLE, CONLE, CONLE, CONLE, BLANK, NUBDO, BLANK, CHIPO, VWALL, CHIPO,
    BLANK, CHIPO, BLANK, BLANK, TLCOR, HWALL, HWALL, NUBRI, CHIPO, HTROT, BLANK, CHIPO, VWALL, ROTAB,
    CONDO, TLCOR, NUBRI, VTROT, VWALL, HEATB, CHIPO, TRROT, CHIPO, NUBLE, HWALL, HWALL, BRCOR, CHIPO,
    CHIPO, NUBDO, BLANK, BLANK, VWALL, CHIPO, CHIPO, BLROT, CHIPO, CONRI, ROTAB, CONLE, CHIPO, CHIPO,
    CONDO, HEATI, BLANK, BLANK, BLCOR, HWALL, HWALL, NUBRI, CHIPO, BLOCK, CONDO, NUBUP, CHIPO, CHIPO,
    HEATB, HEATI, BLANK, EXITS, BLANK, CONLE, HEATB, CONLE, BLANK, HTROT, START, BLCOR, HWALL, NUBRI
  }, {
    // Level 45
    BLANK, CHIPO, CHIPO, HEATB, BLANK, CPUOU, BLANK, BLROT, HTROT, HTROT, HEATB, CHIPO, CHIPO, CHIPO,
    BLANK, CHIPO, CHIPO, CHIPO, CHIPO, NUBUP, BLANK, BLANK, NUBUP, BLANK, ROTAB, CHIPO, CHIPO, BLANK,
    CPUOU, NUBLE, HWALL, HWALL, HWALL, BRCOR, CPUOU, NUBLE, BRCOR, CONDO, CONUP, NUBLE, NUBRI, BLANK,
    BLANK, BLANK, BLANK, BLANK, BLANK, CPUOU, START, EXITS, CPUOU, BLANK, BLANK, BLANK, BLANK, BLANK,
    HEATO, NUBLE, NUBRI, CONUP, NUBLE, TRCOR, CPUOU, TLCOR, HWALL, NUBRI, VTROT, NUBLE, NUBRI, VTROT,
    BLANK, CHIPO, HEATI, HEATO, BLANK, VWALL, BLANK, VWALL, HEATB, CONLE, TLROT, BLANK, TLROT, BLANK,
    ROTAB, CHIPO, HEATI, CHIPO, BLANK, NUBDO, BLANK, NUBDO, CONDO, CHIPO, CHIPO, CHIPO, CHIPO, HTROT,
    CHIPO, CHIPO, HEATI, CHIPO, BLANK, CONLE, BLANK, HTROT, BLANK, CHIPO, CHIPO, BLANK, HTROT, CHIPO,
  }, {
    // Level 46
    CHIPO, CHIPO, HTROT, BLANK, VWALL, VWALL, VWALL, CHIPO, VWALL, VWALL, BLANK, BLROT, CHIPO, HEATB,
    CHIPO, ROTAB, TLROT, BLANK, NUBDO, NUBDO, NUBDO, START, NUBDO, NUBDO, BLANK, BRROT, CHIPO, ROTAB,
    CHIPO, CHIPO, VTROT, BLANK, BLANK, BLANK, CHIPO, BLANK, CHIPO, BLANK, BLANK, HTROT, CHIPO, CHIPO,
    HEATI, HEATI, BLANK, BLANK, NUBUP, CONDO, BLOCK, CHIPO, BLOCK, CONDO, HEATI, BLANK, BLOCK, BLOCK,
    CHIPO, CHIPO, BRROT, BLANK, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, NUBUP, BLANK, CHIPO, CHIPO,
    ROTAB, CHIPO, TRROT, BLANK, BLCOR, TRCOR, BRROT, CHIPO, TLROT, TLCOR, BRCOR, CHIPO, CHIPO, ROTAB,
    CHIPO, CHIPO, TRROT, BLANK, NUBUP, BLCOR, TRCOR, EXITS, TLCOR, BRCOR, NUBUP, CHIPO, CHIPO, CHIPO,
    HWALL, TRCOR, BLANK, BLANK, VWALL, NUBUP, VWALL, NUBUP, VWALL, NUBUP, VWALL, BLANK, CONLE, CONLE,
  } ,
  {
    // Level 47
    BLANK, CONLE, CONLE, CONLE, CONLE, CONLE, BLANK, CONLE, CONLE, CONLE, BLANK, BLANK, BLANK, BLANK,
    BLANK, BLOCK, HEATB, EXITS, ROTAB, BLOCK, CHIPO, BLOCK, BLOCK, BLOCK, BLOCK, CONUP, BLOCK, HEATO,
    BLANK, BLOCK, BLANK, START, BLANK, BLOCK, BLANK, BLANK, CHIPO, BLOCK, BLANK, BLANK, BLOCK, CHIPO,
    BLANK, CONRI, CONRI, BLANK, CONRI, CONRI, BLANK, BLANK, CHIPO, BLOCK, BLANK, BLANK, BLOCK, CHIPO,
    BLANK, BLOCK, BLOCK, VTROT, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, BLANK, BLANK, BLOCK, BLANK,
    BLROT, BLANK, BLANK, BLANK, BLANK, BLANK, CONRI, BLANK, BLANK, BLANK, BLANK, BLANK, HEATI, BLANK,
    BLANK, BLOCK, BLOCK, BLOCK, BLOCK, CHIPO, BLOCK, BLOCK, BLOCK, BLOCK, BLOCK, CHIPO, BLOCK, CHIPO,
    BLROT, BLANK, HEATI, CHIPO, CHIPO, CHIPO, CHIPO, CHIPO, HEATI, BLANK, BLANK, BLANK, BLANK, BLANK,
  }, {
    // Level 48
    HEATB, ROTAB, BLANK, VTROT, TRROT, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, CHIPO, CHIPO, CHIPO,
    CHIPO, NUBLE, HWALL, NUBRI, BLANK, BLANK, TRROT, BRROT, NUBUP, BLANK, BLANK, CHIPO, HEATB, HTROT,
    BLOCK, TRROT, VTROT, BRROT, BLANK, NUBUP, TLROT, BRROT, NUBDO, BLANK, NUBUP, CHIPO, CHIPO, CHIPO,
    CHIPO, BLANK, TRROT, BRROT, BLANK, VWALL, BLANK, TRROT, BLANK, BLANK, BLCOR, HWALL, NUBRI, CONDO,
    NUBUP, HTROT, HTROT, TLROT, BLANK, VWALL, TLROT, VTROT, NUBUP, BLANK, CONLE, CONLE, CONLE, ROTAB,
    NUBDO, TLROT, VTROT, VTROT, BLANK, VWALL, HTROT, TRROT, VWALL, BLANK, HEATI, CHIPO, CHIPO, HEATI,
    CHIPO, NUBLE, HWALL, NUBRI, BLANK, VWALL, BLANK, BRROT, VWALL, BLANK, NUBLE, HWALL, HWALL, NUBRI,
    BLANK, ROTAB, VTROT, HEATI, BLANK, VWALL, TLROT, CHIPO, VWALL, BLANK, BLANK, BLANK, START, EXITS,
  }, {
    // Level 49
    BLANK, BLANK, BLANK, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, CONLE, VWALL,
    BLANK, ROTAB, BLANK, TRROT, TLROT, TLROT, TLROT, HTROT, TLROT, HTROT, BLROT, TLROT, CONUP, VWALL,
    BLANK, START, BLANK, TLROT, TRROT, BLROT, VTROT, TLROT, BRROT, TLROT, BLROT, TRROT, CONUP, VWALL,
    BLANK, BLANK, BLANK, BRROT, TRROT, HTROT, BLANK, BLANK, TRROT, ROTAB, BLANK, HTROT, CONUP, VWALL,
    HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, NUBRI, TRROT, HTROT, CONUP, VWALL,
    BLANK, BLANK, BLANK, LEDOF, TLROT, TRROT, TLROT, HTROT, HTROT, HTROT, HTROT, BRROT, CONUP, VWALL,
    EXITS, BLANK, BLANK, CHIPO, BLROT, BLROT, BRROT, TRROT, CONRI, CONRI, CONRI, CONRI, CONUP, VWALL,
    BLANK, BLANK, BLANK, LEDOF, BLROT, BLROT, BRROT, BRROT, TLCOR, HWALL, HWALL, HWALL, HWALL, BRCOR,
  }, {
    // Level 50
    BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK,
    NUBRI, VTROT, NUBLE, HWALL, HWALL, HWALL, HWALL, HWALL, NUBRI, VTROT, NUBLE, NUBRI, CHIPO, CHIPO,
    BLANK, BLANK, BLANK, CONRI, BLANK, CONLE, BLANK, CHIPO, CHIPO, BLANK, CONLE, CHIPO, CHIPO, CHIPO,
    CHIPO, ROTAB, CHIPO, NUBUP, CHIPO, NUBUP, CHIPO, CHIPO, CHIPO, CHIPO, BLOCK, CHIPO, ROTAB, CHIPO,
    CHIPO, CHIPO, CHIPO, VWALL, CONDO, VWALL, CHIPO, CHIPO, CHIPO, CHIPO, CONRI, BLANK, CHIPO, CHIPO,
    CHIPO, VTROT, CHIPO, VWALL, CONDO, VWALL, CHIPO, TLROT, HTROT, CHIPO, TLCOR, NUBRI, CHIPO, NUBUP,
    NUBRI, VTROT, NUBLE, BRCOR, CONDO, BLCOR, NUBRI, VTROT, VTROT, NUBLE, BRCOR, BLANK, BLANK, VWALL,
    EXITS, BLANK, BLANK, BLANK, BLANK, BLANK, START, BLANK, BLANK, BLANK, BLANK, BLANK, NUBLE, BRCOR
  }
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//              LEVELS END HERE

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


unsigned char currentmap[112] = { //14x8 Tiles
  TLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, TRCOR,
  VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
  VWALL, BLANK, START, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
  VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
  VWALL, BLANK, BLANK, EXITS, BLANK, CHIPO, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
  VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
  VWALL, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, BLANK, VWALL,
  BLCOR, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, HWALL, BRCOR
};

void drawcorners() {
  ardbitmap.drawBitmap(0, 0, tl, 24, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
  ardbitmap.drawBitmap(109, 0, tr, 19, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
  ardbitmap.drawBitmap(0, 56, bl, 43, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
  ardbitmap.drawBitmap(99, 56, br, 29, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
}

void printmenuitem(char menuitem, char y) {
  arduboy.setCursor(68, y);
  if (menuitem >= -5 && menuitem < 50) {
    if (menuitem == -5) {
      arduboy.print(F("Erase Data"));
    } else if (menuitem == -4) {
      arduboy.print(F("Mirror:"));
      if (casting) {
        arduboy.print(F("On"));
      } else {
        arduboy.print(F("Off"));
      }
    } else if (menuitem == -3) {
      arduboy.print(F("Sound: "));
      if (arduboy.audio.enabled()) {
        arduboy.print(F("On"));
      } else {
        arduboy.print(F("Off"));
      }
    } else if (menuitem == -2) {
      arduboy.print(F("Credits"));
    } else if (menuitem == -1) {
      arduboy.print(F("Help"));
    } else {
      arduboy.print(F("Level "));
      arduboy.print((int)(menuitem + 1));
      if (menuitem > maxlevel) {
        ardbitmap.drawBitmap(118, y, lock, 5, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      }
    }
  }
}

void drawtitle() {
  ardbitmap.drawCompressed(0, 0, title, WHITE, ALIGN_NONE, MIRROR_NONE);
  if (220 <= frame && frame < 225) {
    ardbitmap.drawCompressed(17, 21, eyes2, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else if (225 <= frame && frame < 230) {
    ardbitmap.drawCompressed(17, 21, eyes3, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else if (230 <= frame && frame < 235) {
    ardbitmap.drawCompressed(17, 21, eyes2, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else if (235 <= frame && frame < 240) {
    ardbitmap.drawCompressed(17, 21, eyes1, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else if (240 <= frame && frame < 245) {
    ardbitmap.drawCompressed(17, 21, eyes2, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else if (245 <= frame && frame < 250) {
    ardbitmap.drawCompressed(17, 21, eyes3, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else if (250 <= frame && frame < 255) {
    ardbitmap.drawCompressed(17, 21, eyes2, WHITE, ALIGN_NONE, MIRROR_NONE);
  } else {
    ardbitmap.drawCompressed(17, 21, eyes1, WHITE, ALIGN_NONE, MIRROR_NONE);
  }
}

void drawlevel() {
  int currentspot = 0;
  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 14; x++) {
      if (currentmap[currentspot] >= 21 && currentmap[currentspot] <= 24) {
        ardbitmap.drawBitmap(x * 8, y * 8, spritesheet[currentmap[currentspot] + animation / 2 * 4], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      } else if (currentmap[currentspot] < 50) {
        if (currentmap[currentspot] == EXITS) {
          if (levelcomplete) {
            ardbitmap.drawBitmap(x * 8, y * 8, spritesheet[38 + animation / 2], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
          } else {
            ardbitmap.drawBitmap(x * 8, y * 8, spritesheet[38], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
          }
        } else if (currentmap[currentspot] == 12) {
          if (levelcomplete) {
            ardbitmap.drawBitmap(x * 8, y * 8, spritesheet[13], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
          } else {
            ardbitmap.drawBitmap(x * 8, y * 8, spritesheet[12], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
          }
        } else {
          ardbitmap.drawBitmap(x * 8, y * 8, spritesheet[currentmap[currentspot]], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
        }
      }
      currentspot += 1;
    }
  }
}

#define NOISECONVEYER 100
#define NOISEMENU 300
#define NOISEMENUCLICK 350
#define NOISEEXPLOSION1 600
#define NOISEEXPLOSION2 700
#define NOISEEXPLOSION3 400
#define NOISEEXPLOSION4 100
#define NOISEHEATBUTTON 350
#define NOISEROTATEBUTTON 425
#define NOISEROTATE 250
#define NOISECPUOUT 140
#define NOISECHIPOUT 475
#define NOISELEVELCOMPLETE 700
void sound(int pitch, long duration) {
  if (arduboy.audio.enabled()) {
    tunes.tone(pitch, duration);
  }
}


void continuestate(char tostate) {
  if (arduboy.pressed(A_BUTTON) && menubuttonfree) {
    sound(NOISEMENUCLICK, 100);
    gamestate = tostate;
    menubuttonfree = false;
  }
}

void loadlevel(int givenlevel) {
  //digitalWrite(RED_LED, OFF);
  //digitalWrite(GREEN_LED, OFF);
  arduboy.setRGBled(0, 0, 0);
  levelcompletecounter = 0;
  if (givenlevel == 50) {
    gamestate = 6;
  } else {
    if (givenlevel > maxlevel) {
      maxlevel = givenlevel;
      EEPROM.put(SAVELOCATION, maxlevel);
    }
    levelcomplete = false;
    level = givenlevel;
    canswitch = true;
    deathcounter = 0;
    for (int i = 0; i < sizeof(currentmap) / sizeof(currentmap[0]); i++) {
      currentmap[i] = pgm_read_byte(&levelmap[givenlevel][i]);
      if (currentmap[i] == 51) {
        playery = i / 14 * 8;
        playerx = (i - (i / 14) * 14)  * 8;
        playertoy = playery;
        playertox = playerx;
      }
    }
    animation = 0;
    frame = 0;
    levelcomplete = false;
    temperature = 0;
  }
}

bool checkcollision(char locx, char locy) {
  char leftright = locx * 8 - playerx;  //0 = Up/Down, - = Left, + = Right
  char updown = locy * 8 - playery;   //0 = Left/Right, - = Up, + = Down
  char tile = currentmap[locx + locy * 14];
  if (locx < 0 || locy < 0 || locx >= 14 || locy >= 8) {
    return false;
  }
  if (tile <= 15) {
    return false;
  }
  if (leftright < 0) {
    if (tile == CONRI || tile == VTROT || tile == TRROT || tile == BRROT) {
      return false;
    }
  }
  if (leftright > 0) {
    if (tile == CONLE || tile == VTROT || tile == TLROT || tile == BLROT) {
      return false;
    }
  }
  if (updown > 0) {
    if (tile == CONUP || tile == HTROT || tile == TLROT || tile == TRROT) {
      return false;
    }
  }
  if (updown < 0) {
    if (tile == CONDO || tile == HTROT || tile == BLROT || tile == BRROT) {
      return false;
    }
  }
  return true;
}

void checklocation(char locx, char locy) {
  switch (currentmap[locx + locy * 14]) {
    case CONUP:
      playertoy -= 8;
      transitionanimation = 9;
      gamebuttonfree = false;
      sound(NOISECONVEYER, 260);
      break;
    case CONDO:
      playertoy += 8;
      transitionanimation = 9;
      gamebuttonfree = false;
      sound(NOISECONVEYER, 260);
      break;
    case CONLE:
      playertox -= 8;
      transitionanimation = 9;
      gamebuttonfree = false;
      sound(NOISECONVEYER, 260);
      break;
    case CONRI:
      playertox += 8;
      transitionanimation = 9;
      gamebuttonfree = false;
      sound(NOISECONVEYER, 260);
      break;
    case CHIPO:
      sound(NOISECHIPOUT, 100);
      currentmap[locx + locy * 14] = 11;
      break;
    case EXITS:
      if (levelcomplete) {
        sound(NOISELEVELCOMPLETE, 400);
        loadlevel(level + 1);
      }
      break;
    case CPUOU:
      sound(NOISECPUOUT, 150);
      currentmap[locx + locy * 14] = 14;
      break;
    case HEATB:
      if (canswitch) {
        sound(NOISEHEATBUTTON, 150);
        swapall(HEATO, HOLD);
        swapall(HEATI, HEATO);
        swapall(HOLD, HEATI);
        canswitch = false;
      }
      break;
    case ROTAB:
      if (canswitch) {
        sound(NOISEROTATEBUTTON, 150);
        swapall(BRROT, HOLD);
        swapall(TRROT, BRROT);
        swapall(TLROT, TRROT);
        swapall(BLROT, TLROT);
        swapall(HOLD, BLROT);
        swapall(VTROT, HOLD);
        swapall(HTROT, VTROT);
        swapall(HOLD, HTROT);
        canswitch = false;
      }
      break;
  }
}

void rotateinplace(char index) {
  if (currentmap[index] == BRROT) {
    currentmap[index] = BLROT;
    sound(NOISEROTATE, 150);
    return;
  }
  if (currentmap[index] == TRROT) {
    sound(NOISEROTATE, 150);
    currentmap[index] = BRROT;
    return;
  }
  if (currentmap[index] == TLROT) {
    sound(NOISEROTATE, 150);
    currentmap[index] = TRROT;
    return;
  }
  if (currentmap[index] == BLROT) {
    sound(NOISEROTATE, 150);
    currentmap[index] = TLROT;
    return;
  }
  if (currentmap[index] == HTROT) {
    sound(NOISEROTATE, 150);
    currentmap[index] = VTROT;
    return;
  }
  if (currentmap[index] == VTROT) {
    sound(NOISEROTATE, 150);
    currentmap[index] = HTROT;
    return;
  }
}

void swapall(char from, char to) {
  for (int i = 0; i < sizeof(currentmap) / sizeof(currentmap[0]); i++) {
    if (currentmap[i] == from) {
      currentmap[i] = to;
    }
  }
}

void checkcompletion() {
  boolean gotemcoach = true;
  for (int i = 0; i < sizeof(currentmap) / sizeof(currentmap[0]); i++) {
    if (currentmap[i] == 37) {
      gotemcoach = false;
    }
  }
  levelcomplete = gotemcoach;
  if (levelcomplete && levelcompletecounter >= 0) {
    levelcompletecounter--;
    //digitalWrite(GREEN_LED, ON);
    arduboy.setRGBled(0, 255, 0);
  } else {
    //digitalWrite(GREEN_LED, OFF);
    arduboy.setRGBled(0, 0, 0);
    if (levelcompletecounter == 0) {
      levelcompletecounter = 75;
    }
  }
}

void drawgrid(int16_t  x, int16_t  y, int16_t  w, int16_t  h) {
  bool color = true;
  for(int16_t  i = x; i < w + x; i++) {
    color = !color;
    for(int16_t  j = y; j < h + y; j++) {
      if(color) {
        arduboy.drawPixel(i, j, BLACK);
      }
      color = !color;
    }
  }
}

void setup() {
  //arduboy.begin();
  arduboy.boot();
  arduboy.blank();
  arduboy.flashlight();
  arduboy.audio.begin();
  arduboy.bootLogo();
  while(arduboy.buttonsState()) {
    delay(10);
  }
  
  arduboy.setFrameRate(60);
  arduboy.display();

  tunes.initChannel(PIN_SPEAKER_1);
  #ifndef AB_DEVKIT
    tunes.initChannel(PIN_SPEAKER_2);
  #else
    tunes.initChannel(PIN_SPEAKER_1);
    tunes.toneMutesScore(true);
  #endif

  EEPROM.get(SAVELOCATION, maxlevel);
  if (maxlevel > 49) {
    maxlevel = 49;
  }
  if (maxlevel < 0) {
    maxlevel = 0;
  }
  levelchecker = maxlevel;
  //maxlevel = 49;
  //levelchecker = 0;
  casting = true;
  Serial.begin(9600);
  arduboy.clear();
}

void loop() {
  if (!(arduboy.nextFrame())) {
    return;
  }

  frame++;
  if (frame > 260) {
    frame = 0;
  }
  if (frame % 2) {
    animation++;
    if (transitionanimation > 0) {
      transitionanimation -= 1;
      temperature = 0;
    }
  }

  if (animation > 7) {
    animation = 0;
  }

  arduboy.clear();

  switch (gamestate) {
    case 0: //Credits Screen
      arduboy.setRGBled(0, 0, 0);
      ardbitmap.drawCompressed(8, 8, me, WHITE, ALIGN_NONE, MIRROR_NONE);
      arduboy.setCursor(75, 8);
      arduboy.print(F("Jonathan"));
      arduboy.setCursor(87, 18);
      arduboy.print(F("Holmes"));
      arduboy.setCursor(86, 38);
      arduboy.print(F("@crait"));
      arduboy.setCursor(68, 49);
      arduboy.print(F("crait.net"));
      continuestate(1);
      break;
    case 1: //Title Screen
      drawtitle();
      continuestate(2);
      break;
    case 2: //Main Menu
      frame++;
      drawtitle();
      arduboy.fillRect(64, 0, 64, 64, BLACK);
      arduboy.fillRect(51, 1, 12, 17, WHITE);
      if (levelchecker >= 50 || levelchecker <= -6) {
        levelchecker = 0;
      }
      if (arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(UP_BUTTON)) {
        menubuttoncounter = 0;
      }
      if (menubuttoncounter > 10) {
        menubuttonfree = true;
      }
      if (arduboy.pressed(DOWN_BUTTON)) {
        menubuttoncounter++;
        if (menubuttonfree && levelchecker != 49) {
          menubuttonfree = false;
          levelchecker += 1;
          sound(NOISEMENU, 100);
        }
      }
      if (arduboy.pressed(UP_BUTTON)) {
        menubuttoncounter++;
        if (menubuttonfree && levelchecker != -5) {
          menubuttonfree = false;
          levelchecker -= 1;
          sound(NOISEMENU, 100);
        }
      }
      if (arduboy.pressed(A_BUTTON) && menubuttonfree) {
        sound(NOISEMENUCLICK, 100);
        menubuttonfree = false;
        if (levelchecker == -5) {
          gamestate = 4;
          maxlevel = 0;
          EEPROM.put(SAVELOCATION, maxlevel);
        } else if (levelchecker == -4) {
          casting = !casting;
        } else if (levelchecker == -3) {
          if (arduboy.audio.enabled()) {
            arduboy.audio.off();
          } else {
            arduboy.audio.on();
          }
          arduboy.audio.saveOnOff();
        } else if (levelchecker == -2) {
          gamestate = 0;
        } else if (levelchecker == -1) {
          gamestate = 3;
        } else {
          if (levelchecker <= maxlevel) {
            gamestate = 5;
            loadlevel(levelchecker);
          }
        }
      }
      
      printmenuitem(levelchecker - 3, 0);
      printmenuitem(levelchecker - 2, 8);
      printmenuitem(levelchecker - 1, 16);
      printmenuitem(levelchecker, 28);
      printmenuitem(levelchecker + 1, 40);
      printmenuitem(levelchecker + 2, 48);
      printmenuitem(levelchecker + 3, 56);

      arduboy.fillRect(56, 18, 8, 20, WHITE);
      
      ardbitmap.drawBitmap(58, 24, bubble, 8, 16, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(58, 24, bubbleoutline, 8, 16, BLACK, ALIGN_NONE, MIRROR_NONE);     
      
      drawgrid(64, 0, 64, 16);
      drawgrid(64, 48, 64, 16);

      arduboy.drawLine(64, 26, 128, 26, WHITE);
      arduboy.drawLine(64, 25, 128, 25, WHITE);
      arduboy.drawLine(64, 38, 128, 38, WHITE);
      arduboy.drawLine(64, 37, 128, 37, WHITE);

      arduboy.drawLine(127, levelchecker + 5, 127, levelchecker + 11, WHITE);
      arduboy.drawLine(126, levelchecker + 5, 126, levelchecker + 11, BLACK);

      if(levelchecker != -5) {
        ardbitmap.drawBitmap(92, 2, arrowupoutline, 9, 8, BLACK, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(92, 2, arrowup, 9, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      }
      if(levelchecker != 49) {
        ardbitmap.drawBitmap(92, 54, arrowdownoutline, 9, 8, BLACK, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(92, 54, arrowdown, 9, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      }
      
      break;
    case 3: //Controls
      arduboy.setCursor(0, 0);
      arduboy.print(F("       Controls"));
      arduboy.setCursor(0, 18);
      arduboy.print(F(" D-Pad: Move Around"));
      arduboy.setCursor(0, 28);
      arduboy.print(F("Hold A: Restart Level"));
      arduboy.setCursor(0, 38);
      arduboy.print(F(" A + B: Level Select"));
      drawcorners();
      continuestate(31);
      break;
    case 31:  //Instruction Screen
      arduboy.setCursor(0, 0);
      arduboy.print(F("     Instructions"));
      arduboy.setCursor(0, 18);
      arduboy.print(F("    Find all sockets"));
      arduboy.setCursor(0, 28);
      arduboy.print(F("    Insert chips"));
      arduboy.setCursor(0, 38);
      arduboy.print(F("    Proceed to exit"));
      ardbitmap.drawBitmap(6, 18, spritesheet[CHIPO], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(6, 28, spritesheet[CHIPI], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(6, 38, spritesheet[EXITS + animation % 4], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      drawcorners();
      continuestate(32);
      break;
    case 32: //Instructions 2
      arduboy.setCursor(0, 0);
      arduboy.print(F("     Instructions"));
      arduboy.setCursor(0, 18);
      arduboy.print(F("    Insert CPU's"));
      arduboy.setCursor(0, 28);
      arduboy.print(F("    Ride on conveyor"));
      arduboy.setCursor(0, 38);
      arduboy.print(F("    belts"));
      ardbitmap.drawBitmap(6, 28, spritesheet[CONRI + animation / 2 * 4], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(6, 18, spritesheet[animation / 4 ? CPUOU : CPUIN], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      drawcorners();
      continuestate(33);
      break;
    case 33: //Instructions 3
      arduboy.setCursor(0, 0);
      arduboy.print(F("     Instructions"));
      arduboy.setCursor(0, 18);
      arduboy.print(F("    Step on button to"));
      arduboy.setCursor(0, 28);
      arduboy.print(F("    raise/lower walls"));
      ardbitmap.drawBitmap(6, 18, spritesheet[HEATB], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(6, 28, spritesheet[animation / 4 ? HEATO : HEATI], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      drawcorners();
      continuestate(34);
      break;
    case 34: //Instructions 3
      arduboy.setCursor(0, 0);
      arduboy.print(F("     Instructions"));
      arduboy.setCursor(0, 18);
      arduboy.print(F("    Rotating tunnel"));
      arduboy.setCursor(0, 28);
      arduboy.print(F("    Rotating corners"));
      arduboy.setCursor(0, 38);
      arduboy.print(F("    Rotation button"));
      ardbitmap.drawBitmap(6, 18, spritesheet[(frame / 26) % 2 ? VTROT : HTROT], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(6, 28, spritesheet[TLROT + (frame / 8) % 4], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(6, 38, spritesheet[ROTAB], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
      drawcorners();
      continuestate(2);
      break;
    case 4: //Erased
      arduboy.setCursor(0, 0);
      arduboy.print(F("       Complete\n\n"));
      arduboy.print(F("All saved data was\n"));
      arduboy.print(F("erased..."));
      continuestate(2);
      drawcorners();
      break;
    case 5: //Game
      ardbitmap.drawBitmap(112, 0, side, 16, 64, WHITE, ALIGN_NONE, MIRROR_NONE);
      ardbitmap.drawBitmap(112, 56, signals[animation / 2], 16, 8,  WHITE, ALIGN_NONE, MIRROR_NONE);
      if (level >= 9) {
        arduboy.setCursor(114, 29);
      } else {
        arduboy.setCursor(117, 29);
      }
      arduboy.print((int)(level + 1));
      drawlevel();
      if (transitionanimation == 0 && gamebuttonfree) {
        char index = playerx / 8 + playery / 8 * 14;
        if (gamebuttonfree && arduboy.pressed(UP_BUTTON) && checkcollision(playerx / 8, (playery - 8) / 8) && currentmap[index] != HTROT && currentmap[index] != TLROT && currentmap[index] != TRROT) {
          rotateinplace(index);
          playertoy -= 8;
          transitionanimation = 9;
          canswitch = true;
          gamebuttonfree = false;
        }
        if (gamebuttonfree && arduboy.pressed(DOWN_BUTTON) && checkcollision(playerx / 8, (playery + 8) / 8) && currentmap[index] != HTROT && currentmap[index] != BLROT && currentmap[index] != BRROT) {
          rotateinplace(index);
          playertoy += 8;
          transitionanimation = 9;
          canswitch = true;
          gamebuttonfree = false;
        }
        if (gamebuttonfree && arduboy.pressed(LEFT_BUTTON) && checkcollision((playerx - 8) / 8, playery / 8) && currentmap[index] != VTROT && currentmap[index] != TLROT && currentmap[index] != BLROT) {
          rotateinplace(index);
          playertox -= 8;
          transitionanimation = 9;
          canswitch = true;
          gamebuttonfree = false;
        }
        if (gamebuttonfree && arduboy.pressed(RIGHT_BUTTON) && checkcollision((playerx + 8) / 8, playery / 8) && currentmap[index] != VTROT && currentmap[index] != TRROT && currentmap[index] != BRROT) {
          rotateinplace(index);
          playertox += 8;
          transitionanimation = 9;
          canswitch = true;
          gamebuttonfree = false;
        }
      } else {
        menubuttonfree = false;
      }

      if (playery > playertoy) {
        playery -= 1;
      }
      if (playery < playertoy) {
        playery += 1;
      }
      if (playerx > playertox) {
        playerx -= 1;
      }
      if (playerx < playertox) {
        playerx += 1;
      }
      if (playery == playertoy && playerx == playertox) {
        checklocation(playerx / 8, playery / 8);
        gamebuttonfree = true;
      } else {
        arduboy.drawPixel(playertox, playertoy, WHITE);
        arduboy.drawPixel(playertox + 7, playertoy, WHITE);
        arduboy.drawPixel(playertox, playertoy + 7, WHITE);
        arduboy.drawPixel(playertox + 7, playertoy + 7, WHITE);
      }
      if (arduboy.pressed(A_BUTTON) && menubuttonfree && gamebuttonfree) {
        sound(100 + temperature * 10, 100);
        temperature += 1;
      }

      if (arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON) && gamebuttonfree && menubuttonfree) {
        gamestate = 2;
        gamebuttonfree = false;
        menubuttonfree = false;
        break;
      }

      if (temperature > 0) {
        arduboy.setRGBled(temperature * 7, 0, 0);
        arduboy.fillRect(112, 0, 16, 40, BLACK);
        ardbitmap.drawBitmap(112, 0, temptop, 16, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(112, 8, tempmiddle, 16, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(112, 16, tempmiddle, 16, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(112, 24, tempbottom, 16, 16, WHITE, ALIGN_NONE, MIRROR_NONE);
        arduboy.fillRect(119, 37 - temperature, 2, temperature, WHITE);
      }
      if (temperature == 32) {
        deathcounter = 1;
        temperature = 33;
      }

      checkcompletion();

      if (deathcounter == 0) {
        ardbitmap.drawBitmap(playerx, playery, character[0], 8, 8, WHITE, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(playerx, playery, character[1], 8, 8, BLACK, ALIGN_NONE, MIRROR_NONE);
      } else {
        //digitalWrite(RED_LED, ON);
        //digitalWrite(GREEN_LED, OFF);
        arduboy.setRGBled(255, 0, 0);
        if ((deathcounter - 1) / 8 == 0) {
          sound(NOISEEXPLOSION1, 100);
        }
        if ((deathcounter - 1) / 8 == 1) {
          sound(NOISEEXPLOSION2, 100);
        }
        if ((deathcounter - 1) / 8 == 2) {
          sound(NOISEEXPLOSION3, 100);
        }
        if ((deathcounter - 1) / 8 == 3) {
          sound(NOISEEXPLOSION4, 100);
        }
        gamebuttonfree = false;
        temperature = 33;
        deathcounter += 1;
        ardbitmap.drawBitmap(playerx, playery, character[1], 8, 8, BLACK, ALIGN_NONE, MIRROR_NONE);
        ardbitmap.drawBitmap(playerx - 2, playery - 2, deathsheet[(deathcounter - 1) / 8], 12, 12, WHITE, ALIGN_NONE, MIRROR_NONE);
        if (deathcounter == 32) {
          loadlevel(level);
          menubuttonfree = false;
        }
      }
      break;




    case 6:
      arduboy.setCursor(0, 12);
      arduboy.print(F("And now...\nYou will finally see\nwhy Circuit Dude has\nbeen building these\ncomputer chips..."));
      drawcorners();
      continuestate(7);
      break;
    case 7:
      ardbitmap.drawCompressed(0, 0, final1, WHITE, ALIGN_NONE, MIRROR_NONE);
      continuestate(8);
      break;
    case 8:
      ardbitmap.drawCompressed(0, 0, final2, WHITE, ALIGN_NONE, MIRROR_NONE);
      continuestate(9);
      break;
    case 9:
      ardbitmap.drawCompressed(0, 0, final2, WHITE, ALIGN_NONE, MIRROR_NONE);
      arduboy.fillRect(10, 36, 64, 28, WHITE);
      ardbitmap.drawCompressed(10, 36, final3, BLACK, ALIGN_NONE, MIRROR_NONE);
      continuestate(10);
      break;
    case 10:
      ardbitmap.drawCompressed(0, 0, final4, WHITE, ALIGN_NONE, MIRROR_NONE);
      continuestate(11);
      break;
    case 11:
      arduboy.setRGBled(255, 0, 0);
      ardbitmap.drawCompressed(0, 0, final5, WHITE, ALIGN_NONE, MIRROR_NONE);
      continuestate(12);
      break;
    case 12:
      arduboy.setCursor(35, 30);
      arduboy.print(F("The End..."));
      drawcorners();
      continuestate(0);
      break;
  }

  if (arduboy.notPressed(A_BUTTON) && gamestate != 2) {
    menubuttonfree = true;
    temperature = 0;
  }
  if (arduboy.notPressed(DOWN_BUTTON) && arduboy.notPressed(UP_BUTTON) && arduboy.notPressed(A_BUTTON) && gamestate == 2) {
    menubuttonfree = true;
  }
  if(casting) {
    Serial.write(arduboy.getBuffer(), 128 * 64 / 8);
  }
  arduboy.display();
}
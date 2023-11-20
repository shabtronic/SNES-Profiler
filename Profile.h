#ifndef SNES_PROFILE_INCLUDE
#define SNES_PROFILE_INCLUDE

// SNES has 262 scanlines per frame
// only 224 are visibile in ntsc mode
// so this profiler will only show u the cpu usage on visible scanlines
// so no profiling inside VBLANK
// 
// should be almost non-intrusive
// since it's meant to work on main loop outer functions
// and it's only 2 ppu register writes per "profile"
//
// Could probably use a single colour tile in BG3 and make a vertical bar
// and then change that tiles single colour palette to get the same system 
// instead of using those pesky windows

/* usage: 

#include "Profile.h"

InitProfiler();

while (1)
	{
	// it's best to wait until scanline zero somehow
	// before using the profiler!

	ProfileOn();

	ProfileColour(profRED);
	mapUpdate();

	ProfileColour(profWHITE);
	SomeThing();

	ProfileColour(profBLUE);
	SomeOtherThing();

	ProfileOff();
	
	WaitForVBlank();
	}


*/


// comment out to disable the profiler code

#define PROFILE_ENABLE

#define profMAGENTA		0b10111111
#define profCYAN		0b11011111
#define profYELLOW		0b01111111
#define profOLIVE		0b01101111
#define profBLUE		0b10011111
#define profDARKBLUE	0b10001111
#define profGREEN		0b01011111
#define profDARKGREEN	0b01001111
#define profRED			0b00111111
#define profDARKRED		0b00101111
#define profWHITE		0b11111111
#define profGREY		0b11100111
#define profBLACK		0b11100000

#define W1LEFT		(*(vuint8 *)0x2126)
#define W1RIGHT		(*(vuint8 *)0x2127)
#define W2LEFT		(*(vuint8 *)0x2128)
#define W2RIGHT		(*(vuint8 *)0x2129)
#define MWEnable	(*(vuint8 *)0x212E) //...O4321 Main screen window enable
#define SWEnable	(*(vuint8 *)0x212F) //...O4321 Sub  screen window enable
#define COLDATA		(*(vuint8 *)0x2132) // RGBCCCCC
#define CGWSEL		(*(vuint8 *)0x2130)
#define WOBJSEL		(*(vuint8 *)0x2125)
#define WOBJLOG1	(*(vuint8 *)0x212A)
#define WOBJLOG2	(*(vuint8 *)0x212B)
#define CGADSUB 	(*(vuint8 *)0x2131) 


#ifdef PROFILE_ENABLE
// Turn on the profile display
#define ProfileOn() { WOBJSEL = 0b10001000; }
 // Turn off the profile display
#define ProfilerOff() { WOBJSEL = 0; }
 // Set the profile bar colour - gotta clear all colour planes before setting the colours u want
#define ProfileColour(pfc) { COLDATA=0b11100000; COLDATA=pfc; }

void InitProfiler() // init Windows - turn off win1 - use win2 for profile bars
{
	// This is probably incorrect, not everything setup correctly or incorrect stuff 
	MWEnable = 0;
	SWEnable = 0;
	// Enable window 2 colour math
	WOBJSEL = 0b10000000; // Enable color math for window 2
	// Set window mask logic
	WOBJLOG1 = 0;
	WOBJLOG2 = 0;
	// Colour math clip colours
	CGWSEL = 0b10010000;
	// Colour math addition
	CGADSUB = 0b00111111;
	// Hide window1
	W1LEFT = 255;
	W1RIGHT = 254;
	// Put the profile bar at x screen pos 8 to 12
	W2LEFT = 8;
	W2RIGHT = 12;

}
#else
#define ProfileOn()
// Turn off the profile display
#define ProfilerOff()
 // Set the profile bar colour
#define ProfileColour(pfc)

void InitProfiler()
{
}
#endif

#endif
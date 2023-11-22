#ifndef SNES_PROFILE_INCLUDE
#define SNES_PROFILE_INCLUDE

#include <snes/snestypes.h>
#include <snes/background.h>
#include <snes/video.h>


// comment out to disable the profiler code
#define PROFILE_ENABLE
#define PROFILE_NTSC
// PAL Timings not implemented yet
// #define PROFILE_PAL

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
	// This will wait til scanline 261 (0)
	StartProfiler(); 
	

	ProfileColour(profRED);
	mapUpdate();

	ProfileColour(profWHITE);
	SomeThing();

	ProfileColour(profBLUE);
	SomeOtherThing();

	ProfileOff();

	VBlankProfile(); // Optional if you want to profile VBlank code
	WaitForVBlank();
	}


*/

// need to convert this to asm since it's execution time affects the timing results
u16 ReadVCounter()
{
	u16 res = (*(vuint8*)0x2137); // latch h/vcounters into 213C and 213D - this value is discarded 
	res = ((*(vuint8*)0x213D)); // read vcounter - 2 reads - 9 bits in total, top 7 bits are open bus
	res = res | ((((u16)(*(vuint8*)0x213D))&1)<<8); // hibyte is 1 bit so mask it out
	return res;
}

void WaitTilScanline(u16 sl) // sl = scanline to wait til
{
	u8 res = REG_STAT78; // reset latch hi/low selector
	(*(vuint8*)0x4201) = 128; // enable h/vcounters to latch
	while (1)
		if (ReadVCounter() == sl) return;
}

void WaitTilScanlineGE(u16 sl) // sl = scanline to wait til
{
	u8 res = REG_STAT78; // reset latch hi/low selector
	(*(vuint8*)0x4201) = 128; // enable h/vcounters to latch
	while (1)
		if (ReadVCounter() >= sl) return;
}

#ifdef PROFILE_ENABLE
// Turn on the profile display
#define ProfileOn() { REG_WOBJSEL = 0b10001000; }
 // Turn off the profile display
#define ProfilerOff() { REG_WOBJSEL = 0; }
 // Set the profile bar colour - gotta clear all colour planes before setting the colours u want
#define ProfileColour(pfc) { REG_COLDATA=0b11100000; REG_COLDATA=pfc; }

static u16 PreviousVTime = 0;

// NTSC vblank = 224 to 262 - so 38 scanlines 0 to 37 = 0 to 100% or 0 to 255 pixels on the display
// so roughly 7 times vblank scanline time will give us a pixel width profile time to display!

void StartProfiler()
{
	PreviousVTime = ReadVCounter(); // should  be 224 or greater if everything is running under 1 frame!
	REG_WH2 = 8;
	REG_WH3 = 12;
	WaitTilScanline(261); // NTSC line 262 is actually line 0 since there's a 1 scanline delay
	ProfileOn();
}

void VBlankProfile() // This must be the last thing before WaitForVBlank(); in the main loop 
{
	// ToDo: PreviousVTime should be 224(NTSC) when nothing is happening in VBlank
	// for some reason it's always way above that - I have no idea why!
	// possible causes:
	//		ReadVCounter() code too bloaty
	//		Emulator inaccuracy?
	//		Hidden Interuppt code ?

	// NTSC VBLANK is from scanlines 224 thru to 262 - so 38 scanlines in total x8 to get close to 256 pixels

	u16 res = ((PreviousVTime - 223)<<3);
	
	ProfileColour(profYELLOW); // Set the VBlank profile colour
	// Set the bar width and position
	REG_WH2 = 0;
	REG_WH3 = res;
	WaitTilScanlineGE(220); // start drawing the vblank profile bar at line 220 thru to 224
	ProfileOn();	
}
void InitProfiler() // init Windows - turn off win1 - use win2 for profile bars
{
	// This is probably incorrect, not everything setup correctly or incorrect stuff 
	REG_TMW = 0;
	REG_TSW = 0;
	// Enable window 2 colour math
	REG_WOBJSEL = 0b10000000; // Enable color math for window 2
	// Set window mask logic
	REG_WBGLOG = 0;
	REG_WOBJLOG = 0;
	// Colour math clip colours
	REG_CGWSEL = 0b10010000;
	// Colour math addition
	REG_CGADSUB = 0b00111111;
	// Hide window1
	REG_WH0 = 255;
	REG_WH1 = 254;
	// Put the profile bar at x screen pos 8 to 12
	REG_WH2 = 8;
	REG_WH3 = 12;

}
#else
#define ProfileOn()
// Turn off the profile display
#define ProfilerOff()
 // Set the profile bar colour
#define ProfileColour(pfc)
#define InitProfiler() 
#define StartProfiler() 
#define VBlankProfile() 


#endif

#endif

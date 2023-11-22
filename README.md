# SNES-Profiler
A very simple profiler for SNES code

Written for use with the fantastic PVSNESLIB:  https://github.com/alekmaul/pvsneslib

**How it works:**

This uses window 2 to draw the bars at X positions 8 thru to 12 using the REG_COLDATA register. ProfileColour just sets the REG_COLDATA to whatever you like and that reflects on screen at the current scanline.

The VBlank profiler - works by getting the current scanline using the ReadVCounter() after the WaitForVBlank() function. And thus you can calculate how many scanlines any VBlank has taken.
Window2 is then used to draw a yellow profile bar at ypos 220 to 224, with the width representing how much code time was used in VBlank.

All of this relies on two things:

1) Mode 1 is being used
2) Frame time is alway less than 60fps - if you go over a frame, it'll probably draw a horrible mess and slow your game down since there's a waitforscanline function being used.


**Usage:**

This is the code from the ProfileNTSC.sfc example

```
	InitProfiler();
	setScreenOn();

	while (1)
	{
		StartProfiler();
		ProfileColour(profOLIVE);
		CameraUpdate();

		ProfileColour(profBLUE);
		ShipUpdate(0);

		ProfileColour(profGREY);
		oamDynamic32Draw(PlayerShip);
		oamDynamic16Draw(TestSprite);

		ProfileColour(profDARKBLUE);
    		SpritePathFollow();    
		bgSetScroll(1, CameraXPos >> 6, 0);

		ProfileColour(profRED);
		mapUpdate();

		ProfileColour(profGREY);
		oamInitDynamicSpriteEndFrame();
		ProfilerOff();

		VBlankProfile(); // must put this here, profile any vblank cpu usage
		WaitForVBlank();

		oamVramQueueUpdate(); // uploads any sprite tiles needed
		mapVblank(); // Draws the map
	}
```

**Example in game:**

The bar colours are from the code above. The yellow bar is the VBlank code usage - where full screen width equals all VBlank cpu time. (it's not very accurate tho)

![](./Images/ProfileNTSC_Animated.gif)

ToDo:
Audio profiling? - ugh lol!
~~Need a simple system to profile code in VBlank and display that.~~ 
~~Also some handy "WaitTilScanlineZero" code would be great.~~

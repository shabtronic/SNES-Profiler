# SNES-Profiler
A very simple profiler for SNES code

Written for use with the fantastic PVSNESLIB:  https://github.com/alekmaul/pvsneslib

How it works:

usage:
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

		VBlankProfile(); // profile any vblank cpu usage
		WaitForVBlank();

		oamVramQueueUpdate(); // uploads any sprite tiles needed
		mapVblank(); // Draws the map
	}
```

Example in game:

![](./Images/ProfileNTSC.png)

ToDo:

Need a simple system to profile code in VBlank and display that. 
~~Also some handy "WaitTilScanlineZero" code would be great.~~

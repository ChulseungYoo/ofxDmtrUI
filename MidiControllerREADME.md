# ofxDmtrUIMidiController

This is very sketchy yet, so use it in your own risk.  
Specification is changing fast, ignore usage example.  
Tested in AKAI APC20, AKAI MPD32, AKAI APC40 MKII, Novation Launch Control XL.  

## Dependencies
ofxMidi, ofxDmtrUI  

## Roadmap
Possibility of clicking in ofxDmtrUI element and “learning commands” from Midi Controller (Like Ableton Live).  
Actually it is half implemented.

## Usage (NOT VALID ANYMORE)
You create a text file with your midi device name.txt, ex: 
```
Launch Control XL.txt
```

And the contents of the file are something like this:
```
176 9 0 13	uiC	float	camX
176 9 0 14	uiC	float	camY
176 9 0 15	uiC	float	camZ
```

When controls are not assigned you will see the exact address in console, and you can use the same format for the address.
```
176 9 0 13
which means
[status][channel][pitch][control]
```


## 2016 05 25
Updated for The Year club software, Areia and Illusion.  
Now the text file is the same name of the device.  

## 2016 05 21
Created for AREIA performance, AVAV Virada Cultural.  
Used existing code from Mareh Marisco festival former in ofxLicht. now a separate addon.  

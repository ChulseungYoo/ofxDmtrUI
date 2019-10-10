# [DISCONTINUED] use ofxMicroUI instead
# https://github.com/dimitre/ofxMicroUI

———
  
Made to be really simple and lightweight.  
It is my first public openFrameworks addon.  
Requires oF 0.9+   
Suggestions and code improvements will be greatly appreciated.   
Use by your own risk. It is working well here.  
Tested on OS X. Let me know if you are using it. Examples will be created under demand   


![Example](http://hifolio.com/media/1/Screen_Shot_2016-04-15_at_12.45.11_PM.png)



## Features
Load interface layouts from text files.
See section Text File Format in this README for more info
Info is not up to date.

## Motivation
A C++ exercise for me  
first open source project, hope to get collaboration from skilled programmers  
lead an interface project from my own needs  
a way of quickly design a prototype interface and spend time in what matters  
a personal substitute for the beloved and deprecated ofxUI  
An UI that is not constantly redrawing on screen. each time one element is refreshed the screen is saved into an FBO.  
CPU usage is very low. 

## Dependencies
ofxXmlSettings  

# TODO
./Different events for loading xml files and updating with the mouse.
add namespace to classes.  
test events in IOs
./Change some variables named in portuguese  
//integration with ofxMidi for midi controllers use  
//radio buttons rectangles according to width.  
//radio images for presets.
//port to lib cinder  

# Future Ideas
Performance : maybe check inside panel rectangle if mouse event is inside before check in each panel element.
Implement the class Element and make pointers to member functions of draw, check mouse, get val etc.

## 2016 09 27
Improvements. Software mode which handles automatically presets, multiple columns, presets folders. More documentation to follow.
UI used in lots of differents projects now:
http://dmtr.org/nikecube/
Paralympics Rio Opening Ceremony
Paralympics Rio Closing Ceremony

## 2016 07 27
I’m not logging a lot of improvements here.
Now we have radioText which is a radio that loads the options from a text file, separated by new lines.

## 2016 07 03
Major overhaul on xml saving / loading. it will recognize the oldstyle xml files and save in 2.0 version automatically for a while. Let me know if you are already using presets, the conversion process will be stripped out in some weeks.  

## 2016 06 27
New parameter opacity and opacityRest (UI without mouse over)
Lots of improvements not logged here.

## 2016 05 25
added saveX saveY and restoreX restoreY  
updating software for The Year club.  
upgrading ofxDmtrUIMidiController.  

## 2016 05 20
new addon to interface this addon with midi controller
https://github.com/dimitre/ofxDmtrUIMidiController

## 2016 05 20
function clear added. clear all sliders and variables, reset flow.x e flow.y to be ready to build other kinds of elements.

## 2016 05 13
Possibility of coloring down some radio options. Made for Mareh Festival, a visual shortcut to moving heads color wheel options.
Happy, the second commercial project using the interface.

## 2016 05 12
Under demand for Marisco Festival next weekend: Multiple options radio. flowing like a text and saving booleans. saving space too.

## 2016 05 10
Slider2d now working OK, saving and loading
fbo element now possible. just display some FBO in your interface.

In the meantime I’ve written Cinder-DmtrUI as a simple UI for Cinder too. I’ve started by other approach and I like it very much. I’ll try to convert some ideas and concepts from there to here in the near future.

## 2016 05 04
Flow fixes

## 2016 04 29
“margin” affects marginx and marginy
Presets working. load and save presets with thumbnail images.

## 2016 04 27
Slider2d first sketches
Color first sketch

## 2016 04 18
License added. 

## 2016 04 17
toggle matrix elements
```
togglematrix	checkers	5 3
```

## 2016 04 15
Bang! button, only triggering an event  
radio button shortcuts for integers or floats (bang)  
vertical sliders  
create object from a line
Areia, arrays of toggles.
shortcut to float and int sliders

## 2016 04 14
manual for ui text files 
Pointers to pString working on Radio items and DirList.
Pointers to pBool on toggles
Pointers to pInt and pFloat on Sliders
ui.keepSettings = false; ~ a way of automatically loading and saving xml files from ui on setup and exit.  
parameter “auto fit” for dimensioning rectangles.
pass pointer reference of pFloat, pBool to update functions of classes.    

## 2016 04 12
radio buttons flowing correctly
dirtiest working too

## 2016 04 08
radio buttons first draft.

## 2016 04 07
slider integer working OK

## 2016 04 06  
multiple columns now possible
interface building from text file
min max and def working on sliders.

## 2016 04 05  
possibility of creating multiple columns  
Solved drag to outside slider  
ofClamp  
github first draft  
example working inside addon folder  
addon_config.mk added  
ready added.  

## 2016 04 04  
primeiro rascunho da primeira versao do ofxDmtrUI  
toggle, toggle drag event  
package inside an addon and bind events
load save presets in xml
objects containers of panels. 
os eventos de mouse só entram em cada painel primeiramente.  
talvez fazer subcolunas tb.




## Text File Format
Text files are a custom format like TSV <tab separated values>
```
label	ofxDmtrUI
label	Code Dmtr.org 2016

marginy	100
label	First Column
float	r	0 255 127
float	g	0 255 127
float	b	0 255 127
radio	speed2shortcut	1 2 4 8

float	DivNoiseX	0 1 .2
float	DivNoiseY	0 1 .2
bool	toggle1
bool	toggle2
bool	toggle3
bool	ok	1
bool	ok2	1
```

Setup values, values that doesn’t display anything but changes the variables in UI instance.
rect<tab>x y width height
```
rect	240 20 840 940
```
defines the interface rectangle, useful when using more than one UI instance, distributing rectangles on the screen.
```
marginx	20
marginy	280
sliderWidth	240
```
changes the rectangle margins and redefine sliderWidth
```
label	Fita 1
float	posicao1	0 1 0
int	divisao1	1 8 1
float	pan1	-1 1 -.5
float	vol1	0 1 1
```
create a label “Fita 1”
a float variable called “posicao1” with min = 0, max = 1, def = 0
```
newcol
label	Fita 2
float	posicao2	0 1 0
```
newcol jumps to a new column to add more widgets.

```
radio	speed2shortcut	1 2 4 8
```

Radio create a radio with n parameters separated by space. useful to list numbers or words in the software
accessible in the map ui.pString[name]


```
dirlist	fita1	/Users/dd/Desktop/WORK_DMTR/AREIA_AUDIO
```
dirlist creates a radio with files listed in a specific directory, relative or absolute path.



  

  
    
_ ._. ._ _. ... .._ ._. .._. .. _. __.  
Dimitre Lima 2016  
Lunar Calendar 2017 now shipping  
http://dmtr.org/  
http://dimitre.org/  
http://dimitrelima.com/  
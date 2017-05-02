/**********************************************************************************

 Copyright (C) 2016 Dimitre Lima (www.dmtr.org)

 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

 **********************************************************************************/


//#define NOXML

#pragma once

#include "ofMain.h"
#include "ofEvents.h"
#include "ofxXmlSettings.h"
#include "ofxDmtrUIElements.h"

#if defined( TARGET_OF_IPHONE ) || defined( TARGET_OF_IOS ) || defined( TARGET_ANDROID )
	#define DMTRUI_TARGET_TOUCH
#endif


class ofxDmtrUI : public ofBaseApp
{
public:
	void setup();
	void keyPressed(int key);
	void keyReleased(int key);
	void update();
	void draw();
	void exit();
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseAll(int x, int y, int button);
	void mousePressedDragged(int x, int y, int button);

	void onDraw(ofEventArgs &data);
	void onUpdate(ofEventArgs &data);
	void onKeyPressed(ofKeyEventArgs &data);
	void onKeyReleased(ofKeyEventArgs &data);
	void onMousePressed(ofMouseEventArgs &data);
	void onMouseDragged(ofMouseEventArgs &data);
	void onMouseReleased(ofMouseEventArgs &data);
	void onMouseMoved(ofMouseEventArgs &data);

#ifdef DMTRUI_TARGET_TOUCH
	void onTouchDown(ofTouchEventArgs &data);
	void onTouchMoved(ofTouchEventArgs &data);
	void onTouchUp(ofTouchEventArgs &data);
#endif

	void onExit(ofEventArgs &data);

	void createFromText(string file);
	void createFromLine(string line);
	void createSoftwareFromText(string file);

	// rename to createelement
	void create(string nome, string tipo="slider", string valores = "", string valores2 = "", string valores3 = ""); // NULL

	void createRadio(string nome, vector <string> opcoes);

	void save(string xml);
	void load(string xml);
	void expires(int dataInicial, int dias = 10);
	void uiEvents(string & e);
	void uiEventsNeu(dmtrUIEvent & e);
	void autoFit(bool w = true, bool h = true);

	void setFloat(string nome, float val);
	void setBool(string nome, bool val);
	void setRadio(string nome, string val);

	void loadPreset(int n);
	void loadPresetAll(int n);
	void loadNextPresetAll();
	void savePreset(int n);
	void savePresetAll(int n);

	void erasePresetAll(int n);
	// to save presets
	void setFbo(ofFbo &fbo);


	void setFboElement(string nome, ofFbo &fbo);

	vector <string> textToVector(string file);
	vector <string> textToVectorPipe(string file);

	vector <slider> 	sliders;
	vector <toggle> 	toggles;
	vector <label> 	labels;
	vector <radio> 	radios;
	vector <slider2d> sliders2d;
	vector <inspector> inspectors;

	// WOW
	vector <element> elements;

	map <string,float>			pEasy;
	map <string,float>			pFloat;
	map <string,int>				pInt;
	map <string,bool>			pBool;
	map <string,string>			pString;
	map <string,string>			pLabel;
	map <string,ofPoint>			pPoint;
	//map <string,ofFloatColor>	pColor;
	map <string,ofColor>			pColor;

	// NOVO
	map <string,string>			pFolder;

	// 26 jul 2016, Nike
	map <string,string>			pInspector;
	// 10 setembro 2016, DialogosSonoros. temporario?
	map <string,float>			pInspectorFloat;

	// only internal use to backup some variables.
	map <string,float>			pFloatBak;



	ofEvent<string> uiEvent;
	ofEvent<dmtrUIEvent> evento;
	ofEvent<eventoNovo> eventoNovo;

	ofEvent<vector <string> > uiEventMidi;
	

	// Fbo to draw GUI column
	ofFbo fboColumn;
	float easing = 5;
	bool showGui = true;
	bool redraw = true;
	bool columnOver = false;

	// Layout, default
	bool bw = false;
	float marginx = 10;
	float marginy = 10;
	int sliderHeight = 18;
	int sliderWidth  = 200;
	int	sliderMargin = 2;
	// precisa?
	int	lastHeight, lastWidth;
	
	ofPoint flow = ofPoint(marginx, marginy);
	flowDir flowDirection = VERT;

	bool	 saveLoadShortcut = false;
	bool keepSettings = false;
	bool useShortcut = false;
	string presetsFolder = "_presets/";
	string presetsFolderNumber = "";
	string UINAME = "";

	int hueStart = 100;
	bool learnMode = false;
	string lastLearn = "";

	float opacity = 255;
	float opacityRest = 255;
	bool dirListEntireName = false;

	ofColor colunaBackground = ofColor(0,100);
	ofRectangle coluna = ofRectangle(0,0,220,220);
	// 16 x 9 seria 100x56, vou deixar o padrao maior, tipo 35 por enquanto
	ofPoint presetDimensions = ofPoint(100,35);

	// ainda ver direitiho se isso vai rolar.
	map <string, map <string, string> > dirListMap;

	presets allPresets, allPresets2;

	int presetLoaded;

	void re();
	float getNoise(string nome, float a);
	void clear(bool keepVars = false);
	void addAllListeners();
	string getPresetsFolder();

	ofBlendMode blendMode = OF_BLENDMODE_ALPHA;

	string createdFromTextFile = "";

	//5 de agosto de 2016, testando 
	string mousePressedElement = "";
	bool slideFree = true;

	void nextTo(ofxDmtrUI & uiNext);
	void downTo(ofxDmtrUI & uiNext);

	string getFileFullPath(string & nome);

	string allText = "";

	map <string, ofFbo> mapFbos;
	map <string, toggle * > togglesMap;

	map <string, int> togglesIndex;
	map <string, int> radiosIndex;
	map <string, int> slidersIndex;
	map <string, int> sliders2dIndex;

	// quase bom
	slider   * getSlider(string nome);
	radio    * getRadio(string nome);
	toggle   * getToggle(string nome);
	slider2d * getSlider2d(string nome);


	map <string, string> radioUIMap;


	//ofxDmtrUI * uiC = NULL;
	//ofxDmtrUI * uiM = NULL;
	// teste. limpar depois?
	ofxDmtrUI * _uiBak = NULL;
	
	ofxDmtrUI *_uiUnder = NULL;
	ofxDmtrUI *_uiRight = NULL;
	ofxDmtrUI *_uiLast = NULL;
	ofxDmtrUI *_uiFather = NULL;

	map <string, ofxDmtrUI> uis;

	// ponteiro para todas, talvez aqui eu possa atrelar os listeners
	vector <ofxDmtrUI *> allUIs;

	int hue;

	// 27 de setembro. convertendo em software agora
	// 11 de outubro, ja removendo aqui e convertendo pra mapFbos


	// fbo pointer to save presets.
	ofFbo *_fbo;
	
	ofFbo fbo;
	ofFbo fboTrails, fboFade;
	bool debug = false;

	float flowXhuefactor = 0.125;
	float flowYhuefactor = 0.17;

	vector<string> colors;
	int multiSampling = 0;

	string futureLine;
    vector <int> nextPreset;

	int minimumWidth = 100;

	//dmtrUIEvent sendEvent(string & nome, varType var, elementType element, eventoType tipo);

	element * getElement(string & nome, elementType element);

	ofColor getCor(float a=0, string nome="cor");
};

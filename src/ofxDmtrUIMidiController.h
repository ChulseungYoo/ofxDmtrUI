/*
ofxDmtrUIMidiController 
 Created by Dimitre Lima on 18/05/16.
 Copyright 2016 Dmtr.org. All rights reserved.

*/

#pragma once
#include "ofMain.h"
#include "ofEvents.h"
#include "ofxDmtrUI.h"
#include "ofxMidi.h"

class elementListMidiController {
public:
	string ui;
	string tipo;
	string nome;
	string valor;

	int channel;
	int pitch;
};


class ofxDmtrUIMidiController : public ofBaseApp, public ofxMidiListener, public ofxMidiConnectionListener {
public:
	// void setup(string midiDevice);
	// void keyPressed(int key);
	// void keyReleased(int key);
	// void exit();

	// void onKeyPressed(ofKeyEventArgs &data);
	// void onKeyReleased(ofKeyEventArgs &data);
	// void onExit(ofEventArgs &data);

	// void newMidiMessage(ofxMidiMessage& eventArgs);

	// void uiEventMidi(vector <string> & strings);

	// void teste();
	// // midi device (dis)connection event callbacks
	// void midiInputAdded(string name, bool isNetwork);
	// void midiInputRemoved(string name, bool isNetwork);

	// void setUI(ofxDmtrUI &u);

	// void uiEventsNeu(dmtrUIEvent & e);


	// Midicontroller
	ofxMidiIn 	midiControllerIn;
	ofxMidiOut	midiControllerOut;
	ofxMidiMessage midiMessage;
	bool midiKeys[4000];

	ofxDmtrUI * _u = NULL;
	map <int, map <int, map<int, map<int, elementListMidiController> > > > mapaMidiController;

	vector <elementListMidiController *> elements;

	string lastString;

	ofFbo testeFbo;
	ofPixels fboTrailsPixels;
	int testeIndex[4] = { 0,1,3,5 };

	map <string,string>			pString;








	elementListMidiController elementLearn;


	//--------------------------------------------------------------
	void setup(string midiDevice) {
		ofxMidi::setConnectionListener(this);

		testeFbo.allocate(8,5,GL_RGBA);
		fboTrailsPixels.allocate( testeFbo.getWidth(), testeFbo.getHeight(), OF_IMAGE_COLOR_ALPHA);

		testeFbo.begin();
		ofClear(0,255);
		testeFbo.end();

		cout << "ofxDmtrUIMidiController setup ::: " + midiDevice << endl;
		midiControllerIn.openPort(midiDevice);
		midiControllerOut.openPort(midiDevice); // by number
		midiControllerIn.ignoreTypes(false, false, false);
		midiControllerIn.addListener(this);

		string filename = midiDevice + ".txt";

		// isopen - adicionei em 12 de junho de 2016
		if (ofFile::doesFileExist(filename) && midiControllerIn.isOpen()) {

			// 16 de julho de 2016
			// forma que encontrei de associar o primeiro UI da lista e pegar o textToVector dele.
			for (auto & m : _u->textToVector(filename)) {
				if (m != "") {
					elementListMidiController te;
					vector <string> cols = ofSplitString(m, "\t");
					te.ui = 		cols[1];
					te.tipo = 	cols[2];
					te.nome = 	cols[3];

					if (te.tipo == "radio") {
						vector <string> valores = ofSplitString(te.nome, " ");
						te.nome = valores[0];
						te.valor = valores[1];
					}
					//			cout << "ui : "   + te.ui << endl;
					//			cout << "tipo : " + te.tipo << endl;
					//			cout << "nome : " + te.nome << endl;
					vector <string> vals = ofSplitString(cols[0], " ");
					int status = 	ofToInt(vals[0]);
					int channel = 	ofToInt(vals[1]);
					int pitch = 		ofToInt(vals[2]);
					int control = 	ofToInt(vals[3]);
					// mais pra frente fazer namespace aqui.
					te.channel = channel;
					te.pitch = pitch;
					mapaMidiController[status][channel][pitch][control] = te;

					elements.push_back(&mapaMidiController[status][channel][pitch][control]);
				}
			}
		}

		ofAddListener(ofEvents().exit, 			this, &ofxDmtrUIMidiController::onExit);
		ofAddListener(ofEvents().keyPressed, 	this, &ofxDmtrUIMidiController::onKeyPressed);
		ofAddListener(ofEvents().keyReleased,	this, &ofxDmtrUIMidiController::onKeyReleased);

		ofAddListener(_u->evento,this, &ofxDmtrUIMidiController::uiEventsNeu);
	}
	// END SETUP

	//--------------------------------------------------------------
	void teste() {
		testeFbo.begin();
		//ofClear(0,255);
		ofSetColor(0,_u->pInt["trails"]);
		ofDrawRectangle(0,0,100,100);

		float x1 = ofNoise(ofGetFrameNum()/50.0,1) * testeFbo.getWidth();
		float x2 = ofNoise(ofGetFrameNum()/100.0,2) * testeFbo.getWidth();
		float y1 = ofNoise(ofGetFrameNum()/120.0,3) * testeFbo.getHeight();
		float y2 = ofNoise(ofGetFrameNum()/200.0,4) * testeFbo.getHeight();
		ofSetColor(255);
		//ofDrawLine(x1, y1, x2, y2);
		float raio = y1 * 1.0;
		ofDrawEllipse(x1, y1, raio, raio);

		testeFbo.end();

		testeFbo.draw(0,0, testeFbo.getWidth()*10, testeFbo.getHeight()*10);
		testeFbo.readToPixels(fboTrailsPixels);

		int fator = 1;
		int vel = _u->pInt["vel"];

		// APC20
		// verde 1-2
		// laranja 3-4
		// amarelo 5-6 (not blink / blink)

		for (int x=0; x<8; x++) {
			for (int y=0; y<5; y++) {
				int ch = x + 1;
				int pitch = y + 53;
				ofColor cor = fboTrailsPixels.getColor(x * fator, y * fator);
				int vel = testeIndex[int(ofMap(cor.r, 0, 255, 0, 3))];
	//			if ((cor.r + cor.g + cor.b) < 127*3) {
	//				midiControllerOut.sendNoteOff(ch, pitch);
	//			} else
				{
					midiControllerOut.sendNoteOn(ch, pitch, vel);
				}
			}
		}
	}

	//--------------------------------------------------------------
	void uiEventMidi(vector<string> & strings) {
		elementLearn.nome = strings[0];
		elementLearn.ui = strings[1];
		elementLearn.tipo = "float";
	}


	void keyPressed(int key) { }
	void keyReleased(int key) { }
	void onKeyPressed(ofKeyEventArgs &data) { }
	void onKeyReleased(ofKeyEventArgs &data) { }
	void onExit(ofEventArgs &data) {  exit(); }

	//--------------------------------------------------------------
	void newMidiMessage(ofxMidiMessage& msg) {
	//	cout << "status:"  +ofToString(msg.status) << endl;
	//	cout << "pitch:"   +ofToString(msg.pitch) << endl;
	//	cout << "channel:" +ofToString(msg.channel) << endl;
	//	cout << "control:" +ofToString(msg.control) << endl;
	//	cout << "-------" << endl;

		elementListMidiController *te;
		te = &mapaMidiController[msg.status][msg.channel][msg.pitch][msg.control];


		ofxDmtrUI * _ui = &_u->uis[te->ui];
		if (te->ui == "master" || te->ui == "uiM") {
			_ui = _u;
		}

		if (te->tipo == "radio") {
			_ui->getRadio(te->nome)->setValue(te->valor, 1);
			// aqui ter um trigger algo.
		}

		else if (te->tipo == "bang") {
			_ui->getToggle(te->nome)->flip();
		}
		else if (te->tipo == "boolon") {
			_ui->pBool[te->nome] = true;
			midiControllerOut.sendNoteOn(msg.channel, msg.pitch);
		}
		else if (te->tipo == "booloff") {
			_ui->pBool[te->nome] = false;
			midiControllerOut.sendNoteOff(msg.channel, msg.pitch);
		}
		else if (te->tipo == "bool") {
			_ui->pBool[te->nome] = !_ui->pBool[te->nome];
			// midi out pra controladora e colorir os botoes
			if (_ui->pBool[te->nome]) {
				midiControllerOut.sendNoteOn(msg.channel, msg.pitch);
			} else {
				midiControllerOut.sendNoteOff(msg.channel, msg.pitch);
			}
		}

		else if (te->tipo == "preset") {
			// evita o problema no different thread
			if (_u != NULL) {
				_u->nextPreset.push_back(ofToInt(te->nome));
			}
		}

		else if (te->tipo == "float" || te->tipo == "int") {

			slider * s = _ui->getSlider(te->nome);
			if (s != NULL) {
				float valor = ofMap(msg.value, 0, 127, s->min, s->max);
				s->setValue(valor);
			}
		}

		else {
			string message =
			ofToString(msg.status) + " " +
			ofToString(msg.channel) + " " +
			ofToString(msg.pitch) + " " +
			ofToString(msg.control);

			mapaMidiController[msg.status][msg.channel][msg.pitch][msg.control] = elementLearn;

			//cout << "Empty" << endl;
			if (message != lastString) {
				lastString = message;
				cout << lastString << endl;
			}
		}
		if (te->ui != "") {
			_ui->redraw = true;
		}

		midiKeys[msg.pitch] = msg.status == 144;

		midiMessage = msg;
	}


	//--------------------------------------------------------------
	void midiInputAdded(string name, bool isNetwork) {
		cout << "input added" << endl;
		cout << name << endl;
	//	stringstream msg;
	//	msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
	}

	//--------------------------------------------------------------
	void midiInputRemoved(string name, bool isNetwork) {
		cout << "input removed" << endl;
		cout << name << endl;
	//	stringstream msg;
	//	msg << "ofxMidi: input removed: " << name << " network: " << isNetwork << endl;
	}


	//--------------------------------------------------------------
	void setUI(ofxDmtrUI & u) {
		_u = &u;
		// da pra melhorar muito e finalmente remover o uiMap daqui.
	//	for (auto & ui : _u->uis) {
	//		_u->uis[ui.first] = &ui.second;
	//	}
		//_u->uis["uiM"] = _u;
	}

	//--------------------------------------------------------------
	void exit() {
		midiControllerOut.closePort();
		midiControllerIn.closePort();
	}

	//--------------------------------------------------------------
	void uiEventsNeu(dmtrUIEvent & e) {
		if (e.nome == "loadPresetAll") {
			for (auto & e : elements) {
				if (e->tipo == "bool" || e->tipo == "boolon" || e->tipo == "booloff" ) {
					if (_u->uis[e->ui].pBool[e->nome]) {
						midiControllerOut.sendNoteOn(e->channel, e->pitch, 127);
					} else {
						midiControllerOut.sendNoteOff(e->channel, e->pitch, 127);
					}
				}
			}
		}
	}

};

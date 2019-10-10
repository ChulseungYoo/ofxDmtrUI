/*
ofxDmtrUIRemote 
 Created by Dimitre Lima on 18/05/16.
 Copyright 2016 Dmtr.org. All rights reserved.
*/

#pragma once


#if defined( TARGET_OF_IPHONE ) || defined( TARGET_OF_IOS ) || defined( TARGET_ANDROID )
#define DMTRUI_TARGET_TOUCH
#endif



#include "ofEvents.h"
#include "ofxDmtrUI.h"
#include "ofxOsc.h"
//#include "ofxAccelerometer.h"


#ifdef DMTRUI_TARGET_TOUCH
//#include "ofxiOS.h"
//class ofxDmtrUIRemote : public ofxiOSApp {
#else
//#include "ofMain.h"
//class ofxDmtrUIRemote : public ofBaseApp {
#endif


class ofxDmtrUIRemote : public ofBaseApp {
public:
	// void keyPressed(int key);
	// void keyReleased(int key);
	// void update();
	// void draw();
	// void exit();

	// void onDraw(ofEventArgs &data);
	// void onUpdate(ofEventArgs &data);
	// void onKeyPressed(ofKeyEventArgs &data);
	// void onKeyReleased(ofKeyEventArgs &data);
	// void onExit(ofEventArgs &data);

	// void sendInterface(string fileName, bool clear = true);
	// void sendInterfaceString(string text, bool clear = true);
	// void sendVars();

	// void enableAccelerometer();
	// //void setup(string remotehost = "", int port = 9000, bool server = false);
	// void setupServer();
	// void setupRemote();
	// void setListeners();
//	void sendAccel(float x, float y, float z);
//	void uiEvent(dmtrUIEvent & e);


	ofxOscSender 	send;
	ofxOscReceiver 	receive;


	bool interfaced = false;
	bool server = false;
	string lastAdd = "";
	string linha = "";

	ofxDmtrUI ui;
	ofxDmtrUI *_ui = NULL;

	bool sendSetup = false;
	bool useAccelerometer = false;


	ofEvent<ofVec3f> accelEvent;

//	string host;
	string 	serverHostname = "2.16.0.101";
	int 		serverPort = 8000;
	string 	remoteHostname = "";
	int 		remotePort = 9000;



	bool isSetup = false;

	//void sendBasement(string fileName = "");


	bool debugDmtr = false;

	string remoteStyle = "";

	bool debug = false;


	//#include "ofxDmtrUIRemote.h"


	//--------------------------------------------------------------
	void setListeners() {
		if (_ui != NULL) {
			ofAddListener(_ui->evento,	this, &ofxDmtrUIRemote::uiEvent);
		}
		ofAddListener(ofEvents().draw,   		this, &ofxDmtrUIRemote::onDraw);
		ofAddListener(ofEvents().update, 		this, &ofxDmtrUIRemote::onUpdate);
		ofAddListener(ofEvents().keyPressed, 	this, &ofxDmtrUIRemote::onKeyPressed);
		isSetup = true;
	}

	//--------------------------------------------------------------
	void setupServer() {
		receive.setup(serverPort);
		// mudar o nome do send, ta muito generico.
		try {
			send.setup(remoteHostname, remotePort);
			sendSetup = true;

			interfaced = true;
			// precisa?
			server = true;
			cout << "Server is setup, server addr :: " + serverHostname + ":" + ofToString(serverPort) << endl;
			cout << "Server is setup, remote addr :: " + remoteHostname + ":" + ofToString(remotePort) << endl;
			setListeners();


		} catch (exception){
			cout << "ofxDmtrUIRemote :: &&& no internet &&&" << endl;
		}

	}

	//--------------------------------------------------------------
	void setupRemote() {
		receive.setup(remotePort);
		//ui.createFromLine("rect	10 30 1 1");
		ui.setup();
		_ui = &ui;
		cout << "Remote is setup, remote addr :: " + remoteHostname + ":" + ofToString(remotePort) << endl;
		setListeners();
	}


	//--------------------------------------------------------------
	void keyPressed(int key) {
		if (debugDmtr) {
			if (key == 'q') {
				ofxOscMessage m;
				m.setAddress("/clear");
				send.sendMessage(m, false);
			}

			// temporario
			if (key == 'w') {
				{
					ofxOscMessage m;
					m.setAddress("/clear");
					send.sendMessage(m, false);
				}
				for (int a=0; a<10; a++) {
					ofxOscMessage m;
					m.setAddress("/createFromLine");
					m.addStringArg("float	random"+ofToString(ofRandom(0,100))+"	0 1 .5");
					send.sendMessage(m, false);
				}
			}
		}
	}

	//--------------------------------------------------------------
	void keyReleased(int key) { }

	//--------------------------------------------------------------
	void update() {

	#ifdef DMTRUI_TARGET_TOUCH
		if (sendSetup && useAccelerometer) {
	//		ofxOscMessage m;
	//		m.setAddress("/accelerometer");
	//		m.addFloatArg(ofxAccelerometer.getOrientation().x);
	//		m.addFloatArg(ofxAccelerometer.getOrientation().y);
	//		m.addFloatArg(ofxAccelerometer.getOrientation().z);
	//		send.sendMessage(m, false);
		}
	#endif
		
		while(receive.hasWaitingMessages()){
			ofxOscMessage m;
			receive.getNextMessage(m);
			lastAdd = m.getAddress();
			if (debug) {
				cout << "receiving message :: " + m.getAddress() << endl;
			}


	#ifdef DMTRUI_TARGET_TOUCH
			if (m.getAddress() == "/useAccelerometer") {
				if (m.getArgAsBool(0)) {
	//				ofxAccelerometer.setup();
					useAccelerometer = true;
				}
			}
	#endif

			if (m.getAddress() == "/clear") {
				//ui.clear();
			}

			else if (m.getAddress() == "/location")
			{
				remoteHostname = m.getArgAsString(0);
				if (m.getNumArgs() > 1) {
					remotePort = m.getArgAsInt(1);
				}
				send.setup(remoteHostname, remotePort);
				sendSetup = true;
			}

			else if (m.getAddress() == "/interface") {
				ofBuffer blob = m.getArgAsBlob(0);
				//ofSystemAlertDialog("OK");
				//_ui->createFromLine("uiClear");
				for(auto & line: blob.getLines()) {
					_ui->createFromLine(line);
				}
				_ui->addAllListeners();
				_ui->autoFit();
				interfaced = true;
			}

			else if (m.getAddress() == "/createFromLine") {
				_ui->createFromLine(m.getArgAsString(0));
				//_ui->addAllListeners();
			}

			else if (m.getAddress() == "/remote") {
				ofxOscMessage m;
				m.setAddress("/interface");
				//por mais uma linha com "clear" antes.
				ofBuffer buff2 = ofBufferFromFile("remoto.txt"); //remoto.txt
				m.addBlobArg(buff2);
				send.sendMessage(m, false);
			}

			else if (m.getAddress() == "/accelerometer") {
				_ui->pFloat["accelX"] = m.getArgAsFloat(0);
				_ui->pFloat["accelY"] = m.getArgAsFloat(1);
				_ui->pFloat["accelZ"] = m.getArgAsFloat(2);

				ofVec3f acc = ofVec3f(m.getArgAsFloat(0), m.getArgAsFloat(1), m.getArgAsFloat(2));
				ofNotifyEvent(accelEvent, acc);
				// fazer um evento aqui.
			}


			else {
				if (interfaced) {
					vector <string> addr = ofSplitString(m.getAddress(), "/");
					cout << m.getAddress() << endl;
					string var = addr[1];
					//cout << m.getArgType(0) << endl;
					if (m.getArgType(0) == OFXOSC_TYPE_FLOAT) {
						_ui->pFloat[var] = m.getArgAsFloat(0);
					}
					else if (m.getArgType(0) == OFXOSC_TYPE_INT32) {
						_ui->pInt[var] = m.getArgAsInt32(0);
					}
					else if (m.getArgType(0) == OFXOSC_TYPE_STRING) {
						_ui->pString[var] = m.getArgAsString(0);
						radio * te = _ui->getRadio(var);
						if (!te->dirList) {
							_ui->redraw=true;
							_ui->getRadio(var)->setValue(m.getArgAsString(0),3);
						}
					}

					else if (m.getArgType(0) == OFXOSC_TYPE_FALSE) {
						_ui->pBool[var] = false;
					}
					else if (m.getArgType(0) == OFXOSC_TYPE_TRUE) {
						_ui->pBool[var] = true;
					}
				}
			}
			_ui->redraw = true;

			//		if(m.getAddress() == "/corS"){
			//			_ui->sliders[_ui->indexElement[var]].setValue(m.getArgAsFloat(0));
			//			_ui->redraw = true;
			//			cout << m.getArgAsFloat(0) << endl;
			//		}
		}
	}

	//--------------------------------------------------------------
	void draw() {
		if (!server) {
			if (!interfaced) {
				string strAnimation[4] = { "\\", "|", "/", "-" };
				ofDrawBitmapString("Dmtr.org ofxDmtrUI \nWaiting OSC Data " + strAnimation[ofGetFrameNum()%4], 20, 40);
			}
		} else {
		}

		if (debug) {
			ofSetColor(255);
			ofDrawBitmapString(lastAdd, 20, ofGetWindowHeight()-30);
			ofDrawBitmapString(linha, 20, ofGetWindowHeight()-50);
		}
	}

	//--------------------------------------------------------------
	void onDraw(ofEventArgs &data) { draw(); }

	//--------------------------------------------------------------
	void onUpdate(ofEventArgs &data) { update(); }

	//--------------------------------------------------------------
	void onKeyPressed(ofKeyEventArgs &data) {
		keyPressed(data.key);
	}

	//--------------------------------------------------------------
	void onKeyReleased(ofKeyEventArgs &data) { }

	//--------------------------------------------------------------
	void onExit(ofEventArgs &data) {  exit(); }

	//--------------------------------------------------------------
	void exit() { }


	//--------------------------------------------------------------
	void enableAccelerometer() {
		if (sendSetup) {
			ofxOscMessage m;
			m.setAddress("/useAccelerometer");
			m.addBoolArg(true);
			send.sendMessage(m, false);
		}
	}


	//--------------------------------------------------------------
	void sendInterfaceString(string text, bool clear) {
		if (sendSetup) {
			//cout << "ofxDmtrUIRemote :: sendInterface to "+ host +" :: " + fileName << endl;
			if (clear) {
				ofxOscMessage m;
				m.setAddress("/createFromLine");
				m.addStringArg("uiClear");
				send.sendMessage(m, false);
			}
			{
				ofxOscMessage m;
				m.setAddress("/location");
				m.addStringArg(serverHostname);
				m.addIntArg(serverPort);
				send.sendMessage(m, false);
			}
			{
				ofxOscMessage m;
				m.setAddress("/interface");
				m.addBlobArg(ofBuffer(text.c_str(), text.size()));
				//m.addBlobArg(text);
				send.sendMessage(m, false);
			}
		} else {
			cout << "ofxDmtrUIRemote :: osc send not setup yet..." << endl;
		}
	}

	//--------------------------------------------------------------
	void sendInterface(string fileName, bool clear) {
		//cout << "ofxDmtrUIRemote :: sendInterface" << endl;
		if (sendSetup) {
			if (debug) {
				cout << "ofxDmtrUIRemote :: sendInterface to "+ remoteHostname +" :: " + fileName << endl;
			}

			// fazer um bundle mais pra frente.
			{
				ofxOscMessage m;
				m.setAddress("/location");
				m.addStringArg(serverHostname);
				m.addIntArg(serverPort);
				send.sendMessage(m, false);
			}
			{
				ofxOscMessage m;
				m.setAddress("/interface");
				// por mais uma linha com "clear" antes.
				// mais rapido tudo
				// ver como fazer o ofBuffer aqui
				ofBuffer blob;
				if (clear) {
					blob.append("uiClear\n");
				}
				blob.append(remoteStyle+"\n");
				blob.append(ofBufferFromFile(fileName));
				//ofBuffer buff2 = ofBufferFromFile(fileName); //remoto.txt
				//cout << blob << endl;
				m.addBlobArg(blob);
				send.sendMessage(m, false);
			}
		} else {
			cout << "ofxDmtrUIRemote :: osc send not setup yet..." << endl;
		}
	}

	//--------------------------------------------------------------
	void sendVars() {
		if (sendSetup) {
			for (auto & f : _ui->pFloat) {
				ofxOscMessage m;
				m.setAddress("/"+f.first);
				m.addFloatArg(f.second);
				send.sendMessage(m, false);

			}

			for (auto & f : _ui->pInt) {
				ofxOscMessage m;
				m.setAddress("/"+f.first);
				m.addIntArg(f.second);
				send.sendMessage(m, false);
			}

			for (auto & f : _ui->pString) {
				ofxOscMessage m;
				m.setAddress("/"+f.first);
				m.addStringArg(f.second);
				send.sendMessage(m, false);
			}

			for (auto & f : _ui->pBool) {
				ofxOscMessage m;
				m.setAddress("/"+f.first);
				m.addBoolArg(f.second);
				send.sendMessage(m, false);
			}
		}
	}

	//--------------------------------------------------------------
	void sendAccel(float x, float y, float z) {
		if (sendSetup) {
			ofxOscMessage m;
			m.setAddress("/accelerometer");
			m.addFloatArg(x);
			m.addFloatArg(y);
			m.addFloatArg(z);
			send.sendMessage(m, false);
		}
	}

	//--------------------------------------------------------------
	//void sendBasement(string fileName) {
	//	cout << "sendBasement" << endl;
	//	if (isSetup) {
	//		sendInterface("uiAllRemote.txt");
	//		if (_ui->UINAME == "uiS") {
	//			sendInterfaceString(_ui->allText, false);
	//		} else {
	//			if (fileName == "") {
	//				sendInterface(_ui->createdFromTextFile, false);
	//			}
	//			else  {
	//				sendInterface(fileName, false);
	//			}
	//		}
	//		sendVars();
	//	}
	//}


	//--------------------------------------------------------------
	void uiEvent(dmtrUIEvent &e) {

	//	if (e.nome == "setup" || e.nome == "createFromText" || e.nome == "loadPreset") {
	//		cout << e.nome << endl;
	//	}

		if (e.nome == "setup" || e.nome == "createFromText") {
			//cout << e.nome << endl;
			//cout << "ofxUIRemote uiEvent setup" << endl;
			sendInterface(_ui->createdFromTextFile, true);
		}

		else if (e.nome == "loadPreset") {
			//cout << e.nome << endl;
			sendVars();
		}


		// evitar loop infinito
		if (e.tipo != SET) {
			linha = e.nome;
			ofxOscMessage m;
			// ver aqui se preciso especificar o tipo, ex: update
			// pra nao entrar num loop infinito.

			if (e.var == FLOAT) {
				m.addFloatArg(_ui->pFloat[e.nome]);
			}
			else if (e.var == INT) {
				m.addIntArg(_ui->pInt[e.nome]);
			}
			else if (e.var == STRING) {
				m.addStringArg(_ui->pString[e.nome]);
			}
			else if (e.var == BOOLEANO) {
				m.addBoolArg(_ui->pBool[e.nome]);
			}
			m.setAddress("/"+ e.nome);

			if (debug) {
				cout << "sending message :: " + e.nome << endl;
			}

			if (sendSetup) {
				send.sendMessage(m, false);
			}
		}
	}


};

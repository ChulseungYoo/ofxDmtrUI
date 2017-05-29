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

/*

 ofxDmtrUI.h
 Created by Dimitre Lima on 04/06/2016.
 http://dmtr.org/
 
 */


enum flowDir {
	VERT, HORIZ, NO_FLOW
};

// 27 de maio de 2016
enum elementType {
	SLIDER, SLIDERINT, TOGGLE, LABEL, RADIO, RADIOITEM, SLIDER2D, FBO, PRESETS
};

enum toggleType {
	SWITCH, BANG, HOLD
};

enum eventoType {
	// bang fica aqui mesmo?
	UPDATE, LOAD, SET
};

enum varType {
	FLOAT, INT, STRING, BOOLEANO
};


// 27 de maio de 2016
class dmtrUIEvent {
public:
	string *_nome;
	string nome;
	string ui;

	// 16 de julho de 2016
	string msg;



	elementType element;
	eventoType tipo;
	varType var = FLOAT;

	dmtrUIEvent (){};
	//dmtrUIEvent (string & nome, varType var, elementType element, eventoType tipo) {};
	dmtrUIEvent (string & n, varType v, elementType e, eventoType t) {
		nome = n;
		var = v;
		element = e;
		tipo = t;
	};
	//dmtrUIEvent sendEvent(string & nome, varType var, elementType element, eventoType tipo);

	//ofxDmtrUI *_ui;
	// unknown type name.
	//string action; //update, load, etc.
};



class slider {
public:
	string 		nome;
	elementType tipo;
	ofRectangle rect;
	bool 		inside = false;
	ofColor 		cor;
	float		min = 0;
	float		max = 1;
	float		def = .5;
	//float 		min, max, def;

	// com o tempo remover o valor e valorInt e so usar os outros.
	//	float 		valor;
	//	int			valorInt;

	// automatic so it can choose between float and integer :)
	// doesnt work
	// const auto			*_val;
	float 		*_val;
	int			*_valInt;
	// temporario
	unsigned char			*_valChar;

	int			valorPixels = 0;
	bool			isInt = false;
	// 15 04 2016 - boolean for vertical sliders
	bool			vert = false;

	ofEvent<string> uiEvent;

	// 27 05 2016 - tipo de evento mais completo
	ofEvent<dmtrUIEvent> evento;

	// 05 10 2016 - UI maior pra iPhone
	int labelOffsetY;

	void init() {
		labelOffsetY = rect.height/2 + 7;
	}

	void rand() {
		setValue(ofRandom(min, max));
	}

	// 30/12/2016 - nao sei pra que serve esta função.
	void update(int x, int y) {
		dmtrUIEvent te;
		te.nome = nome;
		te._nome = &nome;
		te.tipo = UPDATE;

		// nome pode ser um pointer.
		// fazer algo que cheque se o valor foi modificado da ultima vez pra disparar o evento?
		if (vert) {
			valorPixels = ofClamp(rect.height - (y - rect.y), 0, rect.height);
		} else {
			valorPixels = ofClamp(x - rect.x, 0, rect.width);
		}

		if (isInt) {
			te.element = SLIDERINT;
			te.var = INT;
			if (vert) {
				*_valInt = ofMap(valorPixels, 0, rect.height, min, max);
			} else {
				*_valInt = ofMap(valorPixels, 0, rect.width, min, max);
			}
			string ev = "updateInt_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		} else {
			te.element = SLIDER;

			if (vert) {
				*_val = ofMap(valorPixels, 0, rect.height, min, max);
			} else {
				*_val = ofMap(valorPixels, 0, rect.width, min, max);
			}
			string ev = "updateFloat_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		}

		ofNotifyEvent(evento, te, this);
	}

	float getValue() {
		return isInt ? *_valInt : *_val;
	}

	void setValue(float v, string eventName = "set") {
		v = ofClamp(v, min, max);
		
		// so seta o trigger se o valor for diferente
		//if (v != getValue())
		{
			// posso colocar o evento aqui mas no caso vou dar um trigger qdo estiver carregando XML
			// trigando todos os All e baguncando presets.

			// hoje coloquei eventos aqui pra poder usar o pan_All desde o midi controller
			string ev;
			if (isInt) {
				*_valInt = v;
				ev = eventName + "Int_" + nome;
			} else {
				*_val = v;
				ev = eventName + "Float_" + nome;
			}
			valorPixels = ofMap(v, min, max, 0, rect.width);

			if (ev != "") {
				ofNotifyEvent(uiEvent, ev, this);
			}

		}
		// 29 de dez de 2016 - forma compacta de fazer o dmtrUIEvent.
		dmtrUIEvent e = dmtrUIEvent(nome, isInt ? INT : FLOAT, isInt ? SLIDERINT : SLIDER, UPDATE);
		ofNotifyEvent(evento, e, this);


	}

	//	void draw() {
	void draw(bool learn = false) {
		auto vvv = (isInt ? *_valInt : *_val);
		valorPixels = ofMap(vvv, min, max, 0, rect.width);

		ofSetColor(cor);
		//cout << nome << endl;
		ofDrawRectangle(rect);

		// value rectangle black transparent
		ofSetColor(0,128);
		if (vert) {
			valorPixels = ofMap(vvv, min, max, 0, rect.height);
			//ofDrawRectangle(rect.x, rect.y, rect.width, valorPixels);
			ofDrawRectangle(rect.x, rect.y + rect.height-valorPixels, rect.width, valorPixels);
		} else {
			ofDrawRectangle(rect.x, rect.y, valorPixels, rect.height);
			string label = nome + " "+ofToString(isInt ? *_valInt : *_val);
			ofSetColor(0,128);
			ofDrawBitmapString(label, rect.x+11, rect.y + labelOffsetY);
			ofSetColor(255);
			ofDrawBitmapString(label, rect.x+10, rect.y + labelOffsetY-1);
		}

		if (learn) {
			ofSetColor(255,0,70);
			ofNoFill();
			//ofSetColor(0,50,200,50);
			ofDrawRectangle(rect);
			ofFill();
		}
	}
};


class toggle {
public:
	
	string nome;
	ofRectangle rect;
	bool inside = false;
	ofColor cor;
	//bool 			valor = false;
	bool showLabel = true;
	bool *_val;
	bool def = false;
	bool bang = false; // bang is in fact hold. fix this with available time.
	bool hold = false;
	
	toggleType kind = SWITCH;
	//bool holdType = false;

	ofEvent<string> uiEvent;
	ofEvent<dmtrUIEvent> evento;

	int labelOffsetY;

	void init() {
		labelOffsetY = rect.height/2 + 7;
	}

	void flip() {
		*_val = !*_val;
		string ev = bang ? "bang_" + nome : "updateBool_" + nome;
		ofNotifyEvent(uiEvent, ev, this);

		// 28 maio 2016
		dmtrUIEvent te;
		te.nome = nome;
		te._nome = &nome;
		//te.tipo = BANG;
		//te.tipo = bang == true ? BANG : TOGGLE; // acho que ta errado isso de ser so bang

		te.var = BOOLEANO;
		ofNotifyEvent(evento, te, this);
	}

	// acho que nao preicsa mais? precisa?
	// setvalue aqui ta triggering o evento.
	void setValue(bool v) {
		*_val = v;
		// 19 de abril. vai atrapalhar?
		string ev = "loadBool_" + nome;
		ofNotifyEvent(uiEvent, ev, this);
	}
	
	void mouseRelease() {
		*_val = false;
		if (hold) {
			dmtrUIEvent te;
			te.nome = nome;
			te._nome = &nome;
			te.var = BOOLEANO;
			ofNotifyEvent(evento, te, this);
		}
	}

	void draw() {
		ofSetColor(cor);
		ofDrawRectangle(rect);

		// toggle Label
		if (showLabel) {
			ofSetColor(255);
			ofDrawBitmapString(nome, rect.x + rect.width + 5, rect.y + labelOffsetY);
		}

		if (*_val) {
//			ofSetColor(0);
			ofSetColor(255);
			ofNoFill();
			float off = rect.height / 6.0;
			ofDrawLine(rect.x + off, rect.y + off, 			     rect.x + rect.width -off, rect.y + rect.height -off);
			ofDrawLine(rect.x + off, rect.y + rect.height - off, rect.x + rect.width -off, rect.y + off);
			ofFill();
		}
	}
};


class label {
public:
	string 			nome;
	ofRectangle 		rect;
	ofColor 			cor;
	string			*_val;

	void draw() {
		ofSetColor(cor);
		//ofDrawRectangle(rect);
		ofDrawBitmapString(nome + " " + *_val, rect.x + 0, rect.y+15);
	}
};


class radio  {
public:
	string 			nome;
	ofRectangle 		rect;
	ofColor 			cor;
	string 			*_val = NULL;

	vector <string>	opcoes;
	vector <ofRectangle> 	rects;


	// tentativa de multiple
	bool multiple = false;
	vector <bool *> _vals;
	vector <bool> clicked;

	int offx = 0;
	int offy = 0;
	int height = 20;
	ofEvent<string> uiEvent;
	ofEvent<dmtrUIEvent> evento;

	// 7 de agosto de 2016 pro Areia + ofxDmtrUIRemote, dirlist, resolver no futuro
	bool dirList = false;
	// pra fazer da mareh o lookup de cores.
	vector <ofColor> cores;
	bool useColors = false;

	bool isInit = false;

	int labelOffsetY;


	void init() {
		labelOffsetY = height/2 + 5;
		for (auto & o : opcoes) {
			clicked.push_back(false);
			int largura = 6*2 + o.size() * 8;
			ofRectangle tr = ofRectangle(rect.x + offx, rect.y + offy, largura, height);
			//cout << tr << endl;

			// mudar para sliderwidth
			if ((offx + tr.width) > rect.width) {
				offx = 0;
				offy += height + 1;
				tr = ofRectangle(rect.x + offx, rect.y + offy, largura, height);
			}
			// redimensiona o retangulo pai.
			rect.height = MAX(rect.height, offy + height + 1);
			offx += largura + 1;
			rects.push_back(tr);

			// temporario pras cores de moving heads mareh
			cores.push_back(cor);
		}

		// recalculando a largura total do rect sem mexer no offx e offy.
		//rect.width = widthAll;
		// fazer com que o rect principal seja da largura de todos os outros rects.
	}

	void setValue(string v, int notify = 0) {
		//cout << v << endl;
		//if (*_val != NULL) {
		if (opcoes.size() > 0) {
			*_val = v;
		}
		//}
		dmtrUIEvent te;
		te.nome = nome;
		te._nome = &nome;
		// vamos ver direito tudo isso.
		// mudar tudo pra ingles?
		te.element = RADIO;
		te.var = STRING;

		//			UPDATE, LOAD, SET, BANG

		if (notify == 1) {
			te.tipo = UPDATE;
			string ev = "updateRadio_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		}

		if (notify == 2) {
			te.tipo = LOAD;
			string ev = "loadRadio_" + nome;
			ofNotifyEvent(uiEvent, ev, this);
		}
		if (notify == 3) {
			te.tipo = SET;
		}
		ofNotifyEvent(evento, te, this);
	}

	void checkMouse(int x, int y) {
		int i = 0;
		for (auto & r : rects) {
			if (r.inside(x,y) ) {
				if (multiple) {
					if (!clicked[i]) {
						*_vals[i] ^= 1;

						// ver uma maneira mais firmeza de modularizar isso:
						dmtrUIEvent te;
						te.nome = nome;
						te._nome = &nome;
						te.element = RADIO; // fazer RADIOMULT?
						te.tipo = UPDATE;
						te.var = STRING;
						ofNotifyEvent(evento, te, this);
					}
					clicked[i] = true;
				}

				else {
					if (*_val != opcoes[i]) {
						*_val = opcoes[i];
						string ev = "updateRadio_" + nome;
						ofNotifyEvent(uiEvent, ev, this);

						dmtrUIEvent te;
						te.nome = nome;
						te._nome = &nome;
						te.element = RADIO;
						te.tipo = UPDATE;
						te.var = STRING;
						ofNotifyEvent(evento, te, this);
					}
				}
			}
			// 12 de maio de 2016. tentando evento do multiple radio
			else {
				clicked[i] = false;
			}
			i ++;
		}
	}


	void draw() {
		ofSetColor(255);
		int offy = 15;
		//int offx = 0;
		//ofSetColor(255,0,60,128);
		//ofDrawRectangle(rect);

		int i = 0;
		if (multiple) {
			for (auto & r : rects) {
				auto & o = opcoes[i];
				ofSetColor(*_vals[i] ? cor : ofColor(80,120));
				if (useColors) {
					ofSetColor(cores[i]);
				}
				ofDrawRectangle (r);
				ofSetColor(*_vals[i] ? 0 : 255);

				//labelOffsetY
//				ofDrawBitmapString(o, r.x + 6, r.y + offy);
				ofDrawBitmapString(o, r.x + 6, r.y + labelOffsetY);
				i++;
			}
		} else {

			for (auto & r : rects) {
				auto & o = opcoes[i];
				ofSetColor(*_val == o ? cor : ofColor(80,120));
				if (useColors) {
					ofSetColor(cores[i]);
				}
				ofDrawRectangle (r);
				ofSetColor(*_val == o ? 0 : 255);
//				ofDrawBitmapString(o, r.x + 6, r.y + offy);
				ofDrawBitmapString(o, r.x + 6, r.y + labelOffsetY);
				i++;
			}
		}
	}
};


class slider2d {
public:

	// 12 de setembro de 2016, paraolimpicos
	vector <ofColor> cores;
	ofColor *_valColor;
	string nome;
	ofPoint	 *_val;
	//	float *_valx;
	//	float *_valy;
	float defx = .5;
	float defy = .5;
	ofFbo *_fbo;
	bool		fboSet = false;
	ofRectangle rect;
	ofColor cor;
	ofEvent<string> uiEvent;
	bool inside = false;
	bool isSlider = true;

	bool isColor = false;

	// 24 agosto 2016 - pro ofxLicht
	ofEvent<dmtrUIEvent> evento;


	void setFbo(ofFbo &fbo) {
		_fbo = &fbo;
		fboSet = true;
	}

	void setValue(ofPoint xy) {

		//cout << "setValue Slider2d " + nome << endl;
		*_val = xy;
		// UNIFICAR EVENTOS DEPOIS
		if (isColor) {
			int indexColor = xy.x * cores.size();
			*_valColor = cores[indexColor];
		}

		string ev = "loadSlider2d_" + nome;
		ofNotifyEvent(uiEvent, ev, this);
	}

	void checkMouse(int mx, int my) {
		float x = mx - rect.x;
		float y = my - rect.y;
		*_val = ofPoint(
			ofClamp(ofMap(x, 0, rect.width, 0, 1), 0, 1),
			ofClamp(ofMap(y, 0, rect.height, 0, 1), 0, 1)
		);


		if (isColor) {
			int indexColor = ofClamp(ofMap(x, 0, rect.width, 0, 1), 0, 1) * cores.size();
			*_valColor = cores[indexColor];
		}

		string ev = "updateSlider2d_" + nome;
		ofNotifyEvent(uiEvent, ev, this);

		// modularizar este tipo de evento. fazer uma fun√ß√£o quem sabe...
		dmtrUIEvent te;
		te.nome = nome;
		te._nome = &nome;
		te.element = SLIDER2D;
		te.tipo = UPDATE;
		//te.var = STRING;
		ofNotifyEvent(evento, te, this);
	}

	void draw() {
		if (fboSet) {
			//ofSetColor(255);
			_fbo->draw(rect.x, rect.y);
		}
		else
		{
			ofSetColor(0);
			ofDrawRectangle(rect);
		}

		if (isSlider) {
			//ofSetColor(255,0,0);
			ofNoFill();
			//ofPushMatrix();
			ofPoint xy = *_val;
			float x = xy.x * rect.width + rect.x;
			float y = xy.y * rect.height + rect.y;
			ofDrawLine(x, rect.y, x, rect.y + rect.height);
			ofDrawLine(rect.x, y, rect.x + rect.width, y);
			ofFill();
			float raio = 4;
			ofDrawRectangle(x -raio/2, y-raio/2, raio,raio);
		}
	}
};


class inspector {
public:
	string nome;
	ofRectangle rect;
	ofFbo fbo;
	string *_val;
	float *_valFloat;
	string tipo = "string";
	// nao usado.
	ofColor cor;
	void init() {
		//fbo.allocate(200,20,GL_RGBA);
	}

	void draw() {
		ofSetColor(255);
		//		fbo.begin();
		//		ofClear(0);
		if (tipo == "fps") {
			ofDrawBitmapString(ofToString(ofGetFrameRate()), 4 + rect.x, 15 + rect.y);
		}
		else if (tipo == "string") {
			//			ofDrawBitmapString(*_val, 4, 18);
			ofDrawBitmapString(*_val, 4 + rect.x, 18 + rect.y);
		} else if (tipo == "inspectorFloat") {
			ofSetColor(255,0,0);
			float x = *_valFloat * fbo.getWidth();
			ofDrawLine(x,0,x,fbo.getHeight());
			//ofSetColor(50);
		}
		//		fbo.end();
		//		fbo.draw(rect.x, rect.y);
	}
};


class preset {
public:
	int index;
	string nome;
	ofRectangle	rect;
	ofImage	img;
	ofFbo fbo;
	bool selecionado = false;
	void draw() {
		if (fbo.isAllocated()) {
			fbo.begin();
			ofClear(40,255);
			if (img.isAllocated()) {
				img.draw(0,0);
			} else {
				ofSetColor(127);
				ofPushMatrix();
				ofTranslate(int(fbo.getWidth()/2), int(fbo.getHeight()/2));
				int d = 5;
				ofDrawLine(-d, -d, d, d);
				ofDrawLine(d, -d, -d, d);
				ofPopMatrix();
			}
			if (selecionado) {
				ofSetColor(255,0,80);
				ofDrawRectangle(5,5,20,20);
			}
			fbo.end();

			ofSetColor(255);
			//fbo.draw(rect.x, rect.y, rect.width, rect.height);
			//cout << rect.x << endl;
			fbo.draw(rect.x, rect.y);
		}
	}
};


class presets {
public:
	string nome = "presets";
	ofRectangle rect;
	vector <preset> presets;
	//int	*_val;
	// temporario
	int valor = -1;
	bool ok = false;
	ofEvent<string> uiEvent;

	void draw() {
		for (auto & p : presets) {
			p.draw();
		}
	}

	void update(int novovalor) {
	}

	// 10 de maio, Mareh Marisco, apenas testando.
	void set(int slot) {
		//cout << "set : " + ofToString(slot) << endl;
		if (valor != -1) {
			presets[valor].selecionado = false;
			presets[valor].draw();
		}
		if (presets.size() > 0 && valor < presets.size()) {
			valor = slot;
			presets[valor].selecionado = true;
			presets[valor].draw();
		}

		// 18 de agosto, basement. pra fazer o autoadvance selecionar
		for (auto & p : presets) {
			p.draw();
		}
	}

	void checkMouse(int x, int y) {
		for (auto & p : presets) {
			if (p.rect.inside(x,y)) {
				//cout << "click inside preset" << endl;
				// only trigger events if clicked in different preset slot
				if (ofGetKeyPressed(OF_KEY_CONTROL)) {
					//
					string ev = "erasePreset_" + ofToString(valor);
					ofNotifyEvent(uiEvent, ev, this);
				}

				if (ofGetKeyPressed(OF_KEY_COMMAND)) {
					//cout << "of key command save" << endl;
					if (valor != -1) {
						presets[valor].selecionado = false;
						presets[valor].draw();
					}
					valor = p.index;
					p.selecionado = true;
					string ev = "savePreset_" + ofToString(valor);
					ofNotifyEvent(uiEvent, ev, this);
					p.draw();
				} else {

					//cout << "key command not pressed" << endl;
					if (valor != p.index) {
						if (valor != -1) {
							presets[valor].selecionado = false;
							presets[valor].draw();
						}
						valor = p.index;
						p.selecionado = true;
						string ev = "loadPreset_" + ofToString(valor);
						ofNotifyEvent(uiEvent, ev, this);
						p.draw();
					}
				}
			}
		}
	}
};


class color {
public:
	string nome;
	float *_h;
	float *_s;
	float *_b;
	float *_a;
	slider *_sh;
	slider *_ss;
	slider *_sb;
	slider *_sa;

};


class element {
public:
	string nome;
	elementType tipo;
	ofRectangle *_rect;

	slider *_slider = NULL;
	toggle *_toggle = NULL;
	label *_label = NULL;
	radio *_radio = NULL;
	presets *_presets = NULL;
	slider2d *_slider2d = NULL;

	// fazer aqui o void get (multiplos returns)

	// só funciona com c++ 14. cuidado
	slider * getSlider() {
		return _slider;
	}
	toggle * getToggle() {
		return _toggle;
	}
//	auto * get(string nome, elementType tipo) {
//		if (_slider != NULL) {
//			return &_slider;
//		}
//		else if (_toggle != NULL) {
//			return &_toggle;
//		}
//	}


	void set(slider &e) {
		nome = e.nome;
		_rect = &e.rect;
		_slider = &e;
		// ou sliderint?
		//tipo = SLIDER;
		tipo = e.tipo;
	}

	void set(slider2d &e) {
		nome = e.nome;
		_rect = &e.rect;
		_slider2d = &e;
		tipo = SLIDER2D;
	}

	void set(presets &e) {
		nome = e.nome;
		_rect = &e.rect;
		_presets = &e;
		tipo = PRESETS;
	}

	void set(toggle &e) {
		nome = e.nome;
		_rect = &e.rect;
		_toggle = &e;
		tipo = TOGGLE;
	}

	void set(label &e) {
		nome = e.nome;
		_rect = &e.rect;
		_label = &e;
		tipo = LABEL;
	}

	void set(radio &e) {
		nome = e.nome;
		_rect = &e.rect;
		_radio = &e;
		tipo = RADIO;
	}


	/*
	 busquei o std::bind acho que da pra fazer ponteiro de funcao pro draw
	 */
	//void *_draw;
	// function pointer.
	void (*_draw)(void);
	
	//	typedef void (*funcao)();
	//	funcao _draw;
	
	void draw() {
		if (_slider != NULL) {
			_slider->draw();
		}
	}
};

// não funciona pq o class slider ainda nao conhece o class element.
class eventoNovo {
public:
	// seria lindo se funcionasse
	//auto * _ponteiro = NULL;
	element * _element = NULL;
	elementType type;
};



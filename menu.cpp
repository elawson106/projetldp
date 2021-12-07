#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <string>
#include <math.h>
#include <time.h>
#include <chrono>
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <array>

using namespace std;


struct Point {
	int x, y;
};

class Button {
	Point center;
	string label;
	int w, h;

	Fl_Button* btn;

	public:
		enum ButtonType{nivSelector, nivModificator};
		Button(Point center, char label[], int w, int h, ButtonType type):
			center{center}, label{label}, w{w}, h{h}, type{type} {
				btn = new Fl_Button(center.x, center.y, w, h);
				btn->label(label);
				btn->box(FL_ROUND_UP_BOX);
			}

	private:
		ButtonType type;

	public:

	bool contains(Point p){
		return p.x >= center.x && p.x <= center.x + w && p.y >= center.y && p.y <= center.y + h;
	};

	void callback(){
		if(type == nivSelector){
			cout << 111 << endl;
		}else if(type == nivModificator){
			cout << 222 << endl;
		}
	}
	
};

class Text {
	Point center;
	string s;
	int fontSize;
	Fl_Color color;
	public:
	//Constructor
	Text(string s, Point center, int fontSize = 10, Fl_Color color = FL_BLACK):
    	s{s}, center{center}, fontSize{fontSize}, color{color} {}

	//Draw
	void draw();

	//Setters and getters
	string getString() {
		return s;
	}
	void setString(const string &newString) {
		s = newString;
	}
	int getFontSize() {
		return fontSize;
	}
	void setFontSize(int newFontSize) {
		fontSize = newFontSize;
	}
	Point getCenter() {
		return center;
	}
	void setCenter(Point newCenter) {
		center = newCenter;
	}
};

void Text::draw() {
	fl_color(color);
	fl_font(FL_HELVETICA, fontSize);
	int width, height;
	fl_measure(s.c_str(), width, height, false);
	fl_draw(s.c_str(), center.x-width/2, center.y-fl_descent()+height/2);
}

class Canvas {
	Fl_PNG_Image* img_png;
	Fl_Box* img_box;

	vector<Button> btnList;

	public:
	Canvas();
	void init();
	void draw();
	void test(Fl_Button*, void*);
	void mouseMove(Point mouseLoc);
	void mouseClick(Point mouseLoc);
	void keyPressed(int keyCode);
};

Canvas::Canvas() {
	init();
}

void Canvas::init(){
	img_box = new Fl_Box(0, 0, 500, 500);
	img_png = new Fl_PNG_Image("back_menu.png");
	img_box->image(img_png);

	btnList.push_back({{150, 195}, "Sélecteur de niveau.", 200, 50, static_cast<Button::ButtonType>(0)});
	btnList.push_back({{150, 255}, "Modificateur de niveau.", 200, 50, static_cast<Button::ButtonType>(1)});
}

void Canvas::test(Fl_Button* obj, void*){
	cout << "test" << endl;
}

void Canvas::draw() {
	Text("Miguel Lozano - 495649", {50, 450}, 10).draw();
	Text("Edem Lawson - 499852", {380, 450}, 10).draw();
}
void Canvas::mouseMove(Point p) {
	
}
void Canvas::mouseClick(Point p) {
	for(Button b : btnList){
		if(b.contains(p)){
			b.callback();
		}
	}
}
void Canvas::keyPressed(int i) {}

class MainWindow : public Fl_Window {
  Canvas canvas;
 public:
  MainWindow() : Fl_Window(500, 50, 500, 500, "Menu Principal") {
    Fl::add_timeout(1.0/60, Timer_CB, this);
    //resizable(this);
  }
  void draw() override {
    Fl_Window::draw();
    canvas.draw();
  }
  int handle(int event) override {
    switch (event) {
      case FL_MOVE:
        canvas.mouseMove(Point{Fl::event_x(), Fl::event_y()});
        return 1;
      case FL_PUSH:
        canvas.mouseClick(Point{Fl::event_x(), Fl::event_y()});
        return 1;
      case FL_KEYDOWN:
        canvas.keyPressed(Fl::event_key());
        return 1;
    }
    return 0;
  }
  static void Timer_CB(void *userdata) {
    MainWindow *o = (MainWindow*) userdata;
    o->redraw();
    Fl::repeat_timeout(1.0/60, Timer_CB, userdata);
  }
};


/*--------------------------------------------------

main

Do not edit!!!!

--------------------------------------------------*/


int main(int argc, char *argv[]) {
  srand(time(0));
  MainWindow window;
  window.show(argc, argv);
  return Fl::run();
}
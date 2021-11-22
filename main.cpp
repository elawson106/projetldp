
// These should include everything you might use
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Box.H>
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

/*--------------------------------------------------

Text class.

Use to display text on the screen. For example:

Text("Hello!", {250, 250}).draw();

will create a text temporary instance with the
string "Hello!" centered at 250, 250 and will
call draw on the temporary.

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/

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

/*--------------------------------------------------

Rectangle class.

Use to display a fillend-in rectangle on the screen
with different colors for the fill and the border

It should have all the features you need and you
should not need to edit it.

--------------------------------------------------*/

class Rectangle {
	Point center;
	int w, h;
	Fl_Box* img_box = new Fl_Box(center.x-w/2, center.y-h/2, w, h);
	Fl_PNG_Image* png_img = nullptr;
	int img_nbr;
	Fl_Color fillColor, frameColor;
 	public:
  	Rectangle(Point center, int w, int h,
            Fl_Color frameColor = FL_BLACK,
            int img_nbr = 0);
	void draw();
	Fl_Color getFillColor() {
		return fillColor;
	}
	void setFrameColor(Fl_Color newFrameColor);
	Fl_Color getFrameColor() {
    	return frameColor;
	}
	void setWidth(int neww) {
		w = neww;
	}
	void setHeight(int newh) {
		h = newh;
	}
	int getWidth() {
		return w;
	}
	int getHeight() {
		return h;
	}
	void setCenter(Point c);
	bool contains(Point p);
	Fl_Box* getImage();
	Point getCenter() {
		return center;
	}
};

Rectangle::Rectangle(Point center, int w, int h,
                     Fl_Color frameColor,
                     int img_nbr):
	center{center}, w{w}, h{h}, img_nbr{img_nbr}, frameColor{frameColor} {}

void Rectangle::draw() {
	//fl_draw_box(FL_FLAT_BOX, center.x-w/2, center.y-h/2, w, h, fillColor);
	if(png_img == nullptr){
		switch (img_nbr)
		{
		case 1:
			png_img = new Fl_PNG_Image("bonbon/tile000.png");
			break;

		case 2:
			png_img = new Fl_PNG_Image("bonbon/tile001.png");
			break;

		case 3:
			png_img = new Fl_PNG_Image("bonbon/tile002.png");
			break;

		case 4:
			png_img = new Fl_PNG_Image("bonbon/tile003.png");
			break;

		case 5:
			png_img = new Fl_PNG_Image("bonbon/tile004.png");
			break;

		case 6:
			png_img = new Fl_PNG_Image("bonbon/tile005.png");
			break;
		
		default:
			break;
		}
	}
	img_box->image(png_img);
	array<Point, 5> points{
    Point{center.x-w/2, center.y-h/2},
    Point{center.x-w/2, center.y+h/2},
    Point{center.x+w/2, center.y+h/2},
    Point{center.x+w/2, center.y-h/2},
    Point{center.x-w/2, center.y-h/2}};
	for (auto& point : points) {
		fl_vertex(point.x, point.y);
	}
	//img_box->redraw();
	fl_draw_box(FL_BORDER_FRAME, center.x-w/2, center.y-h/2, w, h, frameColor);
}

void Rectangle::setFrameColor(Fl_Color newFrameColor) {
	frameColor = newFrameColor;
}

bool Rectangle::contains(Point p) {
	return p.x>=center.x-w/2 &&
		p.x<center.x+w/2 &&
		p.y>=center.y-h/2 &&
		p.y<center.y+h/2;
}

void Rectangle::setCenter(Point c){
	center = c;
}

Fl_Box* Rectangle::getImage(){
	return img_box;
}


/*--------------------------------------------------

Cell class.

The Canvas class below will have cells as instance
vraiables and call the methods of Cell
--------------------------------------------------*/
struct Translation {
	Translation(Point p, Fl_Box* img_box, Point center, Rectangle r) {
		fl_push_matrix();
		fl_translate(p.x, p.y);
		//cout << p.x << " - " << p.y << endl;
 		img_box->position(p.x + center.x-96/2, p.y + center.y-96/2);
		r.setCenter({p.x + center.x-96/2, p.y + center.y-96/2});
		img_box->redraw();
	}
	~Translation() {
		fl_pop_matrix();
	}
};

class Cell;

class Animation {
 	int countBounce = 0;
	Cell *c;
	bool isCompleted = False;
	public:
		enum AnimationType {bounce, bounceLeft, bounceRight, bounceHi};
		Animation(Cell *c, int an);
		void draw();
		bool isComplete();
		AnimationType type;
};

Animation::Animation(Cell *c, int an):
  c{c} {
    switch (an)
	{
	case 1:
		type = bounce;
		break;
	case 2:
		type = bounceHi;
		break;
	case 3:
		type = bounceLeft;
		break;
	case 4:
		type = bounceRight;
		break;
	default:
		break;
	}
  }

bool Animation::isComplete(){
  return isCompleted;
}

class Cell {
	Rectangle r;
	int color;
	bool hasToBeSwitched = False;

	vector<Cell *> neighbors;

	Animation *anim = nullptr;

	public:
	// Constructor
	Cell(Point center, int w, int h, int color);

	// Methods that draw and handle events
	void draw();
	void drawWithoutAnimation();
	void mouseMove(Point mouseLoc);
	void mouseClick(Point mouseLoc);
	void addNeighbors(Cell &c);
	vector<Cell *> getNeighbors();
	void setStateSwitch(bool sw);
	bool getStateSwitch();
	Rectangle getRectangle();
	void switchCell(Cell *c1, Cell *c2);

};

Cell::Cell(Point center, int w, int h, int color):
	r(center, w, h, FL_BLACK, color),
	color{color}{}

void Cell::drawWithoutAnimation(){
	r.draw();
}

void Cell::draw() {
	if(getStateSwitch()){
		r.setFrameColor(FL_GREEN);
	}else{
		r.setFrameColor(FL_RED);
	}
	if(anim){
		anim->draw();
		if(anim->isComplete()){
			anim = nullptr;
		}
	}else{
		drawWithoutAnimation();
	}
}

void Cell::addNeighbors(Cell &c){
	neighbors.push_back(&c);
}

vector<Cell *> Cell::getNeighbors(){
	return neighbors;
}

void Cell::setStateSwitch(bool sw){
	hasToBeSwitched = sw;
}

bool Cell::getStateSwitch(){
	return hasToBeSwitched;
}

void Cell::mouseMove(Point mouseLoc) {
	if (r.contains(mouseLoc)) {
		r.setFrameColor(FL_RED);
	} else {
		r.setFrameColor(FL_BLACK);
	}
}

Rectangle Cell::getRectangle(){
	return r;
}


void Cell::switchCell(Cell *c1, Cell *c2){
	c1->setStateSwitch(false);
	c2->setStateSwitch(false);
	swap(c1, c2);
	//c1->neighbors = c2->neighbors;
	//c2->neighbors = *temp;
}


void Cell::mouseClick(Point mouseLoc) {
	if(r.contains(mouseLoc)){
		if(getStateSwitch()){
			setStateSwitch(False);
		}else{
			setStateSwitch(True);
			cout << getNeighbors().size() << endl;
			for(auto n: getNeighbors()){
				if(n->getStateSwitch() == true && getStateSwitch() == true){
					int px = this->getRectangle().getCenter().x;
					int py = this->getRectangle().getCenter().y;
					int px_ = n->getRectangle().getCenter().x;
					int py_ = n->getRectangle().getCenter().y;
					int diffx = px - px_;
					int diffy = py - py_;
					cout << px - px_ << " / " << py - py_ << endl;
					if (diffx > 0){
						anim = new Animation(this, 3);
						n->anim = new Animation(n, 4);
					}else if (diffx < 0){
						anim = new Animation(this, 4);
						n->anim = new Animation(n, 3);
					}else if (diffy > 0){
						anim = new Animation(this, 1);
						n->anim = new Animation(n, 2);
					}else{
						anim = new Animation(this, 2);
						n->anim = new Animation(n, 1);
					}
					switchCell(this, n);
				}
			}
		}
	}
}

void Animation::draw(){
	if(type == bounce){
		countBounce--;
		Translation t{{0, countBounce}, c->getRectangle().getImage(), c->getRectangle().getCenter(), c->getRectangle()};
		c->drawWithoutAnimation();
		if(countBounce == -100){
			isCompleted = True;
			delete this;
		}
	}else if (type == bounceLeft){
		countBounce--;
		Translation t{{countBounce, 0}, c->getRectangle().getImage(), c->getRectangle().getCenter(), c->getRectangle()};
		c->drawWithoutAnimation();
		if(countBounce == -96){
			isCompleted = True;
			delete this;
		}
	}else if (type == bounceRight){
		countBounce++;
		Translation t{{countBounce, 0}, c->getRectangle().getImage(), c->getRectangle().getCenter(), c->getRectangle()};
		c->drawWithoutAnimation();
		if(countBounce == 96){
			isCompleted = True;
			delete this;
		}
	}else if (type == bounceHi){
		countBounce++;
		Translation t{{0, countBounce}, c->getRectangle().getImage(), c->getRectangle().getCenter(), c->getRectangle()};
		c->drawWithoutAnimation();
		if(countBounce == 100){
			isCompleted = True;
			delete this;
		}
	}
}


/*--------------------------------------------------

Canvas class.

One instance of the canvas class is made by the
MainWindow class.

The fltk system via MainWindow calls:

draw 60 times a second
mouseMove whenever the mouse is moved
mouseClick whenever the mouse is clicked
keyPressed whenever a key is pressed

Any drawing code should be called ONLY in draw
or methods called by draw. If you try to draw
elsewhere it will probably crash.
--------------------------------------------------*/


class Canvas {
	vector< vector<Cell> > cells;
	public:
	Canvas();
	void draw();
	void mouseMove(Point mouseLoc);
	void mouseClick(Point mouseLoc);
	void keyPressed(int keyCode);
	void actualizeNeighbors();
};

Canvas::Canvas() {
	string niveau;
	ifstream file;
	file.open("niveaux/1bis.txt");
	getline(file, niveau);

	for (int x = 0; x<9; x++) {
		cells.push_back({});
	}

	int elem = 0;
	for (int y = 0; y<9; y++){
		for (int x = 0; x<9; x++){
			int carre = niveau[elem] - '0';
			cells[x].push_back({{96*x+47, 96*y+97}, 95, 95,carre});
			elem++;
		}
	}

	actualizeNeighbors();
}

void Canvas::actualizeNeighbors(){
	for (int y = 0; y < 9; y++){
		for (int x = 0; x < 9; x++){
			Cell &c = cells[x][y];
			if(x + 1 <= 8 && y + 1 <= 8){
				c.addNeighbors(cells[x+1][y]);
				c.addNeighbors(cells[x][y+1]);
			}else if(x + 1 <= 8){
				c.addNeighbors(cells[x+1][y]);
			}else if(y + 1 <= 8){
				c.addNeighbors(cells[x][y+1]);
			}
			
			if(x - 1 >= 0 && y - 1 >= 0){
				c.addNeighbors(cells[x-1][y]);
				c.addNeighbors(cells[x][y-1]);
			}else if(x - 1 >= 0){
				c.addNeighbors(cells[x-1][y]);
			}else if(y - 1 >= 0){
				c.addNeighbors(cells[x][y-1]);
			}
		}
	} 
}


void Canvas::draw() {
	for (auto &v: cells)
		for (auto &c: v)
			c.draw();
}

void Canvas::mouseMove(Point mouseLoc) {
	for (auto &v: cells)
		for (auto &c: v)
			c.mouseMove(mouseLoc);
}

void Canvas::mouseClick(Point mouseLoc) {
	Cell* toSwitch = nullptr;
	Cell* temp = nullptr;
	for (auto &v: cells)
		for (auto &c: v)
			for (auto &c: v) {
				c.mouseClick(mouseLoc);
				if(c.getStateSwitch()){
					if(toSwitch == nullptr){
						toSwitch = &c;
					}else{
						swap(*toSwitch, c);
					}
				}
			}
}

void Canvas::keyPressed(int keyCode) {
	switch (keyCode) {
		case 'q':
			exit(0);
	}
}

/*--------------------------------------------------

MainWindow class.

Do not edit!!!!

--------------------------------------------------*/

class MainWindow : public Fl_Window {
	Canvas canvas;
	public:
	MainWindow() : Fl_Window(500, 50, 863, 913, "Candy Crush") {
		Fl::add_timeout(1.0/60, Timer_CB, this);
		resizable(this);
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
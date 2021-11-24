/*

Projet LDP2 CANDY CRUSH
EDEM LAWSON 000499852
MIGUEL LOZANO 

*/


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



struct ImageBonbon{
	Fl_Box* box;
	Fl_PNG_Image* png;
};

struct CTS {
	Point center_1;
	Point center_2;

	ImageBonbon img_1;
	ImageBonbon img_2;

	Point coord_1;
	Point coord_2;

	int type_1;
	int type_2;
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
  int w, h, id, b_type;
  Fl_Color fillColor;
  Fl_Color frameColor = FL_BLACK;

  Fl_Box* img_box = new Fl_Box(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image* png_img = nullptr;

 public:
  Rectangle(Point center, int w, int h,
            int id, int b_type);

  //setters
  void setFillColor(Fl_Color newFillColor);
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
  void setCenter(Point newC){
        center = newC;
  }
  void setImageBox(Fl_Box* box){
	  img_box = box;
  }

  void setImagePng(Fl_PNG_Image* png){
	  png_img = png;
  }
  //getters
  int getWidth() {
    return w;
  }
  int getHeight() {
    return h;
  }
  Point getCenter() {
    return center;
  }
  Fl_Box* getImageBox(){
	  return img_box;
  }

  Fl_PNG_Image* getImagePng(){
	  return png_img;
  }
  //others
  void init();
  void draw();
  bool contains(Point p);
};

Rectangle::Rectangle(Point center, int w, int h,
                     int id, int b_type):
                     center{center}, w{w}, h{h}, id{id}, b_type{b_type}
                     {init();}

void Rectangle::init(){
		switch (b_type)
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
	img_box->image(png_img);
}

void Rectangle::draw() {    
    //fl_draw_box(FL_FLAT_BOX, center.x-w/2, center.y-h/2, w, h, fillColor);
    fl_draw_box(FL_BORDER_FRAME, center.x-w/2, center.y-h/2, w, h, frameColor);
    Text(to_string(id), {center.x + 30, center.y + 30}).draw();
	img_box->redraw();
}

void Rectangle::setFillColor(Fl_Color newFillColor) {
  fillColor = newFillColor;
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




/*--------------------------------------------------

Cell class.

The Canvas class below will have cells as instance
vraiables and call the methods of Cell
--------------------------------------------------*/


class Cell {
  Rectangle r;
  int color;
  int id, ligne, colonne;
  bool clicked = False;
  vector<Cell *> neighbors;
 public:
  // Constructor
  Cell(Point center, int w, int h, int color, int id, int ligne, int colonne);
  //Setters
  void setclicked(bool value){
      clicked = value;
  }
  void setNeighbors(const vector<Cell *> newNeighbors) {
    neighbors = newNeighbors;
  }

  void setTypeColor(int col){color = col;}
  void setX(int x){ ligne = x;}
  void setY(int y){ colonne= y;}
  // getters
  bool isClicked(){return clicked;}

  vector<Cell *> getNeighbors(){return neighbors;}

  Rectangle &getRect(){return r;}

  int getX(){return ligne;}

  int getY(){return colonne;}

  int getId(){return id;}

  int getTypeColor(){return color;}

  // Methods that draw and handle events
  void draw();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);

};

Cell::Cell(Point center, int w, int h, int color, int id, int ligne, int colonne):
	r(center, w, h,id, color),
	color{color},
	id{id},
	ligne{ligne},
	colonne{colonne}
{}


void Cell::draw() {
    if (clicked){
        r.setFrameColor(FL_WHITE);
    }
    r.draw();
}

void Cell::mouseMove(Point mouseLoc) {
  if (r.contains(mouseLoc)) {
    r.setFrameColor(FL_RED);
  } else {
    r.setFrameColor(FL_BLACK);
  }
}


void Cell::mouseClick(Point mouseLoc) {
    if (r.contains(mouseLoc)){
        if (isClicked())
        {
            setclicked(False);
        }
        else
        {
            setclicked(True);
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
  void resetClicks();
  void checkClicks();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
  void updateNeighbors();
  void checkNeighbors();
  void checkNeighborsX();
  void checkNeighborsY();
  void printCells();
};

Canvas::Canvas() {
    string niveau;
	ifstream file;
    int b_type, id, elem;
	file.open("niveaux/1.txt");

	for (int x = 0; x<9; x++) {
		cells.push_back({});
	}
    
	for (int x = 0; x<9; x++){
        getline(file, niveau);
        elem = 0;
		for (int y = 0; y<9; y++){
			b_type = niveau[elem] - '0';
      id = y + ((x * 9));
			cells[x].push_back({{100*y+50, 100*x+150}, 100, 100,b_type,id, x, y});
			elem++;
		}
	}
    updateNeighbors();
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
    checkClicks();
    bool switched = False;
	  CTS cts;
    for (auto &v: cells){
        for (auto &c: v){
            c.mouseClick(mouseLoc);
            for (auto &n : c.getNeighbors()){
                if(c.isClicked() && n->isClicked()){
                    switched = True;

                    ImageBonbon ib_1 = {c.getRect().getImageBox(), c.getRect().getImagePng()};
                    ImageBonbon ib_2 = {n->getRect().getImageBox(), n->getRect().getImagePng()};

                    Point coord_1 = {c.getX(), c.getY()};
				          	Point coord_2 = {n->getX(), n->getY()};

				          	CTS cts = {c.getRect().getCenter(), n->getRect().getCenter(), ib_1, ib_2, coord_1, coord_2, c.getTypeColor(), n->getTypeColor()};
                }
            }
        }
    }
    
    if (switched)
    {
          cells[cts.coord_1.x][cts.coord_1.y].getRect().setCenter(cts.center_2);
          cells[cts.coord_2.x][cts.coord_2.y].getRect().setCenter(cts.center_1);

          swap(cells[cts.coord_1.x][cts.coord_1.y], cells[cts.coord_2.x][cts.coord_2.y]);// echange les 2 cells dans la liste cells

          //Cell 1

          cells[cts.coord_1.x][cts.coord_1.y].setX(cts.coord_1.x);
          cells[cts.coord_1.x][cts.coord_1.y].setY(cts.coord_1.y);

          cells[cts.coord_1.x][cts.coord_1.y].getRect().setImageBox(cts.img_2.box);
          cells[cts.coord_1.x][cts.coord_1.y].getRect().setImagePng(cts.img_2.png);
          cells[cts.coord_1.x][cts.coord_1.y].getRect().getImageBox()->position(cts.center_1.x-100/2, cts.center_1.y-100/2);

          cells[cts.coord_1.x][cts.coord_1.y].setTypeColor(cts.type_2);

          //Cell 2

          cells[cts.coord_2.x][cts.coord_2.y].setX(cts.coord_2.x);
          cells[cts.coord_2.x][cts.coord_2.y].setY(cts.coord_2.y);

          cells[cts.coord_2.x][cts.coord_2.y].getRect().setImageBox(cts.img_1.box);
          cells[cts.coord_2.x][cts.coord_2.y].getRect().setImagePng(cts.img_1.png);
          cells[cts.coord_2.x][cts.coord_2.y].getRect().getImageBox()->position(cts.center_2.x-100/2, cts.center_2.y-100/2);

          cells[cts.coord_2.x][cts.coord_2.y].setTypeColor(cts.type_1);

          updateNeighbors();
          resetClicks();
          printCells();
          checkNeighbors();
    }
    checkClicks();
}  


void Canvas::resetClicks(){
    //reinitialise l''etat (isClicked()) de toute les cells 
    for (auto &v: cells)
        for (auto &c: v)
            c.setclicked(False);
}

void Canvas::checkClicks(){
  //resetClicks si deux cells sont activées en même temps 
    int count = 0;
    for (auto &v: cells)
        for (auto &c: v){
            if (c.isClicked()){
                count++;
            }   
        }
    if (count == 2)
    {
        resetClicks();
    }    
}



void Canvas::updateNeighbors(){
    // Mets a jour la liste des voisins de toutes les cells
    for (int x = 0; x<9; x++)
        for (int y = 0; y<9; y++) {
            vector<Cell *> neighbors;
            for (auto &shift: vector<Point>({{-1, 0},{ 0, 1},{ 1, 0},{ 0, -1},})) {
                int neighborx = x+shift.x;
                int neighbory = y+shift.y;
                    if (neighborx >= 0 && // Check if the indicies are in range
                        neighbory >= 0 &&
                        neighborx < cells.size() &
                        neighbory < cells[neighborx].size()){
                        neighbors.push_back(&cells[neighborx][neighbory]);
                        cells[x][y].setNeighbors(neighbors);
                    }
            }
        }
}

void Canvas::checkNeighbors(){
  cout<< "--------------Start Checking----------"<< endl << endl;
	checkNeighborsX();
	checkNeighborsY();
  cout<< "-------------- Check done -------------"<< endl << endl;
}

struct recurCount
{
  int color, amount;
  Point start, finish;
};

class Recurrence {
    //TODO changer le point en structure de 2 int et 2 point (b_type, recurrence, Point(indice debut dans cells), Point(indice fin dans cells))
    // isPouf() retourne les indice de debut et de fin de la chaine
    vector<recurCount> recu;
  public:
    //setters
    void setVec(vector<recurCount> newVec){recu = newVec;}
    //getters
    vector<recurCount> &getVec(){return recu;}
    //others
    void add(recurCount newP); 
    bool isPouf();
};

void Recurrence::add(recurCount newP){
      // add si le dernier point de recu a une couleur differente de celle de newP
      bool lastRecu = False;
      if (!(recu.empty())){
        if (recu.back().color == newP.color)
        {
          lastRecu = True;
        }
      }
      if(!(lastRecu)){
        recu.push_back(newP);
      }
}
bool Recurrence::isPouf(){
    bool pouf = false;
    for(auto &elem : recu){
			if(elem.amount >= 3){
				cout << "Allignement de " << elem.amount << " bonbons de couleur " << elem.color;
        cout << " de la ligne " << elem.start.x << " à " << elem.finish.x << " et de la colonne "<< elem.start.y << " à " << elem.finish.y << endl ;
        pouf = True;
			}
    }
    return pouf;
}

void Canvas::checkNeighborsX(){
	for(int x = 0; x < 9; x++){
		int lastcolor = -1;
		Recurrence recurrence;
		for(int y = 0; y < 9; y++){
			Cell &c = cells[x][y];
			int current = c.getTypeColor();
			recurrence.add({current, 1, {x, y}, {x, y}});
			if (lastcolor == current){
				recurrence.getVec().back().amount++;
        recurrence.getVec().back().finish = {x,y};
			}else{
				lastcolor = current;
			}
		}
		if (recurrence.isPouf()){cout<<"        Pouf sur horrizontal"<<endl<<endl;}
	}
}

void Canvas::checkNeighborsY(){
	for(int x = 0; x < 9; x++){
		int lastcolor = -1;
		Recurrence recurrence;
		for(int y = 0; y < 9; y++){
			Cell &c = cells[y][x];
			int current = c.getTypeColor();
			recurrence.add({current, 1});
			if (lastcolor == current){
				recurrence.getVec().back().amount++;
			}else{
				lastcolor = current;
			}
		}
		if (recurrence.isPouf()){cout<<"        Pouf sur vertical"<<endl;}
	}
}



void Canvas::printCells(){
    for (auto &v: cells){
        for (auto &c: v){
            cout << c.getId() << " ; ";
        }
        cout << endl;
    }
    cout << "--------------------------------------" << endl;
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
  MainWindow() : Fl_Window(500, 50, 900, 1000, "Candy Crush by Edem and Miguel") {
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
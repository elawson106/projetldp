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

struct recurCount
  //stocke la couleur, la taille et les coordonnées(debut et fin) des chaines de bonbons du plateau
{
  int color, amount;
  Point start, finish;
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
   
  void setFrameColor(Fl_Color newFrameColor);
    
  void setWidth(int neww) {w = neww;}

  void setHeight(int newh) {h = newh;}

  void setCenter(Point newC) {center = newC;}

  void setImageBox(Fl_Box* box) {img_box = box;}

  void setImagePng(Fl_PNG_Image* png) {png_img = png;}

  void setImageBonbon(ImageBonbon img_bonbon) {
    img_box = img_bonbon.box;
    png_img = img_bonbon.png;
  }
  //getters
  int getWidth() {return w;}

  int getHeight() {return h;}

  Point getCenter() {return center;}

  Fl_Color getFrameColor() {return frameColor;}

   Fl_Color getFillColor() {return fillColor;}

  ImageBonbon getImageBonbon() {return {img_box, png_img};}

  Fl_Box* getImageBox() {return img_box;}

  Fl_PNG_Image* getImagePng() {return png_img;}
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
  void setTypeColor(int col){color = col;}
  void setX(int x){ ligne = x;}
  void setY(int y){ colonne= y;}
  void setclicked(bool value){
      clicked = value;
  }
  void setNeighbors(const vector<Cell *> newNeighbors) {
    neighbors = newNeighbors;
  }
  void setCoord(Point xy) {
    ligne = xy.x;
    colonne = xy.y;
  }
  // getters
  int getX(){return ligne;}

  int getY(){return colonne;}

  int getId(){return id;}

  int getTypeColor(){return color;}

  Point getCoord() {return {ligne, colonne};}

  bool isClicked(){return clicked;}

  vector<Cell *> getNeighbors(){return neighbors;}

  Rectangle &getRect(){return r;}

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

class Recurrence {
    // vecteur de compteurs de chaines(recurCount)
    vector<recurCount> recu;
  public:
    //getters
    vector<recurCount> &getVec(){return recu;}
    //others
    void add(recurCount newP); 
    bool isPouf();
};

void Recurrence::add(recurCount newP){
      // add si le dernier point du vecteur a une couleur differente de celle de newP
      if (((!recu.empty()) && recu.back().color != newP.color) || (recu.empty())){
        recu.push_back(newP);
      }
}
bool Recurrence::isPouf(){
    // return true si il y a eu une chaine >= 3 dans le vecteur
    bool pouf = false;
    for(auto &elem : recu){
			if(elem.amount >= 3){
				cout << "Allignement de " << elem.amount << " bonbons de couleur " << elem.color << endl;
        cout << "de la ligne " << elem.start.x << " à " << elem.finish.x << " et de la colonne "<< elem.start.y << " à " << elem.finish.y << endl ;
        pouf = True;
			}
    }
    return pouf;
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
	vector<Cell *> toSwap;

  Fl_Box* img_box = new Fl_Box(0, 0, 0, 0);
  Fl_PNG_Image* png_blank = new Fl_PNG_Image("bonbon/blank.png");
 public:
  Canvas();
  void draw();
  void resetClicks();
  void checkClicks();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void keyPressed(int keyCode);
  void updateNeighbors();
  void switchCells(CTS cts);
  void checkNeighbors();
  void checkNeighborsX();
  void checkNeighborsY();
  void pouf(Recurrence recurrence);
  void setNulls();
  void decalageG(vector< vector<Cell> > &cells);
  void printCells();
  void swapP();
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

                    ImageBonbon ib_1 = c.getRect().getImageBonbon();
                    ImageBonbon ib_2 = n->getRect().getImageBonbon();

                    Point coord_1 = c.getCoord();
					Point coord_2 = n->getCoord();

					CTS cts = {c.getRect().getCenter(), n->getRect().getCenter(), ib_1, ib_2, coord_1, coord_2, c.getTypeColor(), n->getTypeColor()};
                }
            }
        }
    }
    if (switched)
    {
          switchCells(cts);
		  checkNeighbors();
    }
    checkClicks();
}  

void Canvas::switchCells(CTS cts){
	cells[cts.coord_1.x][cts.coord_1.y].getRect().setCenter(cts.center_2);
	cells[cts.coord_2.x][cts.coord_2.y].getRect().setCenter(cts.center_1);

	swap(cells[cts.coord_1.x][cts.coord_1.y], cells[cts.coord_2.x][cts.coord_2.y]);  // echange les 2 cells dans la liste cells

	//Cell 1

	cells[cts.coord_1.x][cts.coord_1.y].setCoord({cts.coord_1.x, cts.coord_1.y});

	cells[cts.coord_1.x][cts.coord_1.y].getRect().setImageBonbon({cts.img_2.box, cts.img_2.png});
	cells[cts.coord_1.x][cts.coord_1.y].getRect().getImageBox()->position(cts.center_1.x-100/2, cts.center_1.y-100/2);

	cells[cts.coord_1.x][cts.coord_1.y].setTypeColor(cts.type_2);

	//Cell 2

	cells[cts.coord_2.x][cts.coord_2.y].setCoord({cts.coord_2.x,cts.coord_2.y});

	cells[cts.coord_2.x][cts.coord_2.y].getRect().setImageBonbon({cts.img_1.box, cts.img_1.png});
	cells[cts.coord_2.x][cts.coord_2.y].getRect().getImageBox()->position(cts.center_2.x-100/2, cts.center_2.y-100/2);

	cells[cts.coord_2.x][cts.coord_2.y].setTypeColor(cts.type_1);

	updateNeighbors();
	resetClicks();
	//printCells();
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

void Canvas::setNulls(){
    // mets a zero la color de toutes les cells pointant vers blank (celles qui viennent d'exploser)
    for (auto &v: cells)
        for (auto &c: v){
          if (c.getRect().getImageBox()->image() == png_blank)
          {
            c.setTypeColor(0);
          }
    }
}

void Canvas::swapP(){
	for(auto &cc : toSwap){
		if(cc->getX() > 0){
			int counter = 1;
			Point base = {cc->getX() - 1, cc->getY()};
			while (cells[base.x][base.y].getTypeColor() != 0 && base.x > 0)
			{	
				//cout << "TEEEEEEEEEST   1 - " << base.x << endl;
				Cell &temp = cells[base.x][base.y];
				//cout << "TEEEEEEEEEST   2 - " << base.x << endl;
				CTS cts = {cc->getRect().getCenter(), temp.getRect().getCenter(), 
							cc->getRect().getImageBonbon(), temp.getRect().getImageBonbon(), 
							cc->getCoord(), temp.getCoord(), cc->getTypeColor(), temp.getTypeColor()};
				//cout << "TEEEEEEEEEST    3 - " << base.x << endl;
				cout << "TEEEEEEEEEST    --- - " << cc->getX() << " - " << temp.getX() << endl;
				switchCells(cts);
				setNulls();
				//cout << "TEEEEEEEEEST    4 - " << base.x << endl;
				base.x = temp.getX();
				base.y = temp.getY();
				cout << "TEEEEEEEEEST    6 - " << cc->getX() << " - " << temp.getX() << endl;
				//checkNeighbors();
			}
		}
	}

	toSwap.clear();
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
  	setNulls();
	swapP();
  cout<< "-------------- Check done -------------"<< endl << endl;
}

void Canvas::checkNeighborsX(){
  //check si il y a des allignement de min 3 sur les horrizontales 
	for(int x = 0; x < 9; x++){
		int lastcolor = -1;
		Recurrence recurrence;
		for(int y = 0; y < 9; y++){
			Cell &c = cells[x][y];
			int current = c.getTypeColor();     // Prends la couleur de la case actuelle 
			recurrence.add({current, 1, {x, y}, {x, y}});     // Initialise son compteur dans le vecteur de recurrence/ne fait rien si on est dans une chaine
			if (lastcolor == current){
				recurrence.getVec().back().amount++;            // incremente la compteur de la couleur current si il y a une chaine
        		recurrence.getVec().back().finish = {x,y};      // redefinis la fin de la chaine
			}else{
				lastcolor = current;
			}
		}
		if (recurrence.isPouf()){
			pouf(recurrence);
			cout<<"        Pouf sur horizontal"<<endl<<endl;}
	}
}

void Canvas::checkNeighborsY(){
  //check si il y a des allignement de min 3 sur les verticales 
  for(int x = 0; x < 9; x++){
      int lastcolor = -1;
      Recurrence recurrence;
      for(int y = 0; y < 9; y++){
        Cell &c = cells[y][x];
        int current = c.getTypeColor(); 
        recurrence.add({current, 1, {y, x}, {y, x}});  
        if (lastcolor == current){
          recurrence.getVec().back().amount++;        
          recurrence.getVec().back().finish = {y,x};   
        }else{
          lastcolor = current;                         
        }
      }
      if (recurrence.isPouf()){
        pouf(recurrence);
        cout<<"        Pouf sur vertical"<<endl<<endl;}
        
    }
  }

void Canvas::pouf(Recurrence recurrence){
    //mets les images de toutes les cells en caine de min 3 a blank (implémenter explosion)
    for (auto &count : recurrence.getVec()){
		if(count.amount >= 3){
			for(int i = count.start.x; i <= count.finish.x; i++){
				for(int j = count.start.y; j <= count.finish.y; j++){
					cells[i][j].getRect().getImageBox()->image(png_blank);
					toSwap.push_back(&cells[i][j]);
				}
			}
		}
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
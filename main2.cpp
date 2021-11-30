/*

Projet LDP2 CANDY CRUSH
EDEM LAWSON 000499852
MIGUEL LOZANO 

*/

/*
 TODO definir tous les bonbons une fois dans canvas, faire passer leur adresse dans cell -> rectangle (vercteur<b_rouge, b_vert, etc)
 comme ça on les initialise une seule fois et on ne fait pas trop de new + peut etre faire le switchcase initial de couleur dans canvas
 et juste travailler avec set_imgbox
 rajouter par niveau une ligne contenant tous les bonbons speciaux qui peuvent y apparaitre pour les initialiser aussi
 a la fin du niveau for dans la liste des pointeurs et delete tous (pour contrer le new)
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
#include <thread>
#include <unistd.h>

using namespace std;


struct Point {
	int x, y;
};

struct Color_Image
// stocke pour chaque type de bonbon son indice d'image color et l'adresse de son image en memeoire locImg
{
    int color;
    Fl_PNG_Image* locImg;
};

struct RecurCount
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

Classe Recurrence

Classe comportant un vecteur de compteur, compteur qui contient:
int color : la couleur qui se répète
int amount : la taille de la chaine
Point start : Point contenant les coords de debut de la chaine
Point finish : Point contenant les coords de fin de la chaine

--------------------------------------------------*/
class Recurrence {
    // vecteur de compteurs de chaines(recurCount)
    vector<RecurCount> recu;
  public:
    //getters
    vector<RecurCount> &getVec(){return recu;}
    //others
    void add(RecurCount newP); 
    bool isPouf();
};

void Recurrence::add(RecurCount newP){
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
        pouf = True;
			}
    }
    return pouf;
}


/*--------------------------------------------------

Classe Img_vector

Classe comportant un vecteur de paires, paires Color_image qui contient:
int color : l'indice de la couleur qu'il représente
Fl_PNG_Image* : l'adresse ou est initialiséé l'image correspondante

--------------------------------------------------*/
class Img_vector {
    vector<Color_Image> images;
   public:
  // Constructor
    Img_vector() {init();};
  // getters
    Fl_PNG_Image* blank(){return images[0].locImg;}
    Color_Image getImginf(int color);
  // others
    void init();
    void add(int color);
};

void Img_vector::init(){
  //initialise le vecteur avec Blank comme premier element
  Fl_PNG_Image* png_blank = new Fl_PNG_Image("Images/bonbon/blank.png");
  images.push_back({0, png_blank});
}

void Img_vector::add(int color) {
  //initialise l'image et l'ajoute dans le vecteur avec son int color, sous forme de color_image
  Fl_PNG_Image* png_img;
  bool in = False;
  for (auto &img : images)
    if (img.color == color){
      in = True;
    }
  if (!(in))
  {
    switch (color)
      {
      case 1:
      //basiques
        png_img = new Fl_PNG_Image("Images/bonbon/tile000.png");
        images.push_back({1, png_img});
        break;
      case 2:
        png_img = new Fl_PNG_Image("Images/bonbon/tile001.png");
        images.push_back({2, png_img});
        break;
      case 3:
        png_img = new Fl_PNG_Image("Images/bonbon/tile002.png");
        images.push_back({3, png_img});
        break;
      case 4:
        png_img = new Fl_PNG_Image("Images/bonbon/tile003.png");
        images.push_back({4, png_img});
        break;
      case 5:
        png_img = new Fl_PNG_Image("Images/bonbon/tile004.png");
        images.push_back({5, png_img});
        break;
      case 6:
        png_img = new Fl_PNG_Image("Images/bonbon/tile005.png");
        images.push_back({6, png_img});
        break;
      //sweepers
      case 7:
        png_img = new Fl_PNG_Image("Images/bonbon/tile014.png");
        images.push_back({7, png_img});
        break;
      case 8:
        png_img = new Fl_PNG_Image("Images/bonbon/tile015.png");
        images.push_back({8, png_img});
        break;
      case 9:
        png_img = new Fl_PNG_Image("Images/bonbon/tile016.png");
        images.push_back({9, png_img});
        break;
      case 10:
        png_img = new Fl_PNG_Image("Images/bonbon/tile017.png");
        images.push_back({10, png_img});
        break;
      case 11:
        png_img = new Fl_PNG_Image("Images/bonbon/tile018.png");
        images.push_back({11, png_img});
        break;
      case 12:
        png_img = new Fl_PNG_Image("Images/bonbon/tile019.png");
        images.push_back({12, png_img});
        break;
      //bombes
      case 13:
        png_img = new Fl_PNG_Image("Images/bonbon/tile028.png");
        images.push_back({13, png_img});
        break;
      case 14:
        png_img = new Fl_PNG_Image("Images/bonbon/tile029.png");
        images.push_back({14, png_img});
        break;
      case 15:
        png_img = new Fl_PNG_Image("Images/bonbon/tile030.png");
        images.push_back({15, png_img});
        break;
      case 16:
        png_img = new Fl_PNG_Image("Images/bonbon/tile031.png");
        images.push_back({16, png_img});
        break;
      case 17:
        png_img = new Fl_PNG_Image("Images/bonbon/tile032.png");
        images.push_back({17, png_img});
        break;
      case 18:
        png_img = new Fl_PNG_Image("Images/bonbon/tile033.png");
        images.push_back({18, png_img});
        break;
              
      default:
        break;
      }
  }
  
}

Color_Image Img_vector::getImginf(int color) {
  //renvoie la {color_image} de la couleur color d'entrée
  for (auto &img_inf : images)
    if (img_inf.color == color){
      return img_inf;
    }
  return images[0];   // si on ne trouve pas l'image, return Blank
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
  int w, h, id;
  Fl_Color fillColor;
  Fl_Color frameColor = FL_BLACK;

  Fl_Box* img_box = new Fl_Box(center.x-w/2, center.y-h/2, w, h);
  Fl_PNG_Image* png_img;

 public:
  Rectangle(Point center, int w, int h,
            int id, Fl_PNG_Image* locpng);

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
                     int id, Fl_PNG_Image*  locpng):
                     center{center}, w{w}, h{h}, id{id}, png_img{locpng}
                     {init();}

void Rectangle::init(){
	img_box->image(png_img);
}

void Rectangle::draw() {    
	img_box->redraw();
    //fl_draw_box(FL_BORDER_FRAME, center.x-w/2, center.y-h/2, w, h, frameColor);
    Text(to_string(id), {center.x + 30, center.y + 30}).draw();
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


struct Translation {
  Translation(Point p) {
    fl_push_matrix();
    fl_translate(p.x, p.y);
  }
  ~Translation() {
    fl_pop_matrix();
  }
};




/*--------------------------------------------------

Cell class.

The Canvas class below will have cells as instance
vraiables and call the methods of Cell
--------------------------------------------------*/
class Animation;

class Cell {
  Rectangle r;
  int color;
  int id, ligne, colonne;
  bool clicked = False;
  vector<Cell *> neighbors;
  Animation* anim;
 public:
  // Constructor
  Cell(Point center, int w, int h, Color_Image color, int id, int ligne, int colonne);
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

  Animation *getAnim(){ return anim;}

  // Methods that draw and handle events
  void draw();
  void drawWithoutAnimation();
  void mouseMove(Point mouseLoc);
  void mouseClick(Point mouseLoc);
  void setAnimation(Animation *a);

};


class Animation {

	public:
		enum AnimationType{swap, explode};

	private:
		const int animationTime = 20;
		const int bounceHeight = 200;
		int counter = 0;
		int other = 0;
		int max = 100;
		Cell *base;
		Cell *base2;
    Point coord_base;
    Point coord_base2;

		AnimationType animationType;
		int time{0};
		Point currentTranslation();
	public:
	Animation(Cell* cellToAnimate, Cell* cellAutre, AnimationType animationType):
		base{cellToAnimate}, base2{cellAutre}, animationType{animationType} {
      coord_base = base->getRect().getCenter();
      coord_base2 = base2->getRect().getCenter();
    }
	void draw();
	bool isComplete();

};

void Animation::draw(){
		time++;
		time++;
		base->getRect().getImageBox()->position(currentTranslation().x, currentTranslation().y);
		base->getRect().setCenter({currentTranslation().x + 50, currentTranslation().y + 50});
		base->drawWithoutAnimation();
	//base->drawWithoutAnimation();
}

Point Animation::currentTranslation(){
	int b_x = coord_base.x;
	int b_y = coord_base.y;
	int b2_x = coord_base2.x;
	int b2_y = coord_base2.y;
	int dif_x = b_x - b2_x;
	int dif_y = b_y - b2_y;

	if(dif_y > 0){   // b est en dessous de b2
		return {b_x - 50, b_y -50 - time};
	}else if(dif_y < 0){     // b est au dessu de b2
		return {b_x - 50, b_y -50  + time};
	}

	else if(dif_x > 0){   // b est a droite de b2
		return {b_x -50 - time, b_y - 50};
	}else if(dif_x < 0){    //b est a gauche de b2
		return {b_x -50 + time, b_y - 50};
	}
}

bool Animation::isComplete(){
	return time > max;
}

Cell::Cell(Point center, int w, int h, Color_Image color, int id, int ligne, int colonne):
	r(center, w, h,id, color.locImg),
	color{color.color},
	id{id},
	ligne{ligne},
	colonne{colonne},
	anim{nullptr}
{}


void Cell::draw() {
    if(anim && anim->isComplete()){
		delete anim;
		anim = nullptr;
	}
	if(anim){
		anim->draw();
	}else{
		drawWithoutAnimation();
	}
}

void Cell::drawWithoutAnimation(){
	if (clicked){
        fl_draw_box(FL_BORDER_FRAME, r.getCenter().x-r.getWidth()/2, r.getCenter().y-r.getHeight()/2, r.getWidth(), r.getHeight(), FL_WHITE);
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
	if (!anim)
	{
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
	
    
}

void Cell::setAnimation(Animation *a){
	anim = a;
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
  vector<Point> toSwap;
  ImageBonbon background;
  ImageBonbon bestScore;
  ifstream file;
  int score = 0;
  int highscore;
  Img_vector images;

  bool inAnim = False;
 public:
  Canvas();
  void initBG();
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
  bool setNulls();
  void swapUP();
  void addscore(int longeur);
  void updatehigh();
  void printCells();
  void setrandcolor();
};

Canvas::Canvas() {
  initBG();
  string niveau;
  int b_type, id, elem;
	file.open("niveaux/n1/1.txt");

	for (int x = 0; x<9; x++) {
		cells.push_back({});
	}
	for (int x = 0; x<9; x++){
      getline(file, niveau);
      elem = 0;
      for (int y = 0; y<9; y++){
        b_type = niveau[elem] - '0';
        images.add(b_type);
        id = y + ((x * 9));
        cells[x].push_back({{100*y+50, 100*x+150}, 100, 100,images.getImginf(b_type),id, x, y});
        elem++;
     }
	}
  file.close();
  file.open("niveaux/n1/h1.txt");
  string high;
  getline(file, high);
  highscore = stoi(high);
  file.close();
  updateNeighbors();
}

void Canvas::initBG(){
  background.box = new Fl_Box(0, 0, 900, 1000);
	background.png = new Fl_PNG_Image("Images/Misc/background.png");
	background.box->image(background.png);

	bestScore.box = new Fl_Box(600, 25, 200, 50);
	bestScore.png = new Fl_PNG_Image("Images/Misc/highscore.png");
	bestScore.box->image(bestScore.png);
}

void Canvas::draw() {
  Text(to_string(highscore), {850, 50}, 20).draw();
  Text(to_string(score), {100, 50}, 20).draw();
  Cell *temp = nullptr;
  bool switched = False;
  bool temp_anim;
  CTS cts;
  for (auto &v: cells)
    for (auto &c: v){
		if(c.getAnim()){
			temp_anim = True;
		}
		if(temp == nullptr && c.isClicked()){
			temp = &c;
			cout << "ok - " << temp << endl;
		}else if(c.isClicked()){
			cout << "ogay" << endl;
			for (auto &n : c.getNeighbors()){
                if(c.isClicked() && n->isClicked()){
                    switched = True;
                    ImageBonbon ib_1 = c.getRect().getImageBonbon();
                    ImageBonbon ib_2 = n->getRect().getImageBonbon();
                    Point coord_1 = c.getCoord();
				          	Point coord_2 = n->getCoord();
                   cout << "okey" << endl;

				          	cts = {c.getRect().getCenter(), n->getRect().getCenter(), ib_1, ib_2, coord_1, coord_2, c.getTypeColor(), n->getTypeColor()};
                }
            }
		}
		if (switched && !inAnim)
    	{
			inAnim = True;
			temp_anim = True;
			switchCells(cts);
			  resetClicks();
			checkNeighbors();
    	}
      c.draw();
	}
	cout << "TEMP ANIM - " << temp_anim << endl;
	if(!temp_anim){
		inAnim = False;
	}
	checkNeighbors();
	checkClicks();
}

void Canvas::mouseClick(Point mouseLoc) {
    for (auto &v: cells){
        for (auto &c: v){
            c.mouseClick(mouseLoc);
        }
    }
    
    //checkClicks();
}  

void Canvas::mouseMove(Point mouseLoc) {
  for (auto &v: cells)
    for (auto &c: v)
      c.mouseMove(mouseLoc);
}

void Canvas::switchCells(CTS cts){
          //printf("dddddd");
		  cout << "mag - " << cts.coord_1.x << " - " << cts.coord_1.y<< endl;
          cells[cts.coord_1.x][cts.coord_1.y].getRect().setCenter(cts.center_2);
          cells[cts.coord_2.x][cts.coord_2.y].getRect().setCenter(cts.center_1);
		  cout << "mag" << endl;
				Animation *a = new Animation(&cells[cts.coord_1.x][cts.coord_1.y], &cells[cts.coord_2.x][cts.coord_2.y], static_cast<Animation::AnimationType>(0));
			cells[cts.coord_2.x][cts.coord_2.y].setAnimation(a);
			Animation *aa = new Animation(&cells[cts.coord_2.x][cts.coord_2.y], &cells[cts.coord_1.x][cts.coord_1.y], static_cast<Animation::AnimationType>(0));
			cells[cts.coord_1.x][cts.coord_1.y].setAnimation(aa);
		swap(cells[cts.coord_1.x][cts.coord_1.y], cells[cts.coord_2.x][cts.coord_2.y]);  // echange les 2 cells dans la liste cellsµ
		  cout << "mag" << endl;
          //Cell 1
		
          cells[cts.coord_1.x][cts.coord_1.y].setCoord({cts.coord_1.x, cts.coord_1.y});

          cells[cts.coord_1.x][cts.coord_1.y].getRect().setImageBonbon({cts.img_2.box, cts.img_2.png});
          //cells[cts.coord_1.x][cts.coord_1.y].getRect().getImageBox()->position(cts.center_1.x-100/2, cts.center_1.y-100/2);

          cells[cts.coord_1.x][cts.coord_1.y].setTypeColor(cts.type_2);

          //Cell 2

          cells[cts.coord_2.x][cts.coord_2.y].setCoord({cts.coord_2.x,cts.coord_2.y});

          cells[cts.coord_2.x][cts.coord_2.y].getRect().setImageBonbon({cts.img_1.box, cts.img_1.png});
          //cells[cts.coord_2.x][cts.coord_2.y].getRect().getImageBox()->position(cts.center_2.x-100/2, cts.center_2.y-100/2);

          cells[cts.coord_2.x][cts.coord_2.y].setTypeColor(cts.type_1);

          //printCells();
          updateNeighbors();
          
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
				cout << "ouisiti - " << count << endl;
            }   
        }
    if (count == 2)
    {
        resetClicks();
    }    
}
void Canvas::setrandcolor(){
	if(!inAnim && toSwap.size() == 0){
		for(auto &v : cells)
    for (auto &c : v){
      if (c.getTypeColor() == 0){
        int randColor = (rand() % 6) + 1;
		c.getRect().getImageBonbon().box->position(c.getRect().getImageBonbon().box->x(), c.getRect().getImageBonbon().box->y() - 100);
		c.getRect().setCenter({c.getRect().getCenter().x, c.getRect().getCenter().y - 100});
        c.setTypeColor(randColor);
		Cell c_ = {{c.getRect().getCenter().x, c.getRect().getCenter().y + 100}, 100, 100, images.getImginf(1), c.getId(), c.getX(), c.getY()};
		Animation *a = new Animation(&c, &c_, static_cast<Animation::AnimationType>(0));
		c.setAnimation(a);
        c.getRect().getImageBonbon().box->image(images.getImginf(randColor).locImg); 
      }
    }
	}
}


bool Canvas::setNulls(){
    // mets a zero la color de toutes les cells pointant vers blank (celles qui viennent d'exploser)
    bool poufed = false;
    for (auto &v: cells)
        for (auto &c: v){
          if (c.getRect().getImageBox()->image() == images.blank())
          {
            c.setTypeColor(0); 
            printf("gfd");
            poufed = true;
          }
        }
    return poufed;
}
    
void Canvas::swapUP(){
    // decale la chaine de cells qui vient de pop (toSwap) vers le haut du tableau
    for(auto point : toSwap){
      int counter = 1;
      if(point.x > 0){
        while ((point.x-counter) >= 0 && cells[point.x-counter][point.y].getX() >= 0 && cells[point.x-counter][point.y].getTypeColor() != 0) 
        {
          CTS cts = {cells[point.x-counter+1][point.y].getRect().getCenter(), cells[point.x-counter][point.y].getRect().getCenter(),
                cells[point.x-counter+1][point.y].getRect().getImageBonbon(), cells[point.x-counter][point.y].getRect().getImageBonbon(),
                cells[point.x-counter+1][point.y].getCoord(), cells[point.x-counter][point.y].getCoord(),
                cells[point.x-counter+1][point.y].getTypeColor(), cells[point.x-counter][point.y].getTypeColor()};
				inAnim = True;
          switchCells(cts);
          //CTS.1 vide CTS.2 Celle au dessus de la vide
          counter++;
        }
      }
    }
    toSwap.clear();
	setrandcolor();
	checkNeighbors();
    printCells();
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
	if(!inAnim){
		checkNeighborsX();
		checkNeighborsY();
		if (setNulls())
		{
			swapUP();
		}
	}
}

void Canvas::checkNeighborsX(){
  //check si il y a des allignement de min 3 sur les horizontales 
 for(int x = 0; x < 9; x++){
      int lastcolor = -1;
      Recurrence recurrence;
      for(int y = 0; y < 9; y++){
        int current = cells[x][y].getTypeColor(); 
        if (current != 0){
          recurrence.add({current, 1, {x, y}, {x, y}});  
          if (lastcolor == current){
            recurrence.getVec().back().amount++;        
            recurrence.getVec().back().finish = {x,y};   
          }
          else{
          lastcolor = current;                         
          }
        }
      }
      if (recurrence.isPouf()){
        pouf(recurrence);
      }
  }
}

void Canvas::checkNeighborsY(){
  //check si il y a des allignement de min 3 sur les verticales 
  for(int x = 0; x < 9; x++){
      int lastcolor = -1;
      Recurrence recurrence;
      for(int y = 0; y < 9; y++){
        int current = cells[y][x].getTypeColor(); 
        if (current != 0){
          recurrence.add({current, 1, {y, x}, {y, x}});  
          if (lastcolor == current){
            recurrence.getVec().back().amount++;        
            recurrence.getVec().back().finish = {y,x};   
          }
          else{
          lastcolor = current;                         
          }
        }
      }
      if (recurrence.isPouf()){
        pouf(recurrence);
      }
  }
}

void Canvas::pouf(Recurrence recurrence){
    //mets les images de toutes les cells en caine de min 3 a blank (implémenter explosion)
    for (auto &count : recurrence.getVec()){
      if(count.amount >= 3){
        for(int i = count.start.x; i <= count.finish.x; i++){
          for(int j = count.start.y; j <= count.finish.y; j++){
            cells[i][j].getRect().getImageBox()->image(images.blank());
            toSwap.push_back({i, j});
          }
        }
      }
    }
    addscore(toSwap.size());
}

void Canvas::addscore(int longeur){
    score = score + (100 * longeur);
    if (score > highscore)
    {
      highscore = score;  
      file.open("niveaux/n1/h1.txt");
      ofstream temp("niveaux/n1/h1.txt");
      temp << highscore;
      temp.rdbuf();
      temp.close();
      file.close();
    }
    
}

void Canvas::printCells(){
    for (auto &v: cells){
        for (auto &c: v){
          if (c.getId() < 10)
          {
            cout << "0" << c.getId() << " ; ";
          }
          else{
            cout << c.getId() << " ; ";
          }
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
    Fl::repeat_timeout(1.0/20, Timer_CB, userdata);
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
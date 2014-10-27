#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>

#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"


#ifndef GameObject_H
#define GameObject_H

class vec2i
{
public:
    vec2i()
        : x_(0), y_(0)
    {}
    vec2i(int x, int y)
        : x_(x), y_(y)
    {}
    int get_x() const
    {
        return x_;
    }
    int get_y() const
    {
        return y_;
    }
    int set_x(int x)
    {
        x_ = x;
    }
    int set_y(int y)
    {
        y_ = y;
    }
    vec2i & operator+=(const vec2i & v)
    {
        x_ += v.x_;
        y_ += v.y_;
        return (*this);
    }
    int & operator[](int i)
    {
        return (i == 0 ? x_ : y_);
    }
private:
    int x_, y_;
};



class Object
{
public:
    void score(Surface & surface, int score)
    {
        //printing score
        Font font("fonts/FreeMonoBold.ttf", 25);
        Color c = {255,255,0};
        Color b = {225,0, 0};
        
        Image scoretext(font.render("SCORE: ",c));
        Rect score_text = scoretext.getRect();
        score_text.x = 250;
        score_text.y = H - 200;
        
        surface.put_image(scoretext, score_text);
        
        //converting int to string
        std::stringstream strmsc;
        strmsc << score;
        
        Image yourscore(font.render(strmsc.str().c_str(), b));
        Rect your_score = yourscore.getRect();
        your_score.x = 340;
        your_score.y = H - 200;
        
        surface.put_image(yourscore, your_score);
    }
    
    void level(Surface & surface, int level)
    {
        //printing level
        Font font("fonts/FreeMonoBold.ttf", 25);
        Color c = {255,255,0};
        Color b = {225,0, 0};
        
        Image imagelevel(font.render("Level: ",c));
        Rect leveltext = imagelevel.getRect();
        leveltext.x = 480;
        leveltext.y = H - 35;
        
        surface.put_image(imagelevel, leveltext);
        
        //converting int to string
        std::stringstream strmsc;
        strmsc << level;
        
        Image leveler(font.render(strmsc.str().c_str(), b));
        Rect levels = leveler.getRect();
        levels.x = 575;
        levels.y = H - 35;
        
        surface.put_image(leveler, levels);
    }

    void red(Surface & surface, int red)
    {
        //printing level
        Font font("fonts/FreeMonoBold.ttf", 25);
        Color c = {255,255,0};
        Color b = {225,0, 0};
        
        Image imagelevel(font.render("Red: ",c));
        Rect leveltext = imagelevel.getRect();
        leveltext.x = 270;
        leveltext.y = 100 + 32 * 1;
        
        surface.put_image(imagelevel, leveltext);
        
        //converting int to string
        std::stringstream strmsc;
        strmsc << red;
        
        Image leveler(font.render(strmsc.str().c_str(), b));
        Rect levels = leveler.getRect();
        levels.x = 330;
        levels.y = 100 + 32 * 1;
        
        surface.put_image(leveler, levels);
    }

    
    void flagship(Surface & surface, int flagship)
    {
        //printing level
        Font font("fonts/FreeMonoBold.ttf", 25);
        Color c = {255,255,0};
        Color b = {225,0, 0};
        
        Image imagelevel(font.render("FlagShip: ",c));
        Rect leveltext = imagelevel.getRect();
        leveltext.x = 270;
        leveltext.y = 100;
        
        surface.put_image(imagelevel, leveltext);
        
        //converting int to string
        std::stringstream strmsc;
        strmsc << flagship;
        
        Image leveler(font.render(strmsc.str().c_str(), b));
        Rect levels = leveler.getRect();
        levels.x = 410;
        levels.y = 100;
        
        surface.put_image(leveler, levels);
    }

    
    void purple(Surface & surface, int purple)
    {
        //printing level
        Font font("fonts/FreeMonoBold.ttf", 25);
        Color c = {255,255,0};
        Color b = {225,0, 0};
        
        Image imagelevel(font.render("Purple: ",c));
        Rect leveltext = imagelevel.getRect();
        leveltext.x = 270;
        leveltext.y = 100 + 36 * 2;
        
        surface.put_image(imagelevel, leveltext);
        
        //converting int to string
        std::stringstream strmsc;
        strmsc << purple;
        
        Image leveler(font.render(strmsc.str().c_str(), b));
        Rect levels = leveler.getRect();
        levels.x = 380;
        levels.y = 100 + 36 * 2;
        
        surface.put_image(leveler, levels);
    }

    
    void blue(Surface & surface, int blue)
    {
        //printing level
        Font font("fonts/FreeMonoBold.ttf", 25);
        Color c = {255,255,0};
        Color b = {225,0, 0};
        
        Image imagelevel(font.render("Blue: ",c));
        Rect leveltext = imagelevel.getRect();
        leveltext.x = 270;
        leveltext.y = 100 + 36 * 3;
        
        surface.put_image(imagelevel, leveltext);
        
        //converting int to string
        std::stringstream strmsc;
        strmsc << blue;
        
        Image leveler(font.render(strmsc.str().c_str(), b));
        Rect levels = leveler.getRect();
        levels.x = 350;
        levels.y = 100 + 36 * 3;
        
        surface.put_image(leveler, levels);
    }
  
  
  
    
};




#endif 

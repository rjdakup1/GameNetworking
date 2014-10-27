
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <sstream>
#include "GameObject.h"
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"

#ifndef STARS_H
#define STARS_H

class Star
{
public:
    Star()
    {}
    Star(const vec2i & pos, const vec2i & vel)
        : pos_(pos), vel_(vel)
    {}
    Star(int posx, int posy, int velx, int vely)
        : pos_(posx, posy), vel_(velx, vely)
    {}
    void set_pos(const vec2i & pos)
    {
        pos_ = pos;
    }
    void set_vel(const vec2i & vel)
    {
        vel_ = vel;
    }
    vec2i & pos()
    {
        return pos_;
    }
    vec2i & vel()
    {
        return vel_;
    }
    int x() const
    {
        pos_.get_x();
    }
    int y() const
    {
        pos_.get_y();
    }
    void update()
    {
        pos_ += vel_;
    }
    
    

    
       
private:
    vec2i pos_;
    vec2i vel_;
};

#endif

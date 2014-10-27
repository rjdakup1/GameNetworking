/*****************************************************************************
 Yihsiang Liow
 Copyright
 *****************************************************************************/
#ifndef RECT_H
#define RECT_H

#include <iostream>
#include <vector>

class Rect
{
public:
	Rect(int x0 = 0, int y0 = 0, int w0 = 0, int h0 = 0,int c0 = 0, bool m0 = false)
		: x(x0), y(y0), w(w0), h(h0), move(m0),color(c0)
	{}
    Rect(const SDL_Rect & r)
		: x(r.x), y(r.y), w(r.w), h(r.h)
	{}
	SDL_Rect getSDL_Rect()
	{
		SDL_Rect r = {x, y, w, h,};
		return r;
	}
    bool collide(Rect r) // Checks to see if two objects have collided
    {
       return (((x >= r.x && x <= r.x + r.w) ||
                (x + w >= r.x && x + w <= r.x + r.w))
               && ((y >= r.y && y <= r.y + r.h) || (y + h >= r.y && y + h  <= r.y + r.h)));
    }
    
    bool move;
	int x, y, w, h, color;
    
};

inline std::ostream & operator<<(std::ostream & cout, const Rect & r)
{
	cout << "(" << r.x << "," << r.y << "," << r.w << "," << r.h << ")";
	return cout;
}
#endif

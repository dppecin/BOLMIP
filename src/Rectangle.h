#ifndef Rectangle_h
#define Rectangle_h

#include <vector>

struct Rectangle
{
    Rectangle(NDP& p1, NDP& p2)
    {
        _p1 = p1;
        _p2 = p2;
        _area = (p2.x - p1.x)*(p1.y - p2.y);
    }
    
    Rectangle( double p1x, double p1y, double p2x, double p2y )
    {
        _p1.x = p1x;
        _p1.y = p1y;
        _p2.x = p2x;
        _p2.y = p2y;
        _area = (_p2.x - _p1.x)*(_p1.y - _p2.y);
    }

    NDP _p1, _p2;
    double _area;
};

#endif

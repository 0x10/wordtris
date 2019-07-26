/*!*****************************************************************************
 * @file wt_basic_types.h
 * @brief some common type declarations needed within multiple units
 *
 * This file was developed as part of wordtris
 *
 * @author Christian Kranz
 * 
 * @copyright Copyright 2019 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz
 *
 ******************************************************************************/
#ifndef _WT_BASIC_TYPES_H_
#define _WT_BASIC_TYPES_H_

#include <iostream>
#include <math.h>

/**************************
 *
 *************************/
class WtDim
{
public:
    WtDim() :
        w(0),
        h(0) {}
    WtDim( ssize_t iW, ssize_t iH ) :
        w(iW),
        h(iH) {}
    bool operator!=(const WtDim& rhs) const
    {
        return ( w != rhs.w ) && ( h != rhs.h );
    }

    ssize_t w;
    ssize_t h;
};
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtDim &d);
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtDim &d)
{
    out << "(" << d.w << "," << d.h << ")";
    return out;
}

/**************************
 *
 *************************/
class WtCoord
{
public:    
    WtCoord() :
        x(0),
        y(0) {}
    WtCoord( ssize_t iX, ssize_t iY ) :
        x(iX),
        y(iY) {}
    WtCoord( const WtCoord& c ) :
        x(c.x),
        y(c.y) {}
    /**************************
     *
     *************************/
    WtCoord& operator=( const WtCoord& c )
    {
        x = c.x;
        y = c.y;
        return (*this);
    }
    /**************************
     *
     *************************/
    bool operator==(const WtCoord& rhs) const
    {
        return ( x == rhs.x ) && ( y == rhs.y );
    }
    /**************************
     *
     *************************/
    bool operator!=(const WtCoord& rhs) const
    {
        return ( x != rhs.x ) && ( y != rhs.y );
    }
    /**************************
     *
     *************************/
    WtCoord operator+( const WtCoord& rhs ) const
    {
        return WtCoord( x + rhs.x, y + rhs.y );
    }
    /**************************
     *
     *************************/
    bool near_point( WtCoord pos, size_t distance ) const
    {
        bool result = false;
        ssize_t dist = (( x - pos.x ) * ( x - pos.x )) + (( y - pos.y ) * ( y - pos.y ));
        size_t sqrt_dist = sqrt( dist );
        //std::cout << "dist = " << dist << "; sqrt_dist = " << sqrt_dist << std::endl;
        if ( sqrt_dist  <= distance )
            result = true;
        return result;
    }
    /**************************
     *
     *************************/
    bool in_region( WtCoord start_pos, WtDim size ) const
    {
        return (  ( x >= start_pos.x ) && ( x < start_pos.x+size.w )
               && ( y >= start_pos.y ) && ( y < start_pos.y+size.h ) );
    }
    /**************************
     *
     *************************/
    void moveX( const WtDim& dim )
    {
        x = x + dim.w;
    }

    ssize_t x;
    ssize_t y;
};
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtCoord &c);
/**************************
 *
 *************************/
std::ostream & operator << (std::ostream &out, const WtCoord &c)
{
    out << "(" << c.x << "," << c.y << ")";
    return out;
}




/**************************
 *
 *************************/
class WtFont
{
public:
    WtFont( std::string icolor="#4a90e2", std::string ifontname="text" ) :
        color(icolor),
        fontname(ifontname) {}

    std::string color;
    std::string fontname;
};


#endif /* _WT_BASIC_TYPES_H */

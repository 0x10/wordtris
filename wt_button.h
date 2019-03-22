/*!*****************************************************************************
 * @file wt_button.h
 * @brief button helper class
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
#ifndef _WT_BUTTON_H_
#define _WT_BUTTON_H_

class WtButton
{
public:
    WtButton( uint16_t id,
              WtCoord pos, WtDim size,
              std::string button_image,
              std::string label="" ) :
        m_id( id ),
        m_pos( pos ),
        m_size( size ),
        m_img( button_image ),
        m_label( label )
    {
    }
    
    ~WtButton() {}

    /**************************
     *
     *************************/
    uint16_t id()
    {
        return m_id;
    }

    /**************************
     *
     *************************/
    void set_id( uint16_t id )
    {
        m_id = id;
    }

    /**************************
     *
     *************************/
    std::string label()
    {
        return WtL10n::translate(m_label);
    }

    /**************************
     *
     *************************/
    std::string image()
    {
        return m_img;
    }

    /**************************
     *
     *************************/
    void set_image( std::string img )
    {
        m_img = img;
    }

    /**************************
     *
     *************************/
    size_t width()
    {
        return m_size.w;
    }

    /**************************
     *
     *************************/
    size_t height()
    {
        return m_size.h;
    }

    /**************************
     *
     *************************/
    WtCoord position()
    {
        return m_pos;
    }

    /**************************
     *
     *************************/
    size_t x()
    {
        return m_pos.x;
    }

    /**************************
     *
     *************************/
    void set_x( size_t x )
    {
        m_pos.x = x;
    }

    /**************************
     *
     *************************/
    size_t y()
    {
        return m_pos.y;
    }
private:
    uint16_t m_id;
    WtCoord m_pos;
    WtDim m_size;
    std::string m_img;
    std::string m_label;
};

#endif /* _WT_BUTTON_H_ */

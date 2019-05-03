/*!*****************************************************************************
 * @file wt_textbox.h
 * @brief widget for displaying text
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
#ifndef _WT_TEXTBOX_H_
#define _WT_TEXTBOX_H_

#include "wt_drawing.h"
#include "wt_string_utils.h"

class WtTextbox
{
private:
    const char* m_textbox_bg = "text_flow_box.bmp";

public:
    WtTextbox( WtCoord pos,
               std::string text ) :
        m_pos( pos ),
        m_words( split( text ) )
    {}
    ~WtTextbox() {}
    
    /**************************
     *
     *************************/
    void set_text( const std::string& s )
    {
        m_words = split( s );
    }

    /**************************
     *
     *************************/
    WtCoord position() const
    {
        return m_pos;
    }

    /**************************
     *
     *************************/
    WtCoord text_position() const
    {
        WtCoord pos = m_pos;
        pos.x += 20;
        pos.y += 20;
        return pos;
    }

    /**************************
     *
     *************************/
    WtDim size() const
    {
        return WtDim( 379, 608 );
    }

    /**************************
     *
     *************************/
    std::string background() const
    {
        return std::string( m_textbox_bg );
    }

    /**************************
     *
     *************************/
    std::vector<std::string> lines() const
    {
        std::vector<std::string> text_lines;
        std::string working_line = "";

        WtCoord cursor_pos = m_pos;
        cursor_pos.x += 20;
        WtDim sp_sz = ACTIVE_WINDOW.get_text_size( " " );
        for(size_t w_idx = 0; w_idx < m_words.size(); w_idx++ )
        {
            WtDim w_sz = ACTIVE_WINDOW.get_text_size( m_words[w_idx] );
            if ( (w_sz.w + cursor_pos.x) > ( size().w + m_pos.x ) )
            {
                cursor_pos.x = m_pos.x + 20;
                text_lines.push_back( working_line );
                working_line = "";
            }
            working_line.append( m_words[w_idx] );
            if ( w_idx < m_words.size() - 1 )
                working_line.append( " " );

            cursor_pos.x += ( w_sz.w + sp_sz.w );
        }

        if ( ! working_line.empty() )
            text_lines.push_back( working_line );

        return text_lines;
    }
private:
    WtTextbox( const WtTextbox& ); 
    WtTextbox& operator = (const WtTextbox&);

private:
    WtCoord     m_pos;
    std::vector<std::string> m_words;
};

#endif /* _WT_TEXTBOX_H */

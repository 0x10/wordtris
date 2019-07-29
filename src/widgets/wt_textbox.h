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

#include "wt_string_utils.h"

class WtTextbox
{
private:
    const char* m_textbox_bg = "#182e4b";
    const ssize_t m_text_margin = 20;
public:
    WtTextbox( WtCoord     pos,
               WtDim       size,
               std::string text,
               const WtSdlFont&  font ) :
        m_pos( pos ),
        m_size( size ),
        m_lines(),
        m_font( font )
    {
        set_text( text );
    }
    ~WtTextbox() {}
    
    /**************************
     *
     *************************/
    void set_text( const std::string& s )
    {
        m_lines.clear();

        std::vector<std::string> words = split( s );
        std::string working_line = "";

        WtCoord cursor_pos = m_pos;
        cursor_pos.x += m_text_margin;
        for(size_t w_idx = 0; w_idx < words.size(); w_idx++ )
        {
            std::string word = words[w_idx];
            size_t newline_pos = word.find("\\n");
            while ( newline_pos != std::string::npos )
            {
                std::string subword = word.substr( 0, newline_pos );
                std::cout << "subword found = " << subword << std::endl;
                if (! subword.empty() )
                {
                    append_to_line( subword, working_line, cursor_pos );
                }
                m_lines.push_back( working_line );
                working_line = "";

                word = word.substr( newline_pos+2 );
                std::cout << "process rest = " << word << std::endl;
                newline_pos = word.find("\\n");
            }

            append_to_line( word, working_line, cursor_pos );

            WtDim w_sz = m_font.text_size( word );
            if ( w_idx < words.size() - 1 )
            {
                working_line.append( " " );
                WtDim sp_sz = m_font.text_size( " " );
                w_sz.w = ( w_sz.w + sp_sz.w );
            }
            cursor_pos.x += w_sz.w;
        }

        if ( ! working_line.empty() )
            m_lines.push_back( working_line );
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
        pos.x += m_text_margin;
        pos.y += m_text_margin;
        return pos;
    }

    /**************************
     *
     *************************/
    WtDim size() const
    {
        return m_size;
    }

    /**************************
     *
     *************************/
    std::string background() const
    {
        return m_textbox_bg;
    }

    /**************************
     *
     *************************/
    std::vector<std::string>& lines()
    {
        return m_lines;
    }

    /**************************
     *
     *************************/
    const std::string font() const
    {
        return m_font.name();
    }
private:
    WtTextbox( const WtTextbox& ); 
    WtTextbox& operator = (const WtTextbox&);

    /**************************
     *
     *************************/
    void append_to_line( const std::string& str, std::string& line, WtCoord& cursor_pos )
    {
        WtDim w_sz = m_font.text_size( str );
        if ( (w_sz.w + cursor_pos.x) > ( ( size().w + m_pos.x ) - m_text_margin ) )
        {
            cursor_pos.x = m_pos.x + m_text_margin;
            m_lines.push_back( line );
            line = "";
        }
        line.append( str );
    }
private:
    WtCoord                  m_pos;
    WtDim                    m_size;
    std::vector<std::string> m_lines;
    const WtSdlFont&         m_font;
};

#endif /* _WT_TEXTBOX_H */

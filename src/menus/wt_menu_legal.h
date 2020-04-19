/*!*****************************************************************************
 * @file wt_menu_legal.h
 * @brief menu class for game legal and licenses issues
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
#ifndef _WT_MENU_LEGAL_H_
#define _WT_MENU_LEGAL_H_

#include "wt_view_if.h"

class WtMenuLegal : public WtViewIf
{
private:
           const WtDim   m_standard_btn_size = WtDim( 500, 69 );
    static const ssize_t offset_y = (ACTIVE_WINDOW_HEIGHT / 16);
    static const ssize_t offset_content = 20;
public:
    WtMenuLegal() :
        WtViewIf("#202020", 1, 1 ),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124),
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuLegal::leave ),
                     WtL10n_tr( "B A C K"),
                     WtCoord( 0, 25 ),
                     WtFont( "#a8a8a8", "text_big" ) ),
        m_text_cc0( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a2a2a2",
                      nullptr,
                      "CC BY 3.0",
                      WtCoord(0, -20),
                      WtFont( "#999999" ) ),
        m_text_cc1( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (1*25+0*10)), WtDim( 1, 1 ), "", nullptr, "To view a copy of the CC BY 3.0 license,"),
        m_text_cc2( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (2*25+0*10)), WtDim( 1,1 ), "", nullptr, "visit http://creativecommons.org/licenses/by/3.0/"),
        m_text_ofl0( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*7) ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a2a2a2",
                      nullptr,
                      "SIL OPEN FONT LICENSE",
                      WtCoord(0, -20),
                      WtFont( "#999999" ) ),
        m_text_olf1( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content * 7) + (1*25+0*10)), WtDim( 1, 1 ), "", nullptr, "Version 1.1 - 26 February 2007"),
        m_text_ofl( WtCoord( 0, offset_y + (offset_content * 7) + (1*25+0*10) ), WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font_tiny() ),
        m_text_ofl1( WtCoord( ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content * 7) + (1*25+0*10) ), WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font_tiny() ),
        m_text_ofl2( WtCoord( (ACTIVE_WINDOW_WIDTH/2)-(ACTIVE_WINDOW_WIDTH/4), offset_y + 700 ), WtDim( 0, 0 ),
                   "", ACTIVE_WINDOW.get_text_font_tiny() )
    {

        add_button( m_leave_btn );
        //add_button( m_text_cc0 );
//        add_button( m_text_cc1 );
  //      add_button( m_text_cc2 );
        add_button( m_text_ofl0 );
        add_button( m_text_olf1 );


        std::vector<std::string>& ofl_lines = m_text_ofl.lines();
        std::vector<std::string>& ofl1_lines = m_text_ofl1.lines();
        std::vector<std::string>& ofl2_lines = m_text_ofl2.lines();

        ofl_lines.clear();
        ofl_lines.push_back("PREAMBLE");
        ofl_lines.push_back("");
        ofl_lines.push_back("The goals of the Open Font License (OFL) are to stimulate worldwide");
        ofl_lines.push_back("development of collaborative font projects, to support the font creation");
        ofl_lines.push_back("efforts of academic and linguistic communities, and to provide a free and");
        ofl_lines.push_back("open framework in which fonts may be shared and improved in partnership");
        ofl_lines.push_back("with others.");
        ofl_lines.push_back("");
        ofl_lines.push_back("The OFL allows the licensed fonts to be used, studied, modified and");
        ofl_lines.push_back("redistributed freely as long as they are not sold by themselves. The");
        ofl_lines.push_back("fonts, including any derivative works, can be bundled, embedded,");
        ofl_lines.push_back("redistributed and/or sold with any software provided that any reserved");
        ofl_lines.push_back("names are not used by derivative works. The fonts and derivatives,");
        ofl_lines.push_back("however, cannot be released under any other type of license. The");
        ofl_lines.push_back("requirement for fonts to remain under this license does not apply");
        ofl_lines.push_back("to any document created using the fonts or their derivatives.");
        ofl_lines.push_back("DEFINITIONS");
        ofl_lines.push_back("");
        ofl_lines.push_back("\"Font Software\" refers to the set of files released by the Copyright");
        ofl_lines.push_back("Holder(s) under this license and clearly marked as such. This may");
        ofl_lines.push_back("include source files, build scripts and documentation.");
        ofl_lines.push_back("");
        ofl_lines.push_back("\"Reserved Font Name\" refers to any names specified as such after the");
        ofl_lines.push_back("copyright statement(s).");
        ofl_lines.push_back("");
        ofl_lines.push_back("\"Original Version\" refers to the collection of Font Software components as");
        ofl_lines.push_back("distributed by the Copyright Holder(s).");
        ofl_lines.push_back("");
        ofl_lines.push_back("\"Modified Version\" refers to any derivative made by adding to, deleting,");
        ofl_lines.push_back("or substituting — in part or in whole — any of the components of the");
        ofl_lines.push_back("Original Version, by changing formats or by porting the Font Software to a");
        ofl_lines.push_back("new environment.");
        ofl_lines.push_back("");
        ofl_lines.push_back("\"Author\" refers to any designer, engineer, programmer, technical");
        ofl_lines.push_back("writer or other person who contributed to the Font Software.");
        ofl1_lines.push_back("PERMISSION & CONDITIONS");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("Permission is hereby granted, free of charge, to any person obtaining");
        ofl1_lines.push_back("a copy of the Font Software, to use, study, copy, merge, embed, modify,");
        ofl1_lines.push_back("redistribute, and sell modified and unmodified copies of the Font");
        ofl1_lines.push_back("Software, subject to the following conditions:");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("1) Neither the Font Software nor any of its individual components,");
        ofl1_lines.push_back("in Original or Modified Versions, may be sold by itself.");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("2) Original or Modified Versions of the Font Software may be bundled,");
        ofl1_lines.push_back("redistributed and/or sold with any software, provided that each copy");
        ofl1_lines.push_back("contains the above copyright notice and this license. These can be");
        ofl1_lines.push_back("included either as stand-alone text files, human-readable headers or");
        ofl1_lines.push_back("in the appropriate machine-readable metadata fields within text or");
        ofl1_lines.push_back("binary files as long as those fields can be easily viewed by the user.");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("3) No Modified Version of the Font Software may use the Reserved Font");
        ofl1_lines.push_back("Name(s) unless explicit written permission is granted by the corresponding");
        ofl1_lines.push_back("Copyright Holder. This restriction only applies to the primary font name as");
        ofl1_lines.push_back("presented to the users.");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("4) The name(s) of the Copyright Holder(s) or the Author(s) of the Font");
        ofl1_lines.push_back("Software shall not be used to promote, endorse or advertise any");
        ofl1_lines.push_back("Modified Version, except to acknowledge the contribution(s) of the");
        ofl1_lines.push_back("Copyright Holder(s) and the Author(s) or with their explicit written");
        ofl1_lines.push_back("permission.");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("5) The Font Software, modified or unmodified, in part or in whole,");
        ofl1_lines.push_back("must be distributed entirely under this license, and must not be");
        ofl1_lines.push_back("distributed under any other license. The requirement for fonts to");
        ofl1_lines.push_back("remain under this license does not apply to any document created");
        ofl1_lines.push_back("using the Font Software.");
        ofl1_lines.push_back("TERMINATION");
        ofl1_lines.push_back("");
        ofl1_lines.push_back("This license becomes null and void if any of the above conditions are");
        ofl1_lines.push_back("not met.");
        ofl2_lines.push_back("DISCLAIMER");
        ofl2_lines.push_back("");
        ofl2_lines.push_back("THE FONT SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,");
        ofl2_lines.push_back("EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OF");
        ofl2_lines.push_back("MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT");
        ofl2_lines.push_back("OF COPYRIGHT, PATENT, TRADEMARK, OR OTHER RIGHT. IN NO EVENT SHALL THE");
        ofl2_lines.push_back("COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,");
        ofl2_lines.push_back("INCLUDING ANY GENERAL, SPECIAL, INDIRECT, INCIDENTAL, OR CONSEQUENTIAL");
        ofl2_lines.push_back("DAMAGES, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING");
        ofl2_lines.push_back("FROM, OUT OF THE USE OR INABILITY TO USE THE FONT SOFTWARE OR FROM");
        ofl2_lines.push_back("OTHER DEALINGS IN THE FONT SOFTWARE.");

        add_textbox( m_text_ofl );
        add_textbox( m_text_ofl1 );
        add_textbox( m_text_ofl2 );
    }

    ~WtMenuLegal()
    {
    }
private: // no copy allowed
    WtMenuLegal( const WtMenuLegal& ); 
    WtMenuLegal & operator = (const WtMenuLegal &);


    WtButton         m_leave_btn;
    WtButton         m_text_cc0;
    WtButton         m_text_cc1;
    WtButton         m_text_cc2;
    WtButton         m_text_ofl0;
    WtButton         m_text_olf1;
    WtTextbox        m_text_ofl;
    WtTextbox        m_text_ofl1;
    WtTextbox        m_text_ofl2;
};

#endif /* _WT_MENU_LEGAL_H_ */

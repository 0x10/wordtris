/*!*****************************************************************************
 * @file wt_menu_credits.h
 * @brief menu class for game credits
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
#ifndef _WT_MENU_CREDITS_H_
#define _WT_MENU_CREDITS_H_

#include "wt_view_if.h"

#include "menus/wt_menu_legal.h"



class WtMenuCredits : public WtViewIf
{
private:
    const WtDim                      m_standard_btn_size = WtDim( 500, 69 );

    static const ssize_t offset_x = (ACTIVE_WINDOW_WIDTH / 2) - ((60+60+60+(500/3-60)*2) / 2);
    static const ssize_t offset_y = (ACTIVE_WINDOW_HEIGHT / 16);
    static const ssize_t offset_content = 20;
public:
    WtMenuCredits() :
        WtViewIf("#202020", 1, 1),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124),
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuCredits::leave ),
                     WtL10n_tr( "B A C K"),
                     WtCoord( 0, 25 ),
                     WtFont( "#a8a8a8", "text_big" ) ),

        m_game_design_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a8a8a8",
                      nullptr,
                      WtL10n_tr("Game Design / Idea"),
                      WtCoord(0, -20),
                      WtFont( "#b4b4b4" ) ),
        m_coding_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*8)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a8a8a8",
                      nullptr,
                      WtL10n_tr("Coding"),
                      WtCoord(0, -20),
                      WtFont( "#999999" ) ),
        m_text_gd_ckr( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (1*25+0*10)), WtDim(1,1), "", nullptr, "Christian Kranz"),
        m_text_gd_wkr( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (2*25+1*10)), WtDim(1,1), "", nullptr, "Witold Krzeslowski"),
        m_text_cod_ckr( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*8)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Christian Kranz"),
        m_android_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*14)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a8a8a8",
                      nullptr,
                      WtL10n_tr("Android Support"),
                      WtCoord(0, -20),
                      WtFont( "#b4b4b4" ) ),
        m_text_as_wkr( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*14)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Witold Krzeslowski"),
        m_ui_design_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*20)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a8a8a8",
                      nullptr,
                      WtL10n_tr("Ui Design"),
                      WtCoord(0, -20) ,
                      WtFont( "#b4b4b4" )),
        m_text_ui_sdi( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*20)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Svenja Dittrich"),
        m_sfx_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*26)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a8a8a8",
                      nullptr,
                      WtL10n_tr("Music and Sound Effects"),
                      WtCoord(0, -20),
                      WtFont( "#b4b4b4" ) ),
        m_text_sfx_ckr( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*26)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Christian Kranz"),
        m_text_sfx_lrs( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*26)+ (2*25+1*10)), WtDim(1,1), "", nullptr, "'Loop_EndlessPossibilities_08' by LittleRobotSoundFactory"),
        m_text_sfx_lrs_cc( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*26)+ (3*25+1*10)), WtDim(1,1), "", nullptr, "is licensed under CC BY 3.0"),
        m_text_sfx_val( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*26)+ (4*25+2*10)), WtDim(1,1), "", nullptr, "'Piano bass drums background loop' by Valo"),
        m_text_sfx_val_cc( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*26)+ (5*25+2*10)), WtDim(1,1), "", nullptr, "is licensed under CC BY 3.0"),
        m_font_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*20)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#a8a8a8",
                      nullptr,
                      WtL10n_tr("Font"),
                      WtCoord(0, -20),
                      WtFont( "#999999" ) ),
        m_text_font_muli0( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*20)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Muli Font. (c) 2011-2014"),
        m_text_font_muli1( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*20)+ (2*25+0*10)), WtDim(1,1), "", nullptr, "by vernon adams (vern@newtypography.co.uk)"),
        m_text_font_muli2( WtCoord(ACTIVE_WINDOW_WIDTH/2, offset_y + (offset_content*20)+ (3*25+0*10)), WtDim(1,1), "", nullptr, "is licensed under the SIL Open Font License, Version 1.1"),
        m_legal_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), offset_y + (offset_content*33)  ),
                      WtDim( m_standard_btn_size.w, 40 ),
                      "#080808",
                      WT_BIND_EVENT_HANDLER( WtMenuCredits::enter_legal ),
                      WtL10n_tr("View Licenses >>"),
                      WtCoord(0, 0),
                      WtFont( "#b8b8b8" ) ),
        m_legal_menu()
    {

        add_button( m_leave_btn );
        add_button( m_coding_header );
        add_button( m_text_cod_ckr );
        add_button( m_font_header );
        add_button( m_text_font_muli0 );
        add_button( m_text_font_muli1 );
        add_button( m_text_font_muli2 );
        add_button( m_legal_header );
    }

    ~WtMenuCredits()
    {
    }


    /**************************
     *
     *************************/
    void enter_legal()
    {
        set_fading( -1, -1 );
        enter_child_menu( m_legal_menu );
        set_fading( 1, 1 );
    }

private: // no copy allowed
    WtMenuCredits( const WtMenuCredits& ); 
    WtMenuCredits & operator = (const WtMenuCredits &);


    WtButton         m_leave_btn;
    WtButton         m_game_design_header;
    WtButton         m_coding_header;
    WtButton         m_text_gd_ckr;
    WtButton         m_text_gd_wkr;
    WtButton         m_text_cod_ckr;
    WtButton         m_android_header;
    WtButton         m_text_as_wkr;
    WtButton         m_ui_design_header;
    WtButton         m_text_ui_sdi;
    WtButton         m_sfx_header;
    WtButton         m_text_sfx_ckr;
    WtButton         m_text_sfx_lrs;
    WtButton         m_text_sfx_lrs_cc;
    WtButton         m_text_sfx_val;
    WtButton         m_text_sfx_val_cc;
    WtButton         m_font_header;
    WtButton         m_text_font_muli0;
    WtButton         m_text_font_muli1;
    WtButton         m_text_font_muli2;
    WtButton         m_legal_header;
    
    WtMenuLegal      m_legal_menu;
};

#endif /* _WT_MENU_CREDITS_H_ */

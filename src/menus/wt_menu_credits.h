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
#include "wt_storage.h"

#include "widgets/wt_tristate_button.h"
#include "widgets/wt_checkbox_button.h"


class WtMenuCredits : public WtViewIf
{
private:
    const WtDim                      m_standard_btn_size = WtDim( 500, 69 );

    static const ssize_t offset_x = (ACTIVE_WINDOW_WIDTH / 2) - ((60+60+60+(500/3-60)*2) / 2);
    static const ssize_t offset_y = (ACTIVE_WINDOW_HEIGHT / 8) + (ACTIVE_WINDOW_HEIGHT / 8);
    static const ssize_t offset_content = 20;
public:
    WtMenuCredits() :
        WtViewIf("#122339"),
        m_leave_btn( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (138 / 2), (ACTIVE_WINDOW_HEIGHT - (ACTIVE_WINDOW_HEIGHT / 4))+(124/2) ), 
                     WtDim(138, 124), 
                     "back_btn.bmp",
                     WT_BIND_EVENT_HANDLER( WtMenuCredits::leave ) ),

        m_game_design_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), (ACTIVE_WINDOW_HEIGHT / 4) ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b",
                      nullptr,
                      "Game Design / Idea",
                      WtCoord(0, -20) ),
        m_coding_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*8)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b",
                      nullptr,
                      "Coding",
                      WtCoord(0, -20) ),
        m_text_gd_ckr( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (1*25+0*10)), WtDim(1,1), "", nullptr, "Christian Kranz"),
        m_text_gd_wkr( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (2*25+1*10)), WtDim(1,1), "", nullptr, "Witold Krzeslowski"),
        m_text_cod_ckr( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*8)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Christian Kranz"),
        m_android_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*14)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b",
                      nullptr,
                      "Android Support",
                      WtCoord(0, -20) ),
        m_text_as_wkr( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*14)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Witold Krzeslowski"),
        m_ui_design_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*18)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b",
                      nullptr,
                      "Ui Design",
                      WtCoord(0, -20) ),
        m_text_ui_sdi( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*18)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Svenja Dittrich"),
        m_sfx_header( WtCoord( (ACTIVE_WINDOW_WIDTH / 2) - (m_standard_btn_size.w / 2), (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*22)  ),
                      WtDim( m_standard_btn_size.w, 1 ),
                      "#182e4b",
                      nullptr,
                      "Music and Sound Effects",
                      WtCoord(0, -20) ),
        m_text_sfx_ckr( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*22)+ (1*25+0*10)), WtDim(1,1), "", nullptr, "Christian Kranz"),
        m_text_sfx_lrs( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*22)+ (2*25+1*10)), WtDim(1,1), "", nullptr, "LittleRobotSoundFactory @ freesound.org"),
        m_text_sfx_val( WtCoord(ACTIVE_WINDOW_WIDTH/2, (ACTIVE_WINDOW_HEIGHT / 4) + (offset_content*22)+ (3*25+1*10)), WtDim(1,1), "", nullptr, "Valo @ freesound.org")
    {

        add_button( m_leave_btn );
        add_button( m_game_design_header );
        add_button( m_coding_header );
        add_button( m_text_gd_ckr );
        add_button( m_text_gd_wkr );
        add_button( m_text_cod_ckr );
        add_button( m_android_header );
        add_button( m_text_as_wkr );
        add_button( m_ui_design_header );
        add_button( m_text_ui_sdi );
        add_button( m_sfx_header );
        add_button( m_text_sfx_ckr );
        add_button( m_text_sfx_lrs );
        add_button( m_text_sfx_val );
    }

    ~WtMenuCredits()
    {
    }
private: // no copy allowed
    WtMenuCredits( const WtMenuCredits& ); 
    WtMenuCredits & operator = (const WtMenuCredits &);



    /**************************
     * signal
     *************************/
    virtual void update_view()
    {
        std::string credits =
                              "        "
                              "Special thanks to: "
                              "       A.          "
                              " and all family and friends."
                              "                   ";
    }

private:
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
    WtButton         m_text_sfx_val;
};

#endif /* _WT_MENU_CREDITS_H_ */

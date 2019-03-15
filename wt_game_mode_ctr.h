/*!*****************************************************************************
 * @file wt_game_mode_ctr.h
 * @brief ctr for managing game modes
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
#ifndef _WT_GAME_MODE_CTR_H_
#define _WT_GAME_MODE_CTR_H_

#include "wt_game_mode_guessing.h"
#include "wt_game_mode_tetris.h"
#include "wt_game_mode_abc.h"

#define GAME_MODE_CTR  WtGameModeCtr::instance()
class WtGameModeCtr
{
public:
    static WtGameModeCtr& instance()
    {
        static WtGameModeCtr _instance;
        return _instance;
    }
    ~WtGameModeCtr()
    {
    }

    /**************************
     *
     *************************/
    WtGameModeIf* default_mode()
    {
        return &m_demoMode;
    }

    /**************************
     *
     *************************/
    WtGameModeIf* mode_from_string( std::string mode_id )
    {
        WtGameModeIf* result = NULL;
        for( size_t idx = 0; idx < m_available_modes.size(); idx++ )
        {
            if ( m_available_modes[idx]->get_id_string() == mode_id )
            {
                result = m_available_modes[idx];
                break;
            }
        }
        return result;
    }

    /**************************
     *
     *************************/
    std::vector<WtGameModeIf*>& get_available_modes()
    {
        return m_available_modes;
    }
private:
    WtGameModeCtr()
    {
        m_available_modes.push_back( &m_demoMode );
        m_available_modes.push_back( &m_guessMode );
        m_available_modes.push_back( &m_tetrisMode );
        m_available_modes.push_back( &m_abcMode );
    }
    WtGameModeCtr( const WtGameModeCtr& ); 
    WtGameModeCtr & operator = (const WtGameModeCtr &);


private:
    WtGameModeDemo              m_demoMode;
    WtGameModeGuessing          m_guessMode;
    WtGameModeTetris            m_tetrisMode;
    WtGameModeAbc               m_abcMode;
    std::vector<WtGameModeIf*>  m_available_modes;
};


#endif /* _WT_GAME_MODE_CTR_H_ */

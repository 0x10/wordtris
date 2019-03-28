/*******************************************************************************
 * @file dea.h
 * @brief main library header for dea lib
 *
 * @author Christian Kranz
 *
 * @copyright Copyright 2018 by Christian Kranz
 *            All rights reserved.
 *            None of this file or parts of it may be
 *            copied, redistributed or used in any other way
 *            without written approval of Christian Kranz.
 ******************************************************************************/
#ifndef __DEA_H_
#define __DEA_H_

#include <string>
#include <vector>

#define _TAB    0x09
#define _LF     0x0A
#define _CR     0x0D
#define _SP     0x20

typedef enum {
    CHAR,
    SPECIAL,
    INVALID
} dea_char_type_t;


/* these are reg ex specific chars not dea specific
 * dea specials group n transitions to a single
 */
typedef enum {
    ANY_SYMBOL        = '.',
    ANY_WHITESPACE    = 'w',
    ANY_DIGIT         = 'd'
} dea_special_types_t;


struct dea_input_symbol_t
{
    dea_input_symbol_t( char c, dea_char_type_t t ) :
        symbol( c ),
        type( t )
    {
    }

    char            symbol;
    dea_char_type_t type;
};


class TDeaState;

/*******************************************************************************
 *
 ******************************************************************************/
class TDeaTransition
{
public:
    TDeaTransition( size_t             trg,
                    dea_input_symbol_t input_symbol ) :
        m_input_symbol( input_symbol ),
        m_next_state( trg )
    {
    }
    ~TDeaTransition()
    {
    }


public:
    /**************************************************************************
     *
     **************************************************************************/
    void print()
    {
        if ( CHAR == m_input_symbol.type )
            printf("              |--> \"%c\" ==> %zd\n", m_input_symbol.symbol, m_next_state );
        else
            printf("              |--> [%c] ==> %zd\n", m_input_symbol.symbol, m_next_state );
    }

    /**************************************************************************
     *
     **************************************************************************/
    bool process_symbol( char symbol, unsigned char verbose=0 )
    {
        bool matching = false;
        if ( 0 != verbose ) 
            printf("%c ?= %c ", m_input_symbol.symbol, symbol );

        switch ( m_input_symbol.type )
        {
            case SPECIAL:
                matching = process_special( symbol );
                if ( !matching )
                {
                    if ( 0 != verbose ) 
                        printf("\t FAIL");
                }
                if ( 0 != verbose ) 
                    printf("\n");
                break;
            case CHAR:
                if ( symbol == m_input_symbol.symbol )
                {
                    matching = true;
                }
                break;
            default: 
                break;
        }

        return matching;
    }

    /**************************************************************************
     *
     **************************************************************************/
    size_t get_next_state()
    {
        return m_next_state;
    }

private:
    /**************************************************************************
     *
     **************************************************************************/
    bool process_special( char s )
    {
        bool res = false;

        switch( m_input_symbol.symbol )
        {
            case ANY_SYMBOL:        
                res = true;
                break;
            case ANY_WHITESPACE:
                switch ( s )
                {
                    case _SP:
                    case _TAB:
                    case _LF:
                    case _CR:
                        res = true;
                        break;
                    default: break;
                }
                break;
            case ANY_DIGIT:
                if ( ( s >= '0' ) && ( s <= '9' ) )
                {
                    res = true;
                }
                break;
            default: break;
        }

        return res;
    }

private:
    dea_input_symbol_t m_input_symbol;
    size_t             m_next_state;
};

/*******************************************************************************
 *
 ******************************************************************************/
class TDeaState
{
public:
    TDeaState( bool is_accepting ) :
        m_is_accepting( is_accepting )
    {
    }

    ~TDeaState()
    {
    }

public:
    /**************************************************************************
     *
     **************************************************************************/
    bool is_accepting()
    {
        return m_is_accepting;
    }

    /**************************************************************************
     *
     **************************************************************************/
    size_t transition_count()
    {
        return m_transitions.size();
    }

    /**************************************************************************
     *
     **************************************************************************/
    void print( size_t idx, bool is_current )
    {
        printf("     |--> [%zd @ %p] is_accepting == %d", idx, (void*)this, m_is_accepting );
        if ( false != is_current ) printf(" {*}\n");
        else printf("\n");

        for( size_t t_idx=0; t_idx < m_transitions.size(); t_idx++ )
        {
            m_transitions[t_idx].print();
        }
    }

    /**************************************************************************
     *
     **************************************************************************/
    void new_transition( size_t             trg,
                         dea_input_symbol_t input_symbol )
    {
        m_transitions.push_back( TDeaTransition( trg, input_symbol ) );
    }

    /**************************************************************************
     *
     **************************************************************************/
    size_t process_symbol( size_t self_idx, char symbol, unsigned char verbose=0 )
    {
        size_t result = self_idx;

        for (size_t t_idx = 0; t_idx < m_transitions.size(); t_idx++ )
        {
            bool found = m_transitions[t_idx].process_symbol( symbol, verbose );
            if ( found )
            {
                result = m_transitions[t_idx].get_next_state();
                break;
            }

            if ( 0 != verbose ) 
                printf("\n");
        }

        return result;
    }

private:
    bool                        m_is_accepting;
    std::vector<TDeaTransition> m_transitions;
};

/*******************************************************************************
 *
 ******************************************************************************/
class TDea
{
public:
    TDea()
    {
        m_states.push_back( TDeaState( false ) );
        m_current_state = 0;
    }
    TDea( size_t state_count )
    {
        m_states.resize( state_count, TDeaState( false ) );
        m_current_state = 0;
    }
    TDea( std::string contains_word )
    {
        new_contains( contains_word );
    }
    ~TDea()
    {
    }

public:
    /**************************************************************************
     *
     **************************************************************************/
    void init()
    {
        if ( m_states.size() > 0 )
            m_current_state = 0;
    }

    /**************************************************************************
     *
     **************************************************************************/
    void print()
    {
        printf("dea %p\n", (void*)this);
        printf(" |--> states => %zd @ %p\n", m_states.size(), (void*)&m_states );
        for ( size_t s_idx=0; s_idx < m_states.size() ; s_idx++ )
        {
            m_states[s_idx].print( s_idx, ( s_idx == m_current_state ) );
        }
    }

    /**************************************************************************
     *
     **************************************************************************/
    void process_symbol( char symbol, unsigned char verbose=0 )
    {
       m_current_state = m_states[m_current_state].process_symbol( m_current_state, symbol, verbose );
    }

    /**************************************************************************
     *
     **************************************************************************/
    bool verify_input( std::string input )
    {
        bool result = false;

        size_t input_len = input.length();
        printf("input(%zd) = %s\n", input_len, input.c_str() );
        for ( size_t input_idx = 0; input_idx < input_len; input_idx++ )
        {
            process_symbol( input[input_idx], 1 );
        }

        result = m_states[m_current_state].is_accepting();

        return result;
    }

    /**************************************************************************
     *
     **************************************************************************/
    bool is_current_state_accepting()
    {
        return m_states[m_current_state].is_accepting();
    }
private:
    /**************************************************************************
     *
     **************************************************************************/
    void new_contains( std::string w )
    {
        if ( w.length() > 0 )
        {
            m_states.clear();
            m_states.resize( w.length(), TDeaState( false ) );
            m_states.push_back( TDeaState( true ) );

            m_states[0].new_transition( 1, dea_input_symbol_t( w[0], CHAR ) );

            for (size_t i=1; i<w.length(); i++ )
            {
                m_states[i].new_transition( i+1, dea_input_symbol_t( w[i], CHAR ) );
                m_states[i].new_transition( 1,   dea_input_symbol_t( w[0], CHAR ) );
                m_states[i].new_transition( 0,   dea_input_symbol_t( ANY_SYMBOL, SPECIAL ) );
            }

            init();
        }
    }



private:
    std::vector<TDeaState> m_states;
    size_t                 m_current_state;
};

#endif /* __DEA_H_ */

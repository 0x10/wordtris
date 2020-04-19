/*!*****************************************************************************
 * @file wt_game_mode_guessing.h
 * @brief implementation of game mode "guess the word by building it"
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
#ifndef _WT_GAME_MODE_SUDOKU_H_
#define _WT_GAME_MODE_SUDOKU_H_

#include "wt_game_mode_if.h"
#include "wt_utils.h"
#include "wt_storage.h"
#include <unordered_map>
/**************************
 *
 *************************/
class ErrorMap
{
public:
    ErrorMap() :
        internal_error_list()
    {}
    ~ErrorMap() {}

    /**************************
     *
     *************************/
    size_t size() const
    {
        return internal_error_list.size();
    }

    /**************************
     *
     *************************/
   void retrieve_error( size_t idx, uint8_t& r, uint8_t& c ) const
   {
       if ( idx < size() )
       {
           size_t encoded = internal_error_list[idx];
           r = encoded / 10u;
           c = encoded % 10u;
       }
   }

    /**************************
     *
     *************************/
    void add_error( uint8_t r, uint8_t c )
    {
        size_t encoded = ( r * 10u ) + c;
        if ( find( internal_error_list.begin(), 
                   internal_error_list.end(), 
                   encoded ) == internal_error_list.end() )
        {
            internal_error_list.push_back( encoded );
        }
        else
        {
            //std::cout << "error already added\n";
        }
    }
private:
    std::vector<size_t> internal_error_list;
};

class WtGameModeSudoku : public WtGameModeIf
{
public:
    WtGameModeSudoku( const size_t gridsize ) :
        WtGameModeIf( (gridsize == 9 ? "Sudoku9" : "Sudoku4" ) ),
        m_sudoku_lib9x9{
"379000014060010070080009005435007000090040020000800436900700080040080050850000249",
"070000810000318902281470005400060000690103027000090006900054681106982000057000040",
"000020010504018972080409005000000108690103027702000000900704080146980703050030000",
"080002160000076200605008000006001038040030090850600700000700309002460000037100050",
"009002060004000200020318970076050400240030096003090720068725040002000800030100600",
"089002063004076205000308904000200408200030006803004000108705000502460800430100650",
"780502060000070005020310970900251430041807590053694001068025040500060000030109052",
"080540160000900205620300070900050038040807090850090001060005049502003000037089050",
"062100480403080097009070120500090070200804009090020001075030800120040905084005710",
"062050403003602507000400006500096200030804050008520001900001000106708900304060710",
"700150000003002097800470126500390200030010050008027001975031004120700900000065002",
"002009483000680000800403100540090078030804050690020041005201004000048000384900700",
"904000000500430178370106024600070430047000890031090007780503019495061003000000502",
"014008360006030108078100904000802031047315890830604000702003610405060700063900540",
"014708300000039170370006004009002031040000090830600200700500019095260000003907540",
"028900701500600800300010029080001030060709010040300090790050004006003002802006170",
"600904750509620000300508020000001430060709010047300000090802004000073902032406005",
"620904700009607803000500009000001030260749018040300000700002000406103900002406075",
"900305040020000308030042170041006000095103460000400210013970020802000090070504001",
"000005000100060358530042009701000080295080467060000205400970026852030004000500000",
"080305640000700050500842100701000900290000067008000205003978006050001000079504030",
"000005040104700308036002009001006983000103000368400200400900520802001704070500000",
"003801000078060010604007902041293570000000000052416890405600108080040260000302700",
"093000607270904300000000002001093070006758400050410800400000000007105069109000750",
"590820600200004300014507900800290000906708401000016003005609130007100009009082054",
"000820040200904310010500980801000076006758400750000803025009030087105009060082000",
"074050200905024000030700050219530000008070300000062891090005010000340906007090540",
"000900060960024130832010400000008604640000025703400000006080713021340086080001000",
"000950200065000107800706409210000600600179005003000091406205003501000980007091000",
"106905032905006700080170500030001405860050019201400060007064050008300604640509103",
"100005830905806701080102006000021400004000200001490000300204050508307604042500003",
"106900000905000740080170500009001080064753210050400300007064050098000604000009103",
"070940000905000041083170500730020405804703209201090067007064950590000604000089070",
"003690000015780004706025900000800095541906782390007000007140609100069820000078100",
"023000500915780000080420030002800005540906082300007400030042050000069827009000140",
"420600508000703264700000900000004300541000782008200000007000009154309000209008043",
"003001008905783004080420000072810300500000002008057410000042050100369807200500100",
"206703050500060200008100430700080310340506082082090005053009100001070003090801504",
"200740958500900001970050400000284319040000080182397000003020067400005003697031004",
"010743008030008071008000036705200000040010080000007605850000100420600090600831020",
"006003900004060270070100430000080309049506780102090000053009060021070800007800500",
"502600900340709005090000800680001040100456008020800051004000080200308097007005206",
"512603070008000000096004802600030040000000000020090001904100580000000400030905216",
"502600974308009005706000000005201009009000300400807600000000503200300407837005206",
"500920140000070600841030000000602398000000000389107000000060427006010000073089006",
"507908140030401005801030000000600090024803750080007000000060407200704030073209506",
"007928043000001680000500970000602390004803700089107000098005000056700000470289500",
"500020040030471080800530000700000390624803751089000004000065007050714030070080006",
"567020043930070600801500072700642008020803050300157004190005407006010039470080516",
"000009178200861005980004206574090300100000009009030724608500093700413002342900000",
"056309100200001945001004030570090061000000000860030024010500400795400002002906510",
"056029070207060900901700030000200001020040050800005000010002403005010602040980510",
"456020000030001900000750230574000300003000800009000724018072000005400080000080517",
"406300000200060945001054200500208361023607850869105004008570400795010002000006507",
"600000001009300047000942603060003800928000435005200060704821000350009100800000004",
"003750001000306007500900083060590802908000405105084060790001006300409000800035900",
"603700200280000500070940600400590810900060005035084009004021050006000028002005904",
"000000207000842300043709860730400009000975000100003024069207410008364000402000000",
"081006007007840390000700805700020500824000136005080004309007000018064900400100650",
"001506200050042090003710800706020500020000030005080704009057400010360070002108600",
"901506007607040301240700065030420509000000000105083020360007018508060902400108603",
"001536047600040001200010800000400589804975106195003000009050008500060002470198600",
"900853004000462300230070005680000031507901602310000049800090013001328000400615008",
"076850020108400000034170005080000531507901602312000040800094210000008406020015970",
"076800024100460300204000065080047000507901602000580040860000203001028006420005970",
"076050020008402300234179060000240531000000000312086000060794213001308400020010970",
"006850120050002307034170805000200000047000680000006000805094210701300050023015900",
"900147002100020790082090005027000060000802000040000320200030680016080003300761009",
"065100030100028796080600000020310900093802150001056020000005080416280003050001240",
"005100800030028096002690005000014060603802104040950000200035600410280070008001200",
"960107000034028796700003015020010960090000050041050020270400001416280570000701049",
"806090040500740680074010020207630008003102900600078205060020850048067002020080704",
"830200047510000080000806023207630000480000076000078205760304000040000092320001064",
"000005047500703009074810003000600010080102070090008000700024850100507002320900000",
"000005100500740600070816500257009400080050070001400235009324050008067002005900000",
"030005100002700680000810020057030410083102970091070230060024000048007300005900060",
"040075900901083540007460800630051094000000000510930082005014200028390405006520010",
"840100026901000500000460001002851704009602100504937600300014000008000405470008019",
"003100000901083000257460800602000090780000053010000602005014268000390405000008300",
"740000060000368050060070390031850079000903000290047530016030020080529000050000047",
"740290100109008750000400092000052009074000280200640000910004000087500603002086047",
"003200100100368050060001002601002070070913080090600501900700020080529003002006900",
"040200168009368700060000002031050400570010086008040530900000020007529600352006040",
"006002000021050300480003602040500201090631070807009060502900037004060120000700900",
"006890704000000089400170650000500001205601408800009000062014007970000000108025900",
"356002010701056300000000000640507291095000470817209063000000000004360105030700946",
"300890700721406380080100002640580000090030070000049063500004030074308125008025006",
"180036000400000350060047812201900080500000001040001603712390060098000005000680079",
"100236900000000350060540800001960004500000001800051600002095060098000000004682009",
"100036047020000306000540800001003584006408700849700600002095000608000030350680009",
"000230040000009000963540802200003580036408790049700003702095468000100000050082000",
"005200007000009350903540012000963504530000091809751000710095408098100000300002100",
"700000250850007030004802007010970682040020010286031040600209800020700063079000004",
"091003050802000000004850107500900680007608300086001009605049800000000903070300520",
"001400058802100006004052107500074000047608310000530009605240800400005903170006500",
"090460008852097030000002097010900682047000310286001040630200000020710963100086020",
"091403058000090406060800107000974082007000300280531000605009070408010000170306520",
"872000030006003002040106970000000786000261000463000000087409010100600800020000467",
"802905030906003002340026000001030786758000394463090100000450013100600809020308407",
"072000000916080000345020070090504086008201300460807020080050213000070859000000460"
                    },
        m_sudoku_lib4x4{
"0340400210030210",
"0040403004030100",
"0010400000020300",
"2000003004000001",
"1040000000000102",
"0003324004322000",
"3410020000200143",
"0130200000030210",
"0010400000020300"
        },
        m_active_id(0),
        m_pause(false),
        m_gridsize( gridsize )
    {
    }
    ~WtGameModeSudoku()
    {
    }

    /**************************
     *
     *************************/
    std::string get_title()
    {
        return ( m_gridsize == 9 ? "@dark/label_mode_9x9.bmp" : "@dark/label_mode_4x4.bmp");
    }

    /**************************
     *
     *************************/
    std::string get_name()
    {
        return WtL10n::translate( WtL10n_tr((m_gridsize == 9 ? "Sudoku 9x9" : "Sudoku 4x4" )) );
    }

    /**************************
     * 
     *************************/
    void init_game( WtBoard& board, WtPlayer&, std::string last_game_state )
    {
        WtSettings settings = STORAGE.get_settings();
        settings.gridsize = m_gridsize;
        STORAGE.store_settings( settings );
        std::string next = last_game_state;
        if ( next == "" )
            next = WtRandom::get_random_from_sequence<std::string>( ( m_gridsize == 9 ? m_sudoku_lib9x9 : m_sudoku_lib4x4 ), &m_active_id );
        std::cout << "picked " << next << std::endl;

        m_pause = false;

        board.from_string( next );
    }


    /**************************
     *
     *************************/
    virtual std::string get_current_game_state( WtBoard& board )
    {
        return board.to_string();
    }

    /**************************
     *
     *************************/
    void mark_errors( WtBoard& board, ErrorMap& detected_errors )
    {
      //  std::cout << "error count "  << detected_errors.size() << "\n";
        for ( size_t idx = 0; idx < detected_errors.size(); idx++ )
        {
            uint8_t row = m_gridsize;
            uint8_t col = m_gridsize;

            detected_errors.retrieve_error( idx, row, col );
            
           // std::cout << "error at "  << size_t(row) << ";" << size_t(col) << "\n";

            if ( ( row != m_gridsize ) && ( col != m_gridsize ) )
                board.set_erroneous( row, col );
        }
    }

    /**************************
     *
     *************************/
    virtual void eval_board( WtBoard& board, WtPlayer& player, WtGameModeState& gs )
    {
        if ( m_pause )
        {
            m_pause = false;
        }
        if ( board.is_full() )
        {
            gs.game_over = false;
            ErrorMap detected_errors;
            if ( is_valid_config( board, detected_errors ) )
            {
                player.letter_dropped( 255 );
                gs.game_over = true;
            }
            else
            {
                mark_errors( board, detected_errors );
            }
        }
    }

    /**************************
     *
     *************************/
    virtual void pre_eval_board( WtBoard& board )
    {
        std::cout << "pre eval board\n";
        ErrorMap detected_errors;
        board.clear_all_error_flags();
        if ( ! is_valid_config( board, detected_errors ) )
        {
            std::cout << "mark errors\n";
            mark_errors( board, detected_errors );
        }
    }

    /**************************
     *
     *************************/
    virtual void pause_time()
    {
        m_pause = true;
    }
    /**************************
     *
     *************************/
    char next_letter()
    {
        return ' ';
    }

    /**************************
     *
     *************************/
    bool stone_blocked( WtBoard& board, uint8_t r, uint8_t c )
    {
        return ( ( m_gridsize == 9 ? m_sudoku_lib9x9[m_active_id][ r*board.col_count() + c ] : m_sudoku_lib4x4[m_active_id][ r*board.col_count() + c ] ) != '0' );
    }

    /**************************
     *
     *************************/
    virtual std::string letter_after_next()
    {
        return " ";
    }


    /**************************
     *
     *************************/
    std::string get_hint()
    {
        return WtL10n::translate( WtL10n_tr("Try to solve the sudoku.") );
    }

private:
    WtGameModeSudoku( const WtGameModeSudoku& ); 
    WtGameModeSudoku & operator = (const WtGameModeSudoku &);


    struct ValidatorItemPos
    {
        uint8_t r;
        uint8_t c;
    };


    /**************************
     * 
     *************************/
    bool insert_or_error( std::unordered_map<char, ValidatorItemPos>& m, 
                          char item,
                          ValidatorItemPos item_pos,
                          ErrorMap& errors )
    {
        bool result = true;
        // If it is not an empty cell, 
        // insert value at the cellent cell in the set 
        if ( item != WtBoard::empty_cell )
        {
            std::unordered_map<char, ValidatorItemPos>::iterator existing = m.find(item);
            if ( existing != m.end() )
            {
                errors.add_error( m[item].r, m[item].c );
                errors.add_error( item_pos.r, item_pos.c ); 

               // std::cout << "report error pair " << WtCoord( m[item].r, m[item].c ) << " <> " << WtCoord( item_pos.r, item_pos.c ) << ": " << item << "\n";

                result = false; 
            }
            else
            {
                m[item] = item_pos;
            }
        }
        return result;
    }

    /**************************
     * Checks whether there is any duplicate  
     * in cellent row or not 
     *************************/
    bool not_in_row(WtBoard& board, uint8_t row, ErrorMap& errors ) 
    { 
        bool result = true;
        // Set to store characters seen so far. 
        std::unordered_map<char, ValidatorItemPos> m;

        for (uint8_t c = 0; c < m_gridsize; c++)
        { 
            ValidatorItemPos next{row,c};
            if ( ! insert_or_error( m, board.get_cell(row,c), next, errors ) )
                result = false;
        } 
        return result;
    } 
  
    /**************************
     * Checks whether there is any duplicate 
     * in cellent column or not. 
     *************************/
    bool not_in_col(WtBoard& board, uint8_t col, ErrorMap& errors ) 
    { 
        bool result = true;
        std::unordered_map<char, ValidatorItemPos> m;
      
        for (uint8_t r = 0; r < m_gridsize; r++)
        { 
            ValidatorItemPos next{r,col};
            if ( ! insert_or_error( m, board.get_cell(r,col), next, errors ) )
                result = false;
        } 
        return result;
    }

    /**************************
     * Checks whether there is any duplicate 
     * in cellent 3x3 box or not. 
     *************************/
    bool not_in_box(WtBoard& board, uint8_t startRow, uint8_t startCol, ErrorMap& errors ) 
    { 
        bool result = true;
        std::unordered_map<char, ValidatorItemPos> m;
      
        for (uint8_t row = 0; row < ( m_gridsize == 9 ? 3 : 2 ); row++)
        {
            for (uint8_t col = 0; col < ( m_gridsize == 9 ? 3 : 2 ); col++)
            {
                ValidatorItemPos next{static_cast<uint8_t>(row + startRow),static_cast<uint8_t>(col + startCol)};
                if ( ! insert_or_error( m, board.get_cell(row + startRow, col + startCol), next, errors ) )
                    result = false;
            } 
        } 
        return result;
    } 
    /**************************
     * Checks whether cellent row and cellent column and 
     * cellent 3x3 box is valid or not 
     *************************/ 
    bool is_valid_config( WtBoard& board, ErrorMap& errors ) 
    {
        if ( m_gridsize == 9 )
        {
            not_in_row( board, 0, errors );
            not_in_row( board, 1, errors );
            not_in_row( board, 2, errors );
            not_in_row( board, 3, errors );
            not_in_row( board, 4, errors );
            not_in_row( board, 5, errors );
            not_in_row( board, 6, errors );
            not_in_row( board, 7, errors );
            not_in_row( board, 8, errors );

            not_in_col( board, 0, errors );
            not_in_col( board, 1, errors );
            not_in_col( board, 2, errors );
            not_in_col( board, 3, errors );
            not_in_col( board, 4, errors );
            not_in_col( board, 5, errors );
            not_in_col( board, 6, errors );
            not_in_col( board, 7, errors );
            not_in_col( board, 8, errors );

            not_in_box( board, 0, 0, errors );
            not_in_box( board, 0, 3, errors );
            not_in_box( board, 0, 6, errors );
            not_in_box( board, 3, 0, errors );
            not_in_box( board, 3, 3, errors );
            not_in_box( board, 3, 6, errors );
            not_in_box( board, 6, 0, errors );
            not_in_box( board, 6, 3, errors );
            not_in_box( board, 6, 6, errors );
        }
        else
        {
            not_in_row( board, 0, errors );
            not_in_row( board, 1, errors );
            not_in_row( board, 2, errors );
            not_in_row( board, 3, errors );

            not_in_col( board, 0, errors );
            not_in_col( board, 1, errors );
            not_in_col( board, 2, errors );
            not_in_col( board, 3, errors );

            not_in_box( board, 0, 0, errors );
            not_in_box( board, 0, 2, errors );
            not_in_box( board, 2, 2, errors );
            not_in_box( board, 2, 0, errors );
        }
        return errors.size() == 0;
    } 

private:
    std::vector<std::string> m_sudoku_lib9x9;
    std::vector<std::string> m_sudoku_lib4x4;
    size_t                   m_active_id;
    bool                     m_pause;
    const size_t             m_gridsize;
};

#endif /* _WT_GAME_MODE_SUDOKU_H_ */

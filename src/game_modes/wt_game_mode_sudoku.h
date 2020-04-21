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
#include <algorithm>
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
        m_sudoku_seeds9x9_easy{
"cgi0000ad0f00a00g00h000i00edce00g0000i00d00b0000h00dcfi00g000h00d00h00e0he0000bdi",
"0g0000ha0000cahi0bbhadg000ed000f0000fi0a0c0bg0000i000fi000edfhaa0fihb0000eg0000d0",
"0000b00a0e0d0ahigb0h0d0i00e000000a0hfi0a0c0bgg0b000000i00g0d0h0adfih0g0c0e00c0000",
"0h000baf00000gfb00f0e00h00000f00a0ch0d00c00i0he0f00g00000g00c0i00bdf00000cga000e0",
"00i00b0f000d000b000b0cahig00gf0e0d00bd00c00if00c0i0gb00fhgbe0d000b000h000c0a00f00",
"0hi00b0fc00d0gfb0e000c0hi0d000b00d0hb000c000fh0c00d000a0hg0e000e0bdf0h00dc0a00fe0",
"gh0e0b0f00000g000e0b0ca0ig0i00beadc00dah0gei00ecfid00a0fh0be0d0e000f00000c0a0i0eb",
"0h0ed0af0000i00b0efb0c000g0i000e00ch0d0h0g0i0he00i000a0f000e0die0b00c0000cg0hi0e0",
"0fba00dh0d0c0h00ig00i0g0ab0e000i00g0b00h0d00i0i00b000a0ge0c0h00ab00d0i0e0hd00ega0",
"0fb0e0d0c00cf0be0g000d0000fe000ifb000c0h0d0e000heb000ai0000a000a0fg0hi00c0d0f0ga0",
"g00ae000000c00b0igh00dg0abfe00ci0b000c00a00e000h0bg00aige0ca00dab0g00i000000fe00b",
"00b00idhc000fh0000h00d0ca00ed00i00gh0c0h0d0e0fi00b00da00eb0a00d0000dh000chdi00g00",
"i0d000000e00dc0aghcg0a0f0bdf000g0dc00dg000hi00ca0i000ggh0e0c0aidie0fa00c000000e0b",
"0ad00hcf000f0c0a0h0gha00i0d000h0b0ca0dgcaehi0hc0f0d000g0b00cfa0d0e0f0g000fci00ed0",
"0adg0hc000000ciag0cg000f00d00i00b0ca0d00000i0hc0f00b00g00e000ai0iebf000000ci0ged0",
"0bhi00g0ae00f00h00c000a00bi0h000a0c00f0g0i0a00d0c000i0gi00e000d00f00c00bh0b00fag0",
"f00i0dge0e0ifb0000c00e0h0b000000adc00f0g0i0a00dgc000000i0h0b00d0000gci0b0cbd0f00e",
"fb0i0dg0000if0gh0c000e0000i00000a0c0bf0gdi0ah0d0c00000g0000b000d0fa0ci0000bd0f0ge",
"i00c0e0d00b0000c0h0c00dbag00da00f0000iea0cdf0000d00ba00acig00b0h0b0000i00g0e0d00a",
"00000e000a000f0cehec00db00ig0a0000h0bie0h0dfg0f0000b0ed00ig00bfheb0c000d000e00000",
"0h0c0efd0000g000e0e00hdba00g0a000i00bi00000fg00h000b0e00cigh00f0e000a0000gie0d0c0",
"00000e0d0a0dg00c0h0cf00b00i00a00fihc000a0c000cfhd00b00d00i00eb0h0b00ag0d0g0e00000",
"00ch0a0000gh0f00a0f0d00gi0b0dabiceg00000000000ebdafhi0d0ef00a0h0h00d0bf0000c0bg00",
"0ic000f0gbg0i0dc0000000000b00a0ic0g000fgehd000e0da0h00d0000000000ga0e0fia0i000ge0",
"ei0hb0f00b0000dc000ade0gi00h00bi0000i0fg0hd0a0000af00c00ef0iac000ga0000i00i0hb0ed",
"000hb00d0b00i0dca00a0e00ih0h0a0000gf00fgehd00ge0000h0c0be00i0c00hga0e00i0f00hb000",
"0gd0e0b00i0e0bd0000c0g000e0baiec000000h0g0c000000fbhia0i000e0a0000cd0i0f00g0i0ed0",
"000i000f0if00bdac0hcb0a0d0000000hf0dfd00000beg0cd0000000f0h0gac0bacd00hf0h000a000",
"000ie0b000fe000a0gh00g0fd0iba0000f00f00agi00e00c0000iad0fb0e00ce0a000ih000g0ia000",
"a0fi0e0cbi0e00fg000h0ag0e000c000ad0ehf00e00aib0ad000f000g0fd0e000hc00f0dfd0e0ia0c",
"a0000ehc0i0eh0fg0a0h0a0b00f0000bad0000d000b0000adi0000c00b0d0e0e0hc0gf0d0dbe0000c",
"a0fi00000i0e000gd00h0ag0e0000i00a0h00fdgecba00e0d00c0000g0fd0e00ih000f0d00000ia0c",
"0g0id0000i0e0000da0hcag0e00gc00b0d0eh0dg0cb0ib0a0i00fg00g0fdie0ei0000f0d0000hi0g0",
"00cfi00000aegh000dg0f0bei00000h000ieedai0fghbci000g00000gad0f0ia000fihb00000gha00",
"0bc000e00iaegh00000h0db00c000bh0000eed0i0f0hbc0000gd000c00db0e00000fihbg00i000ad0",
"db0f00e0h000g0cbfdg00000i0000000dc00eda000ghb00hb0000000g00000iaedc0i000b0i00h0dc",
"00c00a00hi0eghc00d0h0db00000gbha0c00e0000000b00h0egda00000db0e0a00cfih0gb00e00a00",
"b0fg0c0e0e000f0b0000ha00dc0g000h0ca0cd0e0f0hb0hb0i000e0ec00ia0000a0g000c0i0h0ae0d",
"b00gd0iehe00i0000aig00e0d00000bhdcai0d00000h0ahbcig00000c0b00fgd0000e00cfig0ca00d",
"0a0gdc00h0c000h0ga00h0000cfg0eb000000d00a00h000000gf0ehe0000a00db0f000i0f00hca0b0",
"00f00ci0000d0f0bg00g0a00dc00000h0c0i0die0fgh0a0b0i00000ec00i0f00ba0g0h0000gh00e00",
"e0bf00i00cd0g0i00e0i0000h00fh000a0d0a00def00h0b0h000ea00d0000h0b00c0h0ig00g00eb0f",
"eabf0c0g000h0000000if00dh0bf000c00d00000000000b00i000ai0da00eh0000000d000c0i0ebaf",
"e0bf00igdc0h00i00eg0f00000000eb0a00i00i000c00d00h0gf00000000e0cb00c00d0ghcg00eb0f",
"e00ib0ad00000g0f00hda0c0000000f0bcih000000000chia0g0000000f0dbg00f0a00000gc0hi00f",
"e0gi0had00c0d0a00eh0a0c0000000f000i00bdh0cge00h000g0000000f0d0gb00g0d0c00gcb0ie0f",
"00gibh0dc00000afh0000e00ig0000f0bci000dh0cg000hia0g0000ih00e0000efg00000dg0bhie00",
"e000b00d00c0dga0h0h00ec0000g00000ci0fbdh0cgea0hi00000d0000fe00g0e0gad0c00g00h000f",
"efg0b00dcic00g0f00h0ae000gbg00fdb00h0b0h0c0e0c00aeg00dai000ed0g00f0a00cidg00h0eaf",
"00000iaghb00hfa00eih000db0fegd0i0c00a0000000i00i0c0gbdf0he000icg00dac00bcdbi00000",
"0efc0ia00b0000aide00a00d0c0eg00i00fa000000000hf00c00bd0a0e00d00gied0000b00bi0fea0",
"0ef0bi0g0b0g0f0i00i0ag000c0000b0000a0b00d00e0h0000e0000a000bd0c00e0a0f0b0d0ih0ea0",
"def0b00000c000ai00000ge0bc0egd000c0000c000h0000i000gbd0ah0gb00000ed000h00000h0eag",
"d0fc00000b000f0ide00a0edb00e00b0hcfa0bcf0ghe0hfia0e00d00heg0d00gie0a000b00000fe0g",
"f0000000a00ic000dg000idbf0c0f000ch00ibh000dce00eb000f0g0dhba000ce000ia00h0000000d",
"00cge000a000c0f00ge00i000hc0f0ei0h0bi0h000d0ea0e0hd0f0gi000a00fc00d0i000h000cei00",
"f0cg00b00bh0000e000g0id0f00d00ei0ha0i000f000e0ce0hd00i00d0ba0e000f0000bh00b00ei0d",
"000000b0g000hdbc000dcg0ihf0gc0d0000i000ige000a0000c0bd0fib0gda000hcfd000d0b000000",
"0ha00f00g00ghd0ci0000g00h0eg000b0e00hbd000acf00e0h000dc0i00g0000ah0fdi00d00a00fe0",
"00ae0fb000e00db0i000cga0h00g0f0b0e000b00000c000e0h0g0d00i0egd000a0cf00g000ba0hf00",
"i0ae0f00gf0g0d0c0abd0g000fe0c0db0e0i000000000a0e0hc0b0cf000g0ahe0h0f0i0bd00a0hf0c",
"00aecf0dgf000d000ab000a0h00000d00ehih0digea0faie00c00000i0e000he000f000bdg0aihf00",
"i00hec00d000dfbc00bc00g000efh00000cae0gi0af0bca00000dih000i00ac00acbh000d00fae00h",
"0gfhe00b0a0hd000000cdag000e0h0000ecae0gi0af0bcab0000d0h000idba000000hd0f0b00aeig0",
"0gfh000bda00df0c00b0d0000fe0h00dg000e0gi0af0b000eh00d0hf0000b0c00a0bh00fdb000eig0",
"0gf0e00b000hd0bc00bcdagi0f0000bd0eca000000000cab0hf0000f0gidbac00ac0hd000b00a0ig0",
"00fhe0ab00e000bc0g0cdag0h0e000b000000dg000fh000000f000h0e0idba0g0ac000e00bc0aei00",
"i00adg00ba000b0gi00hb0i000e0bg0000f0000h0b0000d0000cb0b000c0fh00af0h000cc00gfa00i",
"0fea000c0a000bhgif0h0f000000b0ca0i000ich0bae000a0ef0b000000e0h0dafbh000c0e000abd0",
"00ea00h000c00bh0if00bfi000e0000ad0f0f0ch0ba0d0d0ie0000b000cef00da0bh00g000h00ab00",
"if0a0g0000cd0bhgifg0000c0ae0b00a0if00i00000e00da0e00b0bg0d0000adafbh0eg0000g0a0di",
"h0f0i00d0e00gd0fh00gd0a00b0b0gfc000h00ca0bi00f000ghb0e0f00b0he00dh0fg00b0b00h0g0d",
"hc0b000dgea00000h0000h0f0bcb0gfc0000dh00000gf0000ghb0egf0c0d0000d00000ibcb000a0fd",
"00000e0dge00g0c00i0gdha000c000f000a00h0a0b0g00i000h000g000bdhe0a00e0g00bcb0i00000",
"00000ea00e00gd0f000g0hafe00beg00id000h00e00g000ad00bce00icbd0e000h0fg00b00ei00000",
"0c000ea0000bg00fh0000ha00b00eg0c0da00hca0big00ia0g0bc00f00bd0000dh00gc0000ei000f0",
"0d00gei00i0a0hced000gdf0h00fc00ea0id000000000ea0ic00hb00e0adb000bhci0d0e00feb00a0",
"hd0a000bfi0a000e00000df000a00bheag0d00if0ba00e0dicgf00c000ad00000h000d0edg000h0ai",
"00ca00000i0a0hc000begdf0h00f0b0000i0gh00000ec0a0000f0b00e0adbfh000ci0d0e00000hc00",
"gd00000f0000cfh0e00f00g0ci00cahe00gi000i0c000bi00dgec00af0c00b00h0ebi0000e00000dg",
"gd0bi0a00a0i00hge0000d000ib0000eb00i0gd000bh0b00fd0000ia000d0000hge00f0c00b0hf0dg",
"00cb00a00a00cfh0e00f000a00bf0a00b0g00g0iac0h00i0f00e0ai00g000b00h0ebi00c00b00fi00",
"0d0b00afh00icfhg000f000000b0ca0e0d00eg00a00hf00h0d0ec0i000000b000gebif00ceb00f0d0",
"00f00b0000ba0e0c00dh000cf0b0d0e00b0a0i0fca0g0h0g00i0f0e0bi000cg00d0f0ab0000g00i00",
"00fhi0g0d0000000hid00ag0fe0000e0000ab0ef0ad0hh0000i0000fb0ad00gig0000000a0h0bei00",
"cef00b0a0g0a0efc00000000000fd0e0gbia0ie000dg0hagb0i0fc00000000000dcf0a0e0c0g00idf",
"c00hi0g00gbad0fch00h0a0000bfd0eh00000i00c00g00000di0fce0000d0c00gdc0habe00h0be00f",
"ah00cf000d00000ce00f00dghabb0ai000h0e0000000a0d000af0cgabci00f00ih00000e000fh00gi",
"a00bcfi00000000ce00f0ed0h0000aif000de0000000ah000eaf0000b0ie0f00ih00000000dfhb00i",
"a000cf0dg0b0000c0f000ed0h0000a00cehd00fd0hg00hdig00f0000b0ie000f0h0000c0ce0fh000i",
"000bc00d000000i000ifced0h0bb0000ceh00cfd0hgi00dig0000cg0b0iedfh000a000000e00hb000",
"00eb0000g00000ice0i0ced00ab000ifce0dec00000iah0igea000ga00ied0h0iha00000c0000ba00",
"g00000be0he000g0c000dh0b00g0a0ig0fhb0d00b00a0bhf0ca0d0f00b0ih000b0g000fc0gi00000d",
"0ia00c0e0h0b00000000dhe0a0ge00i00fh000gf0hc000hf00a00if0e0dih00000000i0c0g0c00eb0",
"00ad000ehh0ba0000f00d0eba0ge000gd0000dgf0hca0000ec000if0ebd0h00d0000ei0cag000fe00",
"0i0df000hheb0ig0c000000b0ig0a0i00fhb0dg000ca0bhf00a0d0fc0b000000b0ga0ifca000hf0b0",
"0iad0c0eh0000i0d0f0f0h00a0g000igd0hb00g000c00bh0eca000f0e00i0g0d0h0a0000ag0c0feb0",
"hgb0000c000f00c00b0d0a0fig0000000ghf000bfa000dfc0000000hgd0i0a0a00f00h000b0000dfg",
"h0bi0e0c0i0f00c00bcd00bf00000a0c0ghfgeh000ciddfc0i0a00000de00aca00f00h0i0b0c0hd0g",
"0gb000000iaf0h0000cde0b00g00i0e0d0hf00hb0ac00df0h0g0b00h00e0bac0000g0hei000000df0"
        },
        m_sudoku_seeds9x9_medium{
"ih0af0000b0f0000c0000h0d0i0a0000ec0g00h000i00c0gb0000a0b0i0h0000a0000h0i0000ga0fe",
"c0f00ih0a000hdf00000b00e0d0e0c0f00b000h000f000i00b0d0e0f0i00c00000gec000g0if00e0b",
"cd0b000e000ghd00i000b0ce00f0g0df000i0000i0000f000bg0h0h00ia0c000a00eci000c000d0ab",
"c0f00ihe000g0d0b0000000e00feg0df00bid0h0i0f0gfi00bg0heh00i0000000d0e0i000cif00e0b",
"ba0ic0f00000h00a0b0000a0ci0000e0c00hgb00000fch00b0f0000ic0f0000f0h00i00000b0ha0cf",
"g000b0ec000hgefd000ai0000g0b000c00f0f0e000i0b0i00f000c0e0000hb000beihc000hc0g000i",
"f0b00a00cad0g0f00e0e0b000a00i0c00000d0000000f00000h0c00g000c0f0e00i0b0hac00d00i0g",
"f0bg0000h0d00ihf0b0i0bf00dc0f00000ga000000000da00000f0hb00dg0a0a0dec00h0i0000bd0g",
"00fdg000bhde00i00000g000h00000h00i0c0a0i0b0h0d0i00a00000a000f00000f00ciae000icd00",
"e0c0h0f0idg0if00ecf0000ab00hd00i000000g000a000000b00fd00df0000bif00dc0gag0b0a0d0f",
"b0he0000i0dc00h0b0e00d0bh0000d0e000bi00b0c00df000d0i0000ac0i00e0c0a00fi0g0000db0a",
"0d00000e0h0a0000000e0h0fa00000b0gifd0cdf0aeh0ifbe0d00000cd0h0b0000000c0g0g00000d0",
"00000000000dci0g0a0eaf0h000i0he000gc0de000if0ag000fe0h000a0gce0e0g0bch00000000000",
"c0af0i000g000hba000fhg00000ie0ad00b0000hbe0000h00fg0ea00000dgi000dci000b000b0fe0d",
"000f00b00giedh00000fh0c00000egad00b000f0b0d000h00fgce00000a0gi00000ihfab00i00f000",
"00g00dfi00h000i0b0f0ae00d0c000fe0h000fb000ed000e0bg000b0h00fc0d0c0b000a00eig00b00",
"0a0bh000ig00f000ah000000e00ce0hf0di0h00a0e00g0bf0di0ec00e000000df000h00ei000gf0b0",
"0000h0f0i0ibf0dca000d00a000ce000bd000d00000f000fg000ec000d00i000fai0hgc0i0h0g0000",
"000h000id0000a0gf000efc000h0ef000i0bcg00i00dfa0i000eh0e000bhf000ha0g0000if000c000",
"i00000c0000h0ac0f00aed000h00hcb0g0000bac0dfg0000h0abe00e000iad00c0gd0e0000d00000f",
"0000cia000000000fefb0a000c0dfagh000i000d0c000c000afbdg0a000g0ibbd000000000hbe0000",
"h00fc0a00ai0h0d0fef00000i0000a000000geb000fha000000b0000f00000bbd0i0h0ac00h0ea00f",
"00000iab00000b0000f0da000c0d0ag0bc0i00bd0cf00c0ie0fb0g0a000gh0b0000f00000chb00000",
"0hi00fbc00000b0eih0b0d00a00c0a0i0d00000c0a00000f0d0h0c00e00b0d0igb0e00000cdi00gb0",
"0a0i000b0g00c00i00ci00bd00g0000g00hd00ga0hf00fh00e0000a00dc00gi00c00a00f0e000f0a0",
"0hgf0c00d0a000000g00i0deah00e0d00c0000f000b0000a00b0d00dbef0h00a000000f0h00c0dga0",
"e0000cib00a0i00f0gf0i000ahc000d00c0000faeib0000a00b000gdb000h0ia0c00g0f00fec0000b",
"00ad000i00c00f0b00b0000c0h00eg00d00ih000c000fc00f00gb00f0h0000b00e0b00d00h000ei00",
"00ad00000i000f0be00dfie0a0gf0000d00i0b0e0g0a0c00f0000ea0c0dieg00ie0b000h00000ei00",
"g0c00b0f00000d000ibh00000dc00g0ad0000e0f0i0c0000gc0f00ei00000gbc000f00000g0c00a0f",
"e0000ia0000d0h0e0giagfe0h000gfh00ce0d0000000a0ea00gfd000b0geiacg0e0c0b0000cb0000e",
"00000000a0g0b0edh00dhac0f0e0eg00000f00if0he00d00000ac0h0e0bdia00cde0i0f0g00000000",
"000ie0000h0d000c0000ac00fb0000dh000ceh0b0g0dad000ic0000bg00ie0000e000a0d0000af000",
"d0000ei000e000cab00gb0da000bd0h0000e0h00000c0g0000b0di000fg0ce00cga000f000fc0000a",
"00h000ia00000h000000ibea00ch0ci00f0d000h0f000b0g00da0he00dihc000000g00000cb000g00",
"f0a0000000ibch00000000ab0he000abg00c00e000b00c00eif000ga0bc00000000gaec0000000d0g",
"0h0g0i00de00000ag00cg00000e00fab0h00a00h0c00i00h0ifg00g00000fi00fi00000bb00i0h0a0",
"e0df0000bb0a0ce00igf00000000a00bfd0000e0h0b0000had00f00000000bdh00de0i0gd0000ah0f",
"00e0bh0a0c00a0g0000a00i0h00000fe0bda00dg0bi00afb0hd00000h0f00i0000b0i00g0i0hg0f00",
"0hb000eg0f000gh00cicgea000b000gie00000e000b00000hba000e000fgcbhb00ah000e0gc000af0",
"0a0gcb00fc0i00e000g000000h000cbdha00000e0c00000bafic000c000000a000d00i0ge00cig0f0",
"i000g000c0e0c00h000f000da0i0b00hi00a000a0c000a00ge00f0e0fi000d000c00g0a0g000a000f",
"acfg000hii00fhc000b0g00a0000000g000e0i0e0h0c0g000d0000000a00b0h000bfi00ceb000gifa",
"ac000e0h0i000hc0000hgd0000f0000gbfiefib0a0gcdgecid0000c0000dbg0000bf000c0b0h000fa",
"0i0f000a00b00ai0e00000bhi0000000cd0bb0cg0af0ei0gb0000000iad00000a0hc00f00g000f0c0",
"0000i00a0000be0d0h000g00ice0e00ca00bh0a000f0ci00fh00d0dic00h000e0b0dg0000a00b0000",
"00e000bafa0ib00dg0b0000f0c00e00000hb000d0e000ic00000d00i0e0000g0hb00gc0igaf000h00",
"da000e000c0b0ai00d00hfd0a00aidb0000000cd0gb0000000afde00e0gdi00g00ib0d0f000e000ag",
"d000c0h000g00000f00e0fd0a0ca000efg0000cd0gb0000gci000ef0e0gd0h00h00000e000i0f000g",
"00000e00000b0ai0f00e000b00c0i00ef0chefc000biahb0ci00d0f00a000h00h0ib0d00000e00000",
"d000c00b00000aief000000b0g0a0000f0ch0f0d0g0i0hb0c0000e0c0a000000haib00000d00f000g",
"0000000ed0dfh00000ge00ibhc00bc00hef00h0f0i0a00fic00dh00gdbh00ie00000dgb0ic0000000",
"cf00a0d000d0fcg0bihi000000f0g000ai00000e0i00000ig000h0b000000dhic0hgf0e000h0d00ic",
"0i00c0000e0hb0f00000b0g0hiai00e000h00da0f0ig00e000i00cahe0b0c00000f0db0h0000e00a0",
"00f0edicg000bi0a000iega000df0h0000gi000000000ba0000f0ci000cbge000a0fi000efbdg0c00",
"00h0f000000000c00i0f000hd0a0eb0cg0dfh0d000g0efa0ei0bh0g0ac000b0e00d000000000b0f00",
"i00b00e00c0e00i0dh0ahc0000bf0000g00000c000d00000h0000gh0000cgb0bd0e00h0c00i00b00e",
"000000ef000bde0i0000000c0baf0gi00000cb00d00gi00000bh0dba0c0000000i0gea000ef000000",
"h0g0eia0b00a000h000f0h000e0000g0h0dc0g00i00b0ae0b0d0000h000b0a000e000d00g0icd0b0h",
"00h000e0df0000a00i0i0fd000h00fdec0i00e0a0b0h00c0hfgd00h000cd0g0e00b0000ag0c000h00",
"0gh00i00000e0h00b0cibfd000h0h000000g000a0b000i000000e0h000cdbgf0f00g0i00000i00hd0",
"a0000000e00hide0a00ge00cd000i00efa00d000i000g00fcg00d000ga00hb00f0gbhe00h0000000f",
"bc00fi0ed00000ch00000ag000cf000000a00idcafeb00a000000ic000hb00000ef00000gf0de00cb",
"00i00dcb0e0bi0000a000g0000i0i0f0bdah0ahd0efi0gfda0h0c0f0000i000i0000gb0e0egb00i00",
"c0iea0gf00g0h000e00h0000c000a00h0f0c000b0f000i0f0c00g000d0000a00f000g0h00ih0fed0g",
"h0gc0e00b00000h0f0000gd0c00adib0ch0000h000b0000ed0giaf00c0ib0000i0e00000d00f0ae0i",
"e00g000acc00000d000h000fegbgdbf0e000i00b0h00d000c0dbegbgeh000d000i00000edc000g00i",
"000i000c0d0000h0i0000000e0b0afdc00beh0bf0ei0cce00ibgf0a0e0000000f0c0000h0h000i000",
"gf0e0000000af00ge0c0e00000b0000ifb0d0bg000ci0f0cdg0000h00000i0c0cf00eh0000000c0ae",
"00ie0000hb000c0g000heg0i0f0ea0000b00db00000if00c0000ha0e0a0gid000f0d000gi0000cf00",
"000000ie000fidc0gbbd00g0f00g00ec0d0a00bd0ae00d0e0fg00i00a0e00dffe0gadb000gd000000",
"00c0gh000gfhaeb00c0a0000e0000000000i0ifg0cab0h0000000000d0000c0i00hcegfd000fd0b00",
"ac000d00bf0b000e0c00g00f000000ea0i00cg00b00ae00a0ic000000h00b00i0e000c0hb00c000ef",
"00a0i00f000000b00ghe0a0d0b0000eag0cb0a00000h0bi0dch0000g0i0c0dff00g000000d00h0g00",
"0chg0de00b00f00gi00000i0a0h0e0h000d000ca0if000i000e0a0e0d0f00000gb00h00a00ic0gdh0",
"0ed0f000ba00hb0ef000hi00g00f0c0h00b00000000000g00e0a0h00a00cb000db0ah00ig000i0he0",
"a0ih000gb0000dih00bdh00a00cc00i0h0f00000a00000a0e0g00dd00a00che00ach0000fh000da0i",
"d0h0a0c000000g0h00g00h0i0ea000000bfc0h00000g0cde000000ab0f0d00i00d0c000000i0b0d0e",
"00g0e0ci00000db0g0eb0i0000ai0b0000f00aefbdic00f0000a0ga0000e0hc0c0da00000ei0c0b00",
"b00ec0i0000000i000ief000ab0d00iec000cf00000di000dfh00a0ad000cif000g0000000e0id00g",
"fchiba000ad0f0000b0b00c0000i0d0000ah000h0c000ha0000b0g0000h00f0c0000e0bi000bifagc",
"0g000e000a00000g00hd00i00ecicda0ge0b000i0b000e0hd0ciagge00c00da00c00000f000b000i0",
"00i00e0hda0000hgbi000g00f0000da000f0f00i0b00h0b000ci0000f00i000bice0000fdh0b00c00",
"ebg0i0h0d0000000be0h0d000gf00eah0f00000g0f00000b0ecd00ae000d0f0bi0000000f0d0a0ceh",
"0000c0e00000d00h0b00c00fad0000c0b0ag0bg000de0ca0f0g0000cfe00i00i0d00c00000b0a0000",
"00h0a00d0f000g00i0000db000c0i0g00ac0h00a0c00g0ac00i0h0c000dg0000h00i000d0g00c0h00",
"b0h000gd000000e0000e00bhf0000f0h00c0h00afc00g0a00e0d0000ihd00b0000f000000gb000h0i",
"0agi000c000h0c0000b0i0gea000d000i0fc00ba0ch00ih0f000b000fea0i0h0000i0c000i000bfe0",
"0fi00c000000h00d0c0000igbhf00b0e00ah00a000c00id00c0g00chdeg0000b0e00h000000c00he0",
"0e0dcfhi00c00000e00h000ecadi00f000000g00i00h000000a00cgdab000c00i00000b00bhged0f0",
"0fb000000cad0gfibh0000h0d00000h0a0f000hgfbe000b0c0i00000c0b0000fieda0bgc000000fh0",
"00ehc0fa00f000ah0i0a0000000ei0ah00c0000e0b0000g00fc0he0000000i0a0ig000d00ec0dhb00",
"f0hg000000ga00fbch00eh0bf00g000000d0000fgi0000i000000f00ga0he00hebd00ag000000gh0c",
"bg0i0h00ec000d0i00h0fe00g000bgf0ic0000000000000ca0dbi000h00ae0g00a0b000ig00h0f0ac",
"0g00f000ec00000ifh000e00gd0000fh0ce0f00ceb00a0hc0gd0000fh00a000dca00000ig000i00a0",
"0b0gc0h0000fea000ggi000000ai00ahb0g000hd0eb000f0igc00hf000000dbh000dga0000b0ea0h0",
"0c00e000fh0f0gi00b00bc0fg0e000a0gie00d00000g00ghe0c000f0di0eb00e00fc0a0ic000b00f0",
"00a000cef000i0000000gaf0d0iaed00000g0c0g0f0d0f00000acbc0i0ebh0000000g000gde000b00",
"0i0h0e000a0h0i00e000dfb000hda0bc0h0e0h00000b0g0b0hi0fah000fbi000g00e0f0b000i0a0h0",
"f00hae0cd0b00i0g0f00df000i00000000geeh00000bigc00000000d000bi00i0a0e00d0bf0ida00g"
        },
        m_sudoku_seeds9x9_hard{
"c00f000h0ihg0ad000b000c0a000g000fe00e00d0i00f00ic000b000h0d000e000gf0iah0e000c00b",
"0cbi000f00i000d0bah0000fi0cc0f00g000000fih000000d00f0gd0hg0000faf0h000e00g000cda0",
"f0000000i00gd0e0c0dhic00e000fc000i0b0e00000g0i0d000ch000f00dbae0d0e0bg00e0000000c",
"0i0hf0000f00000g0e0b0a0df0ig0000a0i0b0c0i0e0d0e0c0000gd0ae0i0g0e0b00000h0000hg0e0",
"00be000g000i0gbdh00g0h00e0bibf000000h00iba00f000000ibeb0g00i0e00ihbe0c000a000db00",
"00cd0ef000b0c0f0h00e00g000i0a0000i0c00ea0ig00b0f0000a0h000e00i00f0i0c0d000if0ha00",
"0bg00dc00000g000e000d00agf00a00g000i0gf000ea0i000c00h00cie00h000f000b00000ef00ig0",
"0f00000i0d0h0ifb0e000g00d000bf000achhe00c00dbadc000ef000d00e000f0bia0c0d0g00000h0",
"000c000000g00fh000bf0a0i00gfh0di0a00di00000hf00e0cf0dba00g0c0bi000fb00g000000e000",
"0g0b0000h0e000d00ii0b00hga0c000f00h00be000if00h00b000a0fhi00a0ee00a000i0b0000g0d0",
"a0000000c0i00f00b0g000dhfi00h0e0ai00000f0d00000ei0g0a00dabg000i0g00a00f0c0000000h",
"c00000a00b00hf0g000g000dfb0g000hb0c0000d0f0000h0ig000f0cge000f000e0bh00i00h00000a",
"0i000c000h00efbid00fdig0000c00fi0e00i0000000d00f0cg00i0000bfca00hcgae00f000c000g0",
"00ah0e00000i0a00ecbe00000f0cb0000i0a0a0g0b0c0e0d0000bf0d00000ighg00e0f00000a0ge00",
"00c0a00bh0ha000g00bd00c0efa000hic0000f0a0d0h0000fgb000ace0h00db00d000ai0ib00d0h00",
"0i0f000d0a0de00c000c0000h0e0d0gefi0h000i0a000e0ibdh0c0i0g0000h000c00ed0g0f000g0e0",
"f00cd0hg0c0bh000d00g0f00b000000f000a00fe0hc00i000b000000h00d0f00f000ed0h0ea0hf00c",
"0di00f0000a00i00000fe0gb000f00hd00ab00a000g00be00ag00f000ge0fd00000h00g0000i00ce0",
"00i00e0bf0ef0000i0000af0e00i000g0a000dh000fc000a0h000b00b0df0000a0000bf0fh0b00g00",
"00deh0000b00000h000ihc000fg0febg0000cbi000gae0000cefb0he000dcg000b00000a0000egb00",
"f0eh0d000000000i0f0a000000hh00bdef0a00ig0ab00a0bihc00gb000000g0e0a000000000c0bh0e",
"h00i0a0e0e000g0h0000c000i0g0eg0h00cf000e0f000af00b0di0g0a000e0000e0i000d0b0g0e00h",
"d0h00g00000000ag00f0g00bei00b000d00ic000i000gg00a000f00hcd00i0b00db00000000e00a0h",
"c00hi00g0g0id0000h000c0e00df000000dg00eb0ch00ac000000ee00f0h000h0000dg0f0f00eg00c",
"00000agh0da0i000cf00h0e0000a0b000hf0ge0f0h0da0fd000i0g0000f0d00ed000g0ib0hab00000",
"h0d000c000a000hbi000bcg0d000c0ha0if00f00i00h00hi0fe0b000a0egh000dhi000g000f000e0i",
"00gc000fdf00000ib00i0ba0000h00gfb0c00cb000fh00f0hci00e0000eg0d00gf00000ced000cg00",
"g00a0be000a00c000i0b0f00da0ad00gc0h0000h0a0000c0bf00da0fc00g0e0i000a00f000ac0f00d",
"f000000000d00chei00hca00f000eig000f0d000a000i0g000fcd000g00abe00fhci00a000000000c",
"c0i00e0g000000ae0ie00ih0c000c000b0000hfe0igd0000h000a000a0eg00ff0ca000000d0b00a0e",
"000hd00000000g00idcd0000g0aa00dihb00b0000000g00cbfg00ih0f0000bcgb00h00000000bi000",
"0dcib000000f00000cb0ae00000d00f0a00h00hd0ia00a00c0b00f00000hb0ii00000h000000iefg0",
"00000g00e0c0d00ha000b0ead0g0g000di00d0a000f0h00fa000d0f0gbd0e000dh00i0g0c00g00000",
"00hai0f0c000000d0ga000c000i0iaf00b000hb000gc000g00bid0h000b000di0e000000g0d0eic00",
"00c00fae000fhia0d0a0b0000g00a0e00dbg00g000e00bed00g0f00c0000g0d0f0gahb000bai00f00",
"00bda0e00000h00000hfie0b000b0e0000ca000000000fa0000h0b000f0eidh00000d00000d0ihf00",
"00b0a00h00e0000bi00f00c0a0db0eg000c000hb0ag000a000ch0ba0c0b00d00hf0000b00b00i0f00",
"0heid000aa000ced00d00000f000000h0a0c0a0f0c0h0c0h0e000000g00000f00cea000be000fdhc0",
"000b000gd00d0e000f0i0g00hb0d00c000a000baieg000a000d00e0hc00g0e0g000c0d00id000h000",
"0c0d0gi00ed00ba0f000b0000000a0be00d0f000c000h0i00gh0e0000000e000g0ea00hd00dg0b0c0",
"00e00a000b000d00a0ai00hfg00f0i0000d0e00b0i00g0b0000a0e00bhg00ea0c00i000f000f00h00",
"0ia00c000h0d000c00000fda00b0a00e000cd00c0h00gc000f00e0a00hbd00000b000h0f000i00db0",
"0i00cf0000c00000id00000a00hdfi0b000ch0000000gc000d0hfii00g00000be00000h0000ba00d0",
"c000g000i0000iadcb000b000h0h000a0f0cbc00000idd0g0h000e0g000f000idcab0000f000c000a",
"00b00h00a0h0ba00000d0ci0h00ae00bi0ch0bi0c0da0hc0ag00be00h0dc0f00000fb0h0d00i00b00",
"00b0f00d0e000ibf0c00000e0bh00cf000ai000000000da000ic00gb0i00000c0hbg000f0d00c0g00",
"i0a00000hh0000e0ibfe0d00acg000h0gc00g000c000f00ei0b000eih00c0gaba0g0000ed00000h0c",
"0000dh0ib0000b00cf0000fagh000h0000b0i00h0f00g0c0000f000afei0000hd00c0000eg0fh0000",
"0ed0fi00000c0000d00hid00e0f00e0gd000h0000000d000ba0h00e0a00cfb00c0000i00000ah0de0",
"0000d0e000c000e00b0hdc000af0e000cgd0i0000000c0gch000f0ci000fhb0f00a000i000b0c0000",
"0000d000i00ha00g000ib00e0af0h000af0cg0000000he0ci000b0hg0c00bf000e00gi00b000h0000",
"bea0d00hich000a000g00ih0000ea000i00d00g000i00d00c000ge0000ig00b000e000igig00b0feh",
"b0000000hhd00ia00f00fd00g00db00fg00i00he0ib00g00ba00hd00a00bd00f00ig00cac0000000b",
"00f000ecde000hig0b000e0000000000e0gc00ag0hd00gf0d0000000000a000b0ifg000hagh000f00",
"000hga0f000h0bi00c0b0c00000fc0b00eh00ha000fb00ie00h0cg00000f0d0i00ad0c000a0ghc000",
"0a00cbe0000hi000000e000hf0g0i0he00a000a000g000f00ac0d0c0ie000f000000dh0000bch00g0",
"e0d0b0000gf00h00b00b0ae0f0g000f00g0bb0000000cc0f00i000d0g0ab0h00e00f00ga0000i0b0d",
"000000b00gb0dech0f0dhb000e0eg0i0000000f0b0g0000000f0de0f000bdh0b0dfgh0ac00g000000",
"a0h0000db0e0b00gc00b0d0000f0i00e0f00000h0c00000e0d00b0f0000e0g00gi00h0f0ch0000e0i",
"000d0eb00e000i000h0d0a00egigf0000ie0h0000000d0ic0000bgbca00h0f0f000a000b00gf0b000",
"000i00b0c0bf00c0h0hc0g00000e0c00af000000c000000be00h0i00000g0fa0f0c00dg0g0d00f000",
"0cg0f000af0a0begi0000g000000g00e0h00ea00000bg00h0i00a000000i0000ibeh0a0dd000a0eg0",
"00g00d0ib000i000gd0000b0f000ic0gf00he0000000gg00ce0if000e0c0000dg000b000fc0e00a00",
"0b0g0c0ef000000iab000i00d000ich00gf00000000000hf00dbi000h00e000cab000000fg0b0i0d0",
"0i0fhd000000a0c0di00fi0eh00e0000000a0ah000cg0c0000000f00id0bg00ge0h0i000000eag0i0",
"e0h0g0c00000c000000icd0f0b0ie00000gc00g000d00df00000ei0g0h0bec000000d00000d0i0f0h",
"f000bci000000ei0000big0000000hd000ib0da000fc0ei000ag0000000gch0000ea000000gcf000a",
"0g0a00d000000e0bg0b0000h00fich0f00000e00000c00000i0ghde00i0000b0hf0a000000i00d0f0",
"f00a000i000af000g000c00h0af00h0f0ab00e00000c00fb0i0g00ea0i00c000h000ci000b000d00a",
"00c00gda0g00i00000hi00c000b0gb000a0h000b0a000a0d000be0d000h00ba00000e00c0aif00g00",
"000b0eh0000000c0000ba000i0c0deah0c00h0000000d00f0dbga0d0i000fg0000f0000000hd0a000",
"0c000i0h0gf0c0hde0h0beg00000a00fb000000000000000ac00i00000hei0a0hgd0c0be0e0g000c0",
"000cfe00i0f00b0g0000d00g000he000000bd0cb0fi0hi000000ce000f00a0000a0c00i0g00hia000",
"0ie00cd00h00g00i0000g0000ca0g00e000dd00f0a00ee000b00h0fh0000e0000a00f00c00da00bf0",
"00g0f0i000h0c00000f0c0g00d0a000deci000fb0ia000idfa000b0c00b0h0a00000g0b000b0i0d00",
"0f000he000iha00fb0c000b00hib0000ih000a0d0b0c000ce0000bic00e000g0be00cid000db000e0",
"e0h00c00f00fge00000c000fie00h000b00a0ib000fh0a00h000i00gif000d00000aih00d00c00e0i",
"0ce0d000h0b0e0g0000000h0eg0a00bch00000d000h00000dae00g0if0e0000000g0i0a0e000b0if0",
"0bai000gcdi0f00ha00f0h0000d000cf00d0000b0g0000c00hi000f0000b0h00ab00h0fgig000fcb0",
"0ig0b0e00e000ig0d0b0000000000e00d00hc00a0b00id00i00b0000000000a0d0hf000e00i0a0hc0",
"h0ied0000ca0hf00d0f0d0000000c00i0g0000a000b0000f0c00e0000000c0f0i00ef0ab0000bce0d",
"0000dh0cad00000h00b0hg00i00i00ebgf000b0h0f0a000fdai00b00c00ea0g00b00000fef0ag0000",
"00000h0id00d0e00g0a00g00c00g0icb0a000a00000b000e0hai0g00c00d00f0g00a0d00df0h00000",
"gd0000ia00b0d0000e00f0g000cb0ih0000a000i0c000h0000bg0id000b0a00a0000i0e00ec0000ig",
"0b0f000h00000b0e0d00h0g0c0fg00a0fh0cce00h00dah0ag0c00bb0i0a0d00a0e0f00000g000e0a0",
"000h0a000f0c0e0ga000e0g000000g0a000fhe0c0g0idd000b0c000000i0e000ci0d0f0b000f0b000",
"000h0b00d0000i0bc00be00ag00he0000d0b00b000c00g0d0000if00cg00hb00ga0h0000e00b0i000",
"e00i0c0a0a0c0g00000d000ae00h00fa00c0d0000000b0a00bg00i00da000i00000d0c0g0c0g0i00a",
"e0f00i00b00ia000f00c0b0000i00h0f00000dch0bfa00000a0g00f0000g0i00i000hd00c00i00h0f",
"g0i000d00000d0iaf00f0eb00i0000000i0f00gb0ah00e0h0000000e00ab0h00gah0c00000c000e0a",
"h00i0a00000adg0000egi00f0b00000000a0b0ef0ig0h0d00000000e0c00agb0000fdi00000b0g00f",
"00g0000b0b00f000a0000dh0f0gf000b00e00aicfgbd00d00e000fi0f0ac0000b000f00a0c0000e00",
"ib00e0000000c0ih000g00db00e0ibd00a000f00000e000h00gdb0b00fi00c000eg0c0000000h00fa",
"000d00ea00000ca0i0000bg0f0dc0da0gh000h0c0e0g000gh0fc0ih0f0eb0000b0gh00000gc00d000",
"h0000000d00000h0c0c0fgi0e00gidh00c0f00c000h00b0h00cgdi00b0fdi0h0h0b00000d0000000a",
"a00f00ghee0000g000c0ge00d0f0i00ge0a0000h0i0000e0af00g0i0d00fb0a000i0000ggaf00b00i",
"00000ehdb000000e0g0b0dh000fig0h00000d00b0i00c00000g0idh000gb0f0g0c000000biac00000",
"hgb0000000fd00000i000h00bc00h0i00ef0g00f0h00c0if00b0g00dh00c000a00000hi0000000cda",
"0i000e00gh000g000a0a0hi00d00g00d000f00hifgb00b000a00g00b00ei0f0d000h000ce00d000a0",
"c000b00eh000eg0cf000000c00af00c000he00ah0fd00dh000b00fb00a000000ad0fe000he00c000g"
        },
        m_sudoku_seeds4x4_easy{
"0ab0b00cd00a0cd0",
"abc00d0000d00cba",
"a0000abcbcad00a0",
"da0b0b0000c0a0d0",
"abcd00b0c0d0b00c",
"a0bccb0d0a000cda",
"abcddc0bc0b000d0",
"abc000b0bad00c00",
"b00c0ab0acd000c0",
"da000bada00b0cd0",
"ab00cdb0b0ad00c0",
"0ab0db00acd000c0",
"a0bc00a0c0d00bca",
"adb0cba00a00b00a"
        },
        m_sudoku_seeds4x4_medium{
"00a0a0b00a0b0c00",
"000aabc00cabb000",
"0ab0c000000b0ca0",
"00000abcbcad0000",
"abcd00b0c000b000",
"c0d00000a0b00b0a",
"abc000b0bad00000",
"b00c00b0acd000c0",
"da000bad000b0c00",
"c0d0d00cbd0000bd",
"0b00d0a0c00a0a00"
        },
        m_sudoku_seeds4x4_hard{
"a00000b00c00000d",
"00a0b000000c0d00",
"a0b0000000000a0c",
"0a000b0000c0a0d0",
"00a0000cd000a00d",
"00000000a0b00b0a",
"a0c000b0bad00000",
"b00c00000cd00000",
"0000c0db00000b00",
"a0c00000d00a000c",
"a0b00000c0000dca",
"a0b00b0000000000"
        },
        m_active_selected_orig(""),
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
    void init_game( WtBoard& board, 
                    WtPlayer&, 
                    std::string last_game_state,
                    std::string last_game_orig )
    {
        WtSettings settings = STORAGE.get_settings();
        settings.gridsize = m_gridsize;
        STORAGE.store_settings( settings );
        std::string next       = last_game_state;
        m_active_selected_orig = last_game_orig;
        if ( next == "" )
        {
            wt_difficulty diffi = settings.difficulty;
            if ( m_gridsize == 9 )
            {
                switch( diffi )
                {
                    default:
                    case wt_difficulty_EASY:
                        next = get_next_9x9( m_sudoku_seeds9x9_easy );
                        break;
                    case wt_difficulty_MEDIUM:
                        next = get_next_9x9( m_sudoku_seeds9x9_medium );
                        break;
                    case wt_difficulty_HARD:
                        next = get_next_9x9( m_sudoku_seeds9x9_hard );
                        break;
                }
            }
            else
            {
                switch( diffi )
                {
                    default:
                    case wt_difficulty_EASY:
                        next = get_next_4x4( m_sudoku_seeds4x4_easy );
                        break;
                    case wt_difficulty_MEDIUM:
                        next = get_next_4x4( m_sudoku_seeds4x4_medium );
                        break;
                    case wt_difficulty_HARD:
                        next = get_next_4x4( m_sudoku_seeds4x4_hard );
                        break;
                }
            }
            m_active_selected_orig = next;
        }
        std::cout << "picked " << next << std::endl;

        m_pause = false;

        board.from_string( next );
    }


    /**************************
     *
     *************************/
    std::string get_next_4x4( std::vector<std::string>& seeds )
    {
        size_t selected_id = 0;
        std::string next = WtRandom::get_random_from_sequence<std::string>( seeds, &selected_id );
        std::cout << "### picked seed " << next << std::endl;

        /* randomize replace letters with digits
         * */
        std::vector<char> digits_pool = {'1','2','3','4'};

        // replace a
        std::vector<char>::iterator it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'a', *it );
        digits_pool.erase( it );
        // replace b
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'b', *it );
        digits_pool.erase( it );
        // replace c
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'c', *it );
        digits_pool.erase( it );
        // replace d
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'd', *it );
        digits_pool.erase( it );

        /* randomize rotate by
         * */
        std::vector<size_t> rotation_by = {0,90,180,270}; /* NONE, 90, 180, 270 */
        size_t rotate_by = WtRandom::get_random_from_sequence<size_t>( rotation_by );
        next = rotate_puzzle4x4( next, rotate_by );

        /* randomize flip
         * */
        std::vector<size_t> flip = {0,1,2,3}; /* NONE, hori, vert, both */
        size_t flip_by = WtRandom::get_random_from_sequence<size_t>( flip );
        next = flip_puzzle4x4( next, flip_by );

        return next;
    }

    /**************************
     *
     *************************/
    std::string get_next_9x9( std::vector<std::string>& seeds )
    {
        size_t selected_id = 0;
        std::string next = WtRandom::get_random_from_sequence<std::string>( seeds, &selected_id );
        std::cout << "### picked seed " << next << std::endl;

        /* randomize replace letters with digits
         * */
        std::vector<char> digits_pool = {'1','2','3','4','5','6','7','8','9'};

        // replace a
        std::vector<char>::iterator it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'a', *it );
        digits_pool.erase( it );
        //std::cout << "### replaced " << next << std::endl;
        // replace b
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'b', *it );
        digits_pool.erase( it );
//        std::cout << "### replaced " << next << std::endl;
        // replace c
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'c', *it );
        digits_pool.erase( it );
  //      std::cout << "### replaced " << next << std::endl;
        // replace d
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'd', *it );
        digits_pool.erase( it );
    //    std::cout << "### replaced " << next << std::endl;
        // replace e
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'e', *it );
        digits_pool.erase( it );
      //  std::cout << "### replaced " << next << std::endl;
        // replace f
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'f', *it );
        digits_pool.erase( it );
        //std::cout << "### replaced " << next << std::endl;
        // replace g
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'g', *it );
        digits_pool.erase( it );
 //       std::cout << "### replaced " << next << std::endl;
        // replace h
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'h', *it );
        digits_pool.erase( it );
   //     std::cout << "### replaced " << next << std::endl;
        // replace i
        it = WtRandom::get_random_iter_from_sequence<char>( digits_pool );
        std::replace( next.begin(), next.end(), 'i', *it );
        digits_pool.erase( it );
     //   std::cout << "### replaced " << next << std::endl;

        /* randomize rotate by
         * */
        std::vector<size_t> rotation_by = {0,90,180,270}; /* NONE, 90, 180, 270 */
        size_t rotate_by = WtRandom::get_random_from_sequence<size_t>( rotation_by );
        next = rotate_puzzle9x9( next, rotate_by );
       // std::cout << "### rotated " << next << std::endl;

        /* randomize flip
         * */
        std::vector<size_t> flip = {0,1,2,3}; /* NONE, hori, vert, both */
        size_t flip_by = WtRandom::get_random_from_sequence<size_t>( flip );
        next = flip_puzzle9x9( next, flip_by );
        //std::cout << "### flipped " << next << std::endl;

        return next;
    }

    /**************************
     *
     *************************/
    std::string rotate_puzzle4x4( const std::string puzzle, size_t rotation )
    {
        std::string output = puzzle;

        switch( rotation )
        {
            default:
            case 0: break;

            case 90:
                output[0] = puzzle[3];
                output[1] = puzzle[7];
                output[2] = puzzle[11];
                output[3] = puzzle[15];

                output[4] = puzzle[2];
                output[5] = puzzle[6];
                output[6] = puzzle[10];
                output[7] = puzzle[14];
                
                output[8] = puzzle[1];
                output[9] = puzzle[5];
                output[10] = puzzle[9];
                output[11] = puzzle[13];
                
                output[12] = puzzle[0];
                output[13] = puzzle[4];
                output[14] = puzzle[8];
                output[15] = puzzle[12];
                break;
            case 180:
                output = rotate_puzzle4x4( output, 90 );
                output = rotate_puzzle4x4( output, 90 );
                break;
            case 270:
                output = rotate_puzzle4x4( output, 90 );
                output = rotate_puzzle4x4( output, 90 );
                output = rotate_puzzle4x4( output, 90 );
                break;
        }

        return output;
    }

    /**************************
     *
     *************************/
    std::string rotate_puzzle9x9( const std::string puzzle, size_t rotation )
    {
        std::string output = puzzle;

        switch( rotation )
        {
            default:
            case 0: break;

            case 90:
                output[0] = puzzle[8];
                output[1] = puzzle[17];
                output[2] = puzzle[26];
                output[3] = puzzle[35];
                output[4] = puzzle[44];
                output[5] = puzzle[53];
                output[6] = puzzle[62];
                output[7] = puzzle[71];
                output[8] = puzzle[80];

                output[9] = puzzle[7];
                output[10] = puzzle[16];
                output[11] = puzzle[25];
                output[12] = puzzle[34];
                output[13] = puzzle[43];
                output[14] = puzzle[52];
                output[15] = puzzle[61];
                output[16] = puzzle[70];
                output[17] = puzzle[79];

                output[18] = puzzle[6];
                output[19] = puzzle[15];
                output[20] = puzzle[24];
                output[21] = puzzle[33];
                output[22] = puzzle[42];
                output[23] = puzzle[51];
                output[24] = puzzle[60];
                output[25] = puzzle[69];
                output[26] = puzzle[78];



                output[27] = puzzle[5];
                output[28] = puzzle[14];
                output[29] = puzzle[23];
                output[30] = puzzle[32];
                output[31] = puzzle[41];
                output[32] = puzzle[50];
                output[33] = puzzle[59];
                output[34] = puzzle[68];
                output[35] = puzzle[77];

                output[36] = puzzle[4];
                output[37] = puzzle[13];
                output[38] = puzzle[22];
                output[39] = puzzle[31];
                output[40] = puzzle[40];
                output[41] = puzzle[49];
                output[42] = puzzle[58];
                output[43] = puzzle[67];
                output[44] = puzzle[76];

                output[45] = puzzle[3];
                output[46] = puzzle[12];
                output[47] = puzzle[21];
                output[48] = puzzle[30];
                output[49] = puzzle[39];
                output[50] = puzzle[48];
                output[51] = puzzle[57];
                output[52] = puzzle[66];
                output[53] = puzzle[75];



                output[54] = puzzle[2];
                output[55] = puzzle[11];
                output[56] = puzzle[20];
                output[57] = puzzle[29];
                output[58] = puzzle[38];
                output[59] = puzzle[47];
                output[60] = puzzle[56];
                output[61] = puzzle[65];
                output[62] = puzzle[74];

                output[63] = puzzle[1];
                output[64] = puzzle[10];
                output[65] = puzzle[19];
                output[66] = puzzle[28];
                output[67] = puzzle[37];
                output[68] = puzzle[46];
                output[69] = puzzle[55];
                output[70] = puzzle[64];
                output[71] = puzzle[73];

                output[72] = puzzle[0];
                output[73] = puzzle[9];
                output[74] = puzzle[18];
                output[75] = puzzle[27];
                output[76] = puzzle[36];
                output[77] = puzzle[45];
                output[78] = puzzle[54];
                output[79] = puzzle[63];
                output[80] = puzzle[72];


                break;
            case 180:
                output = rotate_puzzle9x9( output, 90 );
                output = rotate_puzzle9x9( output, 90 );
                break;
            case 270:
                output = rotate_puzzle9x9( output, 90 );
                output = rotate_puzzle9x9( output, 90 );
                output = rotate_puzzle9x9( output, 90 );
                break;
        }

        return output;
    }

    /**************************
     *
     *************************/
    std::string flip_puzzle4x4( const std::string puzzle, size_t flipby )
    {
        std::string output = puzzle;

        switch( flipby )
        {
            default:
            case 0: break;

            case 1: /* horz */
                output[0] = puzzle[12];
                output[1] = puzzle[13];
                output[2] = puzzle[14];
                output[3] = puzzle[15];
                
                output[4] = puzzle[8];
                output[5] = puzzle[9];
                output[6] = puzzle[10];
                output[7] = puzzle[11];
                
                output[8] = puzzle[4];
                output[9] = puzzle[5];
                output[10] = puzzle[6];
                output[11] = puzzle[7];

                output[12] = puzzle[0];
                output[13] = puzzle[1];
                output[14] = puzzle[2];
                output[15] = puzzle[3];
                break;
            case 2: /* vert */
                output[0] = puzzle[3];
                output[1] = puzzle[2];
                output[2] = puzzle[1];
                output[3] = puzzle[0];
                
                output[4] = puzzle[7];
                output[5] = puzzle[6];
                output[6] = puzzle[5];
                output[7] = puzzle[4];
                
                output[8] = puzzle[11];
                output[9] = puzzle[10];
                output[10] = puzzle[9];
                output[11] = puzzle[8];

                output[12] = puzzle[15];
                output[13] = puzzle[14];
                output[14] = puzzle[13];
                output[15] = puzzle[12];
                break;
            case 3: /* both */
                output = flip_puzzle4x4( output, 1 );
                output = flip_puzzle4x4( output, 2 );
                break;
        }

        return output;
    }

    /**************************
     *
     *************************/
    std::string flip_puzzle9x9( const std::string puzzle, size_t flipby )
    {
        std::string output = puzzle;

        switch( flipby )
        {
            default:
            case 0: break;

            case 1: /* horz */
                output[0] = puzzle[72];
                output[1] = puzzle[73];
                output[2] = puzzle[74];
                output[3] = puzzle[75];
                output[4] = puzzle[76];
                output[5] = puzzle[77];
                output[6] = puzzle[78];
                output[7] = puzzle[79];
                output[8] = puzzle[80];

                output[9] = puzzle[63];
                output[10] = puzzle[64];
                output[11] = puzzle[65];
                output[12] = puzzle[66];
                output[13] = puzzle[67];
                output[14] = puzzle[68];
                output[15] = puzzle[69];
                output[16] = puzzle[70];
                output[17] = puzzle[71];

                output[18] = puzzle[54];
                output[19] = puzzle[55];
                output[20] = puzzle[56];
                output[21] = puzzle[57];
                output[22] = puzzle[58];
                output[23] = puzzle[59];
                output[24] = puzzle[60];
                output[25] = puzzle[61];
                output[26] = puzzle[62];



                output[27] = puzzle[45];
                output[28] = puzzle[46];
                output[29] = puzzle[47];
                output[30] = puzzle[48];
                output[31] = puzzle[49];
                output[32] = puzzle[50];
                output[33] = puzzle[51];
                output[34] = puzzle[52];
                output[35] = puzzle[53];

                output[36] = puzzle[36];
                output[37] = puzzle[37];
                output[38] = puzzle[38];
                output[39] = puzzle[39];
                output[40] = puzzle[40];
                output[41] = puzzle[41];
                output[42] = puzzle[42];
                output[43] = puzzle[43];
                output[44] = puzzle[44];

                output[45] = puzzle[27];
                output[46] = puzzle[28];
                output[47] = puzzle[29];
                output[48] = puzzle[30];
                output[49] = puzzle[31];
                output[50] = puzzle[32];
                output[51] = puzzle[33];
                output[52] = puzzle[34];
                output[53] = puzzle[35];



                output[54] = puzzle[18];
                output[55] = puzzle[19];
                output[56] = puzzle[20];
                output[57] = puzzle[21];
                output[58] = puzzle[22];
                output[59] = puzzle[23];
                output[60] = puzzle[24];
                output[61] = puzzle[25];
                output[62] = puzzle[26];

                output[63] = puzzle[9];
                output[64] = puzzle[10];
                output[65] = puzzle[11];
                output[66] = puzzle[12];
                output[67] = puzzle[13];
                output[68] = puzzle[14];
                output[69] = puzzle[15];
                output[70] = puzzle[16];
                output[71] = puzzle[17];

                output[72] = puzzle[0];
                output[73] = puzzle[1];
                output[74] = puzzle[2];
                output[75] = puzzle[3];
                output[76] = puzzle[4];
                output[77] = puzzle[5];
                output[78] = puzzle[6];
                output[79] = puzzle[7];
                output[80] = puzzle[8];


                break;
            case 2: /* vert */
                output[0] = puzzle[8];
                output[1] = puzzle[7];
                output[2] = puzzle[6];
                output[3] = puzzle[5];
                output[4] = puzzle[4];
                output[5] = puzzle[3];
                output[6] = puzzle[2];
                output[7] = puzzle[1];
                output[8] = puzzle[0];

                output[9] = puzzle[17];
                output[10] = puzzle[16];
                output[11] = puzzle[15];
                output[12] = puzzle[14];
                output[13] = puzzle[13];
                output[14] = puzzle[12];
                output[15] = puzzle[11];
                output[16] = puzzle[10];
                output[17] = puzzle[9];

                output[18] = puzzle[26];
                output[19] = puzzle[25];
                output[20] = puzzle[24];
                output[21] = puzzle[23];
                output[22] = puzzle[22];
                output[23] = puzzle[21];
                output[24] = puzzle[20];
                output[25] = puzzle[19];
                output[26] = puzzle[18];



                output[27] = puzzle[35];
                output[28] = puzzle[34];
                output[29] = puzzle[33];
                output[30] = puzzle[32];
                output[31] = puzzle[31];
                output[32] = puzzle[30];
                output[33] = puzzle[29];
                output[34] = puzzle[28];
                output[35] = puzzle[27];

                output[36] = puzzle[44];
                output[37] = puzzle[43];
                output[38] = puzzle[42];
                output[39] = puzzle[41];
                output[40] = puzzle[40];
                output[41] = puzzle[39];
                output[42] = puzzle[38];
                output[43] = puzzle[37];
                output[44] = puzzle[36];

                output[45] = puzzle[53];
                output[46] = puzzle[52];
                output[47] = puzzle[51];
                output[48] = puzzle[50];
                output[49] = puzzle[49];
                output[50] = puzzle[48];
                output[51] = puzzle[47];
                output[52] = puzzle[46];
                output[53] = puzzle[45];



                output[54] = puzzle[62];
                output[55] = puzzle[61];
                output[56] = puzzle[60];
                output[57] = puzzle[59];
                output[58] = puzzle[58];
                output[59] = puzzle[57];
                output[60] = puzzle[56];
                output[61] = puzzle[55];
                output[62] = puzzle[54];

                output[63] = puzzle[71];
                output[64] = puzzle[70];
                output[65] = puzzle[69];
                output[66] = puzzle[68];
                output[67] = puzzle[67];
                output[68] = puzzle[66];
                output[69] = puzzle[65];
                output[70] = puzzle[64];
                output[71] = puzzle[63];

                output[72] = puzzle[80];
                output[73] = puzzle[79];
                output[74] = puzzle[78];
                output[75] = puzzle[77];
                output[76] = puzzle[76];
                output[77] = puzzle[75];
                output[78] = puzzle[74];
                output[79] = puzzle[73];
                output[80] = puzzle[72];


                break;
            case 3: /* both */
                output = flip_puzzle9x9( output, 1 );
                output = flip_puzzle9x9( output, 2 );
                break;
        }

        return output;
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
    virtual std::string get_current_game_orig()
    {
        return m_active_selected_orig;
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
    virtual bool is_input_possible( WtBoard& board, const char input_value )
    {
        return ( board.occurence_count( input_value ) < m_gridsize );
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
        return m_active_selected_orig[ size_t(r)*board.col_count() + size_t(c) ] != '0';
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
    std::vector<std::string> m_sudoku_seeds9x9_easy;
    std::vector<std::string> m_sudoku_seeds9x9_medium;
    std::vector<std::string> m_sudoku_seeds9x9_hard;
    std::vector<std::string> m_sudoku_seeds4x4_easy;
    std::vector<std::string> m_sudoku_seeds4x4_medium;
    std::vector<std::string> m_sudoku_seeds4x4_hard;
    std::string              m_active_selected_orig;
    bool                     m_pause;
    const size_t             m_gridsize;
};

#endif /* _WT_GAME_MODE_SUDOKU_H_ */

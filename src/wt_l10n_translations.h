/*!*****************************************************************************
 * @file wt_l10n_translations.h
 * @brief l10n helper
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
#ifndef _WT_L10N_TRANSLATIONS_H_
#define _WT_L10N_TRANSLATIONS_H_

#include <string>
#include <vector>

typedef struct {
    std::string src;
    std::vector<std::string> texts;
} WtTranslationTexts;

const std::vector<WtTranslationTexts> wt_translations = {
    { "ABC?E",
       { "ABC?E"/* en */,
         "ABC?E"/* de */,
         "ABC?E"/* fr */ }
    },
    { "Complete the sequence!",
       { "Fill the gaps in the sequence!"/* en */,
         "Setze die fallenden Steine in die Sequenz!"/* de */,
         "Mettez les pierres dans les bons trous!"/* fr */ }
    },
    { "Fill Rows",
       { "Fill Rows"/* en */,
         "Fuelle Zeilen"/* de */,
         "Construire lignes"/* fr */ }
    },
    { "Guess it!",
       { "Guess the word!"/* en */,
         "Rate das Wort!"/* de */,
         "Devinez le mot!"/* fr */ }
    },
    { "Guess the word: ",
       { "Guess the word!"/* en */,
         "Rate das Wort!"/* de */,
         "Devinez le mot!"/* fr */ }
    },
    { "Try to place the falling letters at the correct position of the given sequence.",
       { "Try to place the falling letters at the correct position of the given sequence."/* en */,
         "Platziere die fallenden Buchstaben in die richtigen Felder."/* de */,
         "Placez les lettres qui tombent dans les bons champs."/* fr */ }
    },
    { "build rows.\nthe more equal symbols, more points.",
       { "Build rows!\nThe more equal gems, more points!"/* en */,
         "Fuelle Zeilen!\nSammle gleiche Steine pro Zeile"/* de */,
         "Construire lignes!"/* fr */ }
    },
    { "de",
       { "German"/* en */,
         "Deutsch"/* de */,
         "Allemand"/* fr */ }
    },
    { "easy",
       { "Easy"/* en */,
         "Leicht"/* de */,
         "Simple"/* fr */ }
    },
    { "en",
       { "English"/* en */,
         "Englisch"/* de */,
         "Anglais"/* fr */ }
    },
    { "fr",
       { "French"/* en */,
         "Franz."/* de */,
         "Français"/* fr */ }
    },
    { "hard",
       { "Hard"/* en */,
         "Schwer"/* de */,
         "Difficile"/* fr */ }
    },
    { "no help provided",
       { "No help provided."/* en */,
         "Keine Hilfe verfuegbar."/* de */,
         "Aucune aide n'est fournie."/* fr */ }
    },
    { "normal",
       { "Medium"/* en */,
         "Mittel"/* de */,
         "Classique"/* fr */ }
    },
    { "select game mode",
       { "Select Game"/* en */,
         "Spiel auswaehlen"/* de */,
         "Selectionner un jeu"/* fr */ }
    },
    { "you lost! :P",
       { "You lost!"/* en */,
         "Du hast verloren!"/* de */,
         "Tu as perdu!"/* fr */ }
    },
};

#endif /* _WT_L10N_TRANSLATIONS_H_ */

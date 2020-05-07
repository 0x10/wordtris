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
    { "   Y O U R    R E S U L T !  ",
       { "   Y O U R    R E S U L T !  "/* en */,
         "  D E I N   E R G E B N I S !"/* de */,
         " V O T R E  R É S U L T A T !"/* fr */ }
    },
    { "+",
       { "More"/* en */,
         "Mehr"/* de */,
         "Plus"/* fr */ }
    },
    { "-",
       { "Back"/* en */,
         "Zurück"/* de */,
         "Retour"/* fr */ }
    },
    { "B A C K",
       { "B A C K"/* en */,
         "Z U R Ü C K"/* de */,
         "R E T O U R"/* fr */ }
    },
    { "C O N T.",
       { "C O N T."/* en */,
         "W E I T E R"/* de */,
         "C O N T."/* fr */ }
    },
    { "Coding",
       { "Coding"/* en */,
         "Entwicklung"/* de */,
         "Développement"/* fr */ }
    },
    { "D O N E",
       { "D O N E"/* en */,
         "F E R T I G"/* de */,
         "F I N I"/* fr */ }
    },
    { "EXIT GAME",
       { "M A I N  M E N U"/* en */,
         "H A U P T M E N Ü"/* de */,
         "Q U I T T E R"/* fr */ }
    },
    { "Font",
       { "Font"/* en */,
         "Schrift"/* de */,
         "Fonte"/* fr */ }
    },
    { "Loading...",
       { "Loading..."/* en */,
         "Laden..."/* de */,
         "Loading..."/* fr */ }
    },
    { "N E W    H I G H S C O R E !",
       { "N E W    H I G H S C O R E !"/* en */,
         "  N E U E R    R E K O R D !"/* de */,
         "N O U V E A U  R E C O R D !"/* fr */ }
    },
    { "RESTART",
       { "R E S T A R T"/* en */,
         "N E U S T A R T"/* de */,
         "R E D É M A R R E R"/* fr */ }
    },
    { "Restore",
       { "Load"/* en */,
         "Laden"/* de */,
         "Charge"/* fr */ }
    },
    { "S T A R T",
       { "S T A R T"/* en */,
         "S T A R T"/* de */,
         "S T A R T"/* fr */ }
    },
    { "Save",
       { "Save"/* en */,
         "Speichern"/* de */,
         "Enregistrer"/* fr */ }
    },
    { "Show errors immediately",
       { "Show errors immediately"/* en */,
         "Zeige Fehler sofort an"/* de */,
         "Signaler immédi. les erreurs"/* fr */ }
    },
    { "Show supporting grid",
       { "Show supporting grid"/* en */,
         "Zeige Hilfslinien"/* de */,
         "Afficher les guides"/* fr */ }
    },
    { "Sudoku 4x4",
       { "Sudoku 4x4"/* en */,
         "Sudoku 4x4"/* de */,
         "Sudoku 4x4"/* fr */ }
    },
    { "Sudoku 9x9",
       { "Sudoku 9x9"/* en */,
         "Sudoku 9x9"/* de */,
         "Sudoku 9x9"/* fr */ }
    },
    { "Supporting Grid",
       { "Show supporting grid"/* en */,
         "Zeige Hilfslinien"/* de */,
         "Afficher les guides"/* fr */ }
    },
    { "TIME",
       { "TIME"/* en */,
         "ZEIT"/* de */,
         "TEMPS"/* fr */ }
    },
    { "Try to solve the sudoku.",
       { "Try to solve the sudoku!"/* en */,
         "Versuche das Sudoku zu lösen!"/* de */,
         "Essayez de résoudre le sudoku!"/* fr */ }
    },
    { "Ui Design",
       { "Ui Design"/* en */,
         "Ui Design"/* de */,
         "Ui Design"/* fr */ }
    },
    { "Undo",
       { "Undo"/* en */,
         "Rückgängig"/* de */,
         "Annuler"/* fr */ }
    },
    { "View Licenses >>",
       { "View Licenses >>"/* en */,
         "Lizenzen anzeigen >>"/* de */,
         "Voir les licences >>"/* fr */ }
    },
    { "de",
       { "German"/* en */,
         "Deutsch"/* de */,
         "Allemand"/* fr */ }
    },
    { "easy",
       { "Easy"/* en */,
         "Leicht"/* de */,
         "Facile"/* fr */ }
    },
    { "en",
       { "English"/* en */,
         "Englisch"/* de */,
         "Anglais"/* fr */ }
    },
    { "fr",
       { "French"/* en */,
         "Französisch"/* de */,
         "Français"/* fr */ }
    },
    { "hard",
       { "Hard"/* en */,
         "Schwer"/* de */,
         "Difficile"/* fr */ }
    },
    { "no help provided",
       { "no help provided"/* en */,
         "no help provided"/* de */,
         "no help provided"/* fr */ }
    },
    { "normal",
       { "Medium"/* en */,
         "Mittel"/* de */,
         "Moyen"/* fr */ }
    },
};

#endif /* _WT_L10N_TRANSLATIONS_H_ */

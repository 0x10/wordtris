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
    { "POINTS",
       { "POINTS"/* en */,
         "PUNKTE"/* de */,
         "POINTS"/* fr */ }
    },
    {    "   Y O U R    R E S U L T !  ",
       { "   Y O U R    R E S U L T !  "/* en */,
         "  D E I N   E R G E B N I S !"/* de */,
         " V O T R E  R É S U L T A T !"/* fr */ }
    },
    { "Build words of at least ",
       { "Build words of at least "/* en */,
         "Baue Wörter mit mindestens "/* de */,
         "Construisez des mots d'au moins "/* fr */ }
    },
    { " letters by moving around and ordering the fallen letters.",
       { " letters by moving around and ordering the fallen letters."/* en */,
         " Buchstaben. Bewege dazu die herabfallenden Buchstaben an die richtige Position."/* de */,
         " lettres en vous déplaçant et en ordonnant les lettres tombées."/* fr */ }
    },
    { " letters.",
       { " letters."/* en */,
         " Buchstaben."/* de */,
         " lettres."/* fr */ }
    },
    { "Alphabet Challenge",
       { "Alphabet Challenge"/* en */,
         "Alphabet Challenge"/* de */,
         "Alphabet Challenge"/* fr */ }
    },
    { "Android Support",
       { "Android Support"/* en */,
         "Android Unterstützung"/* de */,
         "Android Soutien"/* fr */ }
    },
    { "B A C K",
       { "B A C K"/* en */,
         "Z U R Ü C K"/* de */,
         "R E T O U R"/* fr */ }
    },
    { "Classic",
       { "Classic"/* en */,
         "Classic"/* de */,
         "Classic"/* fr */ }
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
       { "EXIT GAME"/* en */,
         "SPIEL ENDE"/* de */,
         "QUITTER"/* fr */ }
    },
    { "Fill Rows",
       { "Fill Rows"/* en */,
         "Fill Rows"/* de */,
         "Fill Rows"/* fr */ }
    },
    { "Font",
       { "Font"/* en */,
         "Schrift"/* de */,
         "Fonte"/* fr */ }
    },
    { "Game Design / Idea",
       { "Game Design / Idea"/* en */,
         "Design / Idee"/* de */,
         "Design / Idée"/* fr */ }
    },
    { "Guess the word",
       { "Guess the word"/* en */,
         "Rate das Wort"/* de */,
         "Devinez le mot"/* fr */ }
    },
    { "Loading...",
       { "Loading..."/* en */,
         "Laden..."/* de */,
         "Loading..."/* fr */ }
    },
    { "Locked! Buy Pro",
       { "Locked! Buy Pro"/* en */,
         "Locked! Buy Pro"/* de */,
         "Locked! Buy Pro"/* fr */ }
    },
    { "Music and Sound Effects",
       { "Music and Sound Effects"/* en */,
         "Musik und Sound Effekte"/* de */,
         "Musique et effets sonores"/* fr */ }
    },
    { "N E W    H I G H S C O R E !",
       { "N E W    H I G H S C O R E !"/* en */,
         "  N E U E R    R E K O R D !"/* de */,
         "N O U V E A U  R E C O R D !"/* fr */ }
    },
    { "Play music and sounds",
       { "Play music and sounds"/* en */,
         "Aktiviere Töne und Musik"/* de */,
         "Activer les sons et la musique"/* fr */ }
    },
    { "RESTART",
       { "RESTART"/* en */,
         "NEUSTART"/* de */,
         "REDÉMARRER"/* fr */ }
    },
    { "S T A R T",
       { "S T A R T"/* en */,
         "S T A R T"/* de */,
         "S T A R T"/* fr */ }
    },
    {    "SOLVED WORDS",
       { "SOLVED"/* en */,
         "GELÖST"/* de */,
         "RESOLU"/* fr */ }
    },
    { "Show next stone",
       { "Show next stone"/* en */,
         "Zeige Vorschau"/* de */,
         "Afficher l'aperçu"/* fr */ }
    },
    { "Show supporting grid",
       { "Show supporting grid"/* en */,
         "Zeige Hilfslinien"/* de */,
         "Afficher les guides"/* fr */ }
    },
    { "Supporting Grid",
       { "Show supporting grid"/* en */,
         "Zeige Hilfslinien"/* de */,
         "Afficher les guides"/* fr */ }
    },
    { "Try to place the falling letters at the correct position of the given sequence.",
       { "Try to place the falling letters at the correct position of the given sequence."/* en */,
         "Setze die herabfallenden Buchstaben an die richtige Stelle der Reihe."/* de */,
         "Mettez les lettres qui tombent à la bonne place de la ligne."/* fr */ }
    },
    { "Try to solve the puzzled word by placing the stones in correct order.",
       { "Try to solve the puzzled word by placing the letters in correct order."/* en */,
         "Löse das gesuchte Wort, indem du die herabfallenden Steine in die richtige Reihenfolge bringst."/* de */,
         "Résolvez le mot que vous recherchez en mettant les lettres qui tombent dans le bon ordre."/* fr */ }
    },
    { "Ui Design",
       { "Ui Design"/* en */,
         "Ui Design"/* de */,
         "Ui Design"/* fr */ }
    },
    { "View Licenses >>",
       { "View Licenses >>"/* en */,
         "Lizenzen anzeigen >>"/* de */,
         "Voir les licences >>"/* fr */ }
    },
    { "build rows.\nthe more equal symbols, more points.",
       { "build rows.\nthe more equal symbols, more points."/* en */,
         "build rows.\nthe more equal symbols, more points."/* de */,
         "build rows.\nthe more equal symbols, more points."/* fr */ }
    },
    { "easy",
       { "Easy"/* en */,
         "Leicht"/* de */,
         "Facile"/* fr */ }
    },
    { "five",
       { "five"/* en */,
         "fünf"/* de */,
         "cinq"/* fr */ }
    },
    { "four",
       { "four"/* en */,
         "vier"/* de */,
         "quatre"/* fr */ }
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
       { "Normal"/* en */,
         "Normal"/* de */,
         "Normal"/* fr */ }
    },
    { "three",
       { "three"/* en */,
         "drei"/* de */,
         "trois"/* fr */ }
    },
    { "en",
       { "English"/* en */,
         "Englisch"/* de */,
         "Anglais"/* fr */ }
    },
    { "de",
       { "German"/* en */,
         "Deutsch"/* de */,
         "Allemand"/* fr */ }
    },
    { "fr",
       { "French"/* en */,
         "Französisch"/* de */,
         "Français"/* fr */ }
    },
};

#endif /* _WT_L10N_TRANSLATIONS_H_ */

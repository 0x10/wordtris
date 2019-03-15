#!/bin/bash
echo '/*!*****************************************************************************
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

const std::vector<WtTranslationTexts> wt_translations = {';



grep -Proh "WtL10n::tr\(.+?\)" | grep -Po \".+?\" | sort | uniq | sed 's#"\(.*\)"#    { "\1",\n       { ""/* en */,\n         ""/* de */,\n         ""/* fr */ }\n    },#g'



echo "};

#endif /* _WT_L10N_TRANSLATIONS_H_ */";

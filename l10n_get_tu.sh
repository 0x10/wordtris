#!/bin/bash
echo '<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>';
echo "<translations>";
grep -Proh "WtL10n_tr\(.+?\)" | grep -Po \".+?\" | sort | uniq | sed 's#"\(.*\)"#    <tu>\n        <src>\1</src>\n        <l code="en"></l>\n        <l code="de"></l>\n        <l code="fr"></l>\n    </tu>#g'
echo "</translations>";

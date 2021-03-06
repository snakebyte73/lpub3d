/**************************************************************************** 

**

** Copyright (C) 2007-2009 Kevin Clague. All rights reserved.

** Copyright (C) 2015 - 2018 Trevor SANDY. All rights reserved.

**

** This file may be used under the terms of the GNU General Public

** License version 2.0 as published by the Free Software Foundation

** and appearing in the file LICENSE.GPL included in the packaging of

** this file.  Please review the following information to ensure GNU

** General Public Licensing requirements will be met:

** http://www.trolltech.com/products/qt/opensource.html

**

** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE

** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

**

****************************************************************************/

/****************************************************************************
 *
 * This implements a syntax highlighter class that works with the editwindow
 * to display LDraw files with syntax highlighting.
 *
 * Please see lpub.h for an overall description of how the files in LPub
 * make up the LPub program.
 *
 ***************************************************************************/

#include <QtWidgets>
#include "highlighter.h"
#include "version.h"

#include "name.h"
#include "lpub_preferences.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QBrush br01; // Qt Dark green
    QBrush br02; // Qt Dark blue
    QBrush br03; // Qt Dark Red
    QBrush br04; // Qt Blue
    QBrush br05; // Qt Red
    if (Preferences::displayTheme == THEME_DEFAULT) {
        br01 = QBrush(QColor(THEME_HIGHLIGHT_01_DEFAULT));
        br02 = QBrush(QColor(THEME_HIGHLIGHT_02_DEFAULT));
        br03 = QBrush(QColor(THEME_HIGHLIGHT_03_DEFAULT));
        br04 = QBrush(QColor(THEME_HIGHLIGHT_04_DEFAULT));
        br05 = QBrush(QColor(THEME_HIGHLIGHT_05_DEFAULT));
      }
    else
    if (Preferences::displayTheme == THEME_DARK) {
        br01 = QBrush(QColor(THEME_HIGHLIGHT_01_DARK));
        br02 = QBrush(QColor(THEME_HIGHLIGHT_02_DARK));
        br03 = QBrush(QColor(THEME_HIGHLIGHT_03_DARK));
        br04 = QBrush(QColor(THEME_HIGHLIGHT_04_DARK));
        br05 = QBrush(QColor(THEME_HIGHLIGHT_05_DARK));
      }

    officialMetaFormat.setForeground(br04);
    officialMetaFormat.setFontWeight(QFont::Bold);

    QStringList officialMetaPatterns;
    officialMetaPatterns 
       << "\\bFILE[^\n]*" 
       << "\\bNOFILE[^\n]*"
       << "\\bAuthor[^\n]*" 
       << "\\bBFC[^\n]*"
       << "\\b!CATEGORY[^\n]*"
       << "\\bCLEAR[^\n]*"
       << "\\b!CMDLINE[^\n]*"
       << "\\b!COLOUR[^\n]*"
       << "\\b!HELP[^\n]*"
       << "\\b!HISTORY[^\n]*"
       << "\\b!KEYWORDS[^\n]*"
       << "\\b!LDRAW_ORG[^\n]*"
       << "\\bLDRAW_ORG[^\n]*"
       << "\\b!LICENSE[^\n]*"
       << "\\bName[^\n]*" 
       << "\\bPAUSE[^\n]*"
       << "\\bPRINT[^\n]*"
       << "\\bSAVE[^\n]*"
       << "\\bSTEP[^\n]*\\b"
       << "\\bWRITE[^\n]*\\b"
       << "\\bOfficial[^\n]*"
       << "\\bUnofficial[^\n]*" 
       << "\\bUn-official[^\n]*" 
       << "\\bOriginal LDraw[^\n]*"
       << "\\b~Moved to[^\n]*"
       << "\\b//[^\n]*";

    foreach (QString pattern, officialMetaPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = officialMetaFormat;

        highlightingRules.append(rule);
    }

    MLCadMetaFormat.setForeground(br02);
    MLCadMetaFormat.setFontWeight(QFont::Bold);

    QStringList MLCadMetaPatterns;
    MLCadMetaPatterns 
       << "\\bROTATION\\b[^\n]*" 
       << "\\bROTSTEP\\b[^\n]*" 
       << "\\bBUFEXCHG\\b[^\n]*"
       << "\\bMLCAD\\b[^\n]*"
       << "\\bGROUP\\b[^\n]*"
       << "\\bGHOST\\b[^\n]*"
       << "\\bBACKGROUND\\b[^\n]*";

    foreach (QString pattern, MLCadMetaPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = MLCadMetaFormat;

        highlightingRules.append(rule);
    }

    LPubMetaFormat.setForeground(br03);
    LPubMetaFormat.setFontWeight(QFont::Bold);

    QStringList LPubMetaPatterns;
    LPubMetaPatterns
      << "\\bLPUB\\b[^\n]*"
      << "\\b!LPUB\\b[^\n]*"
      << "\\bPLIST\\b[^\n]*";

    foreach (QString pattern, LPubMetaPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = LPubMetaFormat;

        highlightingRules.append(rule);
    }

    LSynthMetaFormat.setForeground(br05);
    LSynthMetaFormat.setFontWeight(QFont::Bold);

    QStringList LSynthMetaPatterns;
    LSynthMetaPatterns
      << "\\bSYNTH\\b[^\n]*"
      << "\\b!SYNTH\\b[^\n]*";

    foreach (QString pattern, LSynthMetaPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = LSynthMetaFormat;

        highlightingRules.append(rule);
    }

    LDCadMetaFormat.setForeground(br05);
    LDCadMetaFormat.setFontWeight(QFont::Bold);

    QStringList LDCadMetaPatterns;
    LDCadMetaPatterns
        << "\\bLDCAD\\b[^\n]*"
        << "\\b!LDCAD\\b[^\n]*";

    foreach (QString pattern, LDCadMetaPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = LDCadMetaFormat;

        highlightingRules.append(rule);
      }

    LPubCommentFormat.setForeground(br01);
    rule.pattern = QRegExp("\\/\\/[^\n]*");
    rule.format = LPubCommentFormat;
    highlightingRules.append(rule);


    multiLineCommentFormat.setForeground(br01);
    commentStartExpression = QRegExp("\\bLPUB FOO BEGIN\\b+[^\n]*");
    commentEndExpression   = QRegExp("\\bLPUB FOO END\\b+[^\n]*");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule rule, highlightingRules) {
        QRegExp expression(rule.pattern);

        int index = text.indexOf(expression);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = text.indexOf(expression, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        int endIndex = text.indexOf(commentEndExpression, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }

        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression,
                                                startIndex + commentLength);
    }
}


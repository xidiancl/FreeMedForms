/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  This program is free software: you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with this program (COPYING.FREEMEDFORMS file).                   *
 *  If not, see <http://www.gnu.org/licenses/>.                            *
 ***************************************************************************/
/***************************************************************************
 *  Main Developers : Eric Maeker <eric.maeker@gmail.com>,                *
 *                    Guillaume Denry <guillaume.denry@gmail.com>          *
 *  Contributors :                                                         *
 *      NAME <MAIL@ADDRESS.COM>                                            *
 ***************************************************************************/
#include "pad_fragment.h"

#include <QTextDocument>
#include <QTextCursor>
#include <QTextDocumentFragment>

#include <QDebug>

using namespace PadTools;

PadFragment::PadFragment() :
    _start(-1),
    _end(-1),
    _id(-1)
{
}

void PadFragment::insertFragment(QTextDocument *source, QTextDocument *out) const
{
    if (_start>=0) {
        QTextCursor cursor(source);
        cursor.setPosition(_start, QTextCursor::MoveAnchor);
        cursor.setPosition(_end, QTextCursor::KeepAnchor);
        QTextCursor toCursor(out);
        toCursor.movePosition(QTextCursor::End);
        toCursor.insertFragment(cursor.selection());
    }
}

void PadFragment::insertText(QTextDocument *out, const QString &text) const
{
    if (_start>=0) {
        QTextCursor toCursor(out);
        toCursor.movePosition(QTextCursor::End);
        toCursor.insertText(text);
    }
}

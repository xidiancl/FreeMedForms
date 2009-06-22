/***************************************************************************
 *   FreeMedicalForms                                                      *
 *   Copyright (C) 2008-2009 by Eric MAEKER                                *
 *   eric.maeker@free.fr                                                   *
 *   All rights reserved.                                                  *
 *                                                                         *
 *   This program is a free and open source software.                      *
 *   It is released under the terms of the new BSD License.                *
 *                                                                         *
 *   Redistribution and use in source and binary forms, with or without    *
 *   modification, are permitted provided that the following conditions    *
 *   are met:                                                              *
 *   - Redistributions of source code must retain the above copyright      *
 *   notice, this list of conditions and the following disclaimer.         *
 *   - Redistributions in binary form must reproduce the above copyright   *
 *   notice, this list of conditions and the following disclaimer in the   *
 *   documentation and/or other materials provided with the distribution.  *
 *   - Neither the name of the FreeMedForms' organization nor the names of *
 *   its contributors may be used to endorse or promote products derived   *
 *   from this software without specific prior written permission.         *
 *                                                                         *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS   *
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT     *
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS     *
 *   FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE        *
 *   COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,  *
 *   INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  *
 *   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;      *
 *   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER      *
 *   CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT    *
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN     *
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE       *
 *   POSSIBILITY OF SUCH DAMAGE.                                           *
 ***************************************************************************/
/***************************************************************************
 *   Main Developper : Eric MAEKER, <eric.maeker@free.fr>                  *
 *   Contributors :                                                        *
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef TKTRANSLATORS_H
#define TKTRANSLATORS_H

#include <QObject>
#include <QMap>
#include <tkExporter.h>

class QTranslator;

/**
 * \file tkTranslators.h
 * \author Eric MAEKER <eric.maeker@free.fr>
 * \version 0.0.5
 * \date 13 April 2009
*/

/**
  \brief This class is a QTranslator manager. All translations files are loaded using the pathToTranslations().

  In first, instanciate this class inside the core of the app.

  Then set the translation path to use with : setPathToTranslations().

  You can add new translations file using members addNewTranslator().
  - specifying filename only (eg : 'myTrans' the suffix ('_lang') is automatcally added (using QLocale::staticDatas).
  - specifying full path of the file.

  Use changeLangage() to automacally reload all translators with the right language.Toolkit manages its own translator.

  Get availables languages and locales with : availableLocales(), availableLanguages(), availableLocalesAndLanguage().

  \ingroup toolkit
  \ingroup object_toolkit
*/
class Q_TK_EXPORT tkTranslators : public QObject
{
    Q_OBJECT
public:
    /** */
    static tkTranslators* instance( QObject * parent = 0) ;
    /** */
     ~tkTranslators();

    /** */
     bool addNewTranslator( const QString & fileName, bool fromDefaultPath = true );
    /** */
     bool addNewTranslator( const QString & path, const QString & file );
    /** */
     QString intToString( const int i );

public Q_SLOTS:
    /** */
     void changeLanguage( const QString & lang );

 public:
    /** */
     static bool setPathToTranslations( const QString & path );
    /** */
     static QString pathToTranslations();

    /** */
     static QStringList availableLocales();
    /** */
     static QStringList availableLanguages();
    /** */
     static QMap<QString, QString> availableLocalesAndLanguages();

 Q_SIGNALS:
    /** */
     void languageChanged();

 protected:
    /** */
     tkTranslators( QObject * parent = 0 );

 private:
     QMap<QString, QTranslator*>   m_Translators;  // String is file location and mask
     static QString                m_PathToTranslations;
     static tkTranslators         *m_Instance;
};

#endif

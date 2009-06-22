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
#ifndef TKUSERBASE_H
#define TKUSERBASE_H

// include toolkit headers
#include <tkDatabase.h>
class tkSettings;

// include usertoolkit headers
#include <tkUserExporter.h>
#include <tkUserConstants.h>
#include <tkUserGlobal.h>
class tkUser;

// include Qt headers
#include <QObject>
class QDateTime;


/**
 * \file mfUserBase.h
 * \author Eric MAEKER <eric.maeker@free.fr>
 * \version 0.07
 * \date 26 Feb 2009
*/

/**
  \brief This class owns the user database mechanism. It should never be directly accessed. Use tkUserModel to access to the database.

  1. Initialization\n
  This class owns a singleton. To instanciate it, use instance(). When instanciate for the first time,
  the initialize() member is called.

  2. Users retreiver\n
  You can retreive users using getUserByUuid() or using getUserByLoginPassword().

  3. Users saver\n
  You can regardless save or update users to database using the unique member : saveUser().

  4. Users datas checkers\n
  You can check the identifiants of users with the checkLogin() member.

  5. Users trace keeper\n
  The recaordLastLogin() update user's database with the actual login's date and time. It also update the login
  history data of the user. This login history need to be saved by hand ( saveUser() ).

  \ingroup usertoolkit database_usertoolkit
  \ingroup usermanager
*/

class Q_TKUSER_EXPORT tkUserBase : public tkDatabase
{
    Q_OBJECT
    friend void initLib();
//    friend void tkUser::warn() const;
    friend class tkUserModel;
    friend class tkUserModelPrivate;
    friend class tkUser;

protected:
    tkUserBase( QObject * parent = 0 );

    // Constructor
    static tkUserBase * instance(); // before calling this you should have created one instance of tkSettings
    virtual ~tkUserBase() {}

    // initialize
    bool initialize(  tkSettings * settings );

    // retreivers (pointers)
    tkUser * getUserById( const QVariant & _id ) const;  // not used
    tkUser * getUserByLoginPassword( const QVariant & login, const QVariant & cryptedPassword ) const;

    // retreivers (references)
    tkUser * getUserByUuid( const QString & uuid ) const;

    // datas retrievers
    QString getUuid( const QString & log64, const QString cryptpass64 );
    QString createNewUuid();

    // savers
    bool     saveUser( tkUser * user );
    bool     deleteUser( const QString & uuid );

    // datas checkers
    bool      checkLogin( const QString & login, const QString & cryptedPassword ) const;
    QDateTime recordLastLogin( const QString & log, const QString & pass );


private:
    bool createDatabase(  const QString & connectionName, const QString & dbName,
                          const QString & pathOrHostName,
                          TypeOfAccess access, AvailableDrivers driver,
                          const QString & login, const QString & pass,
                          CreationOption createOption
                          );
//    void setConnectionName( const QString & c );

    // privates retreivers
    tkUser * getUser( const QHash<int, QString> & conditions ) const;

private:
    static bool m_initialized;
    static QString  m_LastUuid, m_LastLogin, m_LastPass;
    static tkUserBase * m_Instance;
};

#endif // TKUSERBASE_H

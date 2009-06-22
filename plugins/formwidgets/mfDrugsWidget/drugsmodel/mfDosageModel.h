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
 *       NAME <MAIL@ADRESS>                                                *
 ***************************************************************************/
#ifndef MFDOSAGEMODEL_H
#define MFDOSAGEMODEL_H

#include <mfDrugsConstants.h>

#include <QObject>
#include <QSqlTableModel>
#include <QSet>
#include <QFlags>

using namespace mfDosagesConstants;

/**
  database( DOSAGES_DATABASE_NAME ) should be defined BEFORE instance()
  \todo Create a specific user's right for dosage creation/edition/modification/deletion +++.
 \ingroup drugsinteractions drugswidget
*/
class mfDosageModel : public QSqlTableModel
{
    Q_OBJECT
    enum Physio    // QFlag it
    {
        EveryOne     = 0,
        Nourrissons  = 0x00000001,
        Children     = 0x00000010,
        AdultOnly    = 0x00000100,
        InsufRenal   = 0x00001000,
        InsufHepat   = 0x00010000,
        WeigtLimited = 0x00100000,
        ManOnly      = 0x01000000,
        WomanOnly    = 0x10000000
    };

    enum Pregnancy  // QFlag it
    {
        PregnancyOk         = 0x00000000,
        FirstQuaOk          = 0x00000010,
        SecondQuaOk         = 0x00000100,
        ThirdQuaOk          = 0x00001000,
        UsableWithWarn      = 0x00010000,
        NeverDuringPregnacy = 0x00100000
    };

    enum Period
    {
        Seconds      = 0,
        Minutes,
        Hours,
        Days,
        Weeks,
        Months,
        Quarter,
        Year
    };

    enum MealTime
    {
        NoRelationWiyhMeal = 0,
        DuringMeal,
        BeforeMeal,
        AfterMeal,
        OutsideMeal
    };

    enum ScoredTablet
    {
        CompletTablet = 0,
        HalfTablet,
        QuaterTablet
    };

    enum PreDeterminedForms
    {
        DosePerKilo=0,
        SpoonReference,     // cuillère-mesure
        Spoon2_5ml,
        Spoon5ml,
        Puffs,              // bouffées
        Dose,
        Mouthwash,          // bain de bouche
        Inhalation,
        Application,
        Washing,            // lavement
        EyeWash,            // lavage oculaire
        Instillation
    };

public:
    enum DailyScheme
    {
        Undefined    = 0x00000000,
        Morning      = 0x00000001,
        Afternoon    = 0x00000010,
        TeaTime      = 0x00000100,
        Evening      = 0x00001000,
        BedTime      = 0x00010000
    };
    Q_DECLARE_FLAGS( DailySchemes, DailyScheme );


    mfDosageModel( QObject *parent = 0 );

    virtual int columnCount( const QModelIndex & = QModelIndex() ) const { return Dosage::MaxParam; }
    virtual bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    virtual QVariant data( const QModelIndex & item, int role = Qt::DisplayRole ) const;
    virtual bool insertRows( int row, int count, const QModelIndex & parent = QModelIndex() );
    virtual bool removeRows( int row, int count, const QModelIndex & parent = QModelIndex() );
    virtual void revertRow( int row );

    void setTable ( const QString & ) {}

    virtual bool setDrugCIS( const int _CIS );

    bool isDosageValid( const int row );

#ifdef DRUGS_INTERACTIONS_STANDALONE
    bool userCanRead()   { return true; }
    bool userCanWrite()  { return true; }
#else
    bool userCanRead();
    bool userCanWrite();
#endif

public Q_SLOTS:
    bool submitAll();

public:
    //--------------------------------------------------------------------------------------------------------
    //---------------------------------------- STATIC MEMBERS ------------------------------------------------
    //--------------------------------------------------------------------------------------------------------
    // static viewers to use for ui generation
    static void         initStaticDatas()      { retranslate() ; }
    static QStringList  periods();
    static QString      period( int id );
    static int          periodDefault()        { return Days; }
    static QStringList  scoredTabletScheme();
    static QStringList  dailyScheme();
    static QStringList  dailySchemes( const DailySchemes scheme );
    static DailySchemes toDailyScheme( const QStringList & list );
    static QStringList  mealTime();
    static QString      mealTime( int id );
    static QStringList  pregnancy();
    static QString      pregnancy( int id );
    static QStringList  predeterminedForms();

    // non static viewers for ui generation (need to be instanciate first)
    QStringList        forms();

    // Debugging Informations
    void warn( const int row = -1 );

private Q_SLOTS:
    void changeEvent( QEvent * event );
    static void retranslate();

    //--------------------------------------------------------------------------------------------------------
    //----------------------------------------- PRIVATE DATAS ------------------------------------------------
    //--------------------------------------------------------------------------------------------------------
private:
    static QStringList    m_Periods;
    static QStringList    m_DailyScheme;
    static QStringList    m_MealTime;
    static QStringList    m_Physiology;
    static QStringList    m_Pregnancy;
    static QStringList    m_BreastFeeding;
    static QStringList    m_ScoredTabletScheme;
    static QStringList    m_PreDeterminedForms;
    static QString        m_ActualLangage;
    int m_CIS;
    QSet<int> m_DirtyRows;
};
Q_DECLARE_OPERATORS_FOR_FLAGS( mfDosageModel::DailySchemes)

#endif // MFDOSAGEMODEL_H

/***************************************************************************
 *  The FreeMedForms project is a set of free, open source medical         *
 *  applications.                                                          *
 *  (C) 2008-2012 by Eric MAEKER, MD (France) <eric.maeker@gmail.com>      *
 *  All rights reserved.                                                   *
 *                                                                         *
 *  The FreeAccount plugins are free, open source FreeMedForms' plugins.   *
 *                                                                         *
 *  (C) 2013-now by Christian A Reiter and Eric Maeker                     *
 *  (C) 2010-2012 by Pierre-Marie Desombre and Eric Maeker                 *
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
 *  Main Developers :  Eric MAEKER, <eric.maeker@gmail.com>                *
 *  Contributors :                                                         *
 *      NAME <MAIL@ADDRESS.COM>                                            *
 ***************************************************************************/
#include "accountmode.h"

#include <coreplugin/icore.h>
#include <coreplugin/iuser.h>
#include <coreplugin/itheme.h>
#include <coreplugin/isettings.h>
#include <coreplugin/imainwindow.h>
#include <coreplugin/constants_menus.h>
#include <coreplugin/constants_icons.h>
#include <coreplugin/modemanager/modemanager.h>

#include <utils/log.h>
#include <utils/global.h>
#include <translationutils/constants.h>
#include <translationutils/trans_account.h>

#include <QDebug>

using namespace Account2;
using namespace Internal;
using namespace Trans::ConstantTranslations;

static inline Core::ITheme *theme()  { return Core::ICore::instance()->theme(); }
static inline Core::ISettings *settings() { return Core::ICore::instance()->settings(); }
static inline Core::ModeManager *modeManager() {return Core::ICore::instance()->modeManager();}
static inline Core::IMainWindow *mainWindow() { return Core::ICore::instance()->mainWindow(); }

// TEST
#include <QLabel>
#include <QTreeView>
#include <QTableView>
#include <QVBoxLayout>
#include <account2plugin/models/feemodel.h>
#include <account2plugin/models/paymentmodel.h>
#include <account2plugin/models/bankaccountmodel.h>
// END TEST

AccountMode::~AccountMode()
{
    qWarning() << "AccountMode::~AccountMode()"  ;
}

AccountMode::AccountMode(QObject *parent) :
    Core::IMode(parent)
{
    setDisplayName(tr("Account2"));
    setIcon(theme()->icon(Core::Constants::ICONACCOUNTMODE, Core::ITheme::BigIcon));
    setPriority(Core::Constants::P_MODE_ACCOUNT);
    setId(Core::Constants::MODE_ACCOUNT);
    setPatientBarVisibility(true);

//    const QList<int> &context;
//    setContext();
    m_Stack = new QStackedWidget;
    setWidget(m_Stack);

    connect(Core::ICore::instance(), SIGNAL(coreOpened()), this, SLOT(postCoreInitialization()));
    connect(modeManager(), SIGNAL(currentModeChanged(Core::IMode*)), this, SLOT(modeActivated(Core::IMode*)));
}

void AccountMode::setCentralWidget(QWidget *widget)
{
    QWidget *w = m_Stack->currentWidget();
    m_Stack->removeWidget(w);
    delete w;
    w = 0;
    m_Stack->addWidget(widget);
    m_Stack->setCurrentWidget(widget);
}

void AccountMode::postCoreInitialization()
{
    if (Utils::Log::warnPluginsCreation())
        qWarning() << Q_FUNC_INFO;
//    if (m_Stack)
//        m_Stack->addWidget(new ReceiptViewer);

    // TEST (FeeModel)
    qWarning() << "-------------------------------------------- BEGIN";
    QWidget *w = new QWidget(m_Stack);
    QVBoxLayout *lay = new QVBoxLayout(w);
    w->setLayout(lay);

    // Fee view
    QTableView *view = new QTableView(w);
    BasicFilter filter;
    filter.setDateRange(QDateTime::currentDateTime().addDays(-1000), QDateTime::currentDateTime());
    filter.addUser("%");
    filter.addPatient("%");
    FeeModel *model = new FeeModel(this);
    model->setFilter(filter);
    view->setModel(model);
    QLabel *lblFee = new QLabel("Fees Model/View", w);
    lay->addWidget(lblFee);
    lay->addWidget(view);

    // Payment view
    QTreeView *payView = new QTreeView(w);
    PaymentModel *payModel = new PaymentModel(this);
//    payModel->setFilter("%");
    payView->setModel(payModel);
    QLabel *lblPay = new QLabel("Payment Model/View", w);
    lay->addWidget(lblPay);
    lay->addWidget(payView);

    // BankAccount view
    QTableView *bkAccView = new QTableView(w);
    BankAccountModel *bkAccModel = new BankAccountModel(this);
    bkAccModel->setFilter("%");
    bkAccView->setModel(bkAccModel);
    QLabel *lblBkAcc = new QLabel("Bank account Model/View", w);
    lay->addWidget(lblBkAcc);
    lay->addWidget(bkAccView);

    // Manage stack
    m_Stack->addWidget(w);
    m_Stack->setCurrentWidget(w);
    qWarning() << "-------------------------------------------- END";
    // END TEST
}

void AccountMode::modeActivated(Core::IMode *mode)
{
//    if (mode!=this) {
//        return;
//    }
//    if (settings()->firstTimeRunning(Core::Constants::MODE_ACCOUNT)) {
//        QString firstExplanationText = QString("<center><span style=\"font-size:x-large;font-weight:bold;color:darkred;font-variant:small-caps\">%1</span></center>")
//                .arg(tr("The account plugin is still is early alpha stage (do not use in "
//                        "day-to-day practice)."));
//        firstExplanationText += "<br /><br />";
//        firstExplanationText += tr("Please read this explanation before using FreeAccount.\n"
//                                              "FreeAccount is composed of:\n"
//                                              "  - Receipts to get your earnings,\n"
//                                              "  - Movements in your accountancy,\n"
//                                              "  - Asset for your assets,\n"
//                                   "  - Ledger to analyse your accountancy and produce and print your ledger.").replace("\n", "<br />");
//        const QString &detail = tr("The shortcuts for those programs are:\n"
//                                   "  - for your rapid receipt, CTRL+R,\n"
//                                   "  - for the receipts widget, Maj+R,\n"
//                                   "  - for the movements widget, CTRL+M,\n"
//                                   "  - for assets widget, ALT+Z,\n"
//                                   "  - for ledger and analysis widget, ALT+L.\n"
//                                   "Before the first use, open Configuration > Preferences > Accountancy > Defaults,\n"
//                                   "and create defaults.\n"
//                                   "Then parametrize your user, sites, bank and so on.");
//        int b = Utils::withButtonsMessageBox(tkTr(Trans::Constants::ACCOUNTANCY),
//                                             firstExplanationText,
//                                             detail,
//                                             QStringList() << tr("Ok") << tr("Do not show this message again"),
//                                             tkTr(Trans::Constants::ACCOUNTANCY), false);

//        if (b==1) { // Don't show again
//            LOG("Removing Account first run dialog");
//            settings()->noMoreFirstTimeRunning(Core::Constants::MODE_ACCOUNT);
//        }
//    }
}

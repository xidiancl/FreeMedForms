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
 *   Main Developer: Christian A. Reiter <christian.a.reiter@gmail.com>    *
 *   Contributors:                                                         *
 *       Eric Maeker <eric.maeker@gmail.com                                *
 ***************************************************************************/
#include "webcamdialog.h"
#include "ui_webcamdialog.h"
#include "webcamconstants.h"
#include "opencvwidget.h"

#include <coreplugin/constants_icons.h>
#include <coreplugin/icore.h>
#include <coreplugin/itheme.h>

#include <utils/global.h>

//#include <translationutils/constants.h>
//#include <translationutils/trans

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QEvent>
#include <QDebug>


/*TODO:  improve webcam usability:
 * practitioner tells the patient to stand in front of the webcam
 * he clicks on a webcam symbol (that we can add in the identity widget or in a menu or anywhere else)
 * progress bar during the webcam acquiring
 * Dialog appears with e.g. 8 auto-shots (empty slots) + webcam capturing (+ freeze/unfreeze buttons),
 * FreeMedForms automatically takes 8 shots of the patient (2-3 per second) (select face, trim, color normalization)
 * Asks the practitioner to select his preferred one or to freeze/unfreeze the webcam
 * practitioner clicks on his preferred photo and it gets integrated in the patient identity widget
 -> Two clicks feature if practitioner select an auto-shot
 -> three/four clicks if he select the webcam freeze himself.
*/


using namespace Webcam;
using namespace Internal;

static inline Core::ITheme *theme() { return Core::ICore::instance()->theme(); }

WebcamDialog::WebcamDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WebcamDialog()),
    _faces(0)
{
    ui->setupUi(this);
//    qDebug() << this->objectName();
    setObjectName("WebCamDialog");
    setWindowIcon(theme()->icon(Core::Constants::ICONCAMERAVIDEO));
    setWindowTitle(tr("Take a picture from your webcam"));
    
    // Freeze button
    m_freezeButton = ui->buttonBox->addButton(tr("Freeze"), QDialogButtonBox::ActionRole);
    m_freezeButton->setIcon(theme()->icon(Core::Constants::ICONTAKESCREENSHOT));
    m_freezeButton->setCheckable(true);
    connect(m_freezeButton, SIGNAL(clicked(bool)), ui->openCVWidget, SLOT(setFrozen(bool))); 
    connect(ui->openCVWidget, SIGNAL(frozen(bool)), this, SLOT(updatefreezeButton(bool)));
    connect(ui->openCVWidget, SIGNAL(clicked()), m_freezeButton, SLOT(click()));
            
    QPushButton *button;

    // Cancel button
    button = ui->buttonBox->button(QDialogButtonBox::Cancel);
    button->setIcon(theme()->icon(Core::Constants::ICONQUIT));
    
    // Ok button
    button = ui->buttonBox->button(QDialogButtonBox::Ok);
    button->setIcon(theme()->icon(Core::Constants::ICONOK));
    button->setDisabled(true);
    connect(ui->openCVWidget, SIGNAL(imageReady(bool)), button, SLOT(setEnabled(bool)));   
    connect(ui->openCVWidget, SIGNAL(autoFaceShot(QPixmap)), this, SLOT(autoFaceShot(QPixmap)));
//    ui->imagesListView->setModel(ui->openCVWidget->model());
}

WebcamDialog::~WebcamDialog()
{
    delete ui;
}

QPixmap WebcamDialog::photo() const
{
    return ui->openCVWidget->pixmap()->copy(ui->openCVWidget->frame());
}

void WebcamDialog::updatefreezeButton(bool aFreeze)
{
    if (aFreeze) {
        m_freezeButton->setText(tr("Unfreeze"));
        m_freezeButton->setIcon(theme()->icon(Core::Constants::ICONCAMERAVIDEO));
    } else {
        m_freezeButton->setText(tr("Freeze"));
        m_freezeButton->setIcon(theme()->icon(Core::Constants::ICONTAKESCREENSHOT));
    }    
}

void WebcamDialog::autoFaceShot(const QPixmap &pix)
{
    switch (_faces) {
    case 0: ui->photo1->setPixmap(pix.scaled(ui->photo1->size(), Qt::KeepAspectRatio)); break;
    case 1: ui->photo2->setPixmap(pix.scaled(ui->photo1->size(), Qt::KeepAspectRatio)); break;
    case 2: ui->photo3->setPixmap(pix.scaled(ui->photo1->size(), Qt::KeepAspectRatio)); break;
    case 3: ui->photo4->setPixmap(pix.scaled(ui->photo1->size(), Qt::KeepAspectRatio)); break;
    }
    ++_faces;
}

void WebcamDialog::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        if (ui->openCVWidget->isFrozen())
            m_freezeButton->setText(tr("Unfreeze"));
        else
            m_freezeButton->setText(tr("Freeze"));
    }
}

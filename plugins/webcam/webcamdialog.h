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
 *       NAME <MAIL@ADDRESS.COM>                                           *
 ***************************************************************************/
#ifndef WEBCAM_DIALOG_H
#define WEBCAM_DIALOG_H

#include "ui_webcamdialog.h"
#include "capturethread.h"
#include <QDialog>

class TrackController;
class RenderWidget;
class QToolBar;
class QDockWidget;
class SettingsWidget;

namespace ui {
class WebcamDialog;
}

namespace Webcam {
namespace Internal {

class WebcamDialog : public QDialog {
    Q_OBJECT
public:
    WebcamDialog(QWidget *parent);
    ~WebcamDialog();

    QPixmap photo() const;


public slots:
    void onFlipVerticalChanged(bool flip);
    void startTracking();
    void stopTracking();
    void takePhoto();
    void updateStats();
protected:
    void closeEvent(QCloseEvent*);
private:
    Ui::WebcamDialog *ui;
    TrackController* trackController;
    QTimer* updateTimer;
    QPixmap m_snapshot;
};

} // end Internal
} // end Webcam
#endif

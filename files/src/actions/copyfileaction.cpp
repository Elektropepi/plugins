// albert - a simple application launcher for linux
// Copyright (C) 2014-2015 Manuel Schneider
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <QMimeData>
#include <QApplication>
#include <QClipboard>
#include <QUrl>
#include "copyfileaction.h"
#include "albertapp.h"

unsigned int Files::CopyFileAction::usageCounter = 0;

/** ***************************************************************************/
QString Files::CopyFileAction::name(const Query *q) const {
    Q_UNUSED(q);
    return "Copy file to clipboard";
}



/** ***************************************************************************/
QString Files::CopyFileAction::description(const Query *q) const {
    Q_UNUSED(q);
    return _file->absolutePath();
}



/** ***************************************************************************/
QIcon Files::CopyFileAction::icon() const {
    return QIcon::fromTheme("edit-copy");
}



/** ***************************************************************************/
void Files::CopyFileAction::activate(const Query *q) {
    Q_UNUSED(q);

    //  Get clipboard
    QClipboard *cb = QApplication::clipboard();

    // Ownership of the new data is transferred to the clipboard.
    QMimeData* newMimeData = new QMimeData();

    // Copy old mimedata
    const QMimeData* oldMimeData = cb->mimeData();
    for ( const QString &f : oldMimeData->formats())
        newMimeData->setData(f, oldMimeData->data(f));

    // Copy path of file
    newMimeData->setText(_file->absolutePath());

    // Copy file
    newMimeData->setUrls({QUrl::fromLocalFile(_file->absolutePath())});

    // Copy file (f*** you gnome)
    QByteArray gnomeFormat = QByteArray("copy\n").append(QUrl::fromLocalFile(_file->absolutePath()).toEncoded());
    newMimeData->setData("x-special/gnome-copied-files", gnomeFormat);

    // Set the mimedata
    cb->setMimeData(newMimeData);

    qApp->hideWidget();
}



/** ***************************************************************************/
uint Files::CopyFileAction::usage() const {
    return usageCounter;
}
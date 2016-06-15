/*
 * (C) Copyright 2014 Alex Spataru
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef DOWNLOAD_DIALOG_H
#define DOWNLOAD_DIALOG_H

#include <QDir>
#include <QIcon>
#include <QDateTime>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QNetworkAccessManager>

#include <math.h>

namespace Ui
{
class DownloadDialog;
}

class DownloadDialog : public QWidget
{
        Q_OBJECT

    public:
        explicit DownloadDialog (QWidget *parent = 0);
        ~DownloadDialog (void);

        void setIsLdrawDownload (bool isLdrawDownload) {m_isLdrawDownload = isLdrawDownload;}
        void setLdrawArchivePath (const QString &filePath) {m_ldrawArchivePath = filePath;}
        void setIsLdrawUnoffArchive (bool isUnoffArchive) {m_isUnoffArchive = isUnoffArchive;}
        void beginDownload (const QUrl& url);

    private slots:
        void openDownload (void);
        void installUpdate (void);
        void cancelDownload (void);
        void downloadFinished (void);
        void updateProgress (qint64 received, qint64 total);
        void ignoreSslErrors (QNetworkReply *reply, const QList<QSslError>& error);

    private:
        Ui::DownloadDialog *ui;

        QString m_path;
        QString m_ldrawArchivePath;
        bool m_download_paused;
        bool m_isLdrawDownload;
        bool m_isUnoffArchive;

        QNetworkReply *m_reply;
        QNetworkRequest m_downloadRequest;
        QNetworkAccessManager *m_manager;

        uint m_start_time;

        float roundNumber (const float& input);
};

#endif
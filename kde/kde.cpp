/***
Pix  Copyright (C) 2018  Camilo Higuita
This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.
This is free software, and you are welcome to redistribute it
under certain conditions; type `show c' for details.

 This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "kde.h"
#include <KService>
#include <KMimeTypeTrader>
#include <KToolInvocation>
#include <KLocalizedString>
#include <QDebug>
#include <KRun>
#include <QFileInfo>

KDE::KDE(QObject *parent) : QObject(parent)
{



}

static QVariantMap createActionItem(const QString &label, const QString &actionId, const QVariant &argument = QVariant())
{
    QVariantMap map;

    map["serviceLabel"] = label;
    map["actionId"] = actionId;

    if (argument.isValid())
        map["actionArgument"] = argument;


    return map;
}

QVariantList KDE::mimeApps(const QUrl &url)
{
    qDebug()<<"trying to get mimes";
    QVariantList list;

    if (url.isValid())
    {
        auto fileItem = new KFileItem(url);
        fileItem->determineMimeType();

        KService::List services = KMimeTypeTrader::self()->query(fileItem->mimetype(), "Application");

        if (!services.isEmpty())        
            foreach (const KService::Ptr service, services)
            {
                const QString text = service->name().replace('&', "&&");
                QVariantMap item = createActionItem(text, "_kicker_fileItem_openWith", service->entryPath());
                item["serviceIcon"] = service->icon();
                item["serviceExec"] = service->exec();

                list << item;
            }


        list << createActionItem(i18n("Properties"), "_kicker_fileItem_properties");

        return list;
    } else return list;
}

void KDE::openWithApp(const QString &exec, const QString &url)
{
    KService service(exec);
    KRun::runApplication(service,{url}, nullptr);
}

void KDE::attachEmail(const QString &url)
{
    qDebug()<< "invoking email"<< url;
    QFileInfo file(url);

    KToolInvocation::invokeMailer("", "", "", file.baseName(), "Sent from Pix", "", {url});
//    QDesktopServices::openUrl(QUrl("mailto:?subject=test&body=test&attachment;="
//    + url));
}
#include "trayiconnotifer.h"

#include <QSystemTrayIcon>

TrayIconNotifer::TrayIconNotifer(SnoreServer *snore, QSystemTrayIcon *icon):
        Notification_Backend("TrayiconNotifer",snore),
        _trayIcon(icon),
        _id(0)
{}

void TrayIconNotifer::registerApplication(Application *application){

}

int TrayIconNotifer::notify(QSharedPointer<Notification> notification){
    _trayIcon->showMessage(notification->title(),notification->text(),QSystemTrayIcon::NoIcon,notification->timeout()*1000);
    return _id++;
}

void TrayIconNotifer::closeNotification(int id){

}
/*
    SnoreNotify is a Notification Framework based on Qt
    Copyright (C) 2013-2015  Hannah von Reth <vonreth@kde.org>

    SnoreNotify is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SnoreNotify is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with SnoreNotify.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SNARL_BACKEND_H
#define SNARL_BACKEND_H
#include "libsnore/plugins/snorebackend.h"
#include "SnarlInterface.h"

class SnarlWidget;

namespace SnorePlugin {

class Snarl : public Snore::SnoreBackend
{
    Q_OBJECT
    Q_INTERFACES(Snore::SnoreBackend)
    Q_PLUGIN_METADATA(IID "org.Snore.NotificationBackend/1.0" FILE "snore_plugin.json")
public:
    Snarl();
    ~Snarl();

    virtual bool canCloseNotification() const override;
    virtual bool canUpdateNotification() const override;

    bool isReady() override;

protected:
    void setDefaultSettings() override;

private:
    friend class SnarlWidget;
    SnarlWidget *m_eventLoop = nullptr;
    QHash<QString, ::Snarl::V42::SnarlInterface *> m_applications;

public Q_SLOTS:
    void slotRegisterApplication(const Snore::Application &application) override;
    void slotDeregisterApplication(const Snore::Application &application) override;
    void slotNotify(Snore::Notification notification) override;
    void slotCloseNotification(Snore::Notification notification) override;

private:
    QHash<LONG32, Snore::Notification> m_idMap;

    friend class ::SnarlWidget;
};
}

#endif // SNARL_BACKEND_H

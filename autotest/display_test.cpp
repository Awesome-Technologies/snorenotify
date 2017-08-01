#include <libsnore/snore.h>
#include <libsnore/snore_p.h>
#include <libsnore/utils.h>
#include <libsnore/snoreconstants.h>

#include <QTextDocument>

#include <QtTest>

using namespace Snore;

class DisplayTest : public QObject
{
    Q_OBJECT
public:
    DisplayTest():
        app(QStringLiteral("Test"), Icon::defaultIcon())
    {
        SnoreCore &instance = SnoreCore::instance();
        instance.loadPlugins(SnorePlugin::Backend);
        instance.setSettingsValue(Snore::Constants::SettingsKeys::Timeout, 10);
        SnoreCore::instance().registerApplication(app);
    }

    Application app;

private Q_SLOTS:
    void displayTest();
    void displayTestPlain();

private:
    void testString(const QStringList &messages)
    {
        SnoreCore &snore = SnoreCore::instance();
#if 0
        qDebug() << snore.pluginNames(SnorePlugin::Backend);
        QStringList backends({QStringLiteral("Snore")});
#else
        QStringList backends = snore.pluginNames(SnorePlugin::Backend);
#endif
        auto notify = [&backends, &snore, &messages, this](Notification n) {
            qDebug() << n << "closed";
            qDebug() << backends.size();
            if (backends.empty()) {
                return;
            }
            QString old = snore.primaryNotificationBackend();
            while (!backends.empty() && snore.primaryNotificationBackend() == old) {
                QString p = backends.takeLast();
                snore.setSettingsValue(Snore::Constants::SettingsKeys::PrimaryBackend, p);
                SnoreCorePrivate::instance()->syncSettings();
                if (snore.primaryNotificationBackend() == p) {
                    for (const auto &message : messages) {
                        snore.broadcastNotification(Notification(app, app.defaultAlert(), QStringLiteral("Title"), message, app.icon()));
                    }
                }
            }
        };
        auto con = connect(&snore, &SnoreCore::notificationClosed, notify);
        notify(Notification());
        while (!backends.empty()) {
            QTest::qWait(100);
        }
        QTest::qWait(10000);
        disconnect(con);
    }
};

void DisplayTest::displayTest()
{
    app.hints().setValue("use-markup", true);
    testString({QStringLiteral("<b>Test&#937;</b>&#x1f4a9;&#x1f600;"),
                QString::fromUtf8("😀<b>💩Test&#937;</b>&#x1f4a9;&#x1f600;")});
 }

void DisplayTest::displayTestPlain()
{
    app.hints().setValue("use-markup", false);
    testString({QString::fromWCharArray(L"Test\u03A9\U0001F4A9\U0001F600"),
               QString::fromUtf8("TestΩ💩😀")});
}

QTEST_MAIN(DisplayTest)

#include "display_test.moc"

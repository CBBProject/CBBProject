#include <MainWindow.h>
#include <dtkCore/dtkGlobal.h>
#include <dtkCore/dtkPluginManager.h>
#include <dtkLog/dtkLog.h>
#include <dtkLog/dtkLogger.h>
#include <QtCore>
#include <QtGui>

int
main(int argc, char **argv) {

    //QCoreApplication::setAttribute(Qt::AA_X11InitThreads);

    QApplication application(argc, argv);
    application.setApplicationName("cbb-platform");
    application.setApplicationVersion("0.0.1");
    application.setOrganizationName("inria");
    application.setOrganizationDomain("fr");

    dtkLogger::instance().setLevel(dtkLog::Trace);
    dtkLogger::instance().attachFile(QString("CBBProject.log"));
    qDebug() << dtkLogPath(&application);

    dtkPluginManager* pm = dtkPluginManager::instance();
    pm->setVerboseLoading(true);
    pm->initialize();
    pm->printPlugins();

    MainWindow *window = new MainWindow;
    window->show();
    window->raise();

    int status = application.exec();

    delete window;

    dtkPluginManager::instance()->uninitialize();

    return status;
}

#include "mainlauncher.h"
#include "ui_mainlauncher.h"

#include <QProcess>
#include <QLabel>
#include <QTimerEvent>
#include <QDir>
#include <QResizeEvent>

MainLauncher::MainLauncher(QWidget *parent)
    : QDialog(parent)
{
    pBackground = new QFrame(this);
    pBackground->setMinimumSize(550, 220);
    pBackground->setStyleSheet("background-image: url(:/background.jpg);");

    pInterface = new Ui::MainLauncher();
    pInterface->setupUi(this);

    pAntiCheat = new AntiCheat();
    pAntiCheat->start();

    pChangeLog = new ChangeLog(this);

    this->startTimer(HG_INDEX_INTERVAL);
}

MainLauncher::~MainLauncher()
{
    delete pAntiCheat;
    delete pChangeLog;
    delete pBackground;
    delete pInterface;
}

void MainLauncher::timerEvent(QTimerEvent*)
{
    pInterface->webView->reload();
}

void MainLauncher::resizeEvent(QResizeEvent * event)
{
    QSize sizeChange = event->size() - event->oldSize();
    pBackground->setSizeIncrement(sizeChange);
    pInterface->verticalLayoutWidget->setSizeIncrement(sizeChange);
}

void MainLauncher::on_b_play_clicked()
{
    QStringList commands;
    commands << "";
    #ifndef WIN32
        commands << " -opengl";
    #endif

    QProcess *wowApp = new QProcess(this);

    wowApp->start("wow.exe", commands);

    if (!wowApp->waitForFinished())
    {
        QDialog *err = new QDialog(this);

        QLabel *errortxt = new QLabel(err);
        errortxt->setText(wowApp->errorString());

        err->show();
    }
}

void MainLauncher::on_b_changelog_clicked()
{
    pChangeLog->show();
}

void MainLauncher::on_b_clearCache_clicked()
{
    QDir currentDir = QDir::current();  // get app directory
    if (currentDir.exists("Wow.exe"))   // check if it's WoW directory
        if (currentDir.cd("Cache"))     // change directory to Cache to have absolute path
            currentDir.rmpath(currentDir.absolutePath());
}

void MainLauncher::on_webView_urlChanged(QUrl newUrl)
{
    if (newUrl != HG_LAUNCHER_INDEX)
        on_webView_loadFinished(false);
}

void MainLauncher::on_webView_loadFinished(bool bSuccess)
{
    if (!bSuccess)
    {
        pInterface->webView->hide();
        pBackground->show();
    }
}

ChangeLog::ChangeLog(QWidget * /*parent*/)
{
    setWindowTitle("Changelog");
    setGeometry(QRect(50,50, 300, 400));
    setMinimumSize(QSize(250, 420));
    setMaximumSize(QSize(250, 430));

    pView = new QWebView(this);
    pView->setUrl(QUrl("http://localhost/~lukaasm/changelog.html"));
    pView->setMaximumSize(QSize(250, 800));
}

ChangeLog::~ChangeLog()
{
    delete pView;
}

AntiCheat::AntiCheat()
    : bDone(false)
{
}

AntiCheat::~AntiCheat()
{
    bDone = true;
    wait();
}

void AntiCheat::run()
{
    while (!bDone)
    {
        // zrob cos
        // sleep na 5s ;]
        msleep(5000);
    }
}

void MainLauncher::on_webView_linkClicked(const QUrl &/*arg1*/)
{

}

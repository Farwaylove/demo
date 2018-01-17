#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QBoxLayout>
#include "cxmainwindow.h"
#include "PluginManager/cxpluginmanager.h"

CxMainWindow::CxMainWindow(QWidget *parent) : QMainWindow(parent)
{
    setMinimumSize(480, 320);
    QMenuBar *bar  = menuBar();
    listMenu   = bar->addMenu(tr("实例列表"));
    unloadMenu = bar->addMenu(tr("卸载实例"));
    action     = bar->addAction(tr("加载实例"));

    mdiArea = new QMdiArea(this);
    //mdiArea->setViewMode(QMdiArea::TabbedView);
    setCentralWidget(mdiArea);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(TryloadPlugin(bool)));
    connect(listMenu, SIGNAL(triggered(QAction *)), this, SLOT(DisplayDemo(QAction *)));
    connect(unloadMenu, SIGNAL(triggered(QAction *)), this, SLOT(UnloadPlugin(QAction *)));
}

void  CxMainWindow::TryloadPlugin(bool checked)
{

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "./", tr("DLL files ( *.dll); "));
    if (!fileName.isNull())
    {
        CxPluginManager *manager = CxPluginManager::Instance();
        CxLibPlugin     *plugin = manager->Load(fileName);
        if (plugin)
        {
            listMenu->addAction(plugin->PluginName());
            unloadMenu->addAction(plugin->PluginName());
            QMessageBox message;
            message.setText(tr("Loaded plugin \"%1\" success!").arg(plugin->PluginName()));
            message.exec();
        }
    }
}

void            CxMainWindow::DisplayDemo(QAction *action)
{
    CxPluginManager *manager = CxPluginManager::Instance();
    CxLibPlugin     *plugin = manager->find(action->text());
    if (plugin)
    {
        QMdiSubWindow  *subwin = new QMdiSubWindow;
        subwin->setWindowTitle(plugin->PluginName());
        subwin->setAttribute(Qt::WA_DeleteOnClose);
        subwin->setMinimumSize(480, 320);
        mdiArea->addSubWindow(subwin);
        QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, subwin);
        subwin->setLayout(layout);
        CxIDemo *demo = plugin->CreateDemo(subwin);
        demo->Display();
        //qDebug("%s", fileName);
        subwin->show();
    }
}

void            CxMainWindow::UnloadPlugin(QAction *action)
{
    CxPluginManager *manager = CxPluginManager::Instance();
    bool     ret = manager->Unload(action->text());
    if (ret)
    {
        QMessageBox message;
        message.setText(tr("Unloaded plugin \"%1\"!").arg(action->text()));
        message.exec();
        Remove(listMenu, action->text());
        Remove(unloadMenu, action->text());
    }
}

void            CxMainWindow::Remove(QMenu *menu, const QString& action)
{
    QList<QAction*> list = menu->findChildren<QAction*>();
    foreach (QAction* qaction, list) {
        if (qaction->text() == action)
        {
            menu->removeAction(qaction);
        }
    }
}



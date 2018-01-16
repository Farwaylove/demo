#ifndef CXMAINWINDOW_H
#define CXMAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>

class CxMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CxMainWindow(QWidget *parent = nullptr);

signals:

public slots:
    void            TryloadPlugin(bool checked);
    void            DisplayDemo(QAction *action);
    void            UnloadPlugin(QAction *action);
private:
    void            Remove(QMenu *menu, const QString action);
private:
    QMenu          *unloadMenu;
    QMenu          *listMenu;
    QMdiArea       *mdiArea;
    QAction        *action;

};

#endif // CXMAINWINDOW_H

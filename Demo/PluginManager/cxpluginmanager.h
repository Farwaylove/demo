#ifndef CXPLUGINMANAGER_H
#define CXPLUGINMANAGER_H

#include <QLibrary>
#include <QMap>
#include "cxidemo.h"

/**********************************************/
/*     share plugin library class   　　　　　　*/
/**********************************************/
class   QWidget;
typedef CxIDemo* (*CreateDemoPlugin)(QWidget *);
typedef void     (*ReleaseDemoPlugin)(CxIDemo*);
typedef const    QString& (*GetPluginName)();

class  CxLibPlugin: public QLibrary
{
    Q_OBJECT
public:
    explicit CxLibPlugin(QObject *parent = nullptr);
    ~CxLibPlugin();
public:
    bool      Init(const QString &pluginFile); //load library and init class member object.
    CxIDemo*  CreateDemo(QWidget *parent);
    void      ReleaseDemo(CxIDemo* demo);
    const QString& PluginName();
private:
    CreateDemoPlugin  createDemoFun;
    ReleaseDemoPlugin releaseDemoFun;
    GetPluginName     getPluginNameFun;
};


/*********************************************/
/*     plugin libraries manager              */
/*********************************************/
class CxPluginManager: public QObject
{
    Q_OBJECT
private:
    explicit CxPluginManager(QObject *parent = nullptr);
    ~CxPluginManager();
public:
    static CxPluginManager* Instance();
public:
    CxLibPlugin *Load(const QString &pluginFile);
    bool        Unload(const QString &pluginName);
    CxLibPlugin *find(const QString &pluginName);
private:
    QMap<QString, CxLibPlugin*> map;
};

//CxPluginManager* GetPluginManager();

#endif // CXPLUGINMANAGER_H

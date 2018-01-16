#include <QWidget>
#include "cxpluginmanager.h"

/**************class CxLibPlugin ****************/
CxLibPlugin::CxLibPlugin(QObject *parent):QLibrary(parent)
{
    createDemoFun    = nullptr;
    releaseDemoFun   = nullptr;
    getPluginNameFun = nullptr;
}

CxLibPlugin::~CxLibPlugin()
{
}

bool   CxLibPlugin::Init(const QString &pluginFile)
{
    setFileName(pluginFile);
    if (isLoaded()) return false;//TODO: make sure if it is right?

    bool ret  = load();
    if (ret)
    {
        ret   = false;
        createDemoFun     = (CreateDemoPlugin) this->resolve("CreateDemo");
        releaseDemoFun    = (ReleaseDemoPlugin) this->resolve("ReleaseDemo");
        getPluginNameFun  = (GetPluginName) this->resolve("GetDemoName");

        if (createDemoFun && releaseDemoFun && getPluginNameFun)
        {
            ret = true;
        }
        else
        {
            unload();
        }
    }
    return ret;
}

CxIDemo*  CxLibPlugin::CreateDemo(QWidget *parent)
{
    if (createDemoFun)
    {
        return createDemoFun(parent);
    }
    return nullptr;
}
void      CxLibPlugin::ReleaseDemo(CxIDemo* demo)
{
    if (releaseDemoFun)
    {
        releaseDemoFun(demo);
    }
}
const QString& CxLibPlugin::PluginName()
{
    static  QString emptyStr;
    if (getPluginNameFun)
    {
        return getPluginNameFun();
    }
    return emptyStr;
}

/**************class CxPluginManager ****************/
CxPluginManager::CxPluginManager(QObject *parent):QObject(parent)
{
}

CxPluginManager::~CxPluginManager()
{
#if 0
    QMap<QString, QLibrary*>::const_iterator i = map.begin();
    for (; i != map.end(); i++)
    {
        QLibrary *plugin = i.value();
        if (plugin)
        {
            if (plugin->isLoaded())
            {
                plugin->unload();
            }
            delete plugin;
        }
    }
#endif
}

CxPluginManager* CxPluginManager::Instance()
{
    static CxPluginManager *manager = 0;
    if (!manager)
    {
        manager = new CxPluginManager();
    }
    return manager;
}

CxLibPlugin *CxPluginManager::Load(const QString &pluginFile)
{
    CxLibPlugin *plugin = new CxLibPlugin(this);
    if (plugin->Init(pluginFile) == false)
    {
        plugin->setParent(nullptr);
        delete plugin;
        plugin = nullptr;
    }
    else
    {
        QString pluginName = plugin->PluginName();
        map.insert(pluginName, plugin);
    }
    return plugin;
}

bool      CxPluginManager::Unload(const QString &pluginName)
{
    QMap<QString, CxLibPlugin*>::iterator iter = map.end();
    iter = map.find(pluginName);
    if (iter != map.end())
    {
        CxLibPlugin *plugin = iter.value();
        if (plugin)
        {
            plugin->setParent(nullptr);
            delete plugin;
        }
        map.erase(iter);
        return true;
    }
    return false;
}

CxLibPlugin *CxPluginManager::find(const QString &pluginName)
{
    QMap<QString, CxLibPlugin*>::iterator iter = map.end();
    iter = map.find(pluginName);
    if (iter != map.end())
    {
        return iter.value();
    }
    return nullptr;
}

//CxPluginManager* GetPluginManager()
//{
//    return CxPluginManager::Instance();
//}

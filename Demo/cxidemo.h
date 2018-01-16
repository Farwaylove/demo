#ifndef CXIDEMO_H_180115
#define CXIDEMO_H_180115

#include <qglobal.h>
#ifdef  PLUGIN_LIB
# define DEMO_SHARE Q_DECL_EXPORT
#else
# define DEMO_SHARE Q_DECL_IMPORT
#endif

class QString;
class DEMO_SHARE CxIDemo
{
public:
//    void  Load()    = 0;
//    void  Unload()  = 0;
    virtual void  Display() = 0;
    virtual void  Close()   = 0;
    //virtual const QString &GetName() = 0;
};

#endif // CXIDEMO_H

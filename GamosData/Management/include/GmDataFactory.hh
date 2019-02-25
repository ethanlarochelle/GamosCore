#ifndef GmDataFactory_H
#define GmDataFactory_H

#include "GmVData.hh"
#include "PluginManager/PluginFactory.h"

/// Plug-in factory for objects of type GmVData
class GmDataFactory : public seal::PluginFactory<GmVData *()>
{
public:
    static GmDataFactory *get (void);

private:
    GmDataFactory (void);
    static GmDataFactory s_instance;
};


#define DEFINE_GAMOS_DATA(type) \
  DEFINE_SEAL_PLUGIN(GmDataFactory, type,#type)

#endif // GmDataFactory_H

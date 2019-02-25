#include "GmVerbosityFactory.hh"

GmVerbosityFactory GmVerbosityFactory::s_instance;

GmVerbosityFactory::GmVerbosityFactory (void)
    : seal::PluginFactory<GmVVerbosity*()> ("GmVerbosityFactory")
{}

GmVerbosityFactory *
GmVerbosityFactory::get (void)
{ return &s_instance; }

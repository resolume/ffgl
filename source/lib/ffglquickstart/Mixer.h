#pragma once
#include "Plugin.h"
class Mixer : public Plugin
{
public:
	Mixer();
	~Mixer();

	template< typename PluginType >
	static PluginInstance createPlugin( PluginInfo infos );

	FFResult render( ProcessOpenGLStruct* pGL ) override;
};

template< typename PluginType >
inline PluginInstance Mixer::createPlugin( PluginInfo infos )
{
	return Plugin::createPlugin< PluginType >( infos, FF_MIXER );
}

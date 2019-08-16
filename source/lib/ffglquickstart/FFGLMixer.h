#pragma once
#include "FFGLPlugin.h"

namespace ffglqs
{

class Mixer : public Plugin
{
public:
	Mixer();
	~Mixer();

	template< typename PluginType >
	static PluginInstance createPlugin( PluginInfo infos );

	virtual FFResult render( ProcessOpenGLStruct* pGL ) override;
};

template< typename PluginType >
inline PluginInstance Mixer::createPlugin( PluginInfo infos )
{
	return Plugin::createPlugin< PluginType >( infos, FF_MIXER );
}

}//End namespace ffglqs
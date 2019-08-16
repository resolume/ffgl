#pragma once
#include "FFGLPlugin.h"

namespace ffglqs
{

class Source : public Plugin
{
public:
	Source();
	~Source();

	template< typename PluginType >
	static PluginInstance createPlugin( PluginInfo infos );
};

template< typename PluginType >
inline PluginInstance Source::createPlugin( PluginInfo infos )
{
	return Plugin::createPlugin< PluginType >( infos, FF_SOURCE );
}

}//End namespace ffglqs
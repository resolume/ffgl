#pragma once
#include "FFGLPlugin.h"

namespace ffglqs
{
class Source : public Plugin
{
public:
	Source( bool supportTopLeftTextureOrientation = false );
	~Source();

	template< typename PluginType >
	static PluginInstance CreatePlugin( PluginInfo infos );
};

template< typename PluginType >
inline PluginInstance Source::CreatePlugin( PluginInfo infos )
{
	return Plugin::CreatePlugin< PluginType >( infos, FF_SOURCE );
}

}//End namespace ffglqs
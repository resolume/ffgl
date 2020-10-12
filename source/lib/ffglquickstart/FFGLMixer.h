#pragma once
#include "FFGLPlugin.h"

namespace ffglqs
{
class Mixer : public Plugin
{
public:
	Mixer( bool supportTopLeftTextureOrientation = false );
	~Mixer();

	template< typename PluginType >
	static PluginInstance createPlugin( PluginInfo infos );

	virtual FFResult Render( ProcessOpenGLStruct* pGL ) override;
};

template< typename PluginType >
inline PluginInstance Mixer::createPlugin( PluginInfo infos )
{
	return Plugin::CreatePlugin< PluginType >( infos, FF_MIXER );
}

}//End namespace ffglqs
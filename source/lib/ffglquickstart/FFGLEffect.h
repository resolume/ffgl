#pragma once
#include "FFGLPlugin.h"

namespace ffglqs
{
class Effect : public Plugin
{
public:
	Effect( bool supportTopLeftTextureOrientation = false );
	~Effect();

	template< typename PluginType >
	static PluginInstance CreatePlugin( PluginInfo infos );

	virtual FFResult Render( ProcessOpenGLStruct* pGL ) override;
};

template< typename PluginType >
inline PluginInstance Effect::CreatePlugin( PluginInfo infos )
{
	return Plugin::CreatePlugin< PluginType >( infos, FF_EFFECT );
}

}//End namespace ffglqs
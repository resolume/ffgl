#pragma once
#include "FFGLPlugin.h"

namespace ffglqs
{

class Effect : public Plugin
{
public:
	Effect();
	~Effect();

	template< typename PluginType >
	static PluginInstance createPlugin( PluginInfo infos );

	virtual FFResult render( ProcessOpenGLStruct* pGL ) override;
};

template< typename PluginType >
inline PluginInstance Effect::createPlugin( PluginInfo infos )
{
	return Plugin::createPlugin< PluginType >( infos, FF_EFFECT );
}

}//End namespace ffglqs
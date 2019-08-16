#pragma once
#include <memory>
#include <string>
#include "../ffgl/FFGL.h"

namespace ffglqs
{

class Param
{
public:
	static std::shared_ptr< Param > create();
	static std::shared_ptr< Param > create( std::string name );
	static std::shared_ptr< Param > create( std::string name, float value );
	static std::shared_ptr< Param > create( std::string name, FFUInt32 type, float value );

	Param();
	Param( std::string name );
	Param( std::string name, float value );
	Param( std::string name, FFUInt32 type, float value );

	virtual void setValue( float v );

	virtual float getValue() const;
	const std::string& getName() const;
	FFUInt32 getType() const;

protected:
	float value = 0;
	std::string name;
	FFUInt32 type;
};

}//End namespace ffglqs
#pragma once
#include <memory>
#include <string>
#include "../ffgl/FFGL.h"

namespace ffglqs
{
class Param
{
public:
	static std::shared_ptr< Param > Create();
	static std::shared_ptr< Param > Create( std::string name );
	static std::shared_ptr< Param > Create( std::string name, float value );
	static std::shared_ptr< Param > Create( std::string name, FFUInt32 type, float value );

	Param();
	Param( std::string name );
	Param( std::string name, float value );
	Param( std::string name, FFUInt32 type, float value );

	virtual void SetValue( float v );

	virtual float GetValue() const;
	const std::string& GetName() const;
	FFUInt32 GetType() const;

protected:
	float value = 0;
	std::string name;
	FFUInt32 type;
};

}//End namespace ffglqs
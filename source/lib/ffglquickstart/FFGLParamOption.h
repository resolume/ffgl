#pragma once
#include <vector>
#include "FFGLParam.h"

namespace ffglqs
{

class ParamOption : public Param
{
public:
	struct Option
	{
		std::string name;
		float value;
	};
	std::vector< Option > options;

	static std::shared_ptr< ParamOption > create( std::string name, std::vector< Option > options );
	static std::shared_ptr< ParamOption > create( std::string name, std::vector< Option > options, int defaultOption );

	ParamOption( std::string name, std::vector< Option > options );
	ParamOption( std::string name, std::vector< Option > options, int defaultOption );

	void setValue( float newValue ) override;

	float getRealValue() const;

	bool isCurrentOption( const std::string& optionName ) const;

private:
	Option currentOption;
};

}//End namespace ffglqs
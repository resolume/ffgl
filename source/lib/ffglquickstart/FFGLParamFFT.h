#pragma once
#include <vector>
#include "FFGLParam.h"

namespace ffglqs
{
class ParamFFT : public Param
{
public:
	static std::shared_ptr< ParamFFT > Create( std::string name );
	static std::shared_ptr< ParamFFT > Create( std::string name, size_t size );

	ParamFFT( std::string name, size_t size );

	unsigned int index;
	std::vector< float > fftData;
};

}//End namespace ffglqs

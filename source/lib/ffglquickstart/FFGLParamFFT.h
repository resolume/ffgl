#pragma once
#include <vector>
#include "FFGLParam.h"

namespace ffglqs
{

class ParamFFT : public Param
{
public:
	static std::shared_ptr< ParamFFT > create( std::string name );
	static std::shared_ptr< ParamFFT > create( std::string name, size_t size );

	ParamFFT( std::string name, size_t size );

	unsigned int index;
	std::vector< float > fftData;

private:
	static const int DEFAULT_BUFFER_SIZE = 2048;
};

}//End namespace ffglqs
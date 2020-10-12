#include "FFGLParamFFT.h"

namespace ffglqs
{
static constexpr int DEFAULT_BUFFER_SIZE = 2048;

std::shared_ptr< ParamFFT > ParamFFT::Create( std::string name )
{
	return std::make_shared< ParamFFT >( std::move( name ), DEFAULT_BUFFER_SIZE );
}
std::shared_ptr< ParamFFT > ParamFFT::Create( std::string name, size_t size )
{
	return std::make_shared< ParamFFT >( std::move( name ), size );
}

ParamFFT::ParamFFT( std::string name, size_t size ) :
	Param( std::move( name ) ),
	fftData( size )
{
	type = FF_TYPE_BUFFER;
}

}//End namespace ffglqs

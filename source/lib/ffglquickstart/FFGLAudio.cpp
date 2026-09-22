#include "FFGLAudio.h"
#include <algorithm>
#include <cmath>
#include "../ffglex/FFGLUtilities.h"

namespace ffglqs
{
Audio::Audio()
{
}

Audio::~Audio()
{
}

void Audio::Update( std::vector< float > _fft )
{
	fft                   = std::move( _fft );
	float currentVolStore = 0;
	float bassStore       = 0;
	float bassCount       = 0;
	int bassSep           = static_cast< int >( fft.size() / 3 );
	float medStore        = 0;
	float medCount        = 0;
	int highSep           = bassSep * 2;
	float highStore       = 0;
	float highCount       = 0;

	for( int i = 0; i < fft.size(); i++ )
	{
		float bin = fft[ i ] * fft[ i ] * gain;

		if( i < bassSep )
		{
			bassStore += bin;
			bassCount++;
		}
		if( bassSep < i && i < highSep )
		{
			medStore += bin;
			medCount++;
		}
		if( highSep < i )
		{
			highStore += bin;
			highCount++;
		}
		currentVolStore += bin;
	}
	currentVolStore /= (float)fft.size();
	currentVolStore = sqrt( currentVolStore );
	vol.Update( currentVolStore );

	bassStore /= bassCount;
	bassStore = sqrt( bassStore );
	bass.Update( bassStore );

	medStore /= medCount;
	medStore = sqrt( medStore );
	med.Update( medStore );

	highStore /= highCount;
	highStore = sqrt( highStore );
	high.Update( highStore );
}

float Audio::GetVolume()
{
	return vol.GetValue();
}

float Audio::GetBass()
{
	return bass.GetValue();
}

float Audio::GetMed()
{
	return med.GetValue();
}

float Audio::GetHigh()
{
	return high.GetValue();
}

float Audio::GetVolumeFromTo( int fromFreq, int toFreq )
{
	if( fft.empty() || sampleRate <= 0 )
		return 0.0f;

	if( fromFreq > toFreq )
		std::swap( fromFreq, toFreq );

	const int fftSize   = static_cast< int >( fft.size() );
	const float binStep = freqMax / static_cast< float >( fftSize );

	int fromBin = static_cast< int >( fromFreq / binStep );
	int toBin   = static_cast< int >( std::ceil( toFreq / binStep ) );

	//Clamp to the valid bin range, and make sure the range covers at least one bin even if it's
	//narrower than a single bin's frequency width.
	fromBin = std::min( std::max( fromBin, 0 ), fftSize - 1 );
	toBin   = std::min( std::max( toBin, fromBin + 1 ), fftSize );

	float sum = 0.0f;
	for( int i = fromBin; i < toBin; i++ )
		sum += fft[ i ] * fft[ i ] * gain;

	sum /= static_cast< float >( toBin - fromBin );
	return std::sqrt( sum );
}

float Audio::ToDB( float rms )
{
	float db = 20 * std::log10( rms );
	db       = ffglex::map( db, -96, 0, 0, 1 );
	if( std::isinf( db ) )
		db = 0;
	return db;
}

void Audio::SetSmoothness( float smoothness )
{
	vol.SetSmoothness( smoothness );
	bass.SetSmoothness( smoothness );
	med.SetSmoothness( smoothness );
	high.SetSmoothness( smoothness );
}

void Audio::SetSampleRate( int _sampleRate )
{
	sampleRate = _sampleRate;
	freqMax    = (float)sampleRate / 2.f;
}

void Audio::SetGain( float _gain )
{
	gain = pow( 10.f, _gain / 20.f );
}

}//End namespace ffglqs
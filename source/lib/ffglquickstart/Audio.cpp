#include "Audio.h"
#include <cmath>

Audio::Audio()
{
}

Audio::~Audio()
{
}

void Audio::update( std::vector< float > _fft )
{
	fft                   = _fft;
	float currentVolStore = 0;
	float bassStore       = 0;
	float bassCount       = 0;
	int bassSep           = Audio::getBufferSize() / 3;
	float medStore        = 0;
	float medCount        = 0;
	int highSep           = bassSep * 2;
	float highStore       = 0;
	float highCount       = 0;

	float gain = pow( 10.f, gainParam->getRealValue() / 20.f );

	for( int i = 0; i < Audio::getBufferSize(); i++ )
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
	currentVolStore /= (float)Audio::getBufferSize();
	currentVolStore = sqrt( currentVolStore );
	vol.update( currentVolStore );

	bassStore /= bassCount;
	bassStore = sqrt( bassStore );
	bass.update( bassStore );

	medStore /= medCount;
	medStore = sqrt( medStore );
	med.update( medStore );

	highStore /= highCount;
	highStore = sqrt( highStore );
	high.update( highStore );
}

float Audio::getVolume()
{
	return vol.getValue();
}

float Audio::getBass()
{
	return bass.getValue();
}

float Audio::getMed()
{
	return med.getValue();
}

float Audio::getHigh()
{
	return high.getValue();
}

float Audio::getVolumeFromTo( int fromFreq, int toFreq )
{
	return 0.0f;
}

float Audio::toDb( float rms )
{
	float db = 20 * std::log10( rms );
	db       = utils::map( db, -96, 0, 0, 1 );
	if( std::isinf( db ) )
		db = 0;
	return db;
}

void Audio::setSmoothness( float smoothness )
{
	vol.setSmoothness( smoothness );
	bass.setSmoothness( smoothness );
	med.setSmoothness( smoothness );
	high.setSmoothness( smoothness );
}

void Audio::setSampleRate( int _sampleRate )
{
	sampleRate  = _sampleRate;
	freqMax     = (float) sampleRate / 2.f;
	freqBinStep = freqMax / BUFFER_SIZE;
}

void Audio::setGain( ParamRange::Ptr gain )
{
	gainParam = gain;
}

int Audio::getBufferSize()
{
	return BUFFER_SIZE;
}

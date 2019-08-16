#pragma once
#include <vector>
#include "FFGLSmoothValue.h"

namespace ffglqs
{
class Audio
{
public:
	Audio();
	~Audio();

	/// This method is called by the Plugin when a new fft buffer is available
	/// \param fft		A vector of float containing the fft data.
	void Update( std::vector< float > fft );

	/// This method return the global volume represented as a float between 0.0 and 1.0
	/// \return		The global volume amount
	float GetVolume();
	/// This method return the volume of the bass frequencies.
	/// It is represented as a float between 0.0 and 1.0
	/// \return		The volume of the bass frequencies (0-200Hz)
	float GetBass();
	/// This method return the volume of the bass frequencies.
	/// It is represented as a float between 0.0 and 1.0
	/// \return		The volume of the medium frequencies (200-2000Hz)
	float GetMed();
	/// This method return the volume of the bass frequencies.
	/// It is represented as a float between 0.0 and 1.0
	/// \return		The volume of the bass frequency (2000-20_000Hz)
	float GetHigh();
	/// This method return the volume in the frequency range given as argument.
	/// It is represented as a float between 0.0 and 1.0
	/// \return		The volume of the given range
	float GetVolumeFromTo( int fromFreq, int toFreq );

	/// This method convert a value going from 0 to 1 in a linear scale as
	/// a logarithmic representation of it going from -96 to 0db
	/// \return		The volume exprimed in dB
	float ToDB( float value );

	/// This method allows to set the smoothness of the current audio value.
	/// At each update only a certain percentage of the new value is taken into account
	/// This prevent big jump in values that could be ugly if represented visually.
	/// \param smoothness	The new smoothness value, it should be between 0 (not smooth) and 1 (smooth)
	void SetSmoothness( float smoothness );
	/// This method is called by the Plugin when the sample rate as changed in the host
	/// \param sampleRate	The new sampleRate value, usually something like 44100 or 48000
	void SetSampleRate( int sampleRate );

	void SetGain( float gain );

private:
	std::vector< float > fft;
	SmoothValue vol, bass, med, high;
	float freqMax, freqBinStep;
	int sampleRate;
	float gain = 0;
};

}//End namespace ffglqs
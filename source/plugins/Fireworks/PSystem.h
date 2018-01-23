/*
 *  PSystem.h
 *
 *  Created by Daniel Berio on 12/1/10.
 *  http://www.enist.org
 *  Copyright 2010. All rights reserved.
 *
 */

#pragma once

#include <vector>

///////////////////////////////////////////////////////////////////////
/// Manages a pool of particles,
template <class P> class ParticlePool
{
public:
	
	ParticlePool()
	:
	particles(0),
	numParticles(0),
	maxParticles(0)
	{
	}
	
	virtual ~ParticlePool()
	{
	}

	void release();
	void allocate( int maxParticles );
	
	int getNumParticles() const { return numParticles; }
	int getMaxParticles() const { return maxParticles; }
	
	void setNumParticles( int num ) { numParticles = num; }
	
	P*	getParticles() const { return particles; }
		
	inline const P & operator [] ( int index ) const { return particles[index]; }
	inline P & operator [] ( int index )  { return particles[index]; }
	
	/// remove all particles from pool
	void removeAll();
	
	/// removes a particles at index, and swap last particle in
	/// If there are no particles left returns false
	bool removeAndSwap(int index);
    
	/// add a particle, optionally it's possible to pass in a particle with desired data.
	P* add(const P &p = P());

private:
	
	P * particles;
	
	int maxParticles;
	int numParticles;	
	
};


template <class P> class Emitter;



///////////////////////////////////////////////////////////////////////
/// Derive from this class to be able to receive events from an Emitter
template <class P> class ParticleListener 
{
public:
	virtual ~ParticleListener() {}
	
	virtual void onParticleDead( P * particle, Emitter<P> * emitter ) = 0;
	virtual void onParticleAdded( P * particle, Emitter<P> * emitter ) = 0;
	virtual void onEmitterEmpty( Emitter<P> * emitter ) = 0;
	virtual void onParticleEvent( P * particle, Emitter<P> * emitter, int eventId ) = 0;
};





///////////////////////////////////////////////////////////////////////
/// Base particle emitter
template <class P> class Emitter
{
public:
	Emitter()
	{
	}
	
	virtual ~Emitter()
	{
	}
	
	virtual void init( int maxParticles ) { pool.allocate(maxParticles); }
	
	virtual void spawn( float x, float y, float z ) = 0;
	virtual	void integrate( double msecs ) = 0;
		
	void addListener( ParticleListener<P> * listener );	
	void removeListener( ParticleListener<P> * listener );
	
	inline P*	getParticles() const { return pool.getParticles(); }
	inline int getNumParticles() const { return pool.getNumParticles(); }
	
protected: 
	void particleDead( P * particle );
	void particleAdded( P * particle );
	void emitterEmpty();
	void particleEvent( P * particle, int eventId );
	
	ParticlePool<P> pool;
	
private:
	std::vector <ParticleListener<P>*> listeners;
};




////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////



template <class P> void ParticlePool<P>::allocate( int maxParticles_ )
{
	numParticles = 0;
	maxParticles = maxParticles_;
	if( particles )
		delete [] particles;
	particles = new P[maxParticles];
}


template <class P> void ParticlePool<P>::removeAll()
{
	numParticles = 0;
}


template <class P> bool ParticlePool<P>::removeAndSwap( int index )
{
	// Copy the one from the end to here.
	if( index != numParticles-1 )
	{
		particles[index] = particles[numParticles-1]; 
		numParticles--;
		return true;
	}
	else
	{
		numParticles--;
		return false;
	}
}

template <class P> P * ParticlePool<P>::add(const P &p)
{
	if (numParticles >= maxParticles)
		return 0;
	else {
		particles[numParticles] = p;
		P &p = particles[numParticles];

		numParticles++;
		
		return &p;
	}
}




////////////////////////////////////////////////////////////////////////


template <class P> void Emitter<P>::addListener( ParticleListener<P> * listener )
{
	// only add once
	for( int i = 0; i < listeners.size(); i++ )
		if( listeners[i] == listener )
			return;
			
	listeners.push_back(listener);
}

template <class P> void Emitter<P>::removeListener( ParticleListener<P> * listener )
{
	for( int i = 0; i < listeners.size(); i++ )
	{
		if( listeners[i] == listener )
		{
			listeners.erase(listeners.begin()+i);
			return;
		}
	}
}

template <class P> void Emitter<P>::particleDead( P * particle )
{
	for( int i = 0; i < listeners.size(); i++ )
		listeners[i]->onParticleDead( particle, this );
}

template <class P> void Emitter<P>::particleAdded( P * particle )
{
	for( int i = 0; i < listeners.size(); i++ )
		listeners[i]->onParticleAdded( particle, this );
}

template <class P> void Emitter<P>::emitterEmpty()
{
	for( int i = 0; i < listeners.size(); i++ )
		listeners[i]->onEmitterEmpty(  this );
}

template <class P> void Emitter<P>::particleEvent( P * particle, int eventId )
{
	for( int i = 0; i < listeners.size(); i++ )
		listeners[i]->onParticleEvent(  particle, this, eventId );
}

	

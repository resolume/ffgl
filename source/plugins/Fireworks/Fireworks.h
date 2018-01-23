/*
 *  Fireworks.h
 *
 *  Created by Daniel Berio on 12/1/10.
 *  http://www.enist.org
 *  Copyright 2010. All rights reserved.
 *
 */

#pragma once

#include "PSystem.h"
#include "Vec3.h"


///////////////////////////////////////////////////////////////////////

struct Particle
{	
	///////////////////////////
	// this data is also for GL
	Vec3 pos;	
	unsigned int color;
	
	///////////////////////////
	
	Vec3 vel;
	Vec3 accel;	
	Vec3 oldpos;

	double lifetime;
	double t;
	
	float hue;
	
	bool exploding;
	bool hasTrail;
};


///////////////////////////////////////////////////////////////////////

class Firework : public Emitter<Particle>
{
	public:
		Firework();
		~Firework();
		
		void init( int numParticles );
		void spawn( float x, float y , float z );
		
		void integrate( double msecs );
		inline void integrate( Particle & p, double dt, double damp );
		
		void addTrail( Particle & p, double dt );
		
		void explode( Particle & p, int numExplosionParticles, float power );
		
		Particle * addParticle( const Vec3 & pos, 
								const Vec3 & vel,
								const Vec3 & accel,
								float lifetime );
						  
		Particle * getTrailParticles() const { return trailPool.getParticles(); }
		int getNumTrailParticles() const { return trailPool.getNumParticles(); }
		
		float spawnRadius;
		float damping;
		float explosionPower;
		float explosionRandomness;
		float explosionDuration;
		
		static Vec3 gravity;
		static bool blink;
		
		bool exploding;
		bool explosionTrails;
		int explosionType;
		
		ParticlePool <Particle> trailPool;
		
};


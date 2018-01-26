#include "Plugin.h"
#include <FFGL.h>
#include <FFGLLib.h>
#include "../../lib/ffgl/utilities/utilities.h"
#include "particleImg.h"

#ifdef _WIN32
#include <gl/GLU.h>
#pragma comment(lib, "glu32.lib")
#else
#ifdef TARGET_OS_MAC
#include <OpenGL/glu.h>
#endif
#endif



#define spawnRadiusParam (0)
#define explosionPowerParam (1)
#define explosionRandomnessParam (2)
#define explosionDurationParam (3)
#define pointSizeParam (4)
#define autoShootParam (5)
#define spawnTriggerParam (6)
#define rotationXParam (7)
#define rotationYParam (8)
#define doFlashesParam (9)
#define blinkParam (10)

// FFGL Plugin Info Structure

// Insert the info that will appear on the host here
/*!
 !!! Do not change fields marked with *
 */

static CFFGLPluginInfo PluginInfo (
                                   Plugin::CreateInstance,                  // * Create method
                                   "OFPX",                                  // Plugin unique ID
                                   "FireWorks",                             // Plugin name
                                   1,                                       // * API major version number
                                   500,                                     // * API minor version number
                                   1,                                       // Plugin major version number
                                   000,                                     // Plugin minor version number
                                   FF_SOURCE,                               // Plugin type FF_SOURCE or FF_EFFECT ( filters )
                                   "Firework Particles Plugin",             // Plugin description
                                   "by Daniel Berio for Resolume"           // About
                                   ,0
                                   ,NULL
                                   );


Plugin::Plugin() : CFreeFrameGLPlugin()
{
    // Input properties
    SetMinInputs(0);
    SetMaxInputs(0);
    
	// Here we add the parameters for the plugin 
	spawnRadius = 8.0;
	//addFloatParameter("spawn radius",&spawnRadius,2,25.0);
    SetParamInfo( spawnRadiusParam, "Spawn Radius", FF_TYPE_STANDARD, 0.347826087f ); // 8, 2, 25
	
	damping = 4.0;

	explosionPower = 36;
	//addFloatParameter("power",&explosionPower,10,70.0);
    SetParamInfo( explosionPowerParam, "Power", FF_TYPE_STANDARD, 0.6f ); //36, 10, 70

	explosionRandomness = 0.1;
	//addFloatParameter("randomness",&explosionRandomness,0,1);
    SetParamInfo( explosionRandomnessParam, "Randomness", FF_TYPE_STANDARD, 0.1f );

	explosionDuration = 3.1;
	//addFloatParameter("duration",&explosionDuration,1.0,5.0);
    SetParamInfo( explosionDurationParam, "Duration", FF_TYPE_STANDARD, 0.775f ); //3.1, 1, 5

	pointSize = 35;
	//addFloatParameter("particle size",&pointSize,5,130);
    SetParamInfo( pointSizeParam, "Particle Size", FF_TYPE_STANDARD, 0.28f); //35, 5, 130
	
	
	autoShoot = true;
	//addBoolParameter("auto shoot",&autoShoot);
    SetParamInfo( autoShootParam, "Auto Shoot", FF_TYPE_BOOLEAN, true);
	
	//addEventParameter("shoot",&spawnTrigger.val);
    SetParamInfo( spawnTriggerParam, "Shoot", FF_TYPE_EVENT, false );

	camDistance = -65;

    rotx = 0;
	//addFloatParameter("rotation",&rotx,-90,90);
    SetParamInfo( rotationXParam, "Rotation", FF_TYPE_STANDARD, 0.5f); //0, -90, 90
	
	rotySpeed = 0;
	//addFloatParameter("orbit speed",&rotySpeed,-180,180);
    SetParamInfo( rotationYParam, "Orbit Speed", FF_TYPE_STANDARD, 0.5f); //0, -180, 180
	
	doFlashes = false;
	//addBoolParameter("sky flash",&doFlashes);
    SetParamInfo( doFlashesParam, "Sky Flash", FF_TYPE_BOOLEAN, false );
    
	//addBoolParameter("blink",&Firework::blink);
    SetParamInfo (blinkParam, "Blink", FF_TYPE_BOOLEAN, true);
	
	curFirework = 0;
	skyLight = 0;
}

Plugin::~Plugin()
{
    
}


FFResult Plugin::SetFloatParameter(unsigned int dwIndex, float value)
{
    switch (dwIndex)
    {
        case spawnRadiusParam: // 8, 2, 25
            spawnRadius = value * 23.0f + 2.0f;
            break;
        case explosionPowerParam: //36, 10, 70
            explosionPower = value * 60.0f + 10.0f;
            break;
        case explosionRandomnessParam:
            explosionRandomness = value;
            break;
        case explosionDurationParam: //3.1, 1, 5
            explosionDuration = value * 4.0f + 1.0f;
            break;
        case pointSizeParam: //35, 5, 130
            pointSize = value * 125.0f + 5.0f;
            break;
        case autoShootParam:
            autoShoot = value > 0.5;
            break;
        case spawnTriggerParam:
            spawnTrigger = value > 0.5;
            break;
        case rotationXParam: //0, -90, 90
            rotx = (value - 0.5f) * 180.0f;
            break;
        case rotationYParam: //0, -180, 180
            rotySpeed = (value - 0.5f) * 360.0f;
            break;
        case doFlashesParam:
            doFlashes = value > 0.5;
            break;
        case blinkParam:
            Firework::blink = value > 0.5;
            break;
    }
    return FF_SUCCESS;
}

float Plugin::GetFloatParameter(unsigned int index)
{
    switch (index)
    {
        case spawnRadiusParam: // 8, 2, 25
            return (spawnRadius - 2.0f) / 23.0f;
            break;
        case explosionPowerParam: //36, 10, 70
            return (explosionPower - 10.0f) / 60.0f;
            break;
        case explosionRandomnessParam:
            return explosionRandomness;
            break;
        case explosionDurationParam: //3.1, 1, 5
            return (explosionDuration - 1.0f) / 4.0f;
            break;
        case pointSizeParam: //35, 5, 130
            return (pointSize - 5.0f) / 125.0f;
            break;
        case rotationXParam: //0, -90, 90
            return (rotx / 180.0f) + 0.5f;
            break;
        case rotationYParam: //0, -180, 180
            return (rotySpeed / 360.0f) + 0.5f;
            break;
    }
    
    return 0.0f;
}

char* Plugin::GetParameterDisplay( unsigned int index )
{
    unsigned int pType = m_pPlugin->GetParamType( index );
    
    if ( pType != FF_FAIL )
    {
        float fValue = m_pPlugin->GetFloatParameter( index );
        memset( s_Displayvalue, 0, 15 );
        
        switch ( index )
        {
            case rotationXParam:
                sprintf( s_Displayvalue, "%i Degrees", int( rotx ) );
                break;
            default:
                sprintf( s_Displayvalue, "%f", fValue );
                break;
        }
        return s_Displayvalue;
    }
    
    return NULL;
}


//--------------------------------------------------------------
FFResult Plugin::InitGL(const FFGLViewportStruct *vp)
{
    
    width = vp->width;
    height = vp->height;
    
    curFirework = 0;
    
    for( int i = 0; i < NFIREWORKS; i++ )
    {
        f[i].init(5000);
        f[i].addListener(this);
    }
    
    glGenTextures( 1, &ptexHandle );
    glBindTexture( GL_TEXTURE_2D, ptexHandle );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, particleImg );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
    glBindTexture( GL_TEXTURE_2D, 0 );
    
    return FF_SUCCESS;
}

FFResult Plugin::DeInitGL()
{
    glDeleteTextures( 1, &ptexHandle );
    return FF_SUCCESS;
}

FFResult Plugin::ProcessOpenGL(ProcessOpenGLStruct* pGL)
{
    double ticks = getTicks() / 1000.0;
    double lastFrameTime = ticks - pTicks;
    
    if( spawnTrigger )
    {
        // shoot! //best comment ever
        shoot();
    }
    
    static double time = 0.0;
    static double interval = random(1.0,3.0);
    
    if( autoShoot )
    {
        if( time > interval )
        {
            time = 0;
            interval = random(1.0,3.0);
            
            int n = 1 + (rand()%2);
            
            for( int i = 0; i < n; i++ )
                shoot();
        }
        
        time += lastFrameTime;
    }
    
    double msecs = lastFrameTime * 1000;
    for( int i = 0; i < NFIREWORKS; i++ )
    {
        f[i].spawnRadius = spawnRadius;
        f[i].damping = damping;
        f[i].explosionPower = explosionPower;
        f[i].explosionRandomness = explosionRandomness;
        f[i].explosionDuration = explosionDuration;
        f[i].integrate(msecs);
    }
    
    skyLight*=(1.0 - lastFrameTime * 10);
    
    if(doFlashes)
    {
        float l=skyLight;
        glClearColor(l,l,l,l);
    }
    else
    {
        glClearColor(0,0,0,0);
    }
    
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // additive blending
    glEnable( GL_BLEND );
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    
    static float roty = 0;
    roty+=rotySpeed*lastFrameTime;
    while(roty>360)
        roty-=360;
    while(roty<0)
        roty+=360;
    
    // setup perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) width/ (float) height, 0.1, 500.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,camDistance);
    glRotatef(rotx,1,0,0);
    glRotatef(roty,0,1,0);
    
    glColor4f(1,1,1,1);
    
    // enable point sprite ext
    glEnable(GL_POINT_SPRITE_ARB);
    
    // this causes the points to scale with distance
    float quadratic[] =  { 1.0f, 0.0f, 0.01f };
    glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
    
    FFGLTextureStruct ptex = {64, 64, 64, 64, ptexHandle};
    glBindTexture(GL_TEXTURE_2D, ptex.Handle);
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
    drawParticles();
    glBindTexture(GL_TEXTURE_2D,0);
    glDisable(GL_POINT_SPRITE_ARB);
    
    pTicks = getTicks() / 1000.0;
    
    //rest the matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //disable blending
    glDisable( GL_BLEND );
    
    return FF_SUCCESS;
}







//--------------------------------------------------------------


void Plugin::drawFirework( Firework & fw, float ptSize )
{
	glPointSize(ptSize);
	
	// draw trails
	Particle* p = fw.getTrailParticles();

	glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(Particle),&p->color );
	glVertexPointer(	3,
						GL_FLOAT,
						sizeof(Particle),
						&p->pos );
	glDrawArrays(GL_POINTS, 0, fw.getNumTrailParticles()); 
	
	// draw particles
	p = fw.getParticles(); 
	glColorPointer(4,GL_UNSIGNED_BYTE,sizeof(Particle),&p->color );
	glVertexPointer(	3,
						GL_FLOAT,
						sizeof(Particle),
						&p->pos );
	glDrawArrays(GL_POINTS, 0, fw.getNumParticles()); 
	
}


//--------------------------------------------------------------

void  Plugin::drawParticles()
{	
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY); 
	
	for( int i = 0; i < NFIREWORKS; i++ )
	{
		drawFirework(f[i], (f[i].exploding && !f[i].explosionTrails) ? pointSize*2 : pointSize);
	}
	
	glDisableClientState( GL_COLOR_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
}

//--------------------------------------------------------------

void Plugin::shoot()
{
	f[curFirework].spawn(random(-3,3), random(-20,-2), random(-3,3));
	curFirework = (curFirework+1)%NFIREWORKS;
}

//--------------------------------------------------------------

void Plugin::onParticleDead( Particle * particle, Emitter<Particle> * emitter )
{
	Firework * fw = (Firework*)emitter;
	if(particle->exploding && particle->hasTrail)
	{
		// sub explosion
		fw->explosionTrails = false;
		fw->explode(*particle,70,explosionPower*0.5);
	}
}

void Plugin::onParticleAdded( Particle * particle, Emitter<Particle> * emitter )
{

}

void Plugin::onEmitterEmpty( Emitter<Particle> * emitter )
{

}

void Plugin::onParticleEvent( Particle * particle, Emitter<Particle> * emitter, int eventId )
{
	switch(eventId)
	{
		case 0: // main explosion
			skyLight += 0.3;
			break;
			
		case 1: // sub explosion
			//skyLight += 0.001;
			break;
			
		default:	
			break;
	}
}
	


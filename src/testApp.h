#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"
#include "ofxVectorMath.h"
#include "Boids.h"
#include "ofxSimpleGuiToo.h"
#include "ofxFBOTexture.h"
#include "ofxOscReceiver.h"
#include "ofxTrigger.h"
#include "AudioFileSpectrum.h"
#include "AudioLiveSpectrum.h"

#include "Settings.h"
#include "Settings0.h"
#include "Settings1.h"

#define OSC_MESSAGES_TOTAL  10

class Triangle
{
public:
	float	 a;		// angle
	float	 as;	// angular speed.
	float	 r;		// radius.
	float	 rz;	// rotation z-axis.
	float	 rzs;	// rotation z-axis speed.
	float	 rx;	// rotation x-axis.
	float	 rxs;	// rotation x-axis speed.
	ofxVec3f cp;	// center point
	ofxVec3f p1;	// triangle points.
	ofxVec3f p2;	// triangle points.
	ofxVec3f p3;	// triangle points.
	ofxVec3f n1;	// triangle points.
	ofxVec3f n2;	// triangle points.
	ofxVec3f n3;	// triangle points.
	int      c;		// color;
    ofxVec3f v1;
    ofxVec3f v2;
    ofxVec3f v3;
    float    c1;
    float    c2;
    float    c3;
    bool     facingCamera;
    float    audio;
};

class PointLight
{
public:
    GLenum lightNum;
    float materialFront  [ 4 ];
    float materialBack   [ 4 ];
    float diffuseLight   [ 4 ];
    float positionLight  [ 4 ];
    float directionLight [ 4 ];
    float spotCutoff;
    float kc, kl, kq;
    ofxVec3f pos;
    ofxVec3f rot;
    ofxVec3f center;
    float radius;
    float rotInc;
    
    float reach;
    float strength;
};

class Ray
{
public:
	ofxVec3f p;
    ofxVec3f r;
    float rotInc;
    float scale;
};


class testApp : public ofBaseApp
{

public:
	
	void setup	();
	void update	();
	void draw	();
    
    void parseOsc       ( string msg );
    
    void drawTriangles  ();
    void drawRays       ( const ofPoint& p );
    void drawRayEdges   ();
    void drawBoids      ();
    void spotlight      ();
    void pointlight     ( PointLight& light );
    void drawPointLight ( PointLight& light );
    
	void keyPressed		( int key );
	void keyReleased	( int key );
	void mouseMoved		( int x, int y );
	void mouseDragged	( int x, int y, int button );
	void mousePressed	( int x, int y, int button );
	void mouseReleased	( int x, int y, int button );
	void windowResized	( int w, int h );
    
    vector<Triangle*>	triangles;
    vector<PointLight>  lights;
    vector<Ray>         rays;
    
    Boids               boids;
    
    float*              triangleAudio;
    
    ofxSimpleGuiToo     gui;
    
    ofxFBOTexture       fbo;
    float               fboScale;
    ofRectangle         fboRect;
    
    ofxVec3f            center;
    ofxVec3f            camera;
    float               radius;
    float               rotationY;
    
    ofImage             rayImage;
    ofImage             rayStripImage;
    ofImage             boidImage;
    
    bool                bDebug;
    bool                bPause;
    
    //---
    
    ofxTrigger          trigger;
    ofxOscReceiver      oscReceiver;
    string              oscMessage;
    int                 oscMessageIndex;
    string              oscMessages[ OSC_MESSAGES_TOTAL ];
    float               oscMessageTimers[ OSC_MESSAGES_TOTAL ];

    bool                bUseRecordedSound;
    int                 timeNudge;
    
    ofSoundPlayer       sound;
    AudioFileSpectrum   fftFromFile;
    int                 soundTimeMillis;
    int                 soundTimeNudgeMillis;
    float               soundPosition;
    float               soundPositionGui;
    float               soundThreshold;
    float               soundPeakDecay;
    float               soundMaxDecay;
    float               soundScale;
    
    vector<Settings>    settings;
    int                 settingsIndex;
    int                 settingsIndexGui;
    Settings*           currentSettings;
};

#endif

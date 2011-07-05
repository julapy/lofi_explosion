#include "testApp.h"

///////////////////////////////////////////
//	SETUP.
///////////////////////////////////////////

void testApp::setup()
{
    ofSetFrameRate( 30 );
    ofSetVerticalSync( true );
    ofBackground( 0, 0, 0 );

    screenRect.width  = 1024;
    screenRect.height = 768;
    
    radius = 200;
    rotationY = 0;
    center.x = screenRect.width * 0.5;
    center.y = screenRect.height * 0.5;
    
    rayImage.loadImage( "ray.png" );
    rayStripImage.loadImage( "ray_strip.png" );
    boidImage.loadImage( "boid_128x128.png" );
    
    boids.tex = &boidImage.getTextureReference();
    boids.setup();
    
    fboScale        = 1;
    fboRect.width   = screenRect.width  * fboScale;
    fboRect.height  = screenRect.height * fboScale;
    fbo.allocate( fboRect.width, fboRect.height, GL_RGB, 1 );
    
    bDebug      = true;
    bPause      = false;
    bFullScreen = false;
    
    int t = 300;
	for( int i=0; i<t; i++ )
	{
		Triangle* t;
		t = new Triangle();
		
        ofxVec3f cp;
        cp.x = ofRandom( -0.4, 1.0 );
        cp.y = ofRandom( -0.4, 1.0 );
        cp.z = ofRandom( -0.4, 1.0 );
        cp.normalize();
        cp *= radius;
        
        t->cp.set( cp.x, cp.y, cp.z );
        
        //--
        
		float r1 = 40;
		float r2 = 200;
		
		t->r = ofRandom( r1, r2 );
		
		float s;
		s = ( t->r - r1 ) / ( r2 - r1 );
		s = ofClamp( s, 0.2, 1.0 );
        
		float a;
		float r;
		float l1 = 40;
		float l2 = 100;
        
        ofxVec3f p;
        ofxVec3f n;
        
        p = cp.getPerpendicular( ofxVec3f( 1, 0, 0 ) );
        p.rotateRad( ofRandom( 0, TWO_PI * 0.33 ), cp );
        r = ofRandom( l1, l2 ) * ( 1 - s );
		t->p1.x	= cp.x + p.x * r;
		t->p1.y	= cp.y + p.y * r;
        t->p1.z	= cp.z + p.z * r;
        n.set( t->p1.x, t->p1.y, t->p1.z );
        n.normalize();
        n *= -1;
        t->n1.set( n.x, n.y, n.z );
        
        p = cp.getPerpendicular( ofxVec3f( 1, 0, 0 ) );
        p.rotateRad( ofRandom( TWO_PI * 0.33, TWO_PI * 0.66 ), cp );
        r = ofRandom( l1, l2 ) * ( 1 - s );
		t->p2.x	= cp.x + p.x * r;
		t->p2.y	= cp.y + p.y * r;
        t->p2.z	= cp.z + p.z * r;
        n.set( t->p2.x, t->p2.y, t->p2.z );
        n.normalize();
        n *= -1;        
        t->n2.set( n.x, n.y, n.z );

        p = cp.getPerpendicular( ofxVec3f( 1, 0, 0 ) );
        p.rotateRad( ofRandom( TWO_PI * 0.66, TWO_PI ), cp );
        r = ofRandom( l1, l2 ) * ( 1 - s );
		t->p3.x	= cp.x + p.x * r;
		t->p3.y	= cp.y + p.y * r;
        t->p3.z	= cp.z + p.z * r;
        n.set( t->p3.x, t->p3.y, t->p3.z );
        n.normalize();
        n *= -1;        
        t->n3.set( n.x, n.y, n.z );
        
        t->audio = 0;
		
		triangles.push_back( t );
	}
    
    triangleAudio = new float[ t ];
    
    GLenum lightNums[] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };
    for( int i=0; i<16; i++ )
    {
        PointLight light;
        light.lightNum = lightNums[ i ];
        
        light.materialFront[ 0 ] = 0;
        light.materialFront[ 1 ] = 0;
        light.materialFront[ 2 ] = 0;
        light.materialFront[ 3 ] = 1.0;
        
        light.materialBack[ 0 ] = 0;
        light.materialBack[ 1 ] = 0;
        light.materialBack[ 2 ] = 0;
        light.materialBack[ 3 ] = 1.0;
        
        light.diffuseLight[ 0 ] = 1.0;
        light.diffuseLight[ 1 ] = 1.0;
        light.diffuseLight[ 2 ] = 1.0;
        light.diffuseLight[ 3 ] = 1.0;
        
        light.positionLight[ 0 ] = 0;
        light.positionLight[ 1 ] = 0;
        light.positionLight[ 2 ] = 0;
        light.positionLight[ 3 ] = 1.0;
        
        light.directionLight[ 0 ] = 0;
        light.directionLight[ 1 ] = 0;
        light.directionLight[ 2 ] = 0;
        light.directionLight[ 3 ] = 1.0;
        
        light.spotCutoff = 180;
        
        light.kc = 0;
        light.kl = 0;
        light.kq = 0.0009;
        
        light.center = center;
        light.radius = radius * 0.9;
        
        light.pos.x = ofRandom( -1, 1 );
        light.pos.y = ofRandom( -1, 1 );
        light.pos.z = ofRandom( -1, 1 );
        light.pos.normalize();
        light.pos *= light.radius;
        
        light.rot.x = ofRandom( -1, 1 );    // rotation axis.
        light.rot.y = ofRandom( -1, 1 );
        light.rot.z = ofRandom( -1, 1 );
        light.rot.normalize();
        light.rotInc = ofRandom( 0.5, 3.0 );
        
        light.reach     = radius * 0.5;
        light.strength  = 1.0;
        light.scale     = 1.0;
        light.audio     = 1.0;
        
        lights.push_back( light );
    }
    
    for( int i=0; i<40; i++ )
    {
        rays.push_back( Ray() );
        Ray& ray = rays.back();
        
        ray.p.x = ofRandom( -1, 1 );
        ray.p.y = ofRandom( -1, 1 );
        ray.p.z = ofRandom( -1, 1 );
        ray.p.normalize();
        ray.p *= radius;
        ray.r.x = ofRandom( -1, 1 );
        ray.r.y = ofRandom( -1, 1 );
        ray.r.z = ofRandom( -1, 1 );
        ray.r.normalize();
        ray.rotInc  = ofRandom( 4, 8 );
        ray.scale   = ofRandom( 0.5, 1.0 );
    }
    
    //---

    soundPosition = 0;
    soundPositionGui = 0;
    soundTimeMillis = 0;
    soundTimeNudgeMillis = 500;
    soundThreshold = 0.2;
    soundPeakDecay = 0.85;
    soundMaxDecay = 0.99;
    soundScale = 1.2;
    
    //---
    
    settingsIndexGui = settingsIndex;
    settings.push_back( Settings0() );
    settings.push_back( Settings1() );
    settings.push_back( Settings2() );
    settings.push_back( Settings3() );
    settings.push_back( Settings4() );
    settings.push_back( Settings5() );
    settings.push_back( Settings6() );
    settingsIndex = settings.size() - 1;
   
    currentSettings = &settings[ settingsIndex ];
    
    //---

    gui.setPage( "boids" );
    gui.addSlider( "boids.seperationRange", boids.seperationRange, 0, 100 );
    gui.addSlider( "boids.seperationLimit", boids.seperationLimit, 0, 5 );
    gui.addSlider( "boids.seperationScale", boids.seperationScale, 0, 5 );
    
    gui.addSlider( "boids.alignmentRange", boids.alignmentRange, 0, 100 );
    gui.addSlider( "boids.alignmentLimit", boids.alignmentLimit, 0, 5 );
    gui.addSlider( "boids.alignmentScale", boids.alignmentScale, 0, 5 );

    gui.addSlider( "boids.cohesionRange", boids.cohesionRange, 0, 100 );
    gui.addSlider( "boids.cohesionLimit", boids.cohesionLimit, 0, 5 );
    gui.addSlider( "boids.cohesionScale", boids.cohesionScale, 0, 5 );
    
    gui.addSlider( "boids.boundsRange", boids.boundsRange, 0, 100 );
    gui.addSlider( "boids.boundsScale", boids.boundsScale, 0, 100 );
    
    gui.addSlider( "boids.maxSpeed", boids.maxSpeed, 0, 10 );
    
    gui.addPage( "sound" );
    gui.addSlider( "soundPosition", soundPositionGui, 0, 1.0 );
    gui.addSlider( "soundThreshold", soundThreshold, 0, 1.0 );
    gui.addSlider( "soundPeakDecay", soundPeakDecay, 0.5, 1.0 );
    gui.addSlider( "soundMaxDecay", soundMaxDecay, 0.9, 1.0 );
    gui.addSlider( "soundScale", soundScale, 0.1, 5.0 );
    
    gui.addPage( "settings" );
    gui.addSlider( "settingsIndex", settingsIndexGui, 0, settings.size() - 1 );
    for( int i=0; i<settings.size(); i++ )
    {
        string settingName;
        settingName = "settings";
        settingName += ofToString( i );
        settingName += ".position";
        
        gui.addSlider( settingName, settings[ i ].position, 0, 1 );
    }
    
    gui.show();
    gui.page( 3 );
    
    //---
    
    oscMessageIndex = 0;
    
    if( ( bUseRecordedSound = false ) )
    {
        int soundTimeMin = 16;
        int soundTimeSec = 0;
        soundTimeMillis = ( soundTimeMin * 60 + soundTimeSec ) * 1000;
        
        soundPosition = 0.0;
        soundPositionGui = soundPosition;
        
        sound.loadSound( "lofi_pretty_ugly.mp3" );
        sound.play();
        sound.setPosition( soundPosition );
        
        fft = new AudioFileSpectrum();
        ( (AudioFileSpectrum*)fft )->init( &sound );
        fft->setMirrorData( true );
        
        trigger.load( "triggers_pt1.xml" );
        trigger.setStartTimeMillis( -soundTimeNudgeMillis );
        trigger.jumpToTimeMillis( soundTimeMillis * soundPosition );
    }
    else
    {
        oscReceiver.setup( 12345 );
        
        fft = new AudioLiveSpectrum();
        ( (AudioLiveSpectrum*)fft )->init();
        fft->setMirrorData( true );
    }
}

///////////////////////////////////////////
//	UPDATE.
///////////////////////////////////////////

void testApp::update()
{
    if( bPause )
        return;
    
    float position = sound.getPosition();

    bool bUpdateToSound = false;
    if( settingsIndex == 0 )
        ( (Settings0*)currentSettings )->update( bUpdateToSound ? position : -1 );
    else if( settingsIndex == 1 )
        ( (Settings1*)currentSettings )->update( bUpdateToSound ? position : -1 );
    else if( settingsIndex == 2 )
        ( (Settings2*)currentSettings )->update( bUpdateToSound ? position : -1 );
    else if( settingsIndex == 3 )
        ( (Settings3*)currentSettings )->update( bUpdateToSound ? position : -1 );
    else if( settingsIndex == 4 )
        ( (Settings4*)currentSettings )->update( bUpdateToSound ? position : -1 );
    else if( settingsIndex == 5 )
        ( (Settings5*)currentSettings )->update( bUpdateToSound ? position : -1 );
    else if( settingsIndex == 6 )
        ( (Settings6*)currentSettings )->update( bUpdateToSound ? position : -1 );
    
    //--- sound.
    
    if( soundPosition != soundPositionGui )
    {
        soundPosition = soundPositionGui;
        
        sound.setPosition( soundPosition );
        
        trigger.jumpToTimeMillis( soundTimeMillis * soundPosition );
    }
    
    if( settingsIndex != settingsIndexGui )
    {
//        settings[ settingsIndex ].copySettings( settings[ settingsIndexGui ] );     // copy over settings.
        settingsIndex = settingsIndexGui;
        
        currentSettings = &settings[ settingsIndex ];
        if( settingsIndex == 0 )
            ( (Settings0*)currentSettings )->reset();
        else if( settingsIndex == 1 )
            ( (Settings1*)currentSettings )->reset();
        else if( settingsIndex == 2 )
            ( (Settings2*)currentSettings )->reset();
        else if( settingsIndex == 3 )
            ( (Settings3*)currentSettings )->reset();
        else if( settingsIndex == 4 )
            ( (Settings4*)currentSettings )->reset();
        else if( settingsIndex == 5 )
            ( (Settings5*)currentSettings )->reset();
        else if( settingsIndex == 6 )
            ( (Settings6*)currentSettings )->reset();
    }
    
    //---
    
    if( bUseRecordedSound )
    {
        vector<ofxTriggerItem> triggers = trigger.triggersAtTimeInMillis( soundTimeMillis * position );
        
        for( int i=0; i<triggers.size(); i++ )
        {
            oscMessage = triggers[ i ].valueString();
            
            oscMessages[ oscMessageIndex ] = oscMessage;
            oscMessageTimers[ oscMessageIndex ] = ofGetElapsedTimef() + 5.0f;
            oscMessageIndex = ( oscMessageIndex + 1 ) % OSC_MESSAGES_TOTAL;
            oscMessages[ oscMessageIndex ] = "";
            
            parseOsc( oscMessage );
        }
    }
    else
    {
        while( oscReceiver.hasWaitingMessages() )
        {
            ofxOscMessage m;
            oscReceiver.getNextMessage( &m );
            
            oscMessage = m.getAddress();
            oscMessage += ": ";
            for( int i=0; i<m.getNumArgs(); i++ )
            {
                oscMessage += m.getArgTypeName( i );
                oscMessage += ":";
                if( m.getArgType( i ) == OFXOSC_TYPE_INT32 )
                    oscMessage += ofToString( m.getArgAsInt32( i ) );
                else if( m.getArgType( i ) == OFXOSC_TYPE_FLOAT )
                    oscMessage += ofToString( m.getArgAsFloat( i ) );
                else if( m.getArgType( i ) == OFXOSC_TYPE_STRING )
                    oscMessage += m.getArgAsString( i );
                else
                    oscMessage += "unknown";
            }
            
            trigger.addTrigger( "osc", oscMessage );
            
            oscMessages[ oscMessageIndex ] = oscMessage;
            oscMessageTimers[ oscMessageIndex ] = ofGetElapsedTimef() + 5.0f;
            oscMessageIndex = ( oscMessageIndex + 1 ) % OSC_MESSAGES_TOTAL;
            oscMessages[ oscMessageIndex ] = "";
            
            parseOsc( oscMessage );
        }
    }
    
    fft->setThreshold( soundThreshold );
	fft->setPeakDecay( soundPeakDecay );
    fft->setMaxDecay( soundMaxDecay );
    fft->update();
//    fft->getFftData( triangleAudio, triangles.size() );
    fft->getFftPeakData( triangleAudio, triangles.size() );
    
    //---
    
    currentSettings->rotationAudio = fft->getAveragePeak();
    currentSettings->audioAlpha = fft->getAveragePeak();
        
    //---
    
    int boidsTotal = boids.boids.size();
    float* boidsAudio;
    boidsAudio = new float[ boidsTotal ];
    fft->getFftPeakData( boidsAudio, boidsTotal );
    for( int i=0; i<boidsTotal; i++ )
        boids.boids[ i ]->audio = boidsAudio[ i ];
    delete[] boidsAudio;
    
    boids.audioWeight = currentSettings->boidAudioWeight;
    boids.alpha = currentSettings->boidAlpha;
    
    boids.update();
    
    //--- lights.
    
    float* lightsAudio;
    lightsAudio = new float[ lights.size() ];
    fft->getFftPeakData( lightsAudio, lights.size() );
    
    for( int i=0; i<lights.size(); i++ )
    {
        PointLight& light = lights[ i ];
        
        light.pos.rotate( light.rotInc, light.rot );
        
        light.positionLight[ 0 ] = light.pos.x;
        light.positionLight[ 1 ] = light.pos.y; 
        light.positionLight[ 2 ] = light.pos.z;
        
        ofxVec3f lightDir;
        lightDir = light.pos.getNormalized();
        
        light.directionLight[ 0 ] = lightDir.x;
        light.directionLight[ 1 ] = lightDir.y;
        light.directionLight[ 2 ] = lightDir.z;
        
        light.audio = lightsAudio[ i ];
        light.scale = ofClamp( light.audio, 0.5, 1.0 );
    }
    
    delete[] lightsAudio;
    
    for( int i=0; i<rays.size(); i++ )
    {
        Ray& ray = rays[ i ];
        ray.p.rotate( ray.rotInc, ray.r );
    }

    //---
    
    for( int i=0; i<triangles.size(); i++ )
    {
        Triangle& shape = *triangles[ i ];
        
        shape.audio = triangleAudio[ i ];
        
        shape.p1 = shape.p1.getNormalized() * ( radius * currentSettings->triangleRadiusToCenter );
        shape.p2 = shape.p2.getNormalized() * ( radius * currentSettings->triangleRadiusToCenter );
        shape.p3 = shape.p3.getNormalized() * ( radius * currentSettings->triangleRadiusToCenter );
        
        // back face culling.
        // working out which triangle faces the camera.
        // if facing away from camera, then make it black.
        // based on this -> http://wiki.processing.org/w/Backface_culling 
        
        ofxVec3f cameraToFace
        (
            camera.x - shape.p1.x,
            camera.y - shape.p1.y,
            camera.z - shape.p1.z
        );
        
        // we need to find out what direction the face is pointing, 
        // known as the normal. to do this we need to do a cross 
        // product of two of the sides of the face.
        // side 1
        
        ofxVec3f ab
        (
            shape.p2.x - shape.p1.x,
            shape.p2.y - shape.p1.y,
            shape.p2.z - shape.p1.z
        );

        ofxVec3f cb
        (
            shape.p2.x - shape.p3.x,
            shape.p2.y - shape.p3.y,
            shape.p2.z - shape.p3.z
        );
        
        ofxVec3f faceNormal = ab.cross( cb );

        // We now know the vector from the camera to the face, 
        // and the vector that describes which direction the face 
        // is pointing, so we just need to do a dot-product and 
        // based on that we can tell if it's facing the camera or not
        
        float result = cameraToFace.dot( faceNormal );
            
        // if the result is positive, then it is facing the camera.
        
        shape.facingCamera = result > 0;
        
        //--- color.
        
        vector<ofxVec3f*> poly;
        poly.push_back( &( shape.p1 ) );
        poly.push_back( &( shape.p2 ) );
        poly.push_back( &( shape.p3 ) );
        
        vector<ofxVec3f*> edgeRays;
        edgeRays.push_back( &( shape.v1 ) );
        edgeRays.push_back( &( shape.v2 ) );
        edgeRays.push_back( &( shape.v3 ) );
        
        vector<float*> colors;
        colors.push_back( &( shape.c1 ) );
        colors.push_back( &( shape.c2 ) );
        colors.push_back( &( shape.c3 ) );
        
        for( int j=0; j<poly.size(); j++ )
        {
            ofxVec3f& point = *( poly[ j ] );
            ofxVec3f& ray   = *( edgeRays[ j ] );
            ray.set( 0, 0, 0 );
            
            float rot = currentSettings->rotation;
            point.rotate( rot, ofxVec3f( 0, 1, 0 ) );
            
            float& color = *( colors[ j ] );
            color = 0;
            
            for( int k=0; k<lights.size(); k++ )
            {
                PointLight& light = lights[ k ];
                
                float d = light.pos.distance( point );
                
                float re = light.reach;
                re *= light.scale;
                
                if( d <= re )
                {
                    float p = 1 - d / re;
                    float s = cos( p * PI + PI ) * 0.5 + 0.5;
                    s *= s;
                    
                    ofxVec3f v = point - light.pos;
                    v.normalize();
                    v *= s * light.strength;
                    v *= 150;
                    
                    ray += v;
                    ray *= currentSettings->rayScale;
                    
                    color += p;
                }
            }
            
            color = MIN( color, 1.0 );
            color = MAX( color, 0.0 );
        }
    }
}

void testApp :: parseOsc ( string msg )
{
    vector<string> msgSplit;
    msgSplit = ofSplitString( msg, " " );
    
    string address  = msgSplit[ 0 ];
    string value    = msgSplit[ 1 ];
    
    if( address == "/midi/note/10/2:" )
    {
        if( value == "int32:1" )
        {
            currentSettings->triangleRadiusToCenterTarget = 0.2;
        }
    }
    else if( address == "/midi/note/2/2:" )
    {
        if( value == "int32:1" )
        {
            currentSettings->rotationGlitchTarget = -1;
        }
        else if( value == "int32:0" )
        {
            currentSettings->rotationGlitchTarget = 1;
        }
    }
}

///////////////////////////////////////////
//	DRAW.
///////////////////////////////////////////

void testApp::draw()
{
    ofEnableAlphaBlending();
    
    float bc = currentSettings->backgroundColor;
    ofBackground( bc * 255, bc * 255, bc * 255 );
    fbo.clear( bc, bc, bc, 1 );
    
    fbo.begin();

//    spotlight();
    
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glPushMatrix();
//    glScalef( fboScale, fboScale, 0 );
	glTranslatef( center.x, center.y, 0 );
    
    //--- camera position.
    
	float modelview[ 16 ];
	glGetFloatv( GL_MODELVIEW_MATRIX, modelview );
    
	ofxMatrix4x4 mvMat;						// model view matrix, to work out the eye position.
    mvMat.set( &modelview[ 0 ] );
    mvMat.makeInvertOf( mvMat );
    
    camera = ofxVec3f( 0.0, 0.0, 0.0 ) * mvMat;

    //--- 
    
    drawTriangles();

    for( int i=0; i<lights.size(); i++ )
        drawLights( lights[ i ] );
    
    drawRayEdges();
    drawBoids();
    
	glPopMatrix();
    
	glDisable( GL_DEPTH_TEST );

    ofSetColor( 255, 255, 255 );
    
    fbo.end();
    fbo.draw( bFullScreen ? 1440 : 0, 0, screenRect.width, screenRect.height );
    
    if( !bDebug )
        return;
    
    gui.draw();
    
    ofSetColor( 255, 255, 255 );
	for( int i=0; i<OSC_MESSAGES_TOTAL; i++ )
	{
		ofDrawBitmapString( oscMessages[ i ], 300, screenRect.height - OSC_MESSAGES_TOTAL * 15 + 15 * i );
	}
    
    {
        int w = 256;
        int h = 128;
        int x = screenRect.width - w;
        int y = screenRect.height - h;
        fft->draw( x, y, w, h );
    }
}

void testApp :: drawTriangles ()
{
    if( !currentSettings->bDrawTriangles )
        return;
    
    if( currentSettings->enableTriangleAlphaBlending )
    {
        glDepthMask( GL_FALSE );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    }
    
    glColor4f( 1, 1, 1, 1 );
    
    float* ver = new float[ 3 * 3 ];
    float* nor = new float[ 3 * 3 ];
    float* col = new float[ 4 * 3 ];
    
	for( int i=0; i<triangles.size(); i++ )
	{
		Triangle& t = *triangles[ i ];
		
        //--- vertices.
        
        ver[ 0 * 3 + 0 ] = t.p1.x;
        ver[ 0 * 3 + 1 ] = t.p1.y;
        ver[ 0 * 3 + 2 ] = t.p1.z;
        
        ver[ 1 * 3 + 0 ] = t.p2.x;
        ver[ 1 * 3 + 1 ] = t.p2.y;
        ver[ 1 * 3 + 2 ] = t.p2.z;
        
        ver[ 2 * 3 + 0 ] = t.p3.x;
        ver[ 2 * 3 + 1 ] = t.p3.y;
        ver[ 2 * 3 + 2 ] = t.p3.z;
        
        //--- normals.
        
        nor[ 0 * 3 + 0 ] = t.n1.x;
        nor[ 0 * 3 + 1 ] = t.n1.y;
        nor[ 0 * 3 + 2 ] = t.n1.z;
        
        nor[ 1 * 3 + 0 ] = t.n2.x;
        nor[ 1 * 3 + 1 ] = t.n2.y;
        nor[ 1 * 3 + 2 ] = t.n2.z;
        
        nor[ 2 * 3 + 0 ] = t.n3.x;
        nor[ 2 * 3 + 1 ] = t.n3.y;
        nor[ 2 * 3 + 2 ] = t.n3.z;
        
        //--- color.
        
        float r, g, b, a, a1, a2;
        r = 1;
        g = 1;
        b = 1;
        a1 = currentSettings->triangleColorFillOutsideAlpha;
        a2 = currentSettings->triangleColorFillInsideAlpha;
        a  = ( t.facingCamera ) ? a1 : a2;
        
        float c = 1 - t.audio * soundScale;
        c *= currentSettings->triangleColorFillOutside;
        c = ofClamp( c, 0, 1 );
        
        glColor4f( c, c, c, 1 );
        
        float p = currentSettings->triangleColorFillInsideMix;
        
        float c1 = c * p + t.c1 * ( 1 - p );
        float c2 = c * p + t.c2 * ( 1 - p );
        float c3 = c * p + t.c3 * ( 1 - p );
        
        col[ 0 * 4 + 0 ] = c1;
        col[ 0 * 4 + 1 ] = c1;
        col[ 0 * 4 + 2 ] = c1;
        col[ 0 * 4 + 3 ] = a;
        
        col[ 1 * 4 + 0 ] = c2;
        col[ 1 * 4 + 1 ] = c2;
        col[ 1 * 4 + 2 ] = c2;
        col[ 1 * 4 + 3 ] = a;
        
        col[ 2 * 4 + 0 ] = c3;
        col[ 2 * 4 + 1 ] = c3;
        col[ 2 * 4 + 2 ] = c3;
        col[ 2 * 4 + 3 ] = a;
        
        glEnableClientState( GL_VERTEX_ARRAY );
        glVertexPointer( 3, GL_FLOAT, 0, &ver[ 0 ] );
        
        glEnableClientState( GL_NORMAL_ARRAY );
        glNormalPointer( GL_FLOAT, 0, &nor[ 0 ] );
        
        glEnableClientState( GL_COLOR_ARRAY );
        glColorPointer( 4, GL_FLOAT, 0, &col[ 0 ] );
        
        glDrawArrays( GL_TRIANGLES, 0, 3 );
        
        //--- lines.
        {
            float p  = currentSettings->triangleColorLineMix;
            float c  = 0;
            float c1 = currentSettings->triangleColorLine;
            float c2 = 0;
            float a  = currentSettings->triangleColorLineAlpha;
            
            for( int j=0; j<3; j++ )
            {
                c2 = col[ j ];
                c2 += 0.1;
                
                c = c2 * p + c1 * ( 1 - p );
                c = ofClamp( c, 0, 1 );
                
                col[ j * 4 + 0 ] = c;
                col[ j * 4 + 1 ] = c;
                col[ j * 4 + 2 ] = c;
                col[ j * 4 + 3 ] = a;
            }
        }

        glColorPointer( 4, GL_FLOAT, 0, &col[ 0 ] );
        
        glLineWidth( 2.0 );
        glEnable( GL_POLYGON_OFFSET_FILL );
        glPolygonOffset( 1.0, 1.0 );
        glDrawArrays( GL_LINE_LOOP, 0, 3 );
        glDisable( GL_POLYGON_OFFSET_FILL );
        glLineWidth( 1.0 );
        
        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_NORMAL_ARRAY );
        glDisableClientState( GL_COLOR_ARRAY );
	}
    
    delete[] ver;
    delete[] nor;
    delete[] col;
    
    glBlendFunc( GL_ONE, GL_ZERO );
    glDepthMask( GL_TRUE );
}

void testApp :: drawLights ( PointLight& light )
{
    if( !currentSettings->bDrawLights )
        return;
    
    ofPoint& p = light.pos;
    
    glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
	GLfloat plane[] = 
	{
		-0.5, 0.0, 0.0,
         0.5, 0.0, 0.0,
		-0.5, 1.0, 0.0,
         0.5, 1.0, 0.0,
	};
	
	GLfloat planeCenter[] = 
	{
		-0.5, -0.5,  0.0,
         0.5, -0.5,  0.0,
		-0.5,  0.5,  0.0,
         0.5,  0.5,  0.0
	};
    
    ofTexture& tex = rayImage.getTextureReference();
	
    ofPoint p1 = tex.getCoordFromPercent( 0, 1 );
    ofPoint p2 = tex.getCoordFromPercent( 1, 1 );
    ofPoint p3 = tex.getCoordFromPercent( 0, 0 );
    ofPoint p4 = tex.getCoordFromPercent( 1, 0 );
    
	GLfloat planeTex[] = 
	{
		p1.x, p1.y,
		p2.x, p2.y,
		p3.x, p3.y,
		p4.x, p4.y
	};
    
    //---
	
    glVertexPointer( 3, GL_FLOAT, 0, plane );
    glTexCoordPointer( 2, GL_FLOAT, 0, planeTex );
    
    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    
    float c = currentSettings->lightsAlpha * 0.3;
    glColor4f( 1.0, 1.0, 1.0, c );
    
    //---
    
	tex.bind();
	for( int i=0; i<rays.size(); i++ )
	{
        Ray& ray = rays[ i ];
        
		ofxVec3f uv;	// up vector.
		ofxVec3f rv;	// right vector.
		ofxVec3f ov;	// out vector.
        
		uv.set( ray.p );
		rv = camera - uv;
		rv.normalize();
		ov = rv.getCrossed( uv );
		ov.normalize();
		rv = ov.getCrossed( uv );
		rv.normalize();
		
        GLfloat rotMat[] = 
        {
            rv.x, rv.y, rv.z, 0,
            uv.x, uv.y, uv.z, 0,
            ov.x, ov.y, ov.z, 0,
            0,    0,    0,    1
        };
        
        float s = ray.scale * light.audio * 2 + 0.1;
        
		glPushMatrix();
        glTranslatef( p.x, p.y, p.z );
		glMultMatrixf( &rotMat[ 0 ] );
		glRotatef( 90, 0, 1, 0 );
        glScalef( 2.0 * s, 0.15 * s, 0 );
        {
            glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
        }
		glPopMatrix();
	}
	tex.unbind();
    
    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	
    glDepthMask( GL_TRUE );
    glBlendFunc( GL_ONE, GL_ZERO );
}

void testApp :: drawRayEdges ()
{
    if( !currentSettings->bDrawRayEdges )
        return;
    
    glDepthMask( GL_FALSE );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
    float c = currentSettings->rayAlpha * 0.1;
    glColor4f( 1, 1, 1, c );
  
    ofTexture& tex = rayImage.getTextureReference();
    
    ofPoint p1 = tex.getCoordFromPercent( 0, 1 );
    ofPoint p2 = tex.getCoordFromPercent( 1, 1 );
    ofPoint p3 = tex.getCoordFromPercent( 0, 0 );
    ofPoint p4 = tex.getCoordFromPercent( 1, 0 );
    
	float planeTex[] =
	{
		p1.x, p1.y,
		p2.x, p2.y,
		p3.x, p3.y,
		p4.x, p4.y
	};
    
    float* vertices = new float[ 3 * 4 ];
    
    tex.bind();
    
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glTexCoordPointer( 2, GL_FLOAT, 0, planeTex );
    
    for( int i=0; i<triangles.size(); i++ )
    {
        Triangle& shape = *( triangles[ i ] );
        
        vector<ofxVec3f*> poly;
        poly.push_back( &( shape.p1 ) );
        poly.push_back( &( shape.p2 ) );
        poly.push_back( &( shape.p3 ) );
        
        vector<ofxVec3f*> edgeRays;
        edgeRays.push_back( &( shape.v1 ) );
        edgeRays.push_back( &( shape.v2 ) );
        edgeRays.push_back( &( shape.v3 ) );
        
        for( int j=0; j<edgeRays.size(); j++ )
        {
            int k = j + 1;
            if( k > edgeRays.size() - 1 )
                k = 0;
            
            ofxVec3f& p1 = *( poly[ j ] );
            ofxVec3f& p2 = *( poly[ k ] );
            ofxVec3f& v1 = *( edgeRays[ j ] );
            ofxVec3f& v2 = *( edgeRays[ k ] );
            
            vertices[ 0 * 3 + 0 ] = p1.x;
            vertices[ 0 * 3 + 1 ] = p1.y;
            vertices[ 0 * 3 + 2 ] = p1.z;
            
            vertices[ 1 * 3 + 0 ] = p2.x;
            vertices[ 1 * 3 + 1 ] = p2.y;
            vertices[ 1 * 3 + 2 ] = p2.z;
            
            vertices[ 2 * 3 + 0 ] = p1.x + v1.x;
            vertices[ 2 * 3 + 1 ] = p1.y + v1.y;
            vertices[ 2 * 3 + 2 ] = p1.z + v1.z;
            
            vertices[ 3 * 3 + 0 ] = p2.x + v2.x;
            vertices[ 3 * 3 + 1 ] = p2.y + v2.y;
            vertices[ 3 * 3 + 2 ] = p2.z + v2.z;
            
            glEnableClientState( GL_VERTEX_ARRAY );
            glVertexPointer( 3, GL_FLOAT, 0, vertices );
            {
                glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
            }
            glDisableClientState( GL_VERTEX_ARRAY );
        }
    }
    
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    
    tex.unbind();
    
    delete[] vertices;
    
    glDepthMask( GL_TRUE );
    
    glColor4f( 1, 1, 1, 1 );
}

void testApp :: drawBoids ()
{
    if( !currentSettings->bDrawBoids )
        return;
    
    boids.draw();
}

void testApp :: drawPointLight ( PointLight& light )
{
//    float materialFront[]   = { 1, 1, 1, 1 };
//    float materialBack[]    = { 1, 1, 1, 1 };
//    glMaterialfv( GL_FRONT, GL_AMBIENT,             materialFront );
//    glMaterialfv( GL_BACK,  GL_AMBIENT_AND_DIFFUSE, materialBack  );
    
    glPushMatrix();
    glTranslatef( light.pos.x, light.pos.y, light.pos.z );
    glTranslatef( light.center.x, light.center.y, light.center.z );
    {
        ofFill();
        ofSetColor( 255, 255, 255, 255 );
        ofCircle( 0, 0, 10 );
    }
    glPopMatrix();
}

void testApp :: spotlight ()
{
    glEnable( GL_LIGHT0 );
    
    float lmodel_twoside[] = { GL_TRUE };
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside );
    
    float materialFront[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv( GL_FRONT, GL_AMBIENT, materialFront );
    
    float materialBack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv( GL_BACK, GL_AMBIENT_AND_DIFFUSE, materialBack );
    
	GLfloat diffuseLight[ 4 ] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuseLight );
    
	GLfloat positionLight[] = { mouseX, mouseY, 500, 1 };
	glLightfv( GL_LIGHT0, GL_POSITION, positionLight );
    
	GLfloat spotDirectionArray[] = { 0, 0, -1, 1 };
	glLightfv( GL_LIGHT0, GL_SPOT_DIRECTION, spotDirectionArray );
    
	float spotCutoff = 100;
    glLightfv( GL_LIGHT0, GL_SPOT_CUTOFF, &spotCutoff );
    
    float spotConcetration = 60;
    glLightfv( GL_LIGHT0, GL_SPOT_EXPONENT, &spotConcetration );
}

void testApp :: pointlight ( PointLight& light )
{
    glEnable( light.lightNum );
    
    float lmodel_twoside[] = { GL_TRUE };
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside );
    
    glMaterialfv( GL_FRONT, GL_AMBIENT, light.materialFront );
    glMaterialfv( GL_BACK,  GL_AMBIENT_AND_DIFFUSE, light.materialBack );
    
	glLightfv( light.lightNum, GL_DIFFUSE,          light.diffuseLight   );
	glLightfv( light.lightNum, GL_POSITION,         light.positionLight  );
    glLightfv( light.lightNum, GL_SPOT_DIRECTION,   light.directionLight );
    glLightfv( light.lightNum, GL_SPOT_CUTOFF,      &light.spotCutoff    );
    
    glLightf( light.lightNum, GL_CONSTANT_ATTENUATION,   light.kc );
    glLightf( light.lightNum, GL_LINEAR_ATTENUATION,     light.kl );
    glLightf( light.lightNum, GL_QUADRATIC_ATTENUATION,  light.kq );
}

///////////////////////////////////////////
//	HANDLERS.
///////////////////////////////////////////

void testApp::keyPressed(int key)
{
	if( key == '[' ) gui.prevPage();
	if( key == ']' ) gui.nextPage();
    
    if( key == ',' )
    {
        settingsIndexGui = MAX( settingsIndexGui - 1, 0 );
    }
    if( key == '.' )
    {
        settingsIndexGui = MIN( settingsIndexGui + 1, settings.size() - 1 );
    }
    
    if( key == 'd' )
    {
        bDebug = !bDebug;
        
        if( bDebug )
            gui.show();
        else
            gui.hide();
    }
    
    if( key == ' ' )
    {
        bPause = !bPause;
        sound.setPaused( bPause );
    }
    
    if( key == 'f' )
    {
        bFullScreen = !bFullScreen;
        ofToggleFullscreen();
    }
    
    if( key == 'a' )
    {
        for( int i=0; i<OSC_MESSAGES_TOTAL; i++ )
            oscMessages[ i ] = "";
        
        soundPosition = 0;
        soundPositionGui = soundPosition;
        
        sound.setPosition( soundPosition );
        
        trigger.reset();
    }
}

void testApp::keyReleased(int key)
{

}

void testApp::mouseMoved(int x, int y )
{

}

void testApp::mouseDragged(int x, int y, int button)
{

}

void testApp::mousePressed(int x, int y, int button)
{

}

void testApp::mouseReleased(int x, int y, int button)
{

}

void testApp::windowResized(int w, int h)
{

}


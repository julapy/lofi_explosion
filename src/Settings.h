//
//  Settings.h
//  emptyExample
//
//  Created by lukasz karluk on 3/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxSimpleGuiToo.h"

class Settings
{
public:
    Settings ()
    {
        bDrawTriangles = true;
        bDrawLights = true;
        bDrawBoids = true;
        bDrawRayEdges = true;
        
        rotation = 0;
        rotationTarget = 0;
        rotationAudio = 0;
        rotationEase = 0.1;
        
        rotationGlitch = 0;
        rotationGlitchTarget = 0;
        rotationGlitchOriginal = 0;        
        rotationGlitchOn = false;
        rotationGlitchOff = false;
        
        backgroundColor                 = 1.0;
        triangleColorFillOutside        = 1.0;
        triangleColorFillOutsideAlpha   = 1.0;
        triangleColorFillInsideMix      = 1.0;
        triangleColorFillInsideAlpha    = 1.0;
        triangleColorLine               = 0.0;
        triangleColorLineMix            = 0.0;
        triangleColorLineAlpha          = 1.0;
        triangleRadiusToCenter          = 1.0;
        triangleRadiusToCenterTarget    = 0.0;
        
        position        = 0;
        positionStart   = 0;
        positionEnd     = 0;
        
        rayAlpha = 1.0;
        rayScale = 1.0;
        
        lightsAlpha = 1.0;
        
        boidAlpha = 0.0;
        boidAudioWeight = 0.0;
        
        enableTriangleAlphaBlending = false;
        
        audioAlpha = 1.0;
    }
    
    bool bDrawTriangles;
    bool bDrawLights;
    bool bDrawBoids;
    bool bDrawRayEdges;
    
    float rotation;
    float rotationAudio;
    float rotationTarget;
    float rotationEase;
    
    float rotationGlitch;
    float rotationGlitchTarget;
    float rotationGlitchOriginal;
    bool rotationGlitchOn;
    bool rotationGlitchOff;
    
    float position;
    float positionStart;
    float positionEnd;
    
    float backgroundColor;
    float triangleColorFillOutside;
    float triangleColorFillInsideMix;
    float triangleColorFillOutsideAlpha;
    float triangleColorFillInsideAlpha;
    float triangleColorLine;
    float triangleColorLineMix;
    float triangleColorLineAlpha;
    float triangleRadiusToCenter;
    float triangleRadiusToCenterTarget;
    
    float boidAlpha;
    float boidAudioWeight;
    
    float rayAlpha;
    float rayScale;
    
    float lightsAlpha;
    
    bool enableTriangleAlphaBlending;
    
    float audioAlpha;
    
    void copySettings ( Settings& settings )
    {
        //
    }
    
    void setPosition ( float positionStart, float positionEnd )
    {
        this->positionStart = positionStart;
        this->positionEnd   = positionEnd;
    }
    
    void reset ()
    {
        //
    }
    
    void update ( float p = -1 )
    {
        if( p > 0 )
        {
            position = ( p - positionStart ) / ( positionEnd - positionStart );
            position = ofClamp( position, 0, 1 );
        }
        
        rotation += ( rotationTarget - rotation ) * rotationEase;
    }
};
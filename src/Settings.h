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
        
        bRotate = true;
        
        backgroundColor         = 1.0;
        triangleColorFill       = 1.0;
        triangleColorLine       = 0.0;
        triangleRadiusToCenter  = 1.0;
        triangleRadiusToCenterTarget = 0.0;
        
        position        = 0;
        positionStart   = 0;
        positionEnd     = 0;
        
        bLights = false;
        
        rayAlpha = 1.0;
        rayScale = 1.0;
    }
    
    bool bDrawTriangles;
    bool bDrawLights;
    bool bDrawBoids;
    bool bDrawRayEdges;
    
    bool bRotate;
    
    float position;
    float positionStart;
    float positionEnd;
    
    bool  bLights;
    
    float backgroundColor;
    float triangleColorFill;
    float triangleColorLine;
    float triangleRadiusToCenter;
    float triangleRadiusToCenterTarget;
    
    float rayAlpha;
    float rayScale;
    
    void addToGui ( ofxSimpleGuiToo& gui )
    {
        //
    }
    
    void copySettings ( Settings& settings )
    {
        backgroundColor     = settings.backgroundColor;
        triangleColorFill   = settings.triangleColorFill;
        triangleColorLine   = settings.triangleColorLine;
    }
    
    void setPosition ( float positionStart, float positionEnd )
    {
        this->positionStart = positionStart;
        this->positionEnd   = positionEnd;
    }
    
    void update ( float p = -1 )
    {
        if( p > 0 )
        {
            position = ( p - positionStart ) / ( positionEnd - positionStart );
            position = ofClamp( position, 0, 1 );
        }
    }
};
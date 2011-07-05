//
//  Settings5.h
//  emptyExample
//
//  Created by lukasz karluk on 5/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings5 : public Settings
{
public:
    
    Settings5() : Settings()
    {
        backgroundColor = 0.0;
        
        bDrawTriangles = true;
        bDrawLights = true;
        bDrawBoids = true;
        bDrawRayEdges = true;
        
        lightsAlpha = 1;
        triangleColorFillOutside = 0;
        triangleColorFillInsideMix = 0;
        triangleColorLineMix = 1;
        triangleColorLine = 0.2;
        
        rayAlpha = 1;
        rayScale = 2;
        
        boidAudioWeight = 1.0;
        
        rotationTarget = 0.5;
        
        enableTriangleAlphaBlending = true;
        
        triangleColorFillInsideAlpha = 0;
        triangleColorFillOutsideAlpha = 0;
        
        boidAlpha = 1;
        boidAudioWeight = 0;
    }
    
    void reset ()
    {
        //
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        rayAlpha = 1 - position;
        rayScale = ( 1 - position ) * 2;
        triangleColorLineAlpha = ( 1 - position );
    }
    
};
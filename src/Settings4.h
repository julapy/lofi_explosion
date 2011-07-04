//
//  Settings4.h
//  emptyExample
//
//  Created by lukasz karluk on 5/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings4 : public Settings
{
public:
    
    Settings4() : Settings()
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
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        triangleColorFillInsideAlpha = 1 - position;
        triangleColorFillOutsideAlpha = 1 - position;
        
        boidAlpha = position;
        boidAudioWeight = 1 - position;
    }
    
};
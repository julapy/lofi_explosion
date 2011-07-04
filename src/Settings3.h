//
//  Settings1.h
//  emptyExample
//
//  Created by lukasz karluk on 3/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings3 : public Settings
{
public:
    
    Settings3() : Settings()
    {
        backgroundColor = 0.0;
        
        bDrawTriangles = true;
        bDrawLights = true;
        bDrawBoids = false;
        bDrawRayEdges = true;
        
        lightsAlpha = 1;
        triangleColorFillOutside = 0;
        triangleColorFillInsideMix = 0;
        triangleColorLineMix = 1;
        triangleColorLine = 0.2;
        
        rotationTarget = 0.5;
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        rayAlpha = position;
        rayScale = position * 2;
    }
    
};
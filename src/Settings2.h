//
//  Settings2.h
//  emptyExample
//
//  Created by lukasz karluk on 4/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings2 : public Settings
{
public:
    
    Settings2() : Settings()
    {
        backgroundColor = 0.0;
        
        bDrawTriangles = true;
        bDrawLights = true;
        bDrawBoids = false;
        bDrawRayEdges = false;
        
        lightsAlpha = 0;
        
        rotationTarget = 0.5;
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        lightsAlpha = position;
        triangleColorFillOutside = 1 - position;
        triangleColorFillInsideMix = 1 - position;
        triangleColorLineMix = position;
        triangleColorLine = position * 0.2;
    }
    
};
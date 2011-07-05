//
//  Settings6.h
//  emptyExample
//
//  Created by lukasz karluk on 6/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings6 : public Settings
{
public:
    
    Settings6() : Settings()
    {
        backgroundColor = 0.0;
        
        bDrawTriangles = false;
        bDrawLights = true;
        bDrawBoids = true;
        bDrawRayEdges = false;
        
        lightsAlpha = 1;
        
        boidAudioWeight = 1.0;
        
        rotationTarget = 0.5;
        
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
        
        boidAlpha = ( 1 - position ) * audioAlpha;
        lightsAlpha = ( 1 - position ) * audioAlpha;
    }
    
};
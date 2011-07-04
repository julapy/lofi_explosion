//
//  Header.h
//  emptyExample
//
//  Created by lukasz karluk on 3/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings1 : public Settings
{
public:
    
    Settings1() : Settings()
    {
        bDrawTriangles = true;
        bDrawLights = false;
        bDrawBoids = false;
        bDrawRayEdges = false;
        
        rotationTarget = 0.5;
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        backgroundColor = 1 - position;
        
        triangleRadiusToCenterTarget *= 0.92;
        triangleRadiusToCenter += ( ( 1.0 + triangleRadiusToCenterTarget ) - triangleRadiusToCenter ) * 0.5;
        
        float rotationAudioScaled = rotationAudio * 4;
        if( rotationTarget < rotationAudioScaled )
            rotationTarget = rotationAudioScaled;
        rotationTarget *= 0.95;
        rotationTarget = MAX( rotationTarget, 0.5 );
        rotation += ( rotationTarget - rotation ) * rotationEase;
    }
    
};
//
//  Settings0.h
//  emptyExample
//
//  Created by lukasz karluk on 4/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings0 : public Settings
{
public:
    
    Settings0() : Settings()
    {
        backgroundColor = 0.0;
        
        bDrawTriangles = true;
        bDrawLights = false;
        bDrawBoids = false;
        bDrawRayEdges = false;
        
        backgroundColor = 1.0;
        
        rotationTarget = 0;
    }
    
    void reset ()
    {
        //
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        triangleRadiusToCenterTarget *= 0.92;
        triangleRadiusToCenter += ( ( 1.0 + triangleRadiusToCenterTarget ) - triangleRadiusToCenter ) * 0.5;
        
        rotationTarget = 0.5;
        rotationTarget *= position;
        rotation += ( rotationTarget - rotation ) * rotationEase;
    }
    
};
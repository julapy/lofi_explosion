//
//  Header.h
//  emptyExample
//
//  Created by lukasz karluk on 3/07/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Settings.h"

class Settings0 : public Settings
{
public:
    
    Settings0() : Settings()
    {
        bDrawTriangles = true;
        bDrawLights = false;
        bDrawBoids = false;
        bDrawRayEdges = false;
        
        bRotate = true;
    }
    
    void addToGui ( ofxSimpleGuiToo& gui )
    {
        gui.addSlider( "settings0.position", position, 0, 1 );
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
        
        backgroundColor = 1 - position;
        
        triangleRadiusToCenterTarget *= 0.92;
        triangleRadiusToCenter += ( ( 1.0 + triangleRadiusToCenterTarget ) - triangleRadiusToCenter ) * 0.5;
    }
    
};
//
//  Settings1.h
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
        backgroundColor = 0.0;
        
        bDrawTriangles = true;
        bDrawLights = false;
        bDrawBoids = false;
        bDrawRayEdges = false;
    }
    
    void addToGui ( ofxSimpleGuiToo& gui )
    {
        gui.addSlider( "settings1.position", position, 0, 1 );
    }
    
    void update ( float p = -1 )
    {
        Settings :: update( p );
    }
    
};
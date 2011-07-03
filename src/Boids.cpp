//
//  Boids.cpp
//  emptyExample
//
//  Created by lukasz karluk on 29/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "Boids.h"

Boids :: Boids ()
{
    tex = NULL;
    
    seperationRange     = 50;
    seperationLimit     = 0.1;
    seperationScale     = 1.0;
    
    alignmentRange      = 50;
    alignmentLimit      = 0.1;
    alignmentScale      = 1.0;
    
    cohesionRange       = 50;
    cohesionLimit       = 0.1;
    cohesionScale       = 3.0;
    
    boundsRange         = 50;
    boundsScale         = 2.0;
    
    maxSpeed            = 6.5;
}

Boids :: ~Boids ()
{
    for( int i=0; i<boids.size(); i++ )
    {
        Boid* boid = boids[ i ];
        delete boid;
    }
    boids.clear();
}

void Boids :: setup ()
{
    int t = 200;
    for( int i=0; i<t; i++ )
    {
        boids.push_back( new Boid() );
        Boid& boid = *boids.back();
        
        boid.pos.x  = ofRandom( -1, 1 );
        boid.pos.y  = ofRandom( -1, 1 );
        boid.pos.z  = ofRandom( -1, 1 );
        boid.vel.x  = ofRandom( -1, 1 );
        boid.vel.y  = ofRandom( -1, 1 );
        boid.vel.z  = ofRandom( -1, 1 );
        boid.radius = 0;
    }
}

void Boids :: update ()
{
    for( int i=0; i<boids.size(); i++ )
    {
        Boid& boid = *( boids[ i ] ); 
        
        //-- constrain to bounds.
        
        float length;
        float radius;
        float dist;
        
        ofxVec3f bounds;
        bounds  = boid.pos;
        length  = bounds.length();
        radius  = 200;
        
        if( length < radius )
            dist = radius;
        else
            dist = length + radius * 0.01;
        
        bounds.normalize();
        bounds *= dist;
        
        boid.acc += avoid( boid, bounds, boundsRange ) * boundsScale;
        
        //-- flock.
        
        alignment( boid, boids );
        cohesion( boid, boids );
        seperation( boid, boids );
        
        boid.acc += boid.ali * alignmentScale;
        boid.acc += boid.coh * cohesionScale;
        boid.acc += boid.sep * seperationScale;
        
        //-- add it all up.
        
        boid.vel += boid.acc;
        boid.vel.limit( maxSpeed );
        boid.pos += boid.vel;
        boid.acc.set( 0, 0, 0 );
    }
}

void Boids :: draw ()
{
    glDepthMask( GL_FALSE );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    
    for( int i=0; i<boids.size(); i++ )
    {
        Boid& boid      = *boids[ i ];
        ofxVec3f& pos   = boid.pos;
        
        glPushMatrix();
        glTranslatef( pos.x, pos.y, pos.z );
        {
            glColor4f( 1, 0, 0, 1 );
            
//            ofFill();
//            ofCircle( 0, 0, 5 );
            
            if( tex )
            {
                float s = 0.08;
                
                glColor4f( 1, 1, 1, 1 );
                glScalef( s, s, 0 );
                glTranslatef( -tex->getWidth() * 0.5, -tex->getHeight() * 0.5, 0 );
                tex->draw( 0, 0 );
            }
        }
        glPopMatrix();
    }
    
    glDepthMask( GL_TRUE );
}

///////////////////////////////////////////////////////
//  FLOCK.
///////////////////////////////////////////////////////

ofxVec3f Boids :: avoid ( Boid& boid, const ofxVec3f& target, float range )
{
    ofxVec3f vec;
    vec = boid.pos - target;
    
    float d;
    d = boid.pos.distance( target );
    
    vec.normalize();
    
    float s;
    distanceLinearScale( d, range, s );

    vec *= s;
    
    return vec;
}

void Boids :: seperation ( Boid& boid, vector<Boid*>& boids )
{
    boid.sep.set( 0, 0, 0 );
    
    for( int i=0; i<boids.size(); i++ )
    {
        Boid& b = *boids[ i ];
        float d = boid.pos.distance( b.pos );
        
        if( d > 0 && d <= seperationRange )
        {
            float s;
            distanceLinearScale( d, seperationRange, s );
            
            ofxVec3f v = boid.pos - b.pos;
            v.normalize();
            v *= s;
            
            boid.sep += v;
        }
    }
}

void Boids :: alignment ( Boid& boid, vector<Boid*>& boids )
{
    boid.ali.set( 0, 0, 0 );
    
    for( int i=0; i<boids.size(); i++ )
    {
        Boid& b = *boids[ i ];
        float d = boid.pos.distance( b.pos );

        if( d > 0 && d <= alignmentRange )
        {
            float s;
            distanceLinearScale( d, seperationRange, s );

            boid.ali += ( b.vel * s );
        }
    }
    
    boid.ali.limit( alignmentLimit );
}

void Boids :: cohesion ( Boid& boid, vector<Boid*>& boids )
{
    boid.coh.set( 0, 0, 0 );
    
    int count = 0;
    for( int i=0; i<boids.size(); i++ )
    {
        Boid& b = *boids[ i ];
        float d = boid.pos.distance( b.pos );
        
        if( d > 0 && d <= cohesionRange )
        {
            boid.coh += b.pos;          // add to central point.
            count++;
        }
    }
    
    if( count > 0 )
    {
        boid.coh /= (float)count;       // average central point.
    }
    
    boid.coh -= boid.pos;               // vector towards central point.
    boid.coh.limit( cohesionLimit );    // limit.
}

void Boids :: random ( Boid& boid )
{
    
}

void Boids :: distanceLinearScale( float distance, float range, float& scale )
{
    scale = 0;
    
    if( distance < range )     // in range.
    {
        scale = 1 - ( distance / range );
    }
}

//
//  Boids.h
//  emptyExample
//
//  Created by lukasz karluk on 29/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxNoise.h"

class Boid
{
public:
    ofxVec3f pos;
    ofxVec3f vel;
    ofxVec3f acc;
    ofxVec3f ali;
    ofxVec3f coh;
    ofxVec3f sep;
    float radius;
    float audio;
};

class Boids : public ofBaseApp
{
public:
     Boids();
    ~Boids();
    
    void setup  ();
    void update ();
    void draw   ();
    
    ofxVec3f avoid  ( Boid& boid, const ofxVec3f& target, float range );
    void seperation ( Boid& boid, vector<Boid*>& boids );
    void alignment  ( Boid& boid, vector<Boid*>& boids );
    void cohesion   ( Boid& boid, vector<Boid*>& boids );
    void random     ( Boid& boid );
    void distanceLinearScale( float distance, float range, float& scale );
    
    vector<Boid*>   boids;
    ofTexture*      tex;

    float seperationRange;
    float seperationLimit;
    float seperationScale;
    
    float alignmentRange;
    float alignmentLimit;
    float alignmentScale;
    
    float cohesionRange;
    float cohesionLimit;
    float cohesionScale;
    
    float boundsRange;
    float boundsScale;
    
    float maxSpeed;
    
    float alpha;
    float audioWeight;
};
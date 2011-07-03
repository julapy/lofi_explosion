import processing.core.*; 
import processing.xml.*; 

import java.applet.*; 
import java.awt.*; 
import java.awt.image.*; 
import java.awt.event.*; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class flock3d extends PApplet {

/*Main runner area
* Matt Wetmore
* Changelog
* ---------
* 12/14/09: Started work
* 12/18/09: Reimplemented with BoidList class
*/

int initBoidNum = 300; //amount of boids to start the program with
BoidList flock1;//,flock2,flock3;
float zoom=800;
boolean smoothEdges = false,avoidWalls = false;



public void setup()
{
  size(800,600,P3D);
  //create and fill the list of boids
  flock1 = new BoidList(initBoidNum,255);
  //flock2 = new BoidList(100,255);
  //flock3 = new BoidList(100,128);
}

public void draw()
{
  //clear screen
  beginCamera();
  camera();
  rotateX(map(mouseY,0,height,0,TWO_PI));
  rotateY(map(mouseX,width,0,0,TWO_PI));
  translate(0,0,zoom);
  endCamera();
  background(205);
  directionalLight(255,255,255, 0, 1, -100); 
  noFill();
  stroke(0);
  
  line(0,0,300,  0,height,300);
  line(0,0,900,  0,height,900);
  line(0,0,300,  width,0,300);
  line(0,0,900,  width,0,900);
  
  line(width,0,300,  width,height,300);
  line(width,0,900,  width,height,900);
  line(0,height,300,  width,height,300);
  line(0,height,900,  width,height,900);
  
  line(0,0,300,  0,0,900);
  line(0,height,300,  0,height,900);
  line(width,0,300,  width,0,900);
  line(width,height,300,  width,height,900);
  
  flock1.run(avoidWalls);
  //flock2.run();
  //flock3.run();
  if(smoothEdges)
    smooth();
  else
    noSmooth();
}

public void keyPressed()
{
  switch (keyCode)
  {
    case UP: zoom-=10; break;
    case DOWN: zoom+=10; break;
  }
  switch (key)
  {
    case 's': smoothEdges = !smoothEdges; break;
    case 'a': avoidWalls = !avoidWalls; break;
  }
}
/*Boid object class
* Matt Wetmore
* Changelog
* ---------
* 12/14/09: Started work
* 12/16/09: Revised code to work with forces instead of heading. First steering algorithm implemented
* 12/18/09: Arrival steering behavior
* 12/20/09: Alignment added NEED TO FIX COHESION
* 1/6/10: Finished algorithms. Time to clean up the code!
*/

class Boid
{
  //fields
  PVector pos,vel,acc,ali,coh,sep; //pos, velocity, and acceleration in a vector datatype
  float neighborhoodRadius; //radius in which it looks for fellow boids
  float maxSpeed = 4; //maximum magnitude for the velocity vector
  float maxSteerForce = .1f; //maximum magnitude of the steering vector
  float h; //hue
  float sc=3; //scale factor for the render of the boid
  float flap = 0;
  float t=0;
  boolean avoidWalls = false;
  
  //constructors
  Boid(PVector inPos)
  {
    pos = new PVector();
    pos.set(inPos);
    vel = new PVector(random(-1,1),random(-1,1),random(1,-1));
    acc = new PVector(0,0,0);
    neighborhoodRadius = 100;
  }
  Boid(PVector inPos,PVector inVel,float r)
  {
    pos = new PVector();
    pos.set(inPos);
    vel = new PVector();
    vel.set(inVel);
    acc = new PVector(0,0);
    neighborhoodRadius = r;
  }
  
  public void run(ArrayList bl)
  {
    t+=.1f;
    flap = 10*sin(t);
    //acc.add(steer(new PVector(mouseX,mouseY,300),true));
    //acc.add(new PVector(0,.05,0));
    if(avoidWalls)
    {
      acc.add(PVector.mult(avoid(new PVector(pos.x,height,pos.z),true),5));
      acc.add(PVector.mult(avoid(new PVector(pos.x,0,pos.z),true),5));
      acc.add(PVector.mult(avoid(new PVector(width,pos.y,pos.z),true),5));
      acc.add(PVector.mult(avoid(new PVector(0,pos.y,pos.z),true),5));
      acc.add(PVector.mult(avoid(new PVector(pos.x,pos.y,300),true),5));
      acc.add(PVector.mult(avoid(new PVector(pos.x,pos.y,900),true),5));
    }
    flock(bl);
    move();
    checkBounds();
    render();
  }
  
  /////-----------behaviors---------------
  public void flock(ArrayList bl)
  {
    ali = alignment(bl);
    coh = cohesion(bl);
    sep = seperation(bl);
    acc.add(PVector.mult(ali,1));
    acc.add(PVector.mult(coh,3));
    acc.add(PVector.mult(sep,1));
  }
  
  public void scatter()
  {
    
  }
  ////------------------------------------
    
  public void move()
  {
    vel.add(acc); //add acceleration to velocity
    vel.limit(maxSpeed); //make sure the velocity vector magnitude does not exceed maxSpeed
    pos.add(vel); //add velocity to position
    acc.mult(0); //reset acceleration
  }
  
  public void checkBounds()
  {
    if(pos.x>width) pos.x=0;
    if(pos.x<0) pos.x=width;
    if(pos.y>height) pos.y=0;
    if(pos.y<0) pos.y=height;
    if(pos.z>900) pos.z=300;
    if(pos.z<300) pos.z=900;
  }
  
  public void render()
  {
    
    pushMatrix();
    translate(pos.x,pos.y,pos.z);
    rotateY(atan2(-vel.z,vel.x));
    rotateZ(asin(vel.y/vel.mag()));
    stroke(h);
    noFill();
    noStroke();
    fill(h);
    //draw bird
    beginShape(TRIANGLES);
    vertex(3*sc,0,0);
    vertex(-3*sc,2*sc,0);
    vertex(-3*sc,-2*sc,0);
    
    vertex(3*sc,0,0);
    vertex(-3*sc,2*sc,0);
    vertex(-3*sc,0,2*sc);
    
    vertex(3*sc,0,0);
    vertex(-3*sc,0,2*sc);
    vertex(-3*sc,-2*sc,0);
    
    /* wings
    vertex(2*sc,0,0);
    vertex(-1*sc,0,0);
    vertex(-1*sc,-8*sc,flap);
    
    vertex(2*sc,0,0);
    vertex(-1*sc,0,0);
    vertex(-1*sc,8*sc,flap);
    */
    
    vertex(-3*sc,0,2*sc);
    vertex(-3*sc,2*sc,0);
    vertex(-3*sc,-2*sc,0);
    endShape();
    //box(10);
    popMatrix();
  }
  
  //steering. If arrival==true, the boid slows to meet the target. Credit to Craig Reynolds
  public PVector steer(PVector target,boolean arrival)
  {
    PVector steer = new PVector(); //creates vector for steering
    if(!arrival)
    {
      steer.set(PVector.sub(target,pos)); //steering vector points towards target (switch target and pos for avoiding)
      steer.limit(maxSteerForce); //limits the steering force to maxSteerForce
    }
    else
    {
      PVector targetOffset = PVector.sub(target,pos);
      float distance=targetOffset.mag();
      float rampedSpeed = maxSpeed*(distance/100);
      float clippedSpeed = min(rampedSpeed,maxSpeed);
      PVector desiredVelocity = PVector.mult(targetOffset,(clippedSpeed/distance));
      steer.set(PVector.sub(desiredVelocity,vel));
    }
    return steer;
  }
  
  //avoid. If weight == true avoidance vector is larger the closer the boid is to the target
  public PVector avoid(PVector target,boolean weight)
  {
    PVector steer = new PVector(); //creates vector for steering
    steer.set(PVector.sub(pos,target)); //steering vector points away from target
    if(weight)
      steer.mult(1/sq(PVector.dist(pos,target)));
    //steer.limit(maxSteerForce); //limits the steering force to maxSteerForce
    return steer;
  }
  
  public PVector seperation(ArrayList boids)
  {
    PVector posSum = new PVector(0,0,0);
    PVector repulse;
    for(int i=0;i<boids.size();i++)
    {
      Boid b = (Boid)boids.get(i);
      float d = PVector.dist(pos,b.pos);
      if(d>0&&d<=neighborhoodRadius)
      {
        repulse = PVector.sub(pos,b.pos);
        repulse.normalize();
        repulse.div(d);
        posSum.add(repulse);
      }
    }
    return posSum;
  }
  
  public PVector alignment(ArrayList boids)
  {
    PVector velSum = new PVector(0,0,0);
    int count = 0;
    for(int i=0;i<boids.size();i++)
    {
      Boid b = (Boid)boids.get(i);
      float d = PVector.dist(pos,b.pos);
      if(d>0&&d<=neighborhoodRadius)
      {
        velSum.add(b.vel);
        count++;
      }
    }
    if(count>0)
    {
      velSum.div((float)count);
      velSum.limit(maxSteerForce);
    }
    return velSum;
  }
  
  public PVector cohesion(ArrayList boids)
  {
    PVector posSum = new PVector(0,0,0);
    PVector steer = new PVector(0,0,0);
    int count = 0;
    for(int i=0;i<boids.size();i++)
    {
      Boid b = (Boid)boids.get(i);
      float d = dist(pos.x,pos.y,b.pos.x,b.pos.y);
      if(d>0&&d<=neighborhoodRadius)
      {
        posSum.add(b.pos);
        count++;
      }
    }
    if(count>0)
    {
      posSum.div((float)count);
    }
    steer = PVector.sub(posSum,pos);
    steer.limit(maxSteerForce); 
    return steer;
  }
}
/*BoidList object class
* Matt Wetmore
* Changelog
* ---------
* 12/18/09: Started work
*/

class BoidList
{
  ArrayList boids; //will hold the boids in this BoidList
  float h; //for color
  
  BoidList(int n,float ih)
  {
    boids = new ArrayList();
    h = ih;
    for(int i=0;i<n;i++)
      boids.add(new Boid(new PVector(width/2,height/2,600)));
  }
  
  public void add()
  {
    boids.add(new Boid(new PVector(width/2,height/2)));
  }
  
  public void addBoid(Boid b)
  {
    boids.add(b);
  }
  
  public void run(boolean aW)
  {
    for(int i=0;i<boids.size();i++) //iterate through the list of boids
    {
      Boid tempBoid = (Boid)boids.get(i); //create a temporary boid to process and make it the current boid in the list
      tempBoid.h = h;
      tempBoid.avoidWalls = aW;
      tempBoid.run(boids); //tell the temporary boid to execute its run method
    }
  }
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#ECE9D8", "flock3d" });
  }
}

#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int MY_LUCKY_NUMBER = 9;

class StudentWorld;

class Actor : public GraphObject
{
public:
    //  Constructor
    Actor(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection = none)
    : GraphObject(imageID, startX, startY, startDirection),
        m_StudentWorld(myWorld),
        m_dead(false)
    {
        
    }
    virtual ~Actor() {};
    
    //  Accessors
    bool isDead() const { return m_dead; };
    StudentWorld* getWorld() const { return m_StudentWorld; };
    
    //  Mutators
    virtual void doSomething() = 0; //  pure virtual: Actor object should not be created
    void setDead() {
        m_dead = true;
    };
    
private:
    StudentWorld* m_StudentWorld;
    bool m_dead;    //  TO_FIX include here? won't be used by walls
};


class Character : public Actor
{
public:
    //  Constructor
    Character(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth, int startAmmo, Direction startDirection = none)
    : Actor(imageID, startX, startY, myWorld, startDirection),
        m_health(startHealth),
        m_ammo(startAmmo)
    {
        
    }
    
    virtual ~Character() {};
    
    //  Accessors
    int getHealth() const { return m_health; }
    int getAmmo() const { return m_ammo; }
    
    //  Mutators
    virtual void doSomething() = 0; //  pure virtual: Character object should not be created
    virtual void attacked() = 0;    //  TO_FIX to be changed??
    void fire();
    void restoreHealth() { m_health = 20; }    //  only use by Player
    void decHealth(int decBy);   //  TO_FIX may change return type
    void incAmmo(int nToAdd) { m_ammo += nToAdd; }
    void decAmmo() { m_ammo--; }
    
protected:
    
private:
    int m_health;
    int m_ammo;
};

class ImmovableObject : public Actor
{
public:
    ImmovableObject(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Actor(imageID, startX, startY, myWorld, none)
    {
        
    }
    virtual void doSomething() = 0;
    virtual ~ImmovableObject(){}
private:
    
};

class Goodie : public Actor
{
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Actor(imageID, startX, startY, myWorld, none)
    {
        
    }
    virtual void doSomething() = 0;
    virtual ~Goodie() {}
    
private:
    
};


class Robot : public Character
{
public:
    Robot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection, int startHealth);   //  implement in cpp file (don't want to include header)
    virtual void doSomething() = 0;
    virtual void attacked() = 0;    //  TO_FIX to be changed??
    int getTicksToMove() const { return ticksToMove; }
    int getTickCount() const { return tickCount; }
    void incTickCount() { tickCount++; }
    void tickCountReset() { tickCount = 1; }
    virtual ~Robot(){}
    
private:
    int ticksToMove;
    int tickCount;
};

class SnarlBot : public Robot
{
public:
    SnarlBot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection)
    : Robot(imageID, startX, startY, myWorld, startDirection, 10)
    {
        setVisible(true);
    }
    virtual void doSomething();
    virtual void attacked();
    virtual ~SnarlBot() {}
};

class KleptoBot : public Robot
{
public:
    KleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth)
    : Robot(imageID, startX, startY, myWorld, right, startHealth)
    {
        resetDistanceBeforeTurning();
        resetDistanceMoved();
        stolenGoodie = nullptr;
    }
    virtual void doSomething();
    virtual void attacked() = 0;
    bool canKleptoMove();
    int getDistanceBeforeTurning() const { return distanceBeforeTurning; }
    int getDistanceMoved() const { return distanceMoved; }
    void resetDistanceBeforeTurning();
    void resetDistanceMoved() { distanceMoved = 0; };
    void incDistanceMoved() { distanceMoved++; }
    void setStolenGoodie(Goodie* gd) { stolenGoodie = gd; }
    Goodie* getStolenGoodie() const { return stolenGoodie; }
    Direction getRandomDirection() const;
    virtual ~KleptoBot() {}
    
private:
    int distanceBeforeTurning;
    int distanceMoved;
    Goodie* stolenGoodie;
};

class RegularKleptoBot : public KleptoBot
{
public:
    RegularKleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld)
    : KleptoBot(imageID, startX, startY, myWorld, 5)
    {
        setVisible(true);
    }
    virtual void doSomething();
    virtual void attacked();
    virtual ~RegularKleptoBot() {}
    
    
};


class Player : public Character
{
public:
    //  Constructor
    Player(int imageID, int startX, int startY, StudentWorld* myWorld)  //  No Direction parameter
    : Character(imageID, startX, startY, myWorld, 20, 20, right)   //  Player starts out facing right
    {
        setVisible(true);
    }
    
    virtual ~Player() {};   //  TO_FIX ?? nothing here?
    
    //  Accessors
    
    //  Mutators
    virtual void doSomething();
    virtual void attacked();
    //void playerRestoreHealth() { restoreHealth(); }
private:


};


class Wall : public ImmovableObject
{
public:
    //  Constructor
    Wall(int imageID, int startX, int startY, StudentWorld* myWorld)
    : ImmovableObject(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    
    virtual ~Wall() {
        //  TO_FIX write delete here?
        //  don't think so...
    }
    
    virtual void doSomething() { return; }  //  Walls do nothing
    
private:
    
};

class KleptoBotFactory : public ImmovableObject
{
public:
    //  TO_FIX need to specify type of robot to create
    KleptoBotFactory(int imageID, int startX, int startY, StudentWorld* myWorld)
    : ImmovableObject(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    
    virtual ~KleptoBotFactory() {}
    
    virtual void doSoemthing() { return; }  //  TO_FIX!!
    
private:
    
};

class Hole : public ImmovableObject
{
public:
    Hole(int imageID, int startX, int startY, StudentWorld* myWorld)
    : ImmovableObject(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    
    virtual ~Hole() {};
    
    virtual void doSomething();
    
private:
    
};

class Boulder : public Actor
{
public:
    //  Constructor
    Boulder(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Actor(imageID, startX, startY, myWorld, none),
        m_health(10)
    {
        setVisible(true);
    }
    
    virtual ~Boulder() {};  // TO_FIX ?? do nothing here?
    
    virtual void doSomething() { return; }; //  Boulders do nothing
    
    void attacked();
    void push(int x, int y);
    
private:
    int m_health;
};

class Bullet : public Actor
{
public:
    //  Constructor
    Bullet(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection)
    : Actor(imageID, startX, startY, myWorld, startDirection)
    {
        setVisible(true);
    }
    
    virtual ~Bullet() {}    // TO_FIX ?? do nothing here?
    
    virtual void doSomething();
    
    
private:
    
};

class Exit : public Actor
{
public:
    Exit(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Actor(imageID, startX, startY, myWorld, none)
    {
        setVisible(false);
    }
    virtual void doSomething();
    virtual ~Exit() {}
};

class Jewel : public Actor
{
public:
    Jewel(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Actor(imageID, startX, startY, myWorld, none)
    {
        setVisible(true);
    }
    virtual void doSomething();
    virtual ~Jewel() {}
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Goodie(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    virtual void doSomething();
    virtual ~ExtraLifeGoodie() {}
    
private:
    
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Goodie(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    virtual void doSomething();
    virtual ~RestoreHealthGoodie() {}
    
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Goodie(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    virtual void doSomething();
    virtual ~AmmoGoodie() {};
};


#endif // ACTOR_H_

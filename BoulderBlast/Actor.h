#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
const int MY_LUCKY_NUMBER = 9;

class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection = none);
    virtual ~Actor() {};
    
    virtual void doSomething() = 0; //  pure virtual: Actor object should not be created
    virtual void attacked() {}; //  most Actors do nothing when attacked

    virtual bool blocksCharacter() const { return false; }  //  most Actors dont block Character
    virtual bool blocksBoulder() const { return true; } //  most Actors block Boulder
    virtual bool blocksBullet() const { return false; } //  most Actors dont block Bullet
    
    //  Accessors
    bool isDead() const { return m_dead; };
    StudentWorld* getWorld() const { return m_StudentWorld; };

    //  Mutators
    void setDead() { m_dead = true; }
    
private:
    StudentWorld* m_StudentWorld;
    bool m_dead;
};


class Character : public Actor
{
public:
    //  Constructor
    Character(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth, int startAmmo, Direction startDirection = none);
    virtual ~Character() {};
    
    virtual void doSomething() = 0; //  pure virtual: Character object should not be created
    virtual void attacked() = 0;    //  Characters have different behaviors upon attacked
    
    virtual bool blocksCharacter() const { return true; }
    virtual bool blocksBullet() const { return true; }
    
    //  Accessors
    int getHealth() const { return m_health; }
    int getAmmo() const { return m_ammo; }
    
    //  Mutators
    void fire();
    void restoreHealth() { m_health = 20; }    //  only use by Player
    void decHealth(int decBy);
    void incAmmo(int nToAdd) { m_ammo += nToAdd; }
    void decAmmo() { m_ammo--; }
    
private:
    int m_health;
    int m_ammo;
};

class ImmovableObject : public Actor
{
public:
    ImmovableObject(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~ImmovableObject() {}
    
    virtual void doSomething() = 0;
    virtual bool blocksCharacter() const { return true; }
    virtual bool blocksBullet() const { return true; }
};

class Goodie : public Actor
{
public:
    Goodie(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Goodie() {}
    virtual void doSomething() = 0;
};


class Robot : public Character
{
public:
    Robot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection, int startHealth);
    virtual ~Robot() {}
    
    virtual void doSomething() = 0;
    virtual void attacked() = 0;
    
    int getTicksToMove() const { return m_ticksToMove; }
    int getTickCount() const { return m_tickCount; }
    void incTickCount() { m_tickCount++; }
    void tickCountReset() { m_tickCount = 1; }
    
private:
    int m_ticksToMove;
    int m_tickCount;
};

class Player : public Character
{
public:
    Player(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Player() {};
    virtual void doSomething();
    virtual void attacked();
};

class SnarlBot : public Robot
{
public:
    SnarlBot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection);
    virtual ~SnarlBot() {}
    virtual void doSomething();
    virtual void attacked();
};

class KleptoBot : public Robot
{
public:
    KleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth);
    virtual ~KleptoBot() {}
    
    virtual void doSomething(); //  KleptoBots do mostly the same things
    virtual void attacked() = 0;
    
    int getDistanceBeforeTurning() const { return m_distanceBeforeTurning; }
    int getDistanceMoved() const { return m_distanceMoved; }
    Goodie* getStolenGoodie() const { return m_stolenGoodie; }
    
    void resetDistanceBeforeTurning();
    void resetDistanceMoved() { m_distanceMoved = 0; };
    void incDistanceMoved() { m_distanceMoved++; }
    void setStolenGoodie(Goodie* gd) { m_stolenGoodie = gd; }
    bool canKleptoMove();
    
private:
    Direction getRandomDirection() const;
    int m_distanceBeforeTurning;
    int m_distanceMoved;
    Goodie* m_stolenGoodie;
};

class RegularKleptoBot : public KleptoBot
{
public:
    RegularKleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~RegularKleptoBot() {}
    virtual void doSomething();
    virtual void attacked();
};

class AngryKleptoBot : public KleptoBot
{
public:
    AngryKleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~AngryKleptoBot() {}
    virtual void doSomething();
    virtual void attacked();
};

class Wall : public ImmovableObject
{
public:
    Wall(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Wall() {}
    virtual void doSomething() { return; }  //  Walls do nothing
};

class KleptoBotFactory : public ImmovableObject
{
public:
    KleptoBotFactory(int imageID, int startX, int startY, StudentWorld* myWorld, char toMake);
    virtual ~KleptoBotFactory() {}
    virtual void doSomething();
    char getProduct() const { return m_product; }
    
private:
    char m_product;
};

class Hole : public Actor
{
public:
    Hole(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Hole() {};
    
    virtual void doSomething();
    virtual bool blocksCharacter() const { return true; }
    virtual bool blocksBoulder() const { return false; }
};

class Boulder : public Actor
{
public:
    Boulder(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Boulder() {};
    
    virtual void doSomething() { return; }; //  Boulders do nothing
    virtual void attacked();
    virtual bool blocksCharacter() const { return true; }
    virtual bool blocksBullet() const { return true; }
    
private:
    int m_health;
};

class Bullet : public Actor
{
public:
    Bullet(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection);
    virtual ~Bullet() {}
    virtual void doSomething();
};

class Exit : public Actor
{
public:
    Exit(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Exit() {}
    virtual void doSomething();
};

class Jewel : public Actor
{
public:
    Jewel(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~Jewel() {}
    virtual void doSomething();
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~ExtraLifeGoodie() {}
    virtual void doSomething();
};

class RestoreHealthGoodie : public Goodie
{
public:
    RestoreHealthGoodie(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~RestoreHealthGoodie() {}
    virtual void doSomething();
};

class AmmoGoodie : public Goodie
{
public:
    AmmoGoodie(int imageID, int startX, int startY, StudentWorld* myWorld);
    virtual ~AmmoGoodie() {}
    virtual void doSomething();
};


#endif // ACTOR_H_

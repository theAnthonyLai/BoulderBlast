#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

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
    void incHealth(int incBy) { m_health += incBy; }    //  TO_FIX may change return type
    void decHealth(int decBy);   //  TO_FIX may change return type
    void incAmmo(int nToAdd);
    void decAmmo() {
        m_ammo--;
    };
    
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
    Robot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection)
    : Character(imageID, startX, startY, myWorld, 0, 0, startDirection)
    {
        //  TO_FIX might need to take extra parameters
    }
    virtual void doSomething() = 0;
    virtual void attacked() = 0;    //  TO_FIX to be changed??
    virtual ~Robot(){}
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

class Factory : public ImmovableObject
{
public:
    Factory(int imageID, int startX, int startY, StudentWorld* myWorld)
    : ImmovableObject(imageID, startX, startY, myWorld)
    {
        setVisible(true);
    }
    
    virtual ~Factory() {
        
    }
    
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



#endif // ACTOR_H_

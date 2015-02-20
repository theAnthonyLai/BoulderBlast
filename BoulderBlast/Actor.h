#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

class Actor : public GraphObject
{
public:
//***  Inherited from GraphObject   ***//
//    GraphObject(int imageID, int startX, int startY, Direction startDirection = none);
//    void setVisible(bool shouldIDisplay);
//    void getX() const;
//    void getY() const;
//    void moveTo(int x, int y);
//    Direction getDirection() const; // Directions: none, up, down, left, right
//    void setDirection(Direction d); // Directions: none, up, down, left, right
    
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
    void incHealth();   //  TO_FIX may change return type
    void decHealth();   //  TO_FIX may change return type
    void incAmmo(int nToAdd);
    void decAmmo() {
        m_ammo--;
    };
    //  virtual void attacked() = 0;    //  TO_FIX to be changed??
protected:
    virtual bool canMoveHere(int attemptX, int attemptY);
    //  virtual bool canPushBoulder(int attemptX, int attemptY) = 0;    //  TO_FIX
    
private:
    int m_health;
    int m_ammo;
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
    
private:


};


class Wall : public Actor
{
public:
    //  Constructor
    Wall(int imageID, int startX, int startY, StudentWorld* myWorld)
    : Actor(imageID, startX, startY, myWorld, none)
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
    void pushed(Direction dir); //  TO_FIX implement this 
    
private:
    int m_health;
};




#endif // ACTOR_H_

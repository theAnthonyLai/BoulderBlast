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
    
    //  Accessors
    bool isDead() const;
    
    //  Mutators
    virtual void doSomething() = 0; //  pure virtual: Actor object should not be created
    void setDead();
    
    
    StudentWorld getWorld() const;
    
private:
    StudentWorld* m_StudentWorld;
    bool m_dead;    //  TO_FIX include here? won't be used by walls
};


class Character : public Actor
{
public:
    Character(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth, int startAmmo, Direction startDirection = none)
    : Actor(imageID, startX, startY, myWorld, startDirection),
        m_health(startHealth),
        m_ammo(startAmmo)
    {
        
    }
    
    //  Accessors
    int getHealth() { return m_health; }
    int getAmmo() { return m_ammo; }
    
    //  Mutators
    virtual void doSomething() = 0; //  pure virtual: Character object should not be created
    void incHealth();   //  TO_FIX may change return type
    void decHealth();   //  TO_FIX may change return type
    void decAmmo();
    
    
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
    
    //  Accessors
    
    //  Mutators
    virtual void doSomething();
    
private:


};




#endif // ACTOR_H_

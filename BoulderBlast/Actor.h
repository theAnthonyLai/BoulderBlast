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
    
    virtual bool iBlockPlayer() const = 0;
    virtual bool iBlockRobot() const = 0;
    virtual bool iBlockBoulder() const { return true; } //  most Actor block Boulder
    
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
    virtual bool iBlockPlayer() const { return true; } //  all Characters block Player
    virtual bool iBlockRobot() const { return true; }   //  all Characters block Robot
    
    //  Accessors
    int getHealth() const { return m_health; }
    int getAmmo() const { return m_ammo; }
    
    //  Mutators
    virtual void doSomething() = 0; //  pure virtual: Character object should not be created
    virtual void attacked() = 0;    //  TO_FIX to be changed??
    void incHealth(int incBy) { m_health += incBy; }    //  TO_FIX may change return type
    void decHealth(int decBy);   //  TO_FIX may change return type
    void incAmmo(int nToAdd);
    void decAmmo() {
        m_ammo--;
    };
    
protected:
    //  virtual bool canMoveHere(int attemptX, int attemptY);
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
    virtual void attacked();
    
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
    
    virtual bool iBlockPlayer() const { return true; }    //  Walls block Player
    virtual bool iBlockRobot() const { return true; }   //  Walls block Robot
    
    
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
    
    virtual bool iBlockPlayer() const { return true; }  //  should never call this!!
    virtual bool iBlockRobot() const { return true; }   //  Boulders block Robot
    
    
    virtual void doSomething() { return; }; //  Boulders do nothing
    
    void attacked();
    bool push(int x, int y); //  TO_FIX implement this
    
private:
    int m_health;
};




#endif // ACTOR_H_

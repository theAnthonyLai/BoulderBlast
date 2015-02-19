#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Player;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir), m_Bonus(1000)
	{
	}
    
    virtual ~StudentWorld() {}    //  need to work on this
    
    virtual int init();

    virtual int move();

	virtual void cleanUp()
	{
	}
    

private:
    //  private member functions
    void loadLevel(int& imageID, int startX, int startY);
    
    //  data members
    std::list<Actor*> m_Actors;
    Player* m_Player;
    int m_Bonus;
};

#endif // STUDENTWORLD_H_

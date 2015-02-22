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
	 : GameWorld(assetDir)
	{
	}
    
    virtual ~StudentWorld();    //  need to work on this
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //bool anythingHereThatBlocksPlayer(int searchX, int searchY, char searchDir) const;
    //bool anythingHereThatBlocksBoulder(int searchX, int searchY) const;
    //bool contactBullet(int searchX, int searchY) const;
    
    bool isPlayerBlocked() const;
    bool doesBulletAttack(int searchX, int searchY) const;
    void createBullet(Actor* firedActor);
    

private:
    //  private member functions
    void loadLevel(int& imageID, int startX, int startY);
    void displayGameText();
    bool moveBoulder(Actor* boulder) const;
    
    //  data members
    std::list<Actor*> m_Actors;
    Player* m_Player;
    int m_Bonus;
};

#endif // STUDENTWORLD_H_

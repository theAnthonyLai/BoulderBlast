#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Character;
class Player;
class Robot;
class Goodie;
class KleptoBotFactory;

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
	{
	}
    
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    bool isCharacterBlocked(Character* characterToCheck) const;
    bool doesBulletAttack(int searchX, int searchY) const;
    void createBullet(Character* firedCharacter);
    bool swallowBoulder(Actor* holeToCheck);
    bool isPlayerHere(Actor* actorToCheck) const;
    Goodie* isGoodieHere(Actor* actorToCheck) const;
    void restorePlayerHealth() const;
    void addPlayerAmmo() const;
    bool doesRobotFire(Robot* robotToCheck);
    bool doesFactoryProduce(KleptoBotFactory* factory);
    
    bool noMoreJewel() const { return (m_JewelLeft == 0); }
    void decJewel() { m_JewelLeft--; }
    void setFinishLevel() { m_isLevelFinish = true; }

private:
    //  private member functions
    int loadLevel(int level, int& imageID, int startX, int startY, char& special);
    void displayGameText();
    bool moveBoulder(Actor* boulder) const;
    bool isBlockedByObstacle(int startX, int endX, int startY, int endY) const;
    
    //  data members
    std::list<Actor*> m_Actors;
    Player* m_Player;
    int m_Bonus;
    int m_JewelLeft;
    bool m_isLevelFinish;
};

#endif // STUDENTWORLD_H_

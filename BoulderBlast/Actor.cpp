#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/*
bool Character::canMoveHere(int attemptX, int attemptY) {
    //  TO_FIX
    return true;
}
*/

void Character::decHealth(int decBy) {
    m_health -= decBy;
    if (m_health <= 0)
        setDead();
}

void Character::fire() {
    getWorld()->createBullet(this);
}

void Player::doSomething() {
    //  1
    //  do nothing if the player is dead
    if (isDead())
        return;
    
    int keyPressed;
    if (getWorld()->getKey(keyPressed)) {
        //  user hit a key during this tick
        
        int attemptX;
        int attemptY;
        StudentWorld* myWorld = getWorld();
        
        switch (keyPressed) {
            case KEY_PRESS_ESCAPE:
                //  set the player to dead
                setDead();
                break;
            case KEY_PRESS_SPACE:
                //  TO_FIX
                //  add firing codes here
                if (getAmmo() > 0) {
                    fire();
                    decAmmo();
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                //  TO_FIX
                //  moveTo()
                attemptX = getX();
                attemptY = getY() + 1;
                if (!myWorld->isPlayerBlocked())
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                //  TO_FIX
                //  moveTo()
                //  moveTo()
                attemptX = getX();
                attemptY = getY() - 1;
                if (!myWorld->isPlayerBlocked())
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                //  TO_FIX
                //  moveTo()
                //  moveTo()
                attemptX = getX() - 1;
                attemptY = getY();
                if (!myWorld->isPlayerBlocked())
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                //  TO_FIX
                //  moveTo()
                //  moveTo()
                attemptX = getX() + 1;
                attemptY = getY();
                if (!myWorld->isPlayerBlocked())
                    moveTo(attemptX, attemptY);
                break;
        }
    }
}

void Player::attacked() {
    decHealth(2);
    if (!isDead())
        getWorld()->playSound(SOUND_PLAYER_IMPACT);
    else
        getWorld()->playSound(SOUND_ROBOT_DIE);
}

Robot::Robot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection, int startHealth)
: Character(imageID, startX, startY, myWorld, startHealth, 0, startDirection)
{
    //  TO_FIX might need to take extra parameters
    ticksToMove = (28 - getWorld()->getLevel()) / 4;
    if (ticksToMove < 3)
        ticksToMove = 3;
    tickCount = 1;
}

void SnarlBot::doSomething()
{
    if (isDead())
        return;
    if (getTickCount() != getTicksToMove()) {
        //  don't move during this tick
        incTickCount();
        return;
    }
    //  else do something
    if (getWorld()->doesRobotFire(this))
    
    
    /*
    if (getDirection() == right)
        setDirection(left);
    else if (getDirection() == left)
        setDirection(right);*/
    tickCountReset();
        
}

void SnarlBot::attacked()
{
    
}



void Boulder::attacked() {
    m_health -= 2;
    if (m_health == 0)
        setDead();
}

void Boulder::push(int x, int y) {
    moveTo(x, y);
}

void Bullet::doSomething() {
    if (isDead())
        return;
    
    //  a, b, and c of the spec
    int myX = getX();
    int myY = getY();
    if (getWorld()->doesBulletAttack(myX, myY)) {
        setDead();
        return;
    }
    
    //  #3 of the spec
    Direction myDir = getDirection();
    switch (myDir) {
        case up:
            myY++;
            break;
        case down:
            myY--;
            break;
        case right:
            myX++;
            break;
        case left:
            myX--;
            break;
        default:    //  WTF?
            break;
    }
    moveTo(myX, myY);
    
    //  #4 of the spec
    if (getWorld()->doesBulletAttack(myX, myY)) {
        setDead();
        return;
    }
    
}

void Hole::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->swallowBoulder(this))
        setDead();
}

void Exit::doSomething()
{
    if (getWorld()->noMoreJewel() && !isVisible()) {
        setVisible(true);
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    } else if (isVisible() && getWorld()->isPlayerHere(this)) {
        getWorld()->playSound(SOUND_FINISHED_LEVEL);
        getWorld()->increaseScore(2000);
        getWorld()->setFinishLevel();
    }
}

void Jewel::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->increaseScore(50);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->decJewel();
        setDead();
    }
}

void ExtraLifeGoodie::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
        setDead();
    }
}

void RestoreHealthGoodie::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->increaseScore(500);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->restorePlayerHealth();
        setDead();
    }
}

void AmmoGoodie::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->isPlayerHere(this)) {
        getWorld()->increaseScore(100);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addPlayerAmmo();
        setDead();
    }
}



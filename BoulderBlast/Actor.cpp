#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <iostream>

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
                if (!myWorld->isCharacterBlocked(this))
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                //  TO_FIX
                //  moveTo()
                //  moveTo()
                attemptX = getX();
                attemptY = getY() - 1;
                if (!myWorld->isCharacterBlocked(this))
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                //  TO_FIX
                //  moveTo()
                //  moveTo()
                attemptX = getX() - 1;
                attemptY = getY();
                if (!myWorld->isCharacterBlocked(this))
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                //  TO_FIX
                //  moveTo()
                //  moveTo()
                attemptX = getX() + 1;
                attemptY = getY();
                if (!myWorld->isCharacterBlocked(this))
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
    tickCountReset();
    
    if (getWorld()->doesRobotFire(this)) {
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    
    int attemptX = getX();
    int attemptY = getY();
    Direction myDir = getDirection();
    switch (myDir) {
        case right:
            attemptX++;
            break;
        case left:
            attemptX--;
            break;
        case up:
            attemptY++;
            break;
        case down:
            attemptY--;
            break;
        default:    //  WTF
            break;
    }
    
    if (!getWorld()->isCharacterBlocked(this)) {
        moveTo(attemptX, attemptY);
        return;
    } else {
        switch (myDir) {
            case right:
                setDirection(left);
                break;
            case left:
                setDirection(right);
                break;
            case up:
                setDirection(down);
                break;
            case down:
                setDirection(up);
                break;
            default:
                break;
        }
        return;
    }
}

void SnarlBot::attacked()
{
    decHealth(2);
    if (!isDead())
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    else {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        getWorld()->increaseScore(100);
    }
}

bool KleptoBot::canKleptoMove()
{
    if (getDistanceMoved() < getDistanceBeforeTurning() && !getWorld()->isCharacterBlocked(this)) {
        int attemptX = getX();
        int attemptY = getY();
        Direction myDir = getDirection();
        switch (myDir) {
            case right:
                attemptX++;
                break;
            case left:
                attemptX--;
                break;
            case up:
                attemptY++;
                break;
            case down:
                attemptY--;
                break;
            default:    //  WTF
                break;
        }
        moveTo(attemptX, attemptY);
        if (getStolenGoodie() != nullptr)
            //  this Klepto has a Goodie
            //  Goodie needs to move along
            getStolenGoodie()->moveTo(attemptX, attemptY);
        incDistanceMoved();
        return true;
    }
    return false;
}

void KleptoBot::resetDistanceBeforeTurning()
{
    distanceBeforeTurning = (rand() % 6) + 1;
}

void KleptoBot::doSomething()
{
    //  let's try stealing a Goodie
    int random = rand() % 10;
    if (getStolenGoodie() == nullptr && random == MY_LUCKY_NUMBER) {
        //  I don't have a Goodie and I'm lucky!!
        Goodie* gdToPickUp = getWorld()->isGoodieHere(this);
        if (gdToPickUp != nullptr) {    //  yes I get to pick up a Goodie!
            setStolenGoodie(gdToPickUp);
            gdToPickUp->setVisible(false);
            getWorld()->playSound(SOUND_ROBOT_MUNCH);
            return;
        }
    }
    
    //  let's try moving in this direction
    if (canKleptoMove())
        return;
    
    //  alright we need a different direction
    resetDistanceBeforeTurning();
    resetDistanceMoved();
    
    //  create random directions array
    Direction d[4] = { none, none, none, none };
    for (int i = 0; i < 4; i++) {
        while (d[i] == none) {
            Direction dir = getRandomDirection();
            bool isUnique = true;
            for (int j = 0; j < i; j++) {
                if (dir == d[j])
                    isUnique = false;
            }
            if (isUnique)
                d[i] = dir;
        }
    }
    
    //    d[0] = getRandomDirection();
    //    d[1] = d[2] = d[3] = none;
    
    //  try different direction
    for (int i = 0; i < 4; i++) {
        setDirection(d[i]);
        if (canKleptoMove())
            return;
    }
    
    //  can't move
    setDirection(d[0]);
    
    
    /*
     while (d1 == none || d2 == none || d3 == none) {
     if (canKleptoMove())
     return;
     Direction randomDir = none;
     for(;;) {
     randomDir = getRandomDirection();
     if (d1 == none && randomDir != d) {
     
     }
     }
     
     }*/

}

GraphObject::Direction KleptoBot::getRandomDirection() const
{
    int dir = rand() % 4;
    switch (dir) {
        case 0:
            return up;
            break;
        case 1:
            return down;
            break;
        case 2:
            return right;
            break;
        case 3:
            return left;
            break;
        default:    //  should not reach here
            //  TO_FIX  test??
            //std::cout << "SH*T";
            return none;
    }
}

void RegularKleptoBot::doSomething()
{
    if (isDead())
        return;
    
    if (getTickCount() != getTicksToMove()) {
        //  don't move during this tick
        incTickCount();
        return;
    }
    
    //  else do something
    tickCountReset();
    KleptoBot::doSomething();
    
}

void RegularKleptoBot::attacked()
{
    decHealth(2);
    if (!isDead())
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    else {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        if (getStolenGoodie() != nullptr)
            //  this Klepto has a Goodie
            getStolenGoodie()->setVisible(true);
        getWorld()->increaseScore(10);
    }
}

void AngryKleptoBot::doSomething()
{
    if (isDead())
        return;
    
    if (getTickCount() != getTicksToMove()) {
        //  don't move during this tick
        incTickCount();
        return;
    }
    
    //  else do something
    tickCountReset();
    
    //  check if AngryKleptoBot get to shoot
    if (getWorld()->doesRobotFire(this)) {
        getWorld()->playSound(SOUND_ENEMY_FIRE);
        return;
    }
    
    KleptoBot::doSomething();
}

void AngryKleptoBot::attacked()
{
    decHealth(2);
    if (!isDead())
        getWorld()->playSound(SOUND_ROBOT_IMPACT);
    else {
        getWorld()->playSound(SOUND_ROBOT_DIE);
        if (getStolenGoodie() != nullptr)
            //  this Klepto has a Goodie
            getStolenGoodie()->setVisible(true);
        getWorld()->increaseScore(20);
    }
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



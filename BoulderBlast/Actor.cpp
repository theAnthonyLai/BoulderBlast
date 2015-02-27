#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <iostream>

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection)
 : GraphObject(imageID, startX, startY, startDirection),
    m_StudentWorld(myWorld),
    m_dead(false)
{
    
}

Character::Character(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth, int startAmmo, Direction startDirection)
 : Actor(imageID, startX, startY, myWorld, startDirection),
    m_health(startHealth),
    m_ammo(startAmmo)
{
    
}

void Character::decHealth(int decBy) {
    m_health -= decBy;
    if (m_health <= 0)
        setDead();
}

void Character::fire() {
    getWorld()->createBullet(this);
}

ImmovableObject::ImmovableObject(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Actor(imageID, startX, startY, myWorld, none)
{
    
}

Goodie::Goodie(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Actor(imageID, startX, startY, myWorld, none)
{
    
}

Robot::Robot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection, int startHealth)
 : Character(imageID, startX, startY, myWorld, startHealth, 0, startDirection)
{
    m_ticksToMove = (28 - getWorld()->getLevel()) / 4;
    if (m_ticksToMove < 3)
        m_ticksToMove = 3;
    m_tickCount = 1;
}

Player::Player(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Character(imageID, startX, startY, myWorld, 20, 20, right)   //  Player starts out facing right
{
    setVisible(true);
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
                //  add firing codes here
                if (getAmmo() > 0) {
                    fire();
                    decAmmo();
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                attemptX = getX();
                attemptY = getY() + 1;
                if (!myWorld->isCharacterBlocked(this))
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                attemptX = getX();
                attemptY = getY() - 1;
                if (!myWorld->isCharacterBlocked(this))
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                attemptX = getX() - 1;
                attemptY = getY();
                if (!myWorld->isCharacterBlocked(this))
                    moveTo(attemptX, attemptY);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
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

SnarlBot::SnarlBot(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection)
 : Robot(imageID, startX, startY, myWorld, startDirection, 10)
{
    setVisible(true);
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

KleptoBot::KleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld, int startHealth)
 : Robot(imageID, startX, startY, myWorld, right, startHealth)
{
    resetDistanceBeforeTurning();
    resetDistanceMoved();
    m_stolenGoodie = nullptr;
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
    m_distanceBeforeTurning = (rand() % 6) + 1;
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
    
    //  try different direction
    for (int i = 0; i < 4; i++) {
        setDirection(d[i]);
        if (canKleptoMove())
            return;
    }
    
    //  can't move
    setDirection(d[0]);
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
            return none;
    }
}

RegularKleptoBot::RegularKleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld)
 : KleptoBot(imageID, startX, startY, myWorld, 5)
{
    setVisible(true);
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

AngryKleptoBot::AngryKleptoBot(int imageID, int startX, int startY, StudentWorld* myWorld)
 : KleptoBot(imageID, startX, startY, myWorld, 8)
{
    setVisible(true);
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
    
    //  else (I don't fire)
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

Wall::Wall(int imageID, int startX, int startY, StudentWorld* myWorld)
 : ImmovableObject(imageID, startX, startY, myWorld)
{
    setVisible(true);
}

KleptoBotFactory::KleptoBotFactory(int imageID, int startX, int startY, StudentWorld* myWorld, char toMake)
 : ImmovableObject(imageID, startX, startY, myWorld)
{
    setVisible(true);
    m_product = toMake;
}

void KleptoBotFactory::doSomething()
{
    int random = rand() % 50;
    if (random == MY_LUCKY_NUMBER) {
        if (getWorld()->doesFactoryProduce(this))
            getWorld()->playSound(SOUND_ROBOT_BORN);
    }
}

Hole::Hole(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Actor(imageID, startX, startY, myWorld, none)
{
    setVisible(true);
}

void Hole::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->swallowBoulder(this))
        setDead();
}

Boulder::Boulder(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Actor(imageID, startX, startY, myWorld, none),
    m_health(10)
{
    setVisible(true);
}

void Boulder::attacked() {
    m_health -= 2;
    if (m_health == 0)
        setDead();
}

Bullet::Bullet(int imageID, int startX, int startY, StudentWorld* myWorld, Direction startDirection)
 : Actor(imageID, startX, startY, myWorld, startDirection)
{
    setVisible(true);
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

Exit::Exit(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Actor(imageID, startX, startY, myWorld, none)
{
    setVisible(false);
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

Jewel::Jewel(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Actor(imageID, startX, startY, myWorld, none)
{
    setVisible(true);
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

ExtraLifeGoodie::ExtraLifeGoodie(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Goodie(imageID, startX, startY, myWorld)
{
    setVisible(true);
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

RestoreHealthGoodie::RestoreHealthGoodie(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Goodie(imageID, startX, startY, myWorld)
{
    setVisible(true);
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

AmmoGoodie::AmmoGoodie(int imageID, int startX, int startY, StudentWorld* myWorld)
 : Goodie(imageID, startX, startY, myWorld)
{
    setVisible(true);
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

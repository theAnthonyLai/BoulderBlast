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
    
}



#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp



void Player::doSomething() {
    //  1
    //  do nothing if the player is dead
    if (isDead())
        return;
    
    int keyPressed;
    if (getWorld()->getKey(keyPressed)) {
        //  user hit a key during this tick
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
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                //  TO_FIX
                //  moveTo()
                break;
            case KEY_PRESS_LEFT:
                setDirection(left);
                //  TO_FIX
                //  moveTo()
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                //  TO_FIX
                //  moveTo()
                break;
        }
    }
    
    //  TO_FIX
    //  Add codes when attacked
    
    
    
}



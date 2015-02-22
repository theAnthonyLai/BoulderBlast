#include "StudentWorld.h"
#include "Actor.h"
#include "Level.h"
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init()
{
    m_Bonus = 1000;
    int level = getLevel();
    if (level == 100)
        return GWSTATUS_PLAYER_WON;
    
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            int imageID;
            char special = 'n'; //  for v and h SnarlBot (n for none)
            //  call loadLevel to get the object at this position
            int loadLevelStatus = loadLevel(level, imageID, x, y, special);
            if (loadLevelStatus == -1)
                //  format error
                return GWSTATUS_LEVEL_ERROR;
            else if (loadLevelStatus == 1)
                //  no more files, player won
                return GWSTATUS_PLAYER_WON;
            
            switch (imageID) {
                case -1:
                    //  empty here
                    continue;
                    break;
                case IID_PLAYER:
                    m_Player = new Player(imageID, x, y, this);
                    break;
                case IID_WALL:
                    m_Actors.push_back(new Wall(imageID, x, y, this));
                    break;
                case IID_BOULDER:
                    m_Actors.push_back(new Boulder(imageID, x, y, this));
                    break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

StudentWorld::~StudentWorld()
{
    //  duplicate from cleanUp()
    //  in case cleanUp() was never called
    delete m_Player;
    m_Player = nullptr;
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        delete (*it);
        m_Actors.erase(it);
    }
}


int StudentWorld::move()
{
    //  #5 in the spec
    //  update and diplay game text
    displayGameText();
    
    
    m_Player->doSomething();
    if (m_Player->isDead()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    //  TO_FIX
    //  FINISH LEVEL
    
    //  #1 in the spec
    //  ask all Actors that are alive to do something
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if (!(*it)->isDead()) {     //  if the Actor is not dead
            (*it)->doSomething();
            if (m_Player->isDead()) {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
        }
    }
    
    //  #2 in the spec
    //  delete dead Actors
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->isDead()) {
            delete (*it);
            m_Actors.erase(it);
        }
    }
    
    //  #3 in the spec
    //  reduce bonus point for each tick
    if (m_Bonus > 0)
        m_Bonus--;
    
    //  TO_FIX
    //  add #4 here
    //  activate the EXIT if all the jewels are collected
    

    
    
    //  TO_FIX
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
    delete m_Player;
    m_Player = nullptr;
    
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        delete (*it);
        m_Actors.erase(it);
    }
    
}

bool StudentWorld::isPlayerBlocked() const
{
    int attemptX = m_Player->getX();
    int attemptY = m_Player->getY();
    GraphObject::Direction attemptDir = m_Player->getDirection();
    if (attemptDir == GraphObject::up)
        attemptY++;
    else if (attemptDir == GraphObject::down)
        attemptY--;
    else if (attemptDir == GraphObject::right)
        attemptX++;
    else    //  left
        attemptX--;
    
    //  find if anything is here
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == attemptX && (*it)->getY() == attemptY) {
            //  found Actor here
            //  test what's here
            ImmovableObject* im = dynamic_cast<ImmovableObject*>((*it));
            if (im != nullptr)
                //  is ImmovableObject
                return true;
            Robot* rb = dynamic_cast<Robot*>((*it));
            if (rb != nullptr)
                //  is Robot
                return true;

            Boulder* bd = dynamic_cast<Boulder*>((*it));
            if (bd != nullptr)
                //  is Boulder
                return !moveBoulder((*it));
                
        }
    }
            
    return false;
}

bool StudentWorld::moveBoulder(Actor* boulder) const
{
    //  return true if move is success
    //  otherwise return false
    
    //  TO_FIX  duplicate code with isPlayerBlocked
    int attemptX = boulder->getX();
    int attemptY = boulder->getY();
    GraphObject::Direction attemptDir = m_Player->getDirection();
    if (attemptDir == GraphObject::up)
        attemptY++;
    else if (attemptDir == GraphObject::down)
        attemptY--;
    else if (attemptDir == GraphObject::right)
        attemptX++;
    else    //  left
        attemptX--;
    
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == attemptX && (*it)->getY() == attemptY) {
            //  found Actor here
            //  test what's here
            ImmovableObject* im = dynamic_cast<ImmovableObject*>((*it));
            if (im != nullptr)
                //  is ImmovableObject
                return false;
            Robot* rb = dynamic_cast<Robot*>((*it));
            if (rb != nullptr)
                //  is Robot
                return false;

            Boulder* bd = dynamic_cast<Boulder*>((*it));
            if (bd != nullptr)
                //  is Boulder
                return false;
            //  TO_FIX
            //  Goodies should return false
            //  Exit should return false
            //  MORE??
        }

    }
    
    boulder->moveTo(attemptX, attemptY);
    return true;
}

bool StudentWorld::doesBulletAttack(int searchX, int searchY) const
{
    if (m_Player->getX() == searchX && m_Player->getY() == searchY) {
        m_Player->attacked();
        return true;
    }
    bool isFactoryHere = false;
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == searchX && (*it)->getY() == searchY) {
            //  found Actor here
            //  test what's here
            Factory* fa = dynamic_cast<Factory*>((*it));
            if (fa != nullptr)
                //  is Factory
                //  remember there's a Factory here
                //  continue to check if there's a robot here
                isFactoryHere = true;
            Wall* wa = dynamic_cast<Wall*>((*it));
            if (wa != nullptr) {
                //  is Wall
                //  doesn't "attack" but bullet dies
                return true;
            }
            Robot* rb = dynamic_cast<Robot*>((*it));
            if (rb != nullptr) {
                //  is Robot
                rb->attacked();
                return true;
            }
            Boulder* bd = dynamic_cast<Boulder*>((*it));
            if (bd != nullptr) {
                //  is Boulder
                bd->attacked();
                return true;
            }
            //  TO_FIX
            //  MORE??
        }
        
    }
    
    if (isFactoryHere)
        //  occupy the same coordinates as Factory
        //  but no robot here
        return true;
    
    //  continue moving 
    return false;
}

void StudentWorld::createBullet(Actor* firedActor) {
    int attemptX = firedActor->getX();
    int attemptY = firedActor->getY();
    GraphObject::Direction myDir = firedActor->getDirection();
    
    switch (myDir) {
        case GraphObject::up:
            attemptY++;
            break;
        case GraphObject::down:
            attemptY--;
            break;
        case GraphObject::right:
            attemptX++;
            break;
        case GraphObject::left:
            attemptX--;
            break;
        default:    //  WTF?
            break;
    }
    
    m_Actors.push_back(new Bullet(IID_BULLET, attemptX, attemptY, this, myDir));
    
}

int StudentWorld::loadLevel(int level, int& imageID, int startX, int startY, char& special) {
    Level lev(assetDirectory());
    
    ostringstream formattedLevel;
    formattedLevel << "level";
    formattedLevel.fill('0');
    formattedLevel << setw(2) << level;
    formattedLevel << ".dat";
    string levelToLoad = formattedLevel.str();
    
    Level::LoadResult result = lev.loadLevel(levelToLoad);
    if (result == Level::load_fail_file_not_found)
        //  no more level files, player won
        return 1;
    else if (result == Level:: load_fail_bad_format)
        //  format error
        return -1;
    else if (result == Level:: load_success)
    {
        cerr << "Successfully loaded level\n";
        Level::MazeEntry ge = lev.getContentsOf(startX,startY);  // x=5, y=10
        switch (ge)
        {
            //  TO_FIX
            //  Add other classes...add parameter to distinguish vert and horizontal snarlbot??
            case Level::empty:
                imageID = -1;   //  make -1 the identifier of empty
                break;
            case Level::exit:
                imageID = IID_EXIT;
                break;
            case Level::player:
                imageID = IID_PLAYER;
                break;
            case Level::boulder:
                imageID = IID_BOULDER;
                break;
            case Level::horiz_snarlbot:
                imageID = IID_SNARLBOT;
                special = 'h';  //  horizontal
                break;
            case Level::vert_snarlbot:
                imageID = IID_SNARLBOT;
                special = 'v';  //  vertical
                break;
            case Level::kleptobot_factory:
                imageID = IID_ROBOT_FACTORY;
                special = 'r';  //  regular
                break;
            case Level::angry_kleptobot_factory:
                imageID = IID_ROBOT_FACTORY;
                special = 'a';  //  angry
                break;
            case Level::wall:
                imageID = IID_WALL;
                break;
            case Level::hole:
                imageID = IID_HOLE;
                break;
            case Level::jewel:
                imageID = IID_JEWEL;
                break;
            case Level::restore_health:
                imageID = IID_RESTORE_HEALTH;
                break;
            case Level::extra_life:
                imageID = IID_EXTRA_LIFE;
                break;
            case Level::ammo:
                imageID = IID_AMMO;
                break;
        }
    }
    return 0;
}

void StudentWorld::displayGameText()
{
    
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    int healthLeft = (m_Player->getHealth()) / 20 * 100;    //  TO_FIX ??
    int ammoLeft = m_Player->getAmmo();
    int bonusLeft = m_Bonus;
    
    //  Format text
    ostringstream formattedText;
    
    formattedText << "Score: ";
    formattedText.fill('0');
    formattedText << setw(7) << score << "  ";
    
    formattedText << "Level: ";
    formattedText << setw(2) << level << "  ";
    
    formattedText << "Lives: ";
    formattedText.fill(' ');
    formattedText << setw(2) << livesLeft << "  ";
    
    formattedText << "Health: ";
    formattedText << setw(3) << healthLeft << "%  ";
    
    formattedText << "Ammo: ";
    formattedText << setw(3) << ammoLeft << "  ";
    
    formattedText << "Bonus: ";
    formattedText << setw(4) << bonusLeft;
    
    string textToDisplay = formattedText.str();
    setGameStatText(textToDisplay);
    
}



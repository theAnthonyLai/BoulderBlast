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
    m_JewelLeft = 0;
    isLevelFinish = false;
    
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
                case IID_HOLE:
                    m_Actors.push_back(new Hole(imageID, x, y, this));
                    break;
                case IID_EXTRA_LIFE:
                    m_Actors.push_back(new ExtraLifeGoodie(imageID, x, y, this));
                    break;
                case IID_RESTORE_HEALTH:
                    m_Actors.push_back(new RestoreHealthGoodie(imageID, x, y, this));
                    break;
                case IID_AMMO:
                    m_Actors.push_back(new AmmoGoodie(imageID, x, y, this));
                    break;
                case IID_EXIT:
                    m_Actors.push_back(new Exit(imageID, x, y, this));
                    break;
                case IID_JEWEL:
                    m_Actors.push_back(new Jewel(imageID, x, y, this));
                    m_JewelLeft++;
                    break;
                case IID_SNARLBOT:
                    if (special == 'h')
                        m_Actors.push_back(new SnarlBot(imageID, x, y, this, GraphObject::right));
                    else
                        m_Actors.push_back(new SnarlBot(imageID, x, y, this, GraphObject::down));
                    break;
                case IID_ROBOT_FACTORY:
                    if (special == 'r')
                        m_Actors.push_back(new KleptoBotFactory(imageID, x, y, this, 'r'));
                    else
                        m_Actors.push_back(new KleptoBotFactory(imageID, x, y, this, 'a'));
            }
        }
    }
    
//    m_Actors.push_back(new RegularKleptoBot(IID_KLEPTOBOT, 1, 11, this));
//    m_Actors.push_back(new AngryKleptoBot(IID_KLEPTOBOT, 13, 8, this));
//    m_Actors.push_back(new RegularKleptoBot(IID_KLEPTOBOT, 1, 4, this));
//    m_Actors.push_back(new RegularKleptoBot(IID_KLEPTOBOT, 1, 3, this));
//    m_Actors.push_back(new RegularKleptoBot(IID_KLEPTOBOT, 2, 3, this));
//    m_Actors.push_back(new RegularKleptoBot(IID_KLEPTOBOT, 2, 4, this));
    
    
    
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
    
    
    
    //  End of each tick
    if (m_Player->isDead()) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (isLevelFinish) {
        increaseScore(m_Bonus);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
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

bool StudentWorld::isCharacterBlocked(Character* characterToCheck) const
{
    int attemptX = characterToCheck->getX();
    int attemptY = characterToCheck->getY();
    GraphObject::Direction attemptDir = characterToCheck->getDirection();
    if (attemptDir == GraphObject::up)
        attemptY++;
    else if (attemptDir == GraphObject::down)
        attemptY--;
    else if (attemptDir == GraphObject::right)
        attemptX++;
    else    //  left
        attemptX--;
    
    if (attemptX == m_Player->getX() && attemptY == m_Player->getY())
        //  Player is here blocking you
        return true;
    
    //  find if anything is here
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == attemptX && (*it)->getY() == attemptY) {
            //  found Actor here
            //  test what's here
            Boulder* bd = dynamic_cast<Boulder*>((*it));
            if (bd != nullptr) {
                //  is Boulder
                Player* pl = dynamic_cast<Player*>(characterToCheck);
                if (pl != nullptr)
                    //  the Character calling this function is Player
                    //  Player can potentially push Boulder
                    return !moveBoulder((*it));
                else
                    //  the Character calling this function is Robot
                    //  Robot can't move Boulder
                    return true;
            }
//            ImmovableObject* im = dynamic_cast<ImmovableObject*>((*it));
//            if (im != nullptr)
//                //  is ImmovableObject
//                return true;
//            Robot* rb = dynamic_cast<Robot*>((*it));
//            if (rb != nullptr)
//                //  is Robot
//                return true;
            if ((*it)->blocksCharacter())
                return true;

        }
    }
            
    return false;
}

bool StudentWorld::doesBulletAttack(int searchX, int searchY) const
{
    if (m_Player->getX() == searchX && m_Player->getY() == searchY) {
        m_Player->attacked();
        return true;
    }
//    bool isFactoryHere = false;
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == searchX && (*it)->getY() == searchY) {
            //  found Actor here
            //  test what's here
//            KleptoBotFactory* fa = dynamic_cast<KleptoBotFactory*>((*it));
//            if (fa != nullptr)
//                //  is Factory
//                //  remember there's a Factory here
//                //  continue to check if there's a robot here
//                isFactoryHere = true;
//            Wall* wa = dynamic_cast<Wall*>((*it));
//            if (wa != nullptr) {
//                //  is Wall
//                //  doesn't "attack" but bullet dies
//                return true;
//            }
//            Robot* rb = dynamic_cast<Robot*>((*it));
//            if (rb != nullptr) {
//                //  is Robot
//                rb->attacked();
//                return true;
//            }
//            Boulder* bd = dynamic_cast<Boulder*>((*it));
//            if (bd != nullptr) {
//                //  is Boulder
//                bd->attacked();
//                return true;
//            }
            //  TO_FIX
            //  MORE??
            if ((*it)->attacked())
                return true;
            
        }
        
    }
    
//    if (isFactoryHere)
//        //  occupy the same coordinates as Factory
//        //  but no robot here
//        return true;
    
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
    
    m_Actors.push_front(new Bullet(IID_BULLET, attemptX, attemptY, this, myDir));
    
}

bool StudentWorld::swallowBoulder(Actor* holeToCheck) {
    int attemptX = holeToCheck->getX();
    int attemptY = holeToCheck->getY();
    
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == attemptX && (*it)->getY() == attemptY) {
            //  found Actor here
            //  test what's here
            Boulder* bd = dynamic_cast<Boulder*>((*it));
            if (bd != nullptr) {
                //  is Boulder
                bd->setDead();
                return true;
            }
        }
    }
    
    return false;
}

bool StudentWorld::isPlayerHere(Actor* actorToCheck) const {
    if (m_Player->getX() == actorToCheck->getX() && m_Player->getY() == actorToCheck->getY())
        return true;
    
    return false;
}

Goodie* StudentWorld::isGoodieHere(Actor* actorToCheck) const
{
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == actorToCheck->getX() && (*it)->getY() == actorToCheck->getY()) {
            //  found Actor here
            //  test what's here
            Goodie* gd = dynamic_cast<Goodie*>((*it));
            if (gd != nullptr)
                //  is Goodie
                return gd;
        }
    }
    return nullptr;
}

void StudentWorld::restorePlayerHealth() const {
    m_Player->restoreHealth();
}

void StudentWorld::addPlayerAmmo() const {
    m_Player->incAmmo(20);
}

bool StudentWorld::doesRobotFire(Robot* robotToCheck)
{
    int myX = robotToCheck->getX();
    int myY = robotToCheck->getY();
    int playerX = m_Player->getX();
    int playerY = m_Player->getY();
    GraphObject::Direction attemptDir = robotToCheck->getDirection();
    
    switch (attemptDir) {
        case GraphObject::right:
            if (playerY == myY && playerX > myX && !isBlockedByObstacle(myX+1, playerX-1, myY, playerY)) {
                createBullet(robotToCheck);
                return true;
            }
            break;
        case GraphObject::left:
            if (playerY == myY && playerX < myX && !isBlockedByObstacle(playerX+1, myX-1, myY, playerY)) {
                createBullet(robotToCheck);
                return true;
            }
            break;
        case GraphObject::up:
            if (playerX == myX && playerY > myY && !isBlockedByObstacle(myX, playerX, myY+1, playerY-1)) {
                createBullet(robotToCheck);
                return true;
            }
            break;
        case GraphObject::down:
            if (playerX == myX && playerY < myY && !isBlockedByObstacle(myX, playerX, playerY+1, myY-1)) {
                createBullet(robotToCheck);
                return true;
            }
            break;
        default:    //  WTF
            break;
    }
    return false;
}

bool StudentWorld::doesFactoryProduce(KleptoBotFactory* factory)
{
    int leftBound = factory->getX() - 3;
    if (leftBound < 0)
        leftBound = 0;
    int rightBound = factory->getX() + 3;
    if (rightBound > 14)
        rightBound = 14;
    int upperBound = factory->getY() + 3;
    if (upperBound > 14)
        upperBound = 14;
    int lowerBound = factory->getY() - 3;
    if (lowerBound < 0)
        lowerBound = 0;
    
    int KleptoCount = 0;
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        KleptoBot* kb = dynamic_cast<KleptoBot*>((*it));
        if (kb == nullptr)
            // is NOT KleptoBot here
            continue;
        else {
            if ((*it)->getX() == factory->getX() && (*it)->getY() == factory->getY())
                //  KleptoBot on the factory!!
                return false;
            if (leftBound <= (*it)->getX() && (*it)->getX() <= rightBound && lowerBound <= (*it)->getY() && (*it)->getY() <= upperBound)
                KleptoCount++;
        }
    }
    
    if (KleptoCount < 3) {
        if (factory->getProduct() == 'r') {
            m_Actors.push_front(new RegularKleptoBot(IID_KLEPTOBOT, factory->getX(), factory->getY(), this));
            return true;
        } else {
            //  AngryKleptoBot Factory
            m_Actors.push_front(new AngryKleptoBot(IID_KLEPTOBOT, factory->getX(), factory->getY(), this));
            return true;
        }
    }
    
    return false;
}



//  private functions

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
        //  cerr << "Successfully loaded level\n";
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
            Hole* ho = dynamic_cast<Hole*>((*it));  //  Hole needs to be before ImmovableObject
            if (ho != nullptr) {
                //  is Hole
                boulder->moveTo(attemptX, attemptY);
                return true;
            }
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
            Goodie* gd = dynamic_cast<Goodie*>((*it));
            if (gd != nullptr)
                //  is Goodie
                return false;
            Exit* ex = dynamic_cast<Exit*>((*it));
            if (ex != nullptr)
                //  is Exit
                return false;
            Jewel* jw = dynamic_cast<Jewel*>((*it));
            if (jw != nullptr)
                //  is Jewel
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


void StudentWorld::displayGameText()
{
    int score = getScore();
    int level = getLevel();
    int livesLeft = getLives();
    int healthLeft = (m_Player->getHealth()) * 5;
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

bool StudentWorld::isBlockedByObstacle(int startX, int endX, int startY, int endY) const
{
    
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        for (int x = startX; x <= endX; x++) {
            for (int y = startY; y <= endY; y++) {
                if ((*it)->getX() == x && (*it)->getY() == y) {
                    //  found actor here!
                    Hole* ho = dynamic_cast<Hole*>((*it));
                    if (ho !=nullptr)
                        continue;   //  Hole should not block bullet
                    ImmovableObject* im = dynamic_cast<ImmovableObject*>((*it));
                    if (im != nullptr)
                        //  ImmovableObject here
                        return true;
                    Robot* rb = dynamic_cast<Robot*>((*it));
                    if (rb != nullptr)
                        //  Robot here
                        return true;
                    Boulder* bd = dynamic_cast<Boulder*>((*it));
                    if (bd != nullptr)
                        //  Boulder here
                        return true;
                }
            }
        }
    }
    
    return false;
}




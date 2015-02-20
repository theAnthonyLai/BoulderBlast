#include "StudentWorld.h"
#include "GraphObject.h"
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
    
    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            int imageID;
            loadLevel(imageID, x, y);   //  call loadLevel to get the object at this position
            
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
    
    for (list<Actor*>::iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        delete (*it);
        m_Actors.erase(it);
    }
    
}

bool StudentWorld::anythingHereThatBlocksPlayer(int searchX, int searchY, char searchDir) const
{
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == searchX && (*it)->getY() == searchY) {
            //  found Actor here!!
            Boulder* boulderTest = dynamic_cast<Boulder*>((*it));
            if (boulderTest == nullptr) //  not a Boulder
                return (*it)->iBlockPlayer();
            switch (searchDir) {
                case 'u':
                    searchY++;
                    break;
                case 'd':
                    searchY--;
                    break;
                case 'r':
                    searchX++;
                    break;
                case 'l':
                    searchX--;
                default:    //  should not go here
                    break;
            }
            return (!boulderTest->push(searchX, searchY));
        }
    }
    //  no Actor here
    return false;
}

bool StudentWorld::anythingHereThatBlocksBoulder(int searchX, int searchY) const
{
    for (list<Actor*>::const_iterator it = m_Actors.begin(); it != m_Actors.end(); it++) {
        if ((*it)->getX() == searchX && (*it)->getY() == searchY) {
            //  found Actor here!!
            return (*it)->iBlockBoulder();
        }
    }
    //  no Actor here
    return false;
}



void StudentWorld::loadLevel(int& imageID, int startX, int startY) {
    Level lev(assetDirectory());
    
    Level::LoadResult result = lev.loadLevel("level00.dat");
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level00.dat data file\n";
    else if (result == Level:: load_fail_bad_format)
        cerr << "Your level was improperly formatted\n";
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
                cout << "Location 5,10 starts with a horiz. SnarlBot\n";
                break;
            case Level::vert_snarlbot:
                cout << "Location 5,10 starts with a vertical SnarlBot\n";
                break;
            case Level::kleptobot_factory:
                cout << "Location 5,10 holds a KleptoBot Factory\n";
                break;
            case Level::angry_kleptobot_factory:
                cout << "Location 5,10 holds an Angry KleptoBot Factory\n";
                break;
            case Level::wall:
                imageID = IID_WALL;
                break;
        }
    }
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



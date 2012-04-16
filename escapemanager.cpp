#include "escapemanager.h"
#include <math.h>
#include <iostream>

EscapeManager::EscapeManager(QObject *parent) :
    QObject(parent)
{
    currentSize = 36;
    gameInProgress = false;
    moveOnCooldown = false;
    badGuyTimer = new QTimer(this);
    goodGuyTimer = new QTimer(this);

    for(int i=0; i<currentSize; ++i)
    {
        trapAlreadySet.push_back(QList<bool>());
        for (int j = 0; j < currentSize; ++j)
        {
            trapAlreadySet[i].push_back(false);
        }
    }

    /* még jó enélkül is, de nem sokáig */
    /*
    int ind = currentSize-1;
    trapAlreadySet[ind/2][0] = true;
    trapAlreadySet[ind][ind] = true;
    trapAlreadySet[0][ind]   = true;
    */

    QObject::connect(goodGuyTimer, SIGNAL(timeout()),
                     this,         SLOT(resetCooldown()));
    QObject::connect(badGuyTimer, SIGNAL(timeout()),
                     this,        SLOT(badGuyTurn()));
}

bool EscapeManager::trapSetAt(int indX, int indY)
{
    return trapAlreadySet[indX][indY];
}

int EscapeManager::posToIndX(QGraphicsPixmapItem *character)
{
    return character->pos().x() / getPicSize(currentSize).toInt();
}

int EscapeManager::posToIndY(QGraphicsPixmapItem *character)
{
    return character->pos().y() / getPicSize(currentSize).toInt();
}

char EscapeManager::getDirection(QPointF goodGuy, QPointF badGuy)
{
    char dir =  distance(goodGuy.x(), badGuy.x()) >= distance(goodGuy.y(),badGuy.y()) ? 'H' : 'V';
    if (dir == 'H')
    {
        return goodGuy.x() >= badGuy.x() ? 'D' : 'A';
    }
    else //if (dir == 'V')
    {
        return goodGuy.y() >= badGuy.y() ? 'S' : 'W';
    }
}

qreal EscapeManager::distance(qreal point1, qreal point2)
{
    qreal dist = point1 - point2;
    if (dist >= 0)
        return dist;
    else
        return -dist;
}

QString EscapeManager::getPicSize(int n)
{
    if (n == 36)
        return QString().setNum(20);
    else if (n == 24)
        return QString().setNum(40); // át kell írni 30-ra!!
    else
        return QString().setNum(60);
    return QString().setNum(20);
}

void EscapeManager::setTrapAt(int indX, int indY)
{
    trapAlreadySet[indX][indY] = true;
}

bool EscapeManager::wasTrapHit(QGraphicsPixmapItem* character)
{   // DEBUG
    /*
    std::cout << "wasTrapHit called" << std::endl <<
                 "posToIndX: " << posToIndX(character) << std::endl <<
                 "posToIndY: " << posToIndY(character) << std::endl;
                 */

    return trapAlreadySet[posToIndX(character)][posToIndY(character)];
}

void EscapeManager::resetTraps()
{
    for(int i=0; i<currentSize; ++i)
    {
        for (int j = 0; j < currentSize; ++j)
        {
            trapAlreadySet[i][j] = false;
        }
    }
    int ind = currentSize-1;
    trapAlreadySet[ind/2][0] = true;
    trapAlreadySet[ind][ind] = true;
    trapAlreadySet[0][ind]   = true;
}

void EscapeManager::startGame()
{
    gameInProgress = true;
    badGuyTimer->start(1000);
    goodGuyTimer->start(500);
}

void EscapeManager::changeSize(int newSize)
{
    currentSize = newSize;
}

void EscapeManager::startCooldown()
{
    moveOnCooldown = true;
}

void EscapeManager::youLose()
{
    QMessageBox* message = new QMessageBox();
    message->setText("You walked right into my elaborate ruse!");
    message->show();
    stopGame();
}

void EscapeManager::youWin()
{
    QMessageBox* message = new QMessageBox();
    message->setText("You outwitted all your foes!");
    message->show();
    stopGame();
}

void EscapeManager::stopGame()
{
    goodGuyTimer->stop();
    badGuyTimer->stop();
    gameInProgress = false;
    //resetTraps();
    // már gázos ^
    resetField();
}

void EscapeManager::resetCooldown()
{
    moveOnCooldown = false;
}

void EscapeManager::badGuyTurn()
{
    moveBadGuys();
}

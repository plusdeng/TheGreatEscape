#include "escapeview.h"
#include "ui_escapeview.h"

#include <iostream>

EscapeView::EscapeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EscapeView)
{
    ui->setupUi(this);

    manager = new EscapeManager();

    timer = new QTimer(this);
    ui->graphicsView = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    QPen pen(Qt::black);
    QBrush brush(Qt::white);
    for (int i = 0; i < ui->sizeBox->value(); ++i)
    {
        tiles.push_back(QList<QGraphicsRectItem*>());
        for (int j = 0; j < ui->sizeBox->value(); ++j)
        {
            tiles[i].push_back(scene->addRect(i*20,j*20,20,20,pen,brush));
        }
    }

    for (int i = 0; i < ui->sizeBox->value(); ++i)
    {
        trapAlreadySet.push_back(QList<bool>());
        for (int j = 0; j < ui->sizeBox->value(); ++j)
        {
            trapAlreadySet[i].push_back(false);
        }
    }
    /* fix from here */
    // ??? listában tárolt pixmapok ???
    for (int i = 0; i <= 3; ++i)
    {
        evilGuyPics.push_back(new QPixmap(":/images/evildoer" + QString().setNum(i*20) + ".png"));
        goodGuyPics.push_back(new QPixmap(":/images/goodguy"  + QString().setNum(i*20) + ".png"));
    }
    badGuys[0] = scene->addPixmap(QPixmap(":/images/evildoer20.png"));
    badGuys[0]->setX(tiles[0][35]->boundingRect().x());
    badGuys[0]->setY(tiles[0][35]->boundingRect().y());

    badGuys[1] = scene->addPixmap(QPixmap(":/images/evildoer20.png"));
    badGuys[1]->setX(tiles[35][35]->boundingRect().x());
    badGuys[1]->setY(tiles[35][35]->boundingRect().y());

    goodGuy = scene->addPixmap(QPixmap(":/images/goodguy20.png"));
    goodGuy->setX(tiles[17][0]->boundingRect().x());
    goodGuy->setY(tiles[17][0]->boundingRect().y());
    /* to here */

    prevSize = 36;
    ui->graphicsView->setFixedSize(730,730);
    initializeConnections();

}

EscapeView::~EscapeView()
{
    std::cout << "Destructing..." << std::endl;
    delete ui;
}



void EscapeView::adjustSize(int newSize)
{
    for (int i=0; i<prevSize; ++i)
    {
        for (int j = 0; j < prevSize; ++j)
        {
            scene->removeItem(tiles[i][j]);
        }
    }
    qreal tileSize = (36 / (double)newSize) * 20;
    prevSize = newSize;
    QPen pen(Qt::black);
    QBrush brush(Qt::white);
    for (int i = 0; i < newSize; ++i)
    {
         tiles.push_back(QList<QGraphicsRectItem*>());
         for (int j = 0; j < newSize; ++j)
         {
             tiles[i][j] = scene->addRect(i*tileSize,j*tileSize,tileSize,tileSize,pen,brush);
         }
    }
    // Fix this shit!
    setupChars();
}

void EscapeView::setup()
{
    QBrush redBrush     (Qt::red);
    QBrush whiteBrush(Qt::white);
    QTime now = QTime::currentTime();
    qsrand(now.msec());
    int trapsToSet = ui->sizeBox->value();
    for (int i = 0; i < ui->sizeBox->value(); ++i)
    {
        for (int j = 0; j < ui->sizeBox->value(); ++j)
        {
            trapAlreadySet[i][j] = false;
            tiles[i][j]->setBrush(whiteBrush);
        }
    }

    /* a karakterek kezdőpontjait alapból igazra állítjuk */
    int ind = ui->sizeBox->value()-1;
    trapAlreadySet[ind/2][0] = true;
    trapAlreadySet[ind][ind] = true;
    trapAlreadySet[0][ind]    = true;

    // Ezt ki kell később törölni és átrakni resetre
    ui->startButton->setDisabled(manager->isGameInProgress());


    while (trapsToSet != 0)
    {
        int randX = qrand() % ui->sizeBox->value();
        int randY = qrand() % ui->sizeBox->value();

        if (!trapAlreadySet[randX][randY])
        {
            tiles[randX][randY]->setBrush(redBrush);
            trapAlreadySet[randX][randY] = true;
            manager->setTrapAt(randX,randY);
        }
        else
        {

            do
            {
                randX = qrand() % ui->sizeBox->value();
                randY = qrand() % ui->sizeBox->value();
            }while(trapAlreadySet[randX][randY]);
            tiles[randX][randY]->setBrush(redBrush);
            trapAlreadySet[randX][randY] = true;
            manager->setTrapAt(randX, randY);
        }
        --trapsToSet;
    }
    setupChars();

}

QString EscapeView::getPicSize(int n)
{
    if (n == 36)
        return QString().setNum(20);
    else if (n == 24)
        return QString().setNum(40); // át kell írni 30-ra!!
    else
        return QString().setNum(60);
    return QString().setNum(20);
}

void EscapeView::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_W)
        move(goodGuy, 'W');
    else if (event->key() == Qt::Key_S)
        move(goodGuy, 'S');
    else if (event->key() == Qt::Key_A)
        move(goodGuy, 'A');
    else if (event->key() == Qt::Key_D)
        move(goodGuy, 'D');
}

void EscapeView::moveCharacter(QGraphicsPixmapItem *character, char direction)
{
    qreal tileSize = manager->getPicSize(ui->sizeBox->value()).toDouble();
    if(isGoingOutOfBounds(character, direction) || !(manager->isGameInProgress()))
        return;

    if(character == goodGuy && manager->isOnCooldown())
        return;

    switch(direction)
    {
    case 'D':
        character->moveBy(tileSize,0);
        break;
    case 'A':
        character->moveBy(-tileSize, 0);
        break;
    case 'W':
        character->moveBy(0, -tileSize);
        break;
    case 'S':
        character->moveBy(0, tileSize);
        break;
    default:
        break;
    }
    if (manager->wasTrapHit(goodGuy))
        gameOver();
    if (character == goodGuy)
        goodGuyMoved();
    std::cout << "X : goodGuy: " << character->pos().x() << " Y : goodGuy: " << character->pos().y() << std::endl;
}

void EscapeView::moveAllBadGuys()
{
    if(manager->wasTrapHit(badGuys[0]) && manager->wasTrapHit(badGuys[1]))
        badGuysTrapped();

    if(!manager->wasTrapHit(badGuys[0]))
    {
    moveCharacter(badGuys[0],
                  manager->getDirection(goodGuy->pos(), badGuys[0]->pos()));
    }
    if(!manager->wasTrapHit(badGuys[1]))
    {
    moveCharacter(badGuys[1],
                  manager->getDirection(goodGuy->pos(), badGuys[1]->pos()));
    }
}

void EscapeView::initializeConnections()
{
    QObject::connect(ui->sizeBox, SIGNAL(valueChanged(int)),
                     this,        SLOT(adjustSize(int)));
    QObject::connect(ui->startButton, SIGNAL(clicked()),
                     this,            SLOT(setup()));
    QObject::connect(this,        SIGNAL(move(QGraphicsPixmapItem*,char)),
                     this,        SLOT(moveCharacter(QGraphicsPixmapItem*,char)));
    QObject::connect(ui->startButton, SIGNAL(clicked()),
                     manager,         SLOT(startGame()));
    QObject::connect(this,        SIGNAL(goodGuyMoved()),
                     manager,     SLOT(startCooldown()));
    QObject::connect(manager,     SIGNAL(moveBadGuys()),
                     this,        SLOT(moveAllBadGuys()));
    QObject::connect(ui->sizeBox, SIGNAL(valueChanged(int)),
                     manager,     SLOT(changeSize(int)));
    QObject::connect(this,       SIGNAL(gameOver()),
                     manager,    SLOT(youLose()));
    QObject::connect(this,        SIGNAL(badGuysTrapped()),
                     manager,     SLOT(youWin()));
    QObject::connect(manager,    SIGNAL(resetField()),
                     this,       SLOT(setup()));
}

void EscapeView::setupChars()
{
    // buggy
    scene->removeItem(badGuys[0]);
    scene->removeItem(badGuys[1]);
    scene->removeItem(goodGuy);

    int ind = ui->sizeBox->value()-1;
    badGuys[0] = scene->addPixmap(QPixmap(":/images/evildoer" + manager->getPicSize(ui->sizeBox->value()) + ".png"));
    badGuys[0]->setX(tiles[0][ind]->boundingRect().x());
    badGuys[0]->setY(tiles[0][ind]->boundingRect().y());

    badGuys[1] = scene->addPixmap(QPixmap(":/images/evildoer" + manager->getPicSize(ui->sizeBox->value()) + ".png"));
    badGuys[1]->setX(tiles[ind][ind]->boundingRect().x());
    badGuys[1]->setY(tiles[ind][ind]->boundingRect().y());

    goodGuy = scene->addPixmap(QPixmap(":/images/goodguy" + manager->getPicSize(ui->sizeBox->value()) + ".png"));
    goodGuy->setX(tiles[ind/2][0]->boundingRect().x());
    goodGuy->setY(tiles[ind/2][0]->boundingRect().y());
    goodGuy->setZValue(2);
    badGuys[0]->setZValue(2);
    badGuys[1]->setZValue(2);
}

bool EscapeView::isGoingOutOfBounds(QGraphicsPixmapItem *character, char direction)
{
    int tileSize = manager->getPicSize(ui->sizeBox->value()).toInt();
    int posX = character->pos().x();
    int posY = character->pos().y();
    switch(direction)
    {
    case 'D':
        posX += tileSize;
        break;
    case 'S':
        posY += tileSize;
        break;
    case 'A':
        posX -= tileSize;
        break;
    case 'W':
        posY -= tileSize;
        break;
    default:
        break;
    }
    /* debug */
    std::cout <<tileSize*(ui->sizeBox->value()-1) << std::endl;
    std::cout << "PosX < 0? : " << (posX < 0) << std::endl <<
            "PosY < 0? : " << (posY < 0) << std::endl <<
            "posX > tileSize*(ui->sizeBox->value()-1)? : " << (posX > tileSize*(ui->sizeBox->value()-1)) << std::endl <<
            "posY > tileSize*(ui->sizeBox->value()-1)? : " << (posY > tileSize*(ui->sizeBox->value()-1)) << std::endl;
    /* debug end */
    return posX < 0 || posY < 0 || posX > tileSize*(ui->sizeBox->value()-1) || posY > tileSize*(ui->sizeBox->value()-1);
}



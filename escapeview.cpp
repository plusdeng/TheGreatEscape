#include "escapeview.h"
#include "ui_escapeview.h"

#include <iostream>

EscapeView::EscapeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EscapeView)
{
    ui->setupUi(this);



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
    prevSize = 36;
    ui->graphicsView->setFixedSize(730,730);
    QObject::connect(ui->sizeBox, SIGNAL(valueChanged(int)),
                     this,        SLOT(adjustSize(int)));
    QObject::connect(ui->startButton, SIGNAL(clicked()),
                     this,            SLOT(setup()));
}

EscapeView::~EscapeView()
{
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
             //qreal iSize = i;
             //qreal jSize = j;
             tiles[i][j] = scene->addRect(i*tileSize,j*tileSize,tileSize,tileSize,pen,brush);
         }
    }
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
    while (trapsToSet != 0)
    {
        int randX = qrand() % ui->sizeBox->value();
        int randY = qrand() % ui->sizeBox->value();
        std::cout << "randX : " << randX << std::endl <<
                     "randY : " << randY << std::endl;
        if (!trapAlreadySet[randX][randY])
        {
            tiles[randX][randY]->setBrush(redBrush);
            trapAlreadySet[randX][randY] = true;
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
        }
        --trapsToSet;
    }
    std::cout << "done" << std::endl;
}

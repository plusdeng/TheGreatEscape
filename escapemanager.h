#ifndef ESCAPEMANAGER_H
#define ESCAPEMANAGER_H

#include <QObject>
#include <QtCore>
#include <QtGui>

class EscapeManager : public QObject
{
    Q_OBJECT
public:
    explicit EscapeManager  (QObject *parent = 0);
    bool     trapSetAt      (int indX, int indY);
    bool     wasTrapHit     (QGraphicsPixmapItem* character);
    int      posToIndX      (QGraphicsPixmapItem* character);
    int      posToIndY      (QGraphicsPixmapItem* character);
    char     getDirection   (QPointF goodGuy, QPointF badGuy);
    static   qreal distance (qreal point1, qreal point2);
    QString  getPicSize     (int n);
    void     setTrapAt      (int indX, int indY);
    bool     inStartingPos  (int indX, int indY, int currentSize);
    bool     inStartingPos  (QGraphicsPixmapItem* character);
    bool     isGameInProgress() { return gameInProgress; }
    bool     isOnCooldown   ()  { return moveOnCooldown; }
    void     loadPixmaps    (QList<QPixmap>& goodGuyPics, QList<QPixmap>& badGuyPics);

private:
    void resetTraps         ();

    QList<QList <bool> >    trapAlreadySet;
    int                     currentSize;
    bool                    gameInProgress;
    bool                    moveOnCooldown;
    QTimer                 *badGuyTimer;
    QTimer                 *goodGuyTimer;


signals:
    void trapActivated      (QGraphicsPixmapItem* character);
    void moveBadGuys        ();
    void resetField         ();

public slots:
    void startGame          ();
    void changeSize         (int newSize);
    void startCooldown      ();
    void youLose            ();
    void youWin             ();
    void stopGame           ();
private slots:
    void resetCooldown      ();
    void badGuyTurn         ();

};

#endif // ESCAPEMANAGER_H

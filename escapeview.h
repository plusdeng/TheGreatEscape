#ifndef ESCAPEVIEW_H
#define ESCAPEVIEW_H

#include <QWidget>
#include "escapemanager.h"


namespace Ui {
    class EscapeView;
}

class EscapeView : public QWidget
{
    Q_OBJECT

public:
    explicit EscapeView(QWidget *parent = 0);
    ~EscapeView();

signals:
    void                move
                        (QGraphicsPixmapItem* character, char direction);
public slots:
    void adjustSize(int newSize);
    void                 setup();
    void                 moveCharacter
                        (QGraphicsPixmapItem* character, char direction);


private:
    Ui::EscapeView                        *ui;
    QGraphicsScene                     *scene;
    QList<QList<QGraphicsRectItem*> >   tiles;
    QTimer                             *timer;
    QList<QList <bool> >       trapAlreadySet;
    QList<QPixmap *>              evilGuyPics;
    QList<QPixmap *>              goodGuyPics;
    QList<QGraphicsPixmapItem*>       badGuys;
    QGraphicsPixmapItem*              goodGuy;
    int                              prevSize;
    QString                     getPicSize();
    void                        keyPressEvent(QKeyEvent *);
};

#endif // ESCAPEVIEW_H

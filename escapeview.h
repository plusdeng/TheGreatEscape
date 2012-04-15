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

public slots:
    void adjustSize(int newSize);
    void                 setup();

private:
    Ui::EscapeView                        *ui;
    QGraphicsScene                     *scene;
    QList<QList<QGraphicsRectItem*> >   tiles;
    QTimer                             *timer;
    QList<QList <bool> >       trapAlreadySet;
    int                              prevSize;

};

#endif // ESCAPEVIEW_H

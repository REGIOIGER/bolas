#ifndef CAJA_H
#define CAJA_H

#include <QWidget>
#include <QPainter>
#include <QTimer>
#include <QImage>

#include "Particle.h"
#include <vector>

class Caja: public QWidget {
    Q_OBJECT

    QTimer* timer;
    QImage *trajectory;

    int cajaWidth = 640;
    int cajaHeight = 480;

    int boundary = 15;

    int numParticles = 500;
    std::vector<Particle> particles;

    public:
    explicit Caja(QWidget *parent = 0);
            ~Caja();

    protected:
    void paintEvent(QPaintEvent *event);

    public slots:
    void nextFrame();
};

#endif // CAJA_H

#include "caja.h"

#include <QApplication>

Caja::Caja(QWidget *parent) :

QWidget(parent)
{

    // Set size of the window
    setFixedSize(cajaWidth, cajaHeight);

    std::mt19937 rndEngineX, rndEngineY, rndEngineZ;
    std::random_device rdev{};

    std::mt19937 rndEngineVX, rndEngineVY, rndEngineVZ;

    rndEngineX.seed(rdev());
    rndEngineY.seed(rdev());
    rndEngineZ.seed(rdev());

    rndEngineVX.seed(rdev());
    rndEngineVY.seed(rdev());
    rndEngineVZ.seed(rdev());

    particles.reserve(numParticles);


    std::uniform_real_distribution<double> rndXYZ{ -200, 200};
    std::uniform_real_distribution<double> rndV{ -5, 5 };
    int flag = 0;

    for (int i = 0; i < numParticles; ++i)
    {
        flag = 0;
        Particle particle;

        particle.position.X = rndXYZ(rndEngineX);
        particle.position.Y = rndXYZ(rndEngineY);
        particle.position.Z = 0;//rndXYZ(rndEngineZ);


        // now the velocity

        particle.velocity.X = rndV(rndEngineVX);
        particle.velocity.Y = rndV(rndEngineVY);
        particle.velocity.Z = 0;//rndV(rndEngineVZ);


        //particle.velocity = particle.velocity.Normalize();
        particle.radius = 5;
        particle.mass = 1;
        particle.color = 1;
        if(i <= 0)
        {
            particle.radius = 50;
            particle.mass = 50;

            //tx = particle.position.X;
            //ty = particle.position.Y;
        }

        for (int j = 0; j < i; ++j)
        {
            double dd = (particle.position.X - particles[j].position.X)*(particle.position.X - particles[j].position.X) + (particle.position.Y - particles[j].position.Y)*(particle.position.Y - particles[j].position.Y);
            double rr = (particle.radius + particles[j].radius)*(particle.radius + particles[j].radius);
            if(dd <= rr )
            {
                flag = 1;
            }

        }

        if(flag == 1)
        {
            i--;
            continue;
        }
        else
        {
        particles.push_back(particle);
        }
    }

    this->setStyleSheet("background-color: #82CAFF;");
    this->update();

    trajectory = new QImage(cajaWidth, cajaHeight, QImage::Format_ARGB32);
    trajectory->fill(qRgba(0,0,0,0));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    timer->start(32);
}

Caja::~Caja() {
    delete timer;
}

void Caja::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);


    // add after the creation of the painter object
    QPen pen;
    pen.setWidth(2);
    pen.setColor(Qt::black);
    QBrush brush(Qt::yellow);

    painter.setPen (pen);
    painter.setBrush(brush);
    // add before the painter draws the ellipse

    for (int i = 0; i < numParticles; ++i)
    {
        if(particles[i].color == 1)
        {
            brush.setColor(Qt::yellow);
            painter.setBrush(brush);
        }
        else
        {
            brush.setColor(Qt::red);
            painter.setBrush(brush);
        }
        painter.drawEllipse(particles[i].position.X + cajaWidth/2 - particles[i].radius, particles[i].position.Y + cajaHeight/2 - particles[i].radius, 2*particles[i].radius, 2*particles[i].radius);
    }

    //brush.setColor(Qt::red);
    //painter.setBrush(brush);
    //QPoint center;
    //center.setX(cajaWidth/2);
    //center.setY(cajaHeight/2);
    //painter.drawEllipse(center, 25, 25);


    brush.setStyle(Qt::NoBrush);
    painter.setBrush(brush);

    QPolygon house;
    house.putPoints(0, 4, cajaWidth - boundary, boundary, cajaWidth - boundary, cajaHeight - boundary, boundary, cajaHeight - boundary, boundary, boundary);
    painter.drawPolygon(house);

    //tx = particles[0].position.X;
    //ty = particles[0].position.Y;

    QPainter painters(trajectory);
    //painters.setRenderHint(QPainter::Antialiasing, true);

    painters.setPen(QPen(Qt::black, 2));
    //painters.drawLine(QPointF(particles[0].position.X, particles[0].position.Y),QPointF(qrand() % 300,qrand() % 300));
    painters.drawEllipse(particles[0].position.X + cajaWidth/2 - 1, particles[0].position.Y + cajaHeight/2 - 1, 2*1, 2*1);
    painter.drawImage(0, 0, *trajectory);
}

void Caja::nextFrame() {
    // animate


    for (int i = 0; i < numParticles; ++i)
        for (int j = i + 1; j < numParticles; ++j)
        {
            double dd = (particles[i].position.X - particles[j].position.X)*(particles[i].position.X - particles[j].position.X) + (particles[i].position.Y - particles[j].position.Y)*(particles[i].position.Y - particles[j].position.Y);
            double rr = (particles[i].radius + particles[j].radius)*(particles[i].radius + particles[j].radius);
            if(dd <= rr)
            {
                //particles[i].color = 0;
                //particles[j].color = 0;

                double totalMass = particles[i].mass + particles[j].mass;
                Vector3D<double> velDif = particles[i].velocity - particles[j].velocity;
                Vector3D<double> dir = (particles[i].position - particles[j].position).Normalize();

                Vector3D<double> mult = 2 * (velDif * dir) * dir / totalMass;

                particles[i].velocity -= particles[j].mass * mult;
                particles[j].velocity += particles[i].mass * mult;

                if(particles[i].radius > particles[j].radius)
                {
                    Vector3D<double> colpoint(particles[i].position - dir *  particles[i].radius);
                    particles[i].position = colpoint + dir *  particles[i].radius;
                    particles[j].position = colpoint + -dir *  particles[j].radius;
                }
                else
                {
                    Vector3D<double> colpoint(particles[j].position + dir *  particles[j].radius);
                    particles[i].position = colpoint + dir *  particles[i].radius;
                    particles[j].position = colpoint + -dir *  particles[j].radius;
                }

            }

        }



    for (int i = 0; i < numParticles; ++i)
    {
        particles[i].position = particles[i].position + particles[i].velocity;

        if(particles[i].position.X >= (cajaWidth/2 - boundary - particles[i].radius))
        {
            particles[i].velocity.X = -particles[i].velocity.X;
            particles[i].position.X = cajaWidth/2 - boundary - particles[i].radius;
        }
        if(particles[i].position.X <= -(cajaWidth/2 - boundary - particles[i].radius))
        {
            particles[i].velocity.X = -particles[i].velocity.X;
            particles[i].position.X = -(cajaWidth/2 - boundary - particles[i].radius);
        }
        if(particles[i].position.Y >= (cajaHeight/2 - boundary - particles[i].radius))
        {
            particles[i].velocity.Y = -particles[i].velocity.Y;
            particles[i].position.Y = cajaHeight/2 - boundary - particles[i].radius;
        }
        if(particles[i].position.Y <= -(cajaHeight/2 - boundary - particles[i].radius))
        {
            particles[i].velocity.Y = -particles[i].velocity.Y;
            particles[i].position.Y = -(cajaHeight/2 - boundary - particles[i].radius);
        }
    }
    update();
}


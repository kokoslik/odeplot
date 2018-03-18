#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QMouseEvent>
#include <boost/numeric/odeint.hpp>
#include <array>
#include <utility>

using namespace boost::numeric::odeint;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

void MainWindow::paintEvent(QPaintEvent* event)
{
    typedef array<double,2> p2d;
    p2d x0={(a-500)/250.0,(500-b)/250.0};
    vector<p2d> results;
    auto stepper = make_dense_output(1e-6,1e-6,runge_kutta_dopri5< p2d >());
    auto sys = [](const p2d& x, p2d& f, double t){f[0]=-x[1]; f[1]=+x[0]+x[1]+x[1]*x[1]; return;};
    stepper.initialize(x0,0.0,0.01);
    while(stepper.current_time()<100.0)
    {
        //if(stepper.current_time_step()>0.01)
            //stepper.initialize(stepper.current_state(),stepper.current_time(),0.01);
        pair<double,double> delta = stepper.do_step(sys);

        p2d r = stepper.current_state();
        if((r[0]+1.)*(r[0]-1.)*(r[1]+1.)*(r[1]-1.) < 0.0)
        {
            while(delta.second-delta.first>1e-10)
            {
                double t=0.5*(delta.second+delta.first);
                stepper.calc_state(t,r);
                if((r[0]+1)*(r[0]-1)*(r[1]+1)*(r[1]-1) < 0)
                    delta.second=t;
                else
                    delta.first=t;
            }
            results.push_back(r);
            qDebug()<<r[0]<<' '<<r[1]<<endl;
            break;
        }
        else
        {
            results.push_back(r);
        }
    }
    vector<QPointF> points;
    QPointF rectp[]={{250,250},{250,750},{750,750},{750,250},{250,250}};
    for(int i=0;i<results.size();i++)
    {
        points.emplace_back(500+250*results[i][0],500-250*results[i][1]);
    }

    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawPolyline(points.data(), points.size());
    painter.drawPolyline(rectp, 5);
}

void MainWindow::mousePressEvent(QMouseEvent * event)
{
    a=event->x();
    b=event->y();
}

void MainWindow::mouseReleaseEvent(QMouseEvent * event)
{
    this->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

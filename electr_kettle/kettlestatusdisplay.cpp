#include "kettlestatusdisplay.h"
#include "kettlestate.h"
#include "kettlecontrollerimpl.h"
#include <QTimer>
#include <QPainter>
#include <QTime>

constexpr int topMargin = 30;///размер верхнего поля
constexpr int botoomMargin = 20;///размер нижнего поля
constexpr auto tmax = 110.;///максимальная температура на графике
constexpr auto tmin = 0.;///минимальная температура на графике

KettleStatusDisplay::KettleStatusDisplay(QWidget *parent) : QWidget(parent)
{
    m_ketState = new KettleState(this);
    connect(m_ketState, &KettleState::heatingChanged, this, &KettleStatusDisplay::setHeating);
    m_ketCntrl = new KettleControllerImpl(*m_ketState,
                    [this](){m_ketState->startHeating();},
                    [this](){m_ketState->stopHeating();} );
    m_updTimer = new QTimer(this);
    m_updTimer->setInterval(m_refreshPeriod_ms);
    connect(m_updTimer, &QTimer::timeout, this, &KettleStatusDisplay::updateStatus);
    setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
    setMinimumSize(640, 480);
    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    ///The sizeHint() is minimal, and sufficient. The widget can make use of extra space,
    ///so it should get as much space as possible (e.g. the horizontal direction of a horizontal slider).
}

KettleStatusDisplay::~KettleStatusDisplay()
{
    delete m_ketCntrl;
}

void KettleStatusDisplay::run()
{
    m_simulate = true;
    m_updTimer->start();
}

void KettleStatusDisplay::stop()
{
    m_simulate = false;
    m_updTimer->stop();
}

void KettleStatusDisplay::setHeating(bool iHeating)
{
    if(iHeating)
        m_ketState->startHeating();
    else
        m_ketState->stopHeating();
    emit heatingChanged(iHeating);
}

void KettleStatusDisplay::setAutomatic(bool iAutomatic)
{
    m_automatic = iAutomatic;
}

void KettleStatusDisplay::setRandomEvents(bool iRE)
{
    m_randomEvents = iRE;
}

void KettleStatusDisplay::updateStatus()
{
    for(int i = 0; i<m_refreshPeriod_ms; i+=2 )///TODO magic value определяет "ускорение" времени эмуляции -- это глупость
    {
        if(m_automatic)
            m_ketCntrl->controllerProcess();
        m_ketState->calcState(20, m_randomEvents);///Считаем состояние за посление 20 мсек
    }
    if(!m_simulate)
        return;
    m_tempPoints.push_back(m_ketState->waterTemperature());
    m_tempSteamPoints.push_back(m_ketState->steamTempearture());
    update();
}

#include <cmath>

void KettleStatusDisplay::paintEvent(QPaintEvent */*event*/)
{
    QPainter painter(this);
//    painter.drawLine(0,0,width(),height());
//    painter.drawLine(width(),0,0,height());
    const auto bottomEnd = height()-botoomMargin;

    const auto kvert = (bottomEnd-topMargin)/(tmax - tmin);
    auto map_y = [bottomEnd, tempMin = tmin, kvert](double iy){return bottomEnd-(iy-tempMin)*kvert;};
    ///Нарисуем горизонтальные линии шкалы
    for(auto i = int(std::floor(tmin/100.)); i<=tmax; i+=5)
    {
        if(i%10)
            painter.setPen(QPen(QColor(128,128,128)));
        else
            painter.setPen(QPen(QColor(0,0,0)));
        auto y = map_y(i);
        painter.drawLine(0, y, width(), y);
        painter.drawText(5,y-1,QString::number(i));
    }
    {
        auto pen = QPen(QColor(255,0,0));
        drawGraph(m_tempPoints, pen, painter, map_y);
    }
    {
        auto pen = QPen(QColor(0,255,0));
        drawGraph(m_tempSteamPoints, pen, painter, map_y);
    }

    if(m_ketState->kettleOnStand())
        painter.setPen(QPen(QColor(0,200,0)));
    else
        painter.setPen(QPen(QColor(200,0,0)));
    painter.drawText(1+100, 15, tr("На стойке %1").arg(m_ketState->kettleOnStand() ? "V" : "X" ));

    if(m_ketState->heatingSwitch())
        painter.setPen(QPen(QColor(0,200,0)));
    else
        painter.setPen(QPen(QColor(200,0,0)));
    painter.drawText(1+200, 15, tr("Нагрев %1").arg(m_ketState->heatingSwitch() ? "V" : "X" ));

    painter.setPen(QPen(QColor(0,0,0)));
    ///Нарисуем текущие показания "приборов" вверху
    painter.drawText(1    , 15, tr("Время %1").arg(QTime(0,0).addMSecs(m_ketState->time_ms()).toString("hh:mm:ss")));
    painter.drawText(1+300, 15, tr("Объём %1").arg(QString::number(m_ketState->waterVolume() , 'f', 2)) );


    ///Нарисуем текущие показания "приборов" внизу
    int x = 1;
    painter.drawText(x    , height()-1, tr("t воды %1").arg(QString::number(m_ketState->waterTemperature() , 'f', 2)) );
    painter.drawText(x+=90 , height()-1, tr("t нагр %1").arg(QString::number(m_ketState->heaterTempearture(), 'f', 2)) );
    painter.drawText(x+=90, height()-1, tr("t пара %1").arg(QString::number(m_ketState->steamTempearture() , 'f', 2)) );
    painter.drawText(x+=90, height()-1, tr("t возд %1").arg(QString::number(m_ketState->ambAirTempearture(), 'f', 2)) );
    painter.drawText(x+=90, height()-1, tr("t цели %1").arg(QString::number(m_ketState->targetTempearture(), 'f', 2)) );
}

void KettleStatusDisplay::drawGraph(std::vector<double> graph, QPen pen,
                                    QPainter &painter,
                                    std::function<double(double)> mapY)
{
    if (graph.size() < 2)
        return;
    pen.setWidth(3);
    pen.setJoinStyle(
        Qt::RoundJoin); /// TODO порабоать с формой графика при снижении!!!
    painter.setPen(pen);
    const int shift = std::max(int(graph.size() - width()), 0);
    for (int i = 0; i < width() - 1; ++i) {
        if (i + shift + 1 >= (int)graph.size())
            break;
        painter.drawLine(i, mapY(graph[i + shift]), i + 1,
                         mapY(graph[i + shift + 1]));
    }
}

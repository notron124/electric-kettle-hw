#include "kettlestate.h"
#include <QDateTime>
#include <QRandomGenerator>


KettleState::KettleState(QObject *parent):
    QObject(parent)
{
    ///считаем, что последнее состояние получено сейчас
    m_lastState.m_timeStampMs = 0;
}

void KettleState::startHeating()///начать нагрев
{
    if(m_heating)
        return;
    m_heating = true;
    emit heatingChanged(m_heating);
}

void KettleState::stopHeating()///остановить нагрев
{
    if(!m_heating)
        return;
    m_heating = false;
    emit heatingChanged(m_heating);
}

double KettleState::waterTemperature() const///температура воды в чайнике на данный момент
{
    if(m_kettleOnStand)
        return m_lastState.m_waterTemp;
    else
        return -100+QRandomGenerator::global()->bounded(16384)/1000;
}

double KettleState::steamTempearture() const///температура датчика пара
{
    if(m_kettleOnStand)
        return m_lastState.m_lastSteamTemp;
    else
        return -30+QRandomGenerator::global()->bounded(16384)/1000;
}

double KettleState::heaterTempearture() const///температура нагревателя чайника
{
    if(m_kettleOnStand)
        return m_lastState.m_lastHeaterTemp;
    else
        return 150+QRandomGenerator::global()->bounded(16384)/1000;
}

double KettleState::ambAirTempearture() const
{
    return m_lastState.m_airAmbTemp;
}

double KettleState::targetTempearture() const///температура уставки (до которой пользователь хочет греть воду)
{
    return m_targetTemp;
}

double KettleState::waterVolume()
{
    return m_lastState.m_volume_L;
}

///Генерация случайных событий
void KettleState::RandomOperation(const qint64 iTimeStep)
{
    static int lastOper = -1;
    static int timeout = 0;
    if(lastOper!=-1)
    {
        timeout-=iTimeStep;
        if(timeout<=0)
        {
            lastOper = -1;
            timeout = 0;
        }
        else
            return;
    }
    switch(lastOper = QRandomGenerator::global()->bounded(1000))
    {
    case 0:///попытка снять со стойки
    {
        if(!m_kettleOnStand)// он и так снят -- просто пропустим попытку
            break;
        if(m_lastState.m_waterTemp<0.97*m_targetTemp)///Чего его холодным снимать?
        {
            timeout = 60000;///добавим ка таймаута
            break;
        }
        m_kettleOnStand = false;
        m_lastState.m_volume_L = QRandomGenerator::global()->bounded(150)/100.;
        m_lastState.m_waterTemp*=QRandomGenerator::global()->bounded(70)/100.;
        timeout = 10000 + QRandomGenerator::global()->bounded(5000);///таймаут 10-15 секунд
        break;
    }
    case 1:
    {
        if(m_kettleOnStand)
            break;
        m_kettleOnStand = true;
        m_heatingSwitch = true;
        timeout = 60000 + QRandomGenerator::global()->bounded(120000);///таймаут 1-3 минуты
        break;
    }
    case 2:
    case 3:
    {
        m_lastState.m_airAmbTemp += QRandomGenerator::global()->bounded(100)/1000 ;
        timeout = 500;
        break;
    }
    default:
        lastOper = -1;

    }
}


#include <map>
double waterBoilingTemp(int iAtmPress)
{
    std::map<int, double> btemp = { \
         {684,97.08}, {685,97.12}, {686,97.16}, {687,97.20}, {688,97.24}, {689,97.28}, {690,97.32}, {691,97.36},  \
        {692,97.40}, {693,97.44}, {694,97.48}, {695,97.52}, {696,97.56}, {697,97.60}, {698,97.63}, {699,97.67},  \
        {700,97.71}, {701,97.75}, {702,97.79}, {703,97.83}, {704,97.87}, {705,97.91}, {706,97.95}, {707,97.99},  \
        {708,98.03}, {709,98.07}, {710,98.11}, {711,98.14}, {712,98.18}, {713,98.22}, {714,98.26}, {715,98.30},  \
        {716,98.34}, {717,98.38}, {718,98.42}, {719,98.45}, {720,98.49}, {721,98.53}, {722,98.57}, {723,98.61},  \
        {724,98.65}, {725,98.69}, {726,98.72}, {727,98.76}, {728,98.80}, {729,98.84}, {730,98.88}, {731,98.91},  \
        {732,98.95}, {733,98.99}, {734,99.03}, {735,99.07}, {736,99.10}, {737,99.14}, {738,99.18}, {739,99.22},  \
        {740,99.26}, {741,99.29}, {742,99.33}, {743,99.37}, {744,99.41}, {745,99.44}, {746,99.49}, {747,99.52},  \
        {748,99.56}, {749,99.59}, {750,99.63}, {751,99.67}, {752,99.70}, {753,99.74}, {754,99.78}, {755,99.82},  \
        {756,99.85}, {757,99.89}, {758,99.93}, {759,99.96}, {760,100.00}, {761,100.04}, {762,100.07}, {763,100.11},  \
        {764,100.15}, {765,100.18}, {766,100.22}, {767,100.26}, {768,100.29}, {769,100.33}, {770,100.37}, {771,100.40},  \
        {772,100.44}, {773,100.48}, {774,100.51}, {775,100.55}, {776,100.58}, {777,100.62}, {778,100.66}, {779,100.69},  \
        {780,100.73}, {781,100.76}, {782,100.80}, {783,100.84}, {784,100.87}, {785,100.91}, {786,100.94}, {787,100.98},  \
        {788,101.02}, {789,101.05}, {790,101.09}, {791,101.12}, {792,101.16}, {793,101.19}, {794,101.23}, {795,101.26},  \
        {796,101.30}, {797,101.33}, {798,101.35}, {799,101.41}, {800,101.44} };
    return btemp[iAtmPress];
}

void KettleState::calcState(const qint64 iTimeStep, bool iRandomEvents)///рассчитать текущее состояние чайника
{
    Q_ASSERT(iTimeStep>=0);
    if(iRandomEvents)
        RandomOperation(iTimeStep);
    auto waterBoilTemp = waterBoilingTemp(m_lastState.m_atmPressure);
    LastState ns(m_lastState);
    constexpr double time_sec = 0.001;///время расчёта
    if(m_kettleOnStand && m_heating && m_heatingSwitch)
    {///С последнего момента происходит цикл нагрева
        for(auto i = iTimeStep; i!=0; i--)
        {///считаем функцию каждую миллисекунду (может это излишне)
            /// Считаем температуру нагревателя
            auto energy_pass_to_heater = m_heatingPowerWt*time_sec;///Энергия в нагреватель
            auto energy_pass_from_heater = (ns.m_lastHeaterTemp-ns.m_waterTemp)*m_heaterToWaterK;///Энергия из нагревателя в воду
            ns.m_lastHeaterTemp += energy_pass_to_heater/m_heaterCapacity///нагревание нагревателя
                    - energy_pass_from_heater/m_heaterCapacity;///охлаждение нагревателя в воду
            ns.m_waterTemp = ns.m_waterTemp
                    - (ns.m_waterTemp-ns.m_airAmbTemp)*m_coolK*time_sec ///охлаждение
                    + energy_pass_from_heater/*sec*//m_waterHeatCap/ns.m_volume_L;///нагревание
            if(ns.m_waterTemp>waterBoilTemp)
            {
                ns.m_waterTemp = waterBoilTemp;///Защита от перегрева
                ns.m_lastSteamTemp = ns.m_lastSteamTemp + (ns.m_waterTemp-ns.m_lastSteamTemp)*m_steamHeatK;
            }
        }
    }
    else
    {
        for(auto i = iTimeStep; i!=0; i--)
        {///считаем функцию каждую миллисекунду (может это излишне)
            if(ns.m_lastHeaterTemp > ns.m_waterTemp)///нагреватель всё ещё нагревает воду
            {
                auto energy_pass = (ns.m_lastHeaterTemp-ns.m_waterTemp)*time_sec*m_heaterToWaterK;
                ns.m_lastHeaterTemp-=energy_pass/m_heaterCapacity;
                ns.m_waterTemp+=energy_pass/m_waterHeatCap/ns.m_volume_L ///остаточное нагревание
                        - (ns.m_waterTemp-ns.m_airAmbTemp)*m_coolK/1000.;///только охлаждение утечкой
            }
            else
            {///Теперь вода охлаждает нагреватель
                auto energy_pass = (ns.m_waterTemp-ns.m_lastHeaterTemp)*time_sec*m_heaterToWaterK;
                ns.m_lastHeaterTemp-=energy_pass/m_heaterCapacity;
                ns.m_waterTemp-=energy_pass/m_waterHeatCap/ns.m_volume_L ///охлаждение в нагреватель
                        + (ns.m_waterTemp-ns.m_airAmbTemp)*m_coolK/1000.;///только охлаждение
            }
        }
    }
    if(ns.m_waterTemp<waterBoilTemp)
        ns.m_lastSteamTemp = ns.m_lastSteamTemp + (ns.m_airAmbTemp-ns.m_lastSteamTemp)*m_steamCoolK;
    ns.m_timeStampMs = m_lastState.m_timeStampMs+iTimeStep;
    m_lastState = ns;
}

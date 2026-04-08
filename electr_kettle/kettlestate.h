#ifndef KETTLESTATE_H
#define KETTLESTATE_H

#include <QObject>

constexpr int mCalcPeriod_ms = 20;///период расчёта состояния

///Класс состояния чайника
class KettleState : public QObject
{
    Q_OBJECT
public:
    KettleState(QObject* parent = nullptr);
    void startHeating();///начать нагрев
    void stopHeating();///остановить нагрев
    bool heating() const {return m_heating;}
    double waterTemperature() const;///температура воды в чайнике на данный момент
    double steamTempearture() const;///температура датчика пара
    double heaterTempearture() const;///температура нагревателя чайника
    double ambAirTempearture() const;///температура окружающего воздуха
    double targetTempearture() const;///температура уставки (до которой пользователь хочет греть воду)
    bool coverOpen() const {return m_coverOpen;}///состояние крышки чайника (открыта/закрыта)
    bool heatingSwitch() const {return m_heatingSwitch;};///состояние кнопки "нагрев" (включен/выключен)
    bool kettleOnStand() const {return m_kettleOnStand;}///состояние чайника (на базе/снят с базы)
    double waterVolume();///объём воды, Л

    quint64 time_ms(){return m_lastState.m_timeStampMs;}

    void calcState(const qint64 iTimeStep, bool iRandomEvents = false);///рассчитать текущее состояние чайника со сдвиго в iTime мСек

signals:
    void heatingChanged(bool heating);

private:
    void RandomOperation(const qint64 iTimeStep);

    struct LastState {
        double m_airAmbTemp = 20;///температура окружающего воздуха
        double m_waterTemp = 20;///последняя зафиксированная температура (при смене цикла нагрева/охлаждения)
        double m_lastSteamTemp = 20;///последняя зафиксированная температура датчика пара
        double m_lastHeaterTemp = 20;///последняя зафиксированная температура нагревателя
        double m_atmPressure = 736;///атмосферное давление
        double m_volume_L = 1.0;///объём воды, л
        quint64 m_timeStampMs = 0;///MSecsSinceEpoch
    };
    LastState m_lastState;
    bool m_heating = false;///состояние нагревателя (вкл/выкл)
    static constexpr int m_heatingPowerWt = 1000;///Мощность нагревателя Вт = Дж/сек
    static constexpr double m_heaterCapacity = 30;///Теплоёмкость нагревателя Дж/°C
    static constexpr double m_heaterToWaterK= 0.4;///Коэффициент передачи тепла от нагревателя к воде, кВт/°C (Дж/мсек/°C)
    static constexpr double m_coolK = 0.0003;/// Коэфф охлаждения. кВт/°C
    static constexpr double m_steamHeatK = 0.01;/// Коэфф нагревания датчика пара. кВт/°C
    static constexpr double m_steamCoolK = 0.001;/// Коэфф охлаждения датчика пара. кВт/°C
    static constexpr double m_waterHeatCap = (4183.+4220.)/2.;///теплоёмкость воды. Берём среднее между 20C и 100C J/(kg*deg)
    double m_targetTemp = 100;///температура уставки (до которой пользователь хочет греть воду)
    bool m_coverOpen = false;
    bool m_heatingSwitch = true;
    bool m_kettleOnStand = true;

    qint64 m_currentTime;///время в мСек с начала симуляции
};

#endif // KETTLESTATE_H

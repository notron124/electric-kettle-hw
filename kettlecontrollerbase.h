#ifndef KETTLECONTROLLER_H
#define KETTLECONTROLLER_H

#include<functional>

#include "kettlestate.h"

class KettleControllerBase
{
public:
    KettleControllerBase(const KettleState &state, std::function<void(void)> hOn, std::function<void(void)>hOff);

    virtual~ KettleControllerBase() = default;

    ///Функция управления вызывается с частотой 50 Гц (раз в 20 мсек)
    virtual void controllerProcess() = 0;

    ///Период вызова функции
    unsigned int callPeriod_ms() { return 20; }///TODO magic value! Надо бы это поправить!

    /// ПОКАЗАНИЯ ДАТЧИКОВ ЧАЙНИКА

    /// Показания датчика температуры воды
    double waterTemperature() const {return m_state.waterTemperature();}

    /// Показания датчика температуры пара
    double steamTempearture() const {return m_state.steamTempearture();}

    /// Показания датчика температуры нагревателя чайника
    double heaterTempearture() const {return m_state.heaterTempearture();}

    /// Показания датчика температуры окружающего воздуха
    double ambAirTempearture() const {return m_state.ambAirTempearture();}

    /// Целевая температура (уставка, до которой пользователь хочет греть воду)
    double targetTempearture() const {return m_state.targetTempearture();}

    /// Cостояние крышки чайника (открыта/закрыта)
    bool coverOpen() const {return m_state.coverOpen();}

    /// Состояние кнопки "нагрев" (включен/выключен)
    bool heatingSwitch() const {return m_state.heatingSwitch();}

    /// Cостояние чайника (на базе/снят с базы)
    bool kettleOnStand() const {return m_state.kettleOnStand();}

    /// Включение нагревателя
    void heaterOn() const {m_heaterOn();}

    /// Выключение нагревателя
    void heaterOff() const {m_heaterOff();}

private:
    const KettleState& m_state;
    std::function<void(void)>
        m_heaterOn, ///функция включения нагревателя
        m_heaterOff;///функция отключения нагревателя
};

#endif // KETTLECONTROLLER_H

#include "kettlecontrollerimpl.h"
#include <QtLogging>

namespace {
constexpr float differenceTheshold = 0.5;
constexpr float whaterIsColdThershold = 0.5;
}

void KettleControllerImpl::controllerProcess()
{
    switch (kettleMode) {
    case KM_IDLE:
        if (isConditionsSafe()) {
            kettleMode = KM_HEATING;
            heaterOn();
            qInfo("Idle -> Heating");
        }
        break;

    case KM_HEATING:
        if (!isConditionsSafe() || !heatingSwitch()) {
            kettleMode = KM_IDLE;
            heaterOff();
            qInfo("Heating -> Idle");
            return;
        }

        if (!isWaterBoiling() && !isTargetReached()) {
            return;
        }

        kettleMode = KM_COOLING;

        heaterOff();

        qInfo("Heating -> Cooling");
        break;

    case KM_COOLING:
        if (steamTempearture() - ambAirTempearture() <= whaterIsColdThershold) {
            kettleMode = KM_HEATING;
            heaterOn();
            qInfo("Cooling -> Heating");
        }
        break;

    default:
        break;
    }
}

bool KettleControllerImpl::isWaterBoiling() const {
    return ((waterTemperature() - steamTempearture()) <= differenceTheshold) &&
           (steamTempearture() - ambAirTempearture() >= differenceTheshold);
}

bool KettleControllerImpl::isTargetReached() const {
    return (targetTempearture() - waterTemperature() <= differenceTheshold);
}

bool KettleControllerImpl::isConditionsSafe() const {
    return (kettleOnStand() && !coverOpen());
}

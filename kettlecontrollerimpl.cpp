#include "kettlecontrollerimpl.h"
#include <QtLogging>

void KettleControllerImpl::controllerProcess()
{
    switch (kettleMode) {
    case KM_IDLE:
        if (isConditionsSafe()) {
            kettleMode = KM_HEATING;
            heaterOn();
        }
        break;

    case KM_HEATING:
        if (!isConditionsSafe() || !heatingSwitch()) {
            kettleMode = KM_IDLE;
            heaterOff();
        }

        if (!isWaterBoiling() && !isTargetReached()) {
            return;
        }

        kettleMode = KM_COOLING;

        heaterOff();
        endOfHeatingTemperature = waterTemperature();
        break;

    case KM_COOLING:
        if (steamTempearture() - ambAirTempearture() <= 0.5) {
            kettleMode = KM_HEATING;
            heaterOn();
        }
        break;
    }
}

bool KettleControllerImpl::isWaterBoiling() const {
    return (waterTemperature() - steamTempearture()) <= 0.1 &&
           waterTemperature() > minWaterBoilingTemperature;
}

bool KettleControllerImpl::isTargetReached() const {
    return (targetTempearture() - waterTemperature() <= 0.1);
}

bool KettleControllerImpl::isConditionsSafe() const {
    return (kettleOnStand() && !coverOpen());
}

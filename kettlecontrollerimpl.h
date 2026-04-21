#ifndef KETTLECONTROLLERIMPL_H
#define KETTLECONTROLLERIMPL_H

#include "kettlecontrollerbase.h"

class KettleControllerImpl : public KettleControllerBase
{
public:
    enum KettleModes {
        KM_IDLE = 0,
        KM_HEATING,
        KM_COOLING,
    };

    KettleControllerImpl(const KettleState &state, std::function<void(void)> hOn, std::function<void(void)>hOff) :
        KettleControllerBase(state, hOn, hOff) {};

    void controllerProcess() final;

private:
    KettleModes kettleMode = KM_IDLE;
    bool isWaterBoiling() const;
    bool isTargetReached() const;
    bool isConditionsSafe() const;
};

#endif // KETTLECONTROLLERIMPL_H

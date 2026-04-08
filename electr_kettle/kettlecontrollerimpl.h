#ifndef KETTLECONTROLLERIMPL_H
#define KETTLECONTROLLERIMPL_H

#include "kettlecontrollerbase.h"

class KettleControllerImpl : public KettleControllerBase
{
public:
    KettleControllerImpl(const KettleState &state, std::function<void(void)> hOn, std::function<void(void)>hOff) :
        KettleControllerBase(state, hOn, hOff) {};

    void controllerProcess() final;
};

#endif // KETTLECONTROLLERIMPL_H

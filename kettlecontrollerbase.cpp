#include "kettlecontrollerbase.h"

KettleControllerBase::KettleControllerBase(const KettleState &state, std::function<void(void)> hOn, std::function<void(void)>hOff):
    m_state(state),
    m_heaterOn(hOn),
    m_heaterOff(hOff)
{

}

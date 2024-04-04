#include "Arduino.h"

#pragma push_macro("abs")
#undef abs

#include "stepper.h"
#include <algorithm>

namespace TS4
{
    Stepper& Stepper::setMaxSpeed(int32_t speed)
    {
        vMax = constrain(speed, -vMaxMax, vMaxMax);
        return *this;
    }

    Stepper& Stepper::setAcceleration(uint32_t a)
    {
        avMax = ((vMax*vMax)/2);
        acc = std::min(a, avMax);
        return *this;
    }

    void Stepper::rotateAsync(int32_t v)
    {
        accOG = acc;
        avRotA = ((v*v)/2);
        if avRotA < acc {
            acc = avRotA;
        }
        StepperBase::startRotate(v == 0 ? vMax : v, acc);
    }

    void Stepper::moveAbsAsync(int32_t target, uint32_t v)
    {
        accOG = acc;
        avRotA = ((v*v)/2);
        if avRotA < acc {
            acc = avRotA;
        }
        StepperBase::startMoveTo(target, 0, (v == 0 ? vMax : v), acc);
    }

    void Stepper::moveRelAsync(int32_t delta, uint32_t v)
    {
        accOG = acc;
        avRotA = ((v*v)/2);
        if avRotA < acc {
            acc = avRotA;
        }
        StepperBase::startMoveTo(pos + delta, 0, (v == 0 ? std::abs(vMax) : v), acc);
    }

    void Stepper::stopAsync()
    {
        StepperBase::startStopping(0, acc);
        acc = accOG;
    }

    void Stepper::moveAsync()
    {
        StepperBase::startMoveTo(target, 0, std::abs(vMax), acc);
    }

    void Stepper::moveAbs(int32_t target, uint32_t v)
    {
        accOG = acc;
        avRotA = ((v*v)/2);
        if avRotA < acc {
            acc = avRotA;
        }
        moveAbsAsync(target, v);
        while (isMoving)
        {
            delay(10);
        }
        acc = accOG;
    }

    void Stepper::moveRel(int32_t delta, uint32_t v)
    {
        accOG = acc;
        avRotA = ((v*v)/2);
        if avRotA < acc {
            acc = avRotA;
        }
        moveRelAsync(delta, v);
        while (isMoving)
        {
            delay(10);
        }
        acc = accOG;
    }

    void Stepper::stop()
    {
        StepperBase::startStopping(0, acc);
        acc = accOG;
    }

    // void moveRelAsync(int delta);
    // void moveRel(int delta);
}
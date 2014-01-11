#include "Timer.h"
#include <iostream>

Timer* Timer::mInstance = NULL;

Timer::Timer() : mElapsed(0.0){
}


Timer::~Timer() {
}
Timer& Timer::getInstance() {
    if ( mInstance == NULL ) {
        mInstance = new Timer();
    }
    return *mInstance;
}

double Timer::getElapsed() {
    return mElapsed;
}

void Timer::update(const double pDelta) {
    mElapsed += pDelta;
}
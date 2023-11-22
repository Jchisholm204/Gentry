#include "arm.hpp"
#include <Arduino.h>

fsr::fsr(int port, double limit){
    dport = port;
    dlim = limit;
    pinMode(dport, INPUT);
}

double fsr::getReading(){
    return analogRead(dport);
}

bool fsr::getState(){
    return (analogRead(dport) > dlim);
}

void fsr::setLimit(double newLimit){
    dlim = newLimit;
}
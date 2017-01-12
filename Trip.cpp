#include "Trip.h"

Trip::Trip(int id, Point* s, Point* e, int np, double t, int st) {
    rideId = id;
    meters = 0;
    start = s;
    end = e;
    numPassengers = np;
    tariff = t;
    startTime = st;
}
Trip::Trip() {
    rideId = 0;
    meters = 0;
    start = NULL;
    end = NULL;
    numPassengers = 0;
    tariff = 0;
    startTime = 0;
}
Trip::~Trip() {
    delete start;
    delete end;
}

Point* Trip::getStart() {
    return start;
}

void Trip::setStart(Point* s1) {
    start = s1;
}

void Trip::setEnd(Point* e1) {
    end = e1;
}

Point* Trip::getEnd() {
    return end;
}

int Trip::getMeters() {
    return meters;
}

void Trip::setMeters() {
    meters++;
}

int Trip::getNumPassengers() {
    return numPassengers;
}

void Trip::setNumPassengers(int np) {
    numPassengers = np;
}

double Trip::getTariff() {
    return tariff;
}

void Trip::setTariff(double t) {
    tariff = t;
}

int Trip::getStartTime() {
    return startTime;
}
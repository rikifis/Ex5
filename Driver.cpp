#include "Driver.h"

Driver::Driver(int id1, int age1, MaritalStatus status1, int cabId1, int experience1) {
    id = id1;
    age = age1;
    status = status1;
    cabId = cabId1;
    experience = experience1;
    map = NULL;
    avSatisfaction = 0;
    customers = 0;
    driving = false;
    firstLocation = new GridPt(Point(0,0));
    location = NULL;
    route = NULL;
    newTrip = false;
    trip = NULL;
}
Driver::Driver() {

}
Driver::~Driver() {
    delete firstLocation;
    delete route;
}
int Driver::getId() {
    return id;
}
void Driver::setAge(int a) {
    age = a;
}
int Driver::getAge() {
    return age;
}
void Driver::setStatus(MaritalStatus st) {
    status = st;
}
MaritalStatus Driver::getStatus() {
    return status;
}
void Driver::setExperience(int exp) {
    experience = exp;
}
int Driver::getExperience() {
    return experience;
}
void Driver::setSatisfaction(int score) {
    avSatisfaction = ((avSatisfaction * (getNumCustomers() - 1)+ score) / getNumCustomers());
}
double Driver::getSatisfaction() {
    return avSatisfaction;
}
int Driver::getCabId() {
    return cabId;
}
void Driver::setCab(Taxi* t) {
    cab = t;
}
Taxi* Driver::getCab() {
    return cab;
}
void Driver::addCustomers(int c) {
    customers += c;
}
int Driver::getNumCustomers() {
    return customers;
}
void Driver::setMap(Map* m) {
    map = m;
    location = map->getPoint(((GridPt(Point(0,0))).getPt()));
}
GridPt* Driver::getLocation() {
    if (location == NULL) {
        return firstLocation;
    }
    return location;
}
void Driver::setLocation(GridPt* loc) {
    location = loc;
}
void Driver::setTrip(Trip* trip1) {
    trip = trip1;
}
Trip* Driver::getTrip() {
    return trip;
}
void Driver::calcRoute(Node* start, Node* end) {
    if (route != NULL) {
        delete route;
    }
    Bfs b = Bfs();
    map->initialize();
    start->setPassed();
    route = b.bfs(start, end);
}
deque<Node*>* Driver::getRoute() {
    return route;
}
void Driver::setRoute(deque<Node*>* route1) {
    route = route1;
}

void Driver::drive() {
    int i = 0;
    // regular taxi goes one block and luxury two blocks
    while (i < cab->getType()) {
        if (!route->empty()) {
            location = map->getPoint(((GridPt*) (route->front()))->getPt());
            route->pop_front();
            cab->setKm(0.001);
        }
        i++;
    }
    if (route->empty()) {
        // stop driving.
        setDriving();
    }
}
bool Driver::isDriving() {
    return driving;
}
void Driver::setDriving() {
    driving = !driving;
}
bool Driver::gotNewTrip() {
    return newTrip;
}
void Driver::setNewTrip() {
    newTrip = !newTrip;
}

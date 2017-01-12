//
// Created by riki on 11/01/17.
//

#include "DriverDescriptor.h"

DriverDescriptor::DriverDescriptor(Driver* driver1, int socketDescriptor1) {
    driver = driver1;
    socketDescriptor = socketDescriptor1;
}
DriverDescriptor::DriverDescriptor() {

}
void DriverDescriptor::setDescriptor(int sd) {
    socketDescriptor = sd;
}
int DriverDescriptor::getDescriptor() {
    return socketDescriptor;
}
Driver* DriverDescriptor::getDriver() {
    return driver;
}
//
// Created by riki on 11/01/17.
//

#ifndef EX5_DRIVERDESCRIPTOR_H
#define EX5_DRIVERDESCRIPTOR_H

#include "Driver.h"

class DriverDescriptor {
    private:
        Driver* driver;
        int socketDescriptor;

        friend class boost::serialization::access;
        template<class Archive>
        /**
         * seralize a driver object.
         * @param ar
         * @param version
         */
        void serialize(Archive &ar, const unsigned int version) {
            ar & driver;
            ar & socketDescriptor;
        }
    public:
        DriverDescriptor(Driver* driver1, int socketDescriptor1);
        DriverDescriptor();
        void setDescriptor(int sd);
        int getDescriptor();
        Driver* getDriver();
};


#endif //EX5_DRIVERDESCRIPTOR_H

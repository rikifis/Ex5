#include "TaxiFlow.h"

BOOST_CLASS_EXPORT_GUID(Node, "Node");
BOOST_CLASS_EXPORT_GUID(GridPt, "GridPt");
BOOST_CLASS_EXPORT_GUID(Taxi, "Taxi");
BOOST_CLASS_EXPORT_GUID(Luxury, "Luxury");


int main(int argc, char *argv[]) {
    // checks we got a correct number of args.
    if (argc < 2) {
        return 0;
    }
    // initializes the servers socket.
    Socket* socket = new Tcp(1, atoi(argv[1]));
    TaxiFlow* flow = new TaxiFlow(socket);
    // gets the input from the user and runs the taxi center.
    flow->getInput();
    delete  flow;
    return 0;
}

TaxiFlow::TaxiFlow(Socket* socket1) {
    socket = socket1;
    socket->initialize();
    clients = new vector<DriverDescriptor*>;
    pthread_mutex_init(&acceptMutex, 0);
    pthread_mutex_init(&addMutex, 0);
}

TaxiFlow::~TaxiFlow() {
    delete socket;
    // deletes the clients.
    while (clients->size() != 0) {
        delete clients->front();
        clients->erase(clients->begin());
    }
    delete clients;
    pthread_mutex_destroy(&acceptMutex);
    pthread_mutex_destroy(&addMutex);

}

void TaxiFlow::getInput() {
    int x, y, numOfObs;
    // will read the ','.
    char comma;
    Point mapSize;
    // reads the map size.
    cin >> skipws >> x >> y;
    mapSize = Point(x,y);
    Map* map = new Map(mapSize);
    // reads the number of obstacles.
    cin >> skipws >> numOfObs;
    // reads the obstacle.
    for (int i = 0; i < numOfObs; i++) {
        cin >> x >> comma >> y;
        GridPt obs = GridPt(Point(x,y));
        //obstacles.push_back(&obs);
        map->addObstacle(&obs);
    }
    // creates the taxi center with the given map.
    center = TaxiCenter(map);
    // tuns the commands.
    run();
    delete map;
}

void TaxiFlow::run() {
    // the command from the user.
    int command;
    do {
        // reads the command.
        cin >> command;
        switch (command) {
            // for '1' -adds drivers.
            case 1:
                addDrivers();
                break;
            // for '2' -adds a trip.
            case 2:
                addTrip();
                break;
            // for '3' -adds a cab.
            case 3:
                addCab();
                break;
            // for '4' - gets a drivers location.
            case 4:
                getDriverLocation();
                break;
            case 7:
                closeClients();
                break;
            // for '9' - drives the cars.
            case 9:
                drive();
                break;
            default:
                break;
        }
    // for '7' - end program.
    } while (command != 7);
}

void TaxiFlow::addDrivers() {
    int numDrivers;
    // gets the number of drivers from the user.
    cin >> numDrivers;
    vector<pthread_t> threadsVec(numDrivers);
    cout << "receiving " << numDrivers << " drivers:" << endl;
    for (int i = 0; i < numDrivers; i++) {
        cout << i << " ";
        pthread_create(&threadsVec[i], NULL, getClientsWrapper, (void*)this);
    }
    for (int i = 0; i < numDrivers; i++) {
         //void* driverDes;
         pthread_join (threadsVec[i], NULL);//&
         cout << "join " << i << " done" << endl;
         //DriverDescriptor* ds = (DriverDescriptor*)driverDes;
    }
}

void TaxiFlow::addTrip() {
    int id, xStart, yStart, xEnd, yEnd, numPassengers, startTime;
    double tariff;
    // skips the punctuation marks.
    char skip;
    // gets the trip's details from the user.
    cin >> id >> skip >> xStart >> skip >> yStart >> skip >> xEnd
        >> skip >> yEnd >> skip >> numPassengers >> skip >> tariff >> skip >> startTime;
    // gets the starting point from the map.
    GridPt* start = center.getMap()->getPoint(Point(xStart,yStart));//center.getMap()->getPoint(Point(xStart,yStart));
    // gets the ending point from the map.
    GridPt* end = center.getMap()->getPoint(Point(xEnd,yEnd));//center.getMap()->getPoint(Point(xEnd,yEnd));
    // creates the new trip.
    Trip* trip = new Trip(id, start, end, numPassengers, tariff, startTime);
    int vecSize = calcRouteThreads.size();
    calcRouteThreads.resize(vecSize + 1);

    center.getMap()->initialize(); ///?????????????????????

    pthread_create(&calcRouteThreads[vecSize], NULL, trip->calcRoute, (void*)trip);
    // adds the trip to the center.
    center.addTrip(trip);

    pthread_join (calcRouteThreads[vecSize], NULL);//////////////////////not here

    cout << "trip of time " << trip->getStartTime()
         << " passed join. start is " << *((GridPt*)trip->getRoute()->at(0))
         << "end is " << *((GridPt*)trip->getRoute()->at(trip->getRoute()->size() - 1)) << endl;
}

void TaxiFlow::addCab() {
    int id, type;
    char manufacturerSign, colorSign;
    // skips the punctuation marks.
    char skip;
    MANUFACTURER manufacturer;
    COLOR color;
    Taxi* taxi;
    // gets the cab's details from the user.
    cin >> id >> skip >> type >> skip >> manufacturerSign >> skip >> colorSign;
    // assigns the cars manufacturer.
    switch (manufacturerSign) {
        case 'H':
            manufacturer = HONDA;
            break;
        case 'S':
            manufacturer = SUBARO;
            break;
        case 'T':
            manufacturer = TESLA;
            break;
        case 'F':
            manufacturer = FIAT;
            break;
        default:
            break;
    }
    // assigns the cars color.
    switch (colorSign) {
        case 'R':
            color = RED;
            break;
        case 'B':
            color = BLUE;
            break;
        case 'G':
            color = GREEN;
            break;
        case 'P':
            color = PINK;
            break;
        case 'W':
            color = WHITE;
            break;
        default:
            break;
    }
    // assigns the taxis type.
    if (type == 1) {
        taxi = new Taxi(id, manufacturer, color);
    } else if (type == 2) {
        taxi = new Luxury(id, manufacturer, color);
    } else {
        return;
    }
    // adds the taxi.
    center.addTaxi(taxi);
}

void TaxiFlow::getDriverLocation() {
    int id;
    // gets the drivers id.
    cin >> id;
    // gets the drivers from the center.
    vector<Driver*> drivers = center.getDrivers();
    // looks for the driver.
    for (int i = 0; i < drivers.size(); i++) {
        if (id == drivers.at(i)->getId()) {
            // prints the location of the given driver.
            cout << *((GridPt*)(drivers.at(i)->getLocation())) << endl;
            break;
        }
    }
}

void TaxiFlow::drive() {
    // increases the time.
    center.setTime();
    // sends the drivers to drive.
    for (int i = 0; i < center.getDrivers().size(); i++) {
        if (center.getDrivers().at(i)->isDriving()) {
            // tells the client to be prepared to drive.
            socket->sendData("go", socket->getDescriptorCommunicateClient());
            // drives the car.
            center.getDrivers().at(i)->drive();
            // sends the new location to client.
            std::string serial_str1;
            boost::iostreams::back_insert_device<std::string> inserter1(serial_str1);
            boost::iostreams::stream
                    <boost::iostreams::back_insert_device<std::string> > s1(inserter1);
            boost::archive::binary_oarchive oa1(s1);
            GridPt* newLocation = new GridPt(center.getDrivers().at(i)->getLocation()->getPt());
            // serilizes the new location.
            oa1 << newLocation;
            // flush the stream to finish writing into the buffer
            s1.flush();
            // sends the data.
            socket->sendData(serial_str1, socket->getDescriptorCommunicateClient());
            delete newLocation;
        }
    }
    // assign trips to the drivers who are ready for them.
    center.sendTaxi();
    for (int i = 0; i < center.getDrivers().size(); i++) {
        // if the driver just got a new trip.
        if (center.getDrivers().at(i)->gotNewTrip()) {
            // tells the client to be prepared to get a new trip.
            socket->sendData("trip", socket->getDescriptorCommunicateClient());
            // sends the trip.
            std::string serial_str;
            boost::iostreams::back_insert_device<std::string> inserter(serial_str);
            boost::iostreams::stream
                    <boost::iostreams::back_insert_device<std::string> > s(inserter);
            boost::archive::binary_oarchive oa(s);
            Trip* newTrip = center.getDrivers().at(i)->getTrip();
            // serilizes the trip.
            oa << newTrip;
            // flush the stream to finish writing into the buffer.
            s.flush();
            // sentd the new trip.
            socket->sendData(serial_str, socket->getDescriptorCommunicateClient());
            // deletes the trip from the taxiCenter.
            delete (center.getDrivers().at(i)->getTrip());
            center.getDrivers().at(i)->setNewTrip();
        }
    }
}

void TaxiFlow::closeClients() {
    int numClients = clients->size();
    for (int i = 0; i < numClients; i++) {
        // tells the client he can close now.
        socket->sendData("exit", clients->at(i)->getDescriptor());
    }
}

void* TaxiFlow::getClientsWrapper(void* tf) {
    ((TaxiFlow*)tf)->getDriversFromClients();
}

void TaxiFlow::getDriversFromClients() {//void* socket) {
    char buffer[1000];
    // get the driver.

    pthread_mutex_lock(&acceptMutex);

    cout << "in accept mutex " << endl;


    ((Tcp*)socket)->acceptClient();
    int descriptorComm = ((Tcp*)socket)->getDescriptorCommunicateClient();

    pthread_mutex_unlock(&acceptMutex);

    cout << "waiting for driver " << endl;

    ((Tcp*) socket)->receiveData(buffer, sizeof(buffer), descriptorComm);
    //DriverDescriptor* driverDes;
    Driver *driver;// = driverDes->getDriver();
    // gets the driver from client.
    boost::iostreams::basic_array_source<char> device(buffer, sizeof(buffer));
    boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
    // deserizlizes the driver from client.
    boost::archive::binary_iarchive ia(s);
    ia >> driver;
    // sets the drivers map.
    driver->setMap(center.getMap());
    //assigns the driver his cab.
    center.assignCab(driver);
    //sends the cab to the driver.
    Taxi *t = driver->getCab();
    std::string serial_str2;
    boost::iostreams::back_insert_device<std::string> inserter2(serial_str2);
    boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s2(inserter2);
    boost::archive::binary_oarchive oa2(s2);
    // serilizes the taxi.
    oa2 << t;
    // flush the stream to finish writing into the buffer
    s2.flush();
    // sends the taxi.
    ((Tcp*) socket)->sendData(serial_str2, descriptorComm);
    // adds the driver to the taxi center.
    //center.addDriver(driver);
    DriverDescriptor* ds = new DriverDescriptor(driver, descriptorComm);

    pthread_mutex_lock(&addMutex);

    cout << ds->getDriver()->getId() << " " << ds->getDriver()->getAge() << endl;

    clients->push_back(ds);
    center.addDriver(ds->getDriver());

    pthread_mutex_unlock(&addMutex);

}

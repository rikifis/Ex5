//
// Created by riki on 17/01/17.
//
#include "TaxiFlow.h"
#include "Tcp.h"
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
    delete flow;
    return 0;
}
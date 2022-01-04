//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates

#include <iostream>
#include <zmq.hpp>
#include <cstdlib>
#include <ctime>

int main() {

    // Add from the original code
    std::cout << "Publishing updates at weather server..." << std::endl;
    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5556");

    //  Initialize random number generator
    srand((unsigned)time(NULL));
    while (1) {

        int zipcode, temperature, relhumidity;

        //  Get values that will fool the boss
        zipcode = rand() % 99999 + 1;
        temperature = rand() % 215 - 80;
        relhumidity = rand() % 50 + 10;

        //  Send message to all subscribers
        zmq::message_t message(20);
        snprintf((char*)message.data(), 20,
            "%05d %d %d", zipcode, temperature, relhumidity);
        publisher.send(message, zmq::send_flags::none);

    }
    return 0;
}
#include <zmq.hpp>
#include <iostream>
#include <sstream>
#include <chrono>
#include <thread>


int main(int argc, char* argv[])
{
    using namespace std::chrono_literals;
    zmq::context_t context(1);

    //  Socket to talk to server
    std::cout << "Collecting updates from weather server..." << std::endl;
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5556");
   
    //  Subscribe to zipcode, default is NYC, 10001
    const char* filter = (argc > 1) ? argv[1] : "10001 ";
    subscriber.set(zmq::sockopt::subscribe, filter);
    
    //  Process 20 updates
    int update_nbr;
    long total_temp = 0;
    for (update_nbr = 0; update_nbr < 20; update_nbr++) {

        zmq::message_t update;
        int zipcode, temperature, relhumidity;

        subscriber.recv(update, zmq::recv_flags::none);

        std::istringstream iss(static_cast<char*>(update.data()));
        iss >> zipcode >> temperature >> relhumidity;

        total_temp += temperature;
        // Add from the original code
        std::cout << "Receive temperature for zipcode '" << filter << "' was " << temperature << "F" << std::endl;
        std::this_thread::sleep_for(1s);
    }
    std::cout << "Average temperature for zipcode '" << filter 
        << "' was " << (int)(total_temp / update_nbr) << "F" << std::endl;
    return 0;
}
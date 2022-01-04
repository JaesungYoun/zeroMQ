
#include <iostream>
#include <zmq.hpp>

int main() {


    //context and sockets
    zmq::context_t context(1);
    zmq::socket_t publisher(context, zmq::socket_type::pub);
    publisher.bind("tcp://*:5557");
    zmq::socket_t collector(context, zmq::socket_type::pull);
    collector.bind("tcp://*:5558"); 
    while (true) {
        zmq::message_t z_in;
        collector.recv(z_in, zmq::recv_flags::none);
        std::cout << "server: publishing update => " << z_in.to_string() << std::endl;
        publisher.send(z_in, zmq::send_flags::none);

    }
    return 0;
}
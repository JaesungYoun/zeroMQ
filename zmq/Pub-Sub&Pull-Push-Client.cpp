#include <iostream>
#include <cstdlib>
#include <ctime>
#include <zmq.hpp>

int main() {


    //context and sockets
    zmq::context_t ctx(1);
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.set(zmq::sockopt::subscribe,"");
    subscriber.connect("tcp://localhost:5557");
    zmq::socket_t publisher(ctx, zmq::socket_type::push);
    publisher.connect("tcp://localhost:5558");

    zmq::pollitem_t items[] = {
        { subscriber, 0, ZMQ_POLLIN, 0 },
        { publisher, 0 , ZMQ_POLLIN, 0}
    };
    
    srand((unsigned)time(NULL));
    while (true) {
        if (zmq::poll(items, 2, std::chrono::milliseconds(100)) == -1)
            break;
        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            subscriber.recv(message, zmq::recv_flags::none);
            std::cout << "I: received message " << message.to_string() << std::endl;
        }
        else {
            int ran_num = rand() % 100 + 1;
            std::string msg_out = std::to_string(ran_num);
            zmq::message_t ran(msg_out);
            if (ran_num < 10) {
                publisher.send(ran, zmq::send_flags::none);
                std::cout << "I: sending message " << ran_num << std::endl;
            }
        }
    }
    return 0;
}
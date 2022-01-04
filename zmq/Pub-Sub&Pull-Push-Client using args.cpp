#include <iostream>
#include <cstdlib>
#include <ctime>
#include <zmq.hpp>
#include <thread>

using namespace std::chrono_literals;

int main(int argc, char* argv[]) {

    //context and sockets
    zmq::context_t ctx(1);
    zmq::socket_t subscriber(ctx, zmq::socket_type::sub);
    subscriber.set(zmq::sockopt::subscribe, "");
    subscriber.connect("tcp://localhost:5557");
    zmq::socket_t publisher(ctx, zmq::socket_type::push);
    publisher.connect("tcp://localhost:5558");

    zmq::pollitem_t items[] = {
        { subscriber, 0, ZMQ_POLLIN, 0 },
        { publisher, 0 , ZMQ_POLLIN, 0}
    };
  
    std::string clientID = argv[1];
    srand((unsigned)time(NULL));
    while (true) {
        if (zmq::poll(items, 2, std::chrono::milliseconds(100)) == -1)
            break;
        if (items[0].revents & ZMQ_POLLIN) {
            zmq::message_t message;
            subscriber.recv(message, zmq::recv_flags::none);
            std::cout << clientID << ": received status => " << message.to_string() << std::endl;
        }
        else {
            int ran_num = rand() % 100 + 1;
            if (ran_num < 10) {
                std::this_thread::sleep_for(1s);
                std::string message = "(" + clientID + ":ON)";
                zmq::message_t msg(message);
                publisher.send(msg, zmq::send_flags::none);
                std::cout << clientID << ": send status - activated" << std::endl;
            }
            else if (ran_num > 90) {
                std::this_thread::sleep_for(1s);
                std::string message = "(" + clientID + ":OFF)";
                zmq::message_t msg(message);
                publisher.send(msg, zmq::send_flags::none);
                std::cout << clientID << ": send status - deactivated" << std::endl;
            }
        }
    }
    return 0;
}
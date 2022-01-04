#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include <functional>
#include <zmq.hpp>
#include <zmq_addon.hpp>

using namespace std::chrono_literals;

//  This is our client task class.
//  It connects to the server, and then sends a request once per second
//  It collects responses as they arrive, and it prints them out. We will
//  run several client tasks in parallel, each with a different random ID.
//  Attention! -- this random work well only on linux.

class client_task {
    std::string id;
public:
    client_task() :ctx_(1),
        client_socket_(ctx_, zmq::socket_type::dealer) {}

    client_task(std::string id)
        : ctx_(1),
        client_socket_(ctx_, zmq::socket_type::dealer), id(id)
    {}

 
    void recvHandler() {
        std::string identity = id;
        zmq::pollitem_t items[] = {
       { client_socket_, 0, ZMQ_POLLIN, 0 } };
        while (true) {
            zmq::poll(items, 1, std::chrono::milliseconds(1000));
            if (items[0].revents & ZMQ_POLLIN) {
                std::vector<zmq::message_t> rec;
                zmq::recv_multipart(client_socket_, std::back_inserter(rec));
                std::cout << identity << " received: " << rec[0].to_string() << std::endl;
            }
        }
    }

    void start() {
        std::string identity = id;

        client_socket_.connect("tcp://localhost:5570");
        std::cout << "Client " << identity << " started" << std::endl;
        int request_nbr = 0;
      
        std::thread client_thread1(&client_task::recvHandler, this);
        client_thread1.detach();
     
    
        try {
            while (true) {
                
                request_nbr = request_nbr + 1;
                std::string request_string = "request #" + std::to_string(request_nbr);
                std::cout << "Req #" << request_nbr << " sent.." << std::endl;

                std::vector<zmq::const_buffer> send_msgs = { zmq::buffer(request_string),
                        zmq::buffer(identity) };
                zmq::send_multipart(client_socket_, send_msgs);
                std::this_thread::sleep_for(1s);

               
            }
    
        }
        catch (std::exception& e) {}
    }
   
private:
    zmq::context_t ctx_;
    zmq::socket_t client_socket_;
};

int main(int argc, char* argv[])
{

    std::string s = argv[1];
    client_task ct(s);
    //ct.start();
    std::thread t(std::bind(&client_task::start, &ct));
 
    t.detach();
 
    getchar();
    return 0;
}
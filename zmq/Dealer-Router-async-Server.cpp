#include <vector>
#include <thread>
#include <memory>
#include <iostream>
#include <functional>
#include "zmq.hpp"
#include <functional>
#include <zmq_addon.hpp>
using namespace std::chrono_literals;
//  .split worker task
//  Each worker task works on one request at a time and sends a random number
//  of replies back, with random delays between replies:

class server_worker {
    int id;
public:

    server_worker(zmq::context_t& ctx, int sock_type, int id_)
        : ctx_(ctx),
        worker_(ctx_, sock_type), id(id_)
    {}

    void run() {
        worker_.connect("inproc://backend");
        std::cout << "Worker#" << id << " started" << std::endl;
        
        try {
            while (true) {
                
 
                std::vector<zmq::message_t> recv_msgs;
                
                zmq::recv_multipart(worker_, std::back_inserter(recv_msgs));      
                
                std::cout << "Worker#" << id << " received " << recv_msgs[1].to_string() << " from " << recv_msgs[2].to_string() << std::endl;
                std::this_thread::sleep_for(1s);
                zmq::send_multipart(worker_, recv_msgs);
               
            }
        }
        catch (std::exception e) {}
    }



private:
    zmq::context_t& ctx_;
    zmq::socket_t worker_;
};

//  .split server task
//  This is our server task.
//  It uses the multithreaded server model to deal requests out to a pool
//  of workers and route replies back to clients. One worker can handle
//  one request at a time but one client can talk to multiple workers at
//  once.


class server_task {
private:
    zmq::context_t ctx_;
    zmq::socket_t frontend_;
    zmq::socket_t backend_;
    int num_server;
  
public:

    server_task(int num_s)
        : ctx_(1),
        frontend_(ctx_, zmq::socket_type::router),
        backend_(ctx_, zmq::socket_type::dealer), num_server(num_s)
    {}

    void run() {
        frontend_.bind("tcp://*:5570");
        backend_.bind("inproc://backend");


        
        std::vector<server_worker*> worker;
        std::vector<std::thread*> worker_thread;
        for (int i = 0; i < num_server; i++) {
            worker.push_back(new server_worker(ctx_, ZMQ_DEALER, i));;
            worker_thread.push_back(new std::thread(std::bind(&server_worker::run, worker[i])));
            std::this_thread::sleep_for(1s);
            worker_thread[i]->detach();
            
        }
  
        try {
            zmq::proxy(frontend_,backend_);
        }
        catch (std::exception& e) {}

        for (int i = 0; i < num_server; ++i) {
            delete worker[i];
            delete worker_thread[i];
        }
    }
};



//  The main thread simply starts several clients and a server, and then
//  waits for the server to finish.

int main(int argc, char* argv[])
{
    int x = std::stoi(argv[1]);
    server_task st(x);
    //st.run();
    std::thread t(std::bind(&server_task::run, &st));
    t.detach();
 

    getchar();
    return 0;
}

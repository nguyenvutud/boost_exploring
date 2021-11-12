//Timer.5 - Synchronising handlers in multithreaded programs
// https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tuttimer5.html

//how to use the strand class template to synchronise callback handlers in a multithreaded program.
// calling io_context::run() from only one thread ensures that callback handlers cannot run concurrently.

//The single threaded approach is usually the best place to start when developing applications using asio.
//The downside is the limitations it places on programs, particularly servers, including:
// - Poor responsiveness when handlers can take a long time to complete.
// - An inability to scale on multiprocessor systems.

#include <iostream>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>


//This class will extend the previous tutorial by running two timers in parallel.
class printer
{
public:
    printer(boost::asio::io_context& io)
        : strand_(boost::asio::make_strand(io)),
            timer1_(io, boost::asio::chrono::seconds(1)),
            timer2_(io, boost::asio::chrono::seconds(1)),
            count_(0)
            {
                //When initiating the asynchronous operations, each callback handler is "bound" to an boost::asio::strand<boost::asio::io_context::executor_type> object. 
                //The boost::asio::bind_executor() function returns a new handler that automatically dispatches its contained handler through the strand object. 
                //By binding the handlers to the same strand, we are ensuring that they cannot execute concurrently.
                timer1_.async_wait(boost::asio::bind_executor(strand_,
                boost::bind(&printer::print1, this)));
                timer2_.async_wait(boost::asio::bind_executor(strand_,
                boost::bind(&printer::print2, this)));
            }
    ~printer()
    {
        std::cout << "Final count is " << count_ << std::endl;
    }
    //In a multithreaded program, the handlers for asynchronous operations should be synchronised if they access shared resources
    //the shared resources used by the handlers (print1 and print2) are std::cout and the count_ data member.
    void print1()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 1: " << count_ << std::endl;
            ++count_;

            timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
            timer1_.async_wait(boost::asio::bind_executor(strand_,
            boost::bind(&printer::print1, this)));
        }
    }

    void print2()
    {
        if (count_ < 10)
        {
            std::cout << "Timer 2: " << count_ << std::endl;
            ++count_;

            timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));
            timer2_.async_wait(boost::asio::bind_executor(strand_,
            boost::bind(&printer::print2, this)));
        }
    }
private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};

//The main function now causes io_context::run() to be called from two threads: the main thread and one additional thread. 
//This is accomplished using an boost::thread object
//Just as it would with a call from a single thread, concurrent calls to io_context::run() will continue to execute while there is "work" left to do.
//The background thread will not exit until all asynchronous operations have completed.
int main()
{
    boost::asio::io_context io;
    printer p(io);
    boost::thread t(boost::bind(&boost::asio::io_context::run, &io));
    io.run();
    t.join();

    return 0;
}
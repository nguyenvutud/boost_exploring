//Timer.3 - Binding arguments to a handler
//https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tuttimer3.html

#include <iostream>
#include <unistd.h>
//All of the asio classes can be used by simply including the "asio.hpp" header file
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>

// the callback function will need to be able to access the timer object.
// add two new params: a pointer to a timer object, a counter that we can stop the program the the timer fires for the sixth time.
void print(const boost::system::error_code& /*e*/,
boost::asio::steady_timer* t, int* count)
{
    // By not starting a new asynchronous wait on the timer when count reaches 5, the io_context will run out of work and stop running.
    if (*count < 5)
    {
        std::cout << "Inside asynchronously func!\n";
        std::cout << *count <<std::endl;
        ++(*count);
        //move the expiry time for the timer along by one second from the previous expiry time
        //By calculating the new expiry time relative to the old, we can ensure that 
        //the timer does not drift away from the whole-second mark due to any delays in processing the handler.
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        //the boost::bind() function is used to associate the extra parameters with your callback handler.
        //if using boost::bind(), you must specify only the arguments that match the handler's parameter list
        t->async_wait(boost::bind(print, boost::asio::placeholders::error, t, count));
    }
}
int main()
{
    boost::asio::io_service io;
    
    int count = 0;    
    //Next we declare an object of type boost::asio::steady_timer
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(1));
    t.async_wait(boost::bind(print, boost::asio::placeholders::error, &t, &count));

    std::cout<< "After performing wait() function.\n";
    
    //We MUST call the io_context::run() member func on the io_context object.
    //The io_context::run() func will also CONTINUE to run while there is still 'work' to do.
    //The work is the asynchronous wait on the timer, so the call will not return until the timer has expired and the callback has completed.
    io.run();
    
    std::cout << "Final count is " << count << std::endl;
    return 0;
}

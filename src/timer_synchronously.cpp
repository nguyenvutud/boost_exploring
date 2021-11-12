//Timer.1 - Using a timer synchronously
//https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tuttimer1.html

#include <iostream>
//All of the asio classes can be used by simply including the "asio.hpp" header file
#include <boost/asio.hpp>

//all programs that use asio need to have at least one I/O execution context,
//such as an io_context or thread_pool object.
//An I/O execution context provides access to I/O functionality. 
//We declare an object of type io_context first thing in the main function.
int main()
{
    boost::asio::io_context io;
    //Next we declare an object of type boost::asio::steady_timer
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5)); //set the timer to expire 5 seconds from now

    std::cout<< "Before performing wait() function.\n";
    //perform a blocking wait on the timer.
    t.wait();

    std::cout << "Test the blocking time" << std::endl;
    return 0;
}

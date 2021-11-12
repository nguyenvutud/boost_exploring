//Timer.1 - Using a timer synchronously
//https://www.boost.org/doc/libs/1_77_0/doc/html/boost_asio/tutorial/tuttimer2.html

#include <iostream>
#include <unistd.h>
//All of the asio classes can be used by simply including the "asio.hpp" header file
#include <boost/asio.hpp>

//Using asio's asynchronous functionality means having a callback function that will be called when an asynchronous operation completes. 
//In this program we define a function called print to be called when the asynchronous wait finishes.
void print(const boost::system::error_code& /*e*/)
{
    int n = 10;
    for(int i = 0; i < n; i++)
    {
        std::cout << "Inside asynchronously func!\n";
        sleep(1);
    }
}
int main()
{
    boost::asio::io_service io;
    //Next we declare an object of type boost::asio::steady_timer
    boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5)); //set the timer to expire 5 seconds from now

    std::cout<< "Before performing wait() function.\n";
    //instead of doing a blocking wait as in synchronously timer, we call the
    //steady_timer::asyns_wait() function to perform an asynchronous wait.
    //When calling this func we pass the print callback handler that was defined above.
    
    t.async_wait(&print);

    std::cout<< "After performing wait() function.\n";
    
    //We MUST call the io_context::run() member func on the io_context object.
    //The io_context::run() func will also CONTINUE to run while there is still 'work' to do.
    //The work is the asynchronous wait on the timer, so the call will not return until the timer has expired and the callback has completed.
    io.run();
    
    std::cout << "Finish a timer asynchronously" << std::endl;
    return 0;
}

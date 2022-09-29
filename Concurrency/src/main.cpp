#include <iostream>
#include <thread>

void doWork()
{
    std::cout << "Doing work.\n";
}

int main()
{
    std::thread t{ doWork };
    std::cout << "Hello, world!\n";
    t.join();
}

#include <iostream>
#include <thread>
#include "restaurant.h"



int main()
{
    std::thread order(newOrder);
    std::thread kitchen(cooking);
    std::thread delivery(takeReadyDishes);

    kitchen.detach();
    order.detach();
    delivery.join();
}
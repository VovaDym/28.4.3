#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <chrono>



enum Dishes
{
    PIZZA,
    SOUP,
    STEAK,
    SALAD,
    SUSHI,
    AMOUNT_DISHES,
    NO_DISHES = -1
};

std::vector <Dishes> orders;
std::vector <Dishes> readyDishes;
std::mutex orderQueue;
std::mutex readyDishesQueue;
std::mutex outputQueue;

std::string menuRestaurant(const Dishes dishes)
{
    switch (dishes)
    {
        case PIZZA: return "pizza";
        case SOUP:  return "soup";
        case STEAK: return "steak";
        case SALAD: return "salad";
        case SUSHI: return "sushi";
    }
}

void newOrder()
{
    std::srand(std::time(nullptr));

    while (true)
    {
        int sec = rand() % 6 + 5;
        std::this_thread::sleep_for(std::chrono::seconds(sec));
        orderQueue.lock();
        Dishes newDish = static_cast<Dishes> (rand() % AMOUNT_DISHES);
        orders.push_back(newDish);
        outputQueue.lock();
        std::cout << "New order: " << menuRestaurant(newDish) << std::endl;
        outputQueue.unlock();
        orderQueue.unlock();
    }
}

void cooking()
{
    std::srand(std::time(nullptr));

    while (true)
    {
        orderQueue.lock();
        Dishes cook;
        if (orders.size() >= 1)
        {
            cook = orders[0];
        }
        else
        {
            cook = NO_DISHES;
        }
        
        if (cook != NO_DISHES)
        {
            outputQueue.lock();
            std::cout << "Cooking: " << menuRestaurant(orders[0]) << std::endl;
            outputQueue.unlock();
            for (int i = 0; i < orders.size() - 1; ++i)
            {
                orders[i] = orders[i + 1];
            }   
            orders.pop_back();
        }
        orderQueue.unlock();

        int sec = rand() % 11 + 5;
        std::this_thread::sleep_for(std::chrono::seconds(sec));
        readyDishesQueue.lock();
        readyDishes.push_back(cook);
        readyDishesQueue.unlock();
    }
}

void takeReadyDishes()
{
    for (int numberDelivery = 0; numberDelivery < 10; ++numberDelivery)
    {
        std::this_thread::sleep_for(std::chrono::seconds(30));
        readyDishesQueue.lock();
        outputQueue.lock();
        std::cout << "The courier received an order of " << readyDishes.size() << " dishes" << std::endl;
        outputQueue.unlock();
        readyDishes.clear();
        readyDishesQueue.unlock();
        outputQueue.lock();
        std::cout << numberDelivery + 1 <<"the order has been delivered " << std::endl;
        outputQueue.unlock();
    }
}

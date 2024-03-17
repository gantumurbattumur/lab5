#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>

const int NUM_PHILOSOPHERS = 5;
const int THINK_TIME_MIN = 1;
const int THINK_TIME_MAX = 5;
const int EAT_TIME_MIN = 1;
const int EAT_TIME_MAX = 3;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> think_time_dist(THINK_TIME_MIN, THINK_TIME_MAX);
std::uniform_int_distribution<> eat_time_dist(EAT_TIME_MIN, EAT_TIME_MAX);

std::vector<std::mutex> chopsticks(NUM_PHILOSOPHERS);

void philosopher(int idx) {
    while (true) {
        // Think
        int think_time = think_time_dist(gen);
        std::cout << "Philosopher " << idx << " is thinking for " << think_time << " seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(think_time));

        // Get chopsticks
        std::mutex& left_chopstick = chopsticks[idx];
        std::mutex& right_chopstick = chopsticks[(idx + 1) % NUM_PHILOSOPHERS];

        // Acquire chopsticks in a specific order to avoid deadlock
        if (idx < (NUM_PHILOSOPHERS - 1)) {
            left_chopstick.lock();
            std::cout << "Philosopher " << idx << " picked up his left-hand chopstick." << std::endl;
            right_chopstick.lock();
            std::cout << "Philosopher " << idx << " picked up his right-hand chopstick." << std::endl;
        } else {
            right_chopstick.lock();
            std::cout << "Philosopher " << idx << " picked up his right-hand chopstick." << std::endl;
            left_chopstick.lock();
            std::cout << "Philosopher " << idx << " picked up his left-hand chopstick." << std::endl;
        }

        // Eat
        int eat_time = eat_time_dist(gen);
        std::cout << "Philosopher " << idx << " is eating for " << eat_time << " seconds." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(eat_time));

        // Put down chopsticks
        if (idx < (NUM_PHILOSOPHERS - 1)) {
            left_chopstick.unlock();
            std::cout << "Philosopher " << idx << " put down his left-hand chopstick." << std::endl;
            right_chopstick.unlock();
            std::cout << "Philosopher " << idx << " put down his right-hand chopstick." << std::endl;
        } else {
            right_chopstick.unlock();
            std::cout << "Philosopher " << idx << " put down his right-hand chopstick." << std::endl;
            left_chopstick.unlock();
            std::cout << "Philosopher " << idx << " put down his left-hand chopstick." << std::endl;
        }
    }
}

int main() {
    std::vector<std::thread> philosophers;

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        philosophers.emplace_back(philosopher, i);
    }

    // Wait for threads to finish (they run indefinitely in this case)
    for (auto& t : philosophers) {
        t.join();
    }

    return 0;
}
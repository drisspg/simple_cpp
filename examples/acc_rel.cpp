#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <vector>
#include <atomic>

class toy_queue {
    unsigned head;
    std::atomic_uint tail;
    unsigned size;
    void **entries;
    
public:
    toy_queue(unsigned capacity) : head(0), tail(0), size(capacity) {
        entries = new void*[size];
        init();
    }
    
    ~toy_queue() {
        delete[] entries;
    }
    
    void init() {
        head = 0; 
        tail = 0;
        memset(entries, 0, size * sizeof(void *));
    }
    
    void put(void *entry) {
        auto my_tail = tail.load(std::memory_order_relaxed);
        unsigned i = my_tail % size;
        entries[i] = entry;
        tail.store(my_tail + 1, std::memory_order_release);
    }
    
    bool poll(void **entry) {
        auto my_tail = tail.load(std::memory_order_acquire);
        if (my_tail > head) {
            unsigned i = head % size;
            *entry = entries[i];
            head++;
            return true;
        } else {
            return false; // empty
        }
    }
};

// Global queue instance
toy_queue *queue;

// Producer thread function
void producer(int id, int num_items) {
    for (int i = 0; i < num_items; i++) {
        // Allocate new integer
        int *value = new int(i);
        queue->put(static_cast<void*>(value));
        
        std::println("Producer {} inserted: {}", id, i);
    }
}

// Consumer thread function
void consumer(int id, int num_items) {
    int items_consumed = 0;
    
    while (items_consumed < num_items) {
        void *value = nullptr;
        if (queue->poll(&value)) {
            if (value != nullptr) {
                int *int_value = static_cast<int*>(value);
                std::println("Consumer {} got: {}", id, *int_value);
                delete int_value;  // Clean up the allocated integer
                items_consumed++;
            }
            else{
                std::println("OOOPS! You got a null even though you shouldn't!");
            }
        }
    }
}

bool parse_args(int argc, char* argv[], int& NUM_PRODUCER_CONSUMER, int& NUM_ENTIRES_PER) {
    // Fixed value for producer/consumer pairs
    NUM_PRODUCER_CONSUMER = 1;
    
    // Default value for entries
    NUM_ENTIRES_PER = 50;

    // Override default if command line argument is provided
    if (argc == 2) {
        NUM_ENTIRES_PER = std::atoi(argv[1]);
    } else if (argc != 1) {
        std::println("Usage: {} <entries_per_thread>", argv[0]);
        std::println("Using default: {} entries per thread", NUM_ENTIRES_PER);
    }

    if (NUM_ENTIRES_PER <= 0) {
        std::println("Number of entries must be a positive integer");
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int NUM_PRODUCER_CONSUMER;
    int NUM_ENTIRES_PER;
    if (!parse_args(argc, argv, NUM_PRODUCER_CONSUMER, NUM_ENTIRES_PER)) {
        return 1;
    }

    // Initialize queue
    queue = new toy_queue(NUM_PRODUCER_CONSUMER * NUM_ENTIRES_PER);

    // Create producer and consumer threads
    std::vector<std::thread> threads;
    
    for (int i = 0; i < NUM_PRODUCER_CONSUMER; i++) {
        threads.push_back(std::thread(producer, i, NUM_ENTIRES_PER));
    }
    
    for (int i = 0; i < NUM_PRODUCER_CONSUMER; i++) {
        threads.push_back(std::thread(consumer, i, NUM_ENTIRES_PER));
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Cleanup
    delete queue;
    
    return 0;
}
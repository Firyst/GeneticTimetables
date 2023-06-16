#ifndef UNIQUEPRIORITYQUEUE_H
#define UNIQUEPRIORITYQUEUE_H


#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>

/**
 * @brief The UniquePriorityQueue class basically a std::priority_queue but with uniquness check
 */
template<typename T>
class UniquePriorityQueue {
public:
    /**
     * @brief push basic push
     * @param val value to push
     * @return true if is unique, false if was in container already
     */
    bool push(const T& val) {
        // check if in
        if (uniqueElements.find(val) != uniqueElements.end()) {
            // contains
            return false;
        }

        // add to queue
        priorityQueue.push(val);
        uniqueElements.insert(val);

        return true;
    }

    /**
     * @brief pop pops the least value
     */
    void pop() {
        if (!priorityQueue.empty()) {
            // remove from set
            uniqueElements.erase(priorityQueue.top());
            // remove from queue
            priorityQueue.pop();
        }
    }

    /**
     * @brief top get the best in queue
     * @return best element
     */
    T top() {
        if (!priorityQueue.empty()) {
            return priorityQueue.top();
        }
        // no top
        throw std::logic_error("Queue is empty.");
    }

    /**
     * @brief empty check if empty
     * @return bool
     */
    bool empty() {
        return priorityQueue.empty();
    }

    /**
     * @brief size get size
     * @return size_t size
     */
    size_t size() {
        return priorityQueue.size();
    }

private:
    std::priority_queue<T> priorityQueue; ///< used queue
    std::unordered_set<T> uniqueElements; ///< used set
};

#endif // UNIQUEPRIORITYQUEUE_H

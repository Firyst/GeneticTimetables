#ifndef UNIQUEPRIORITYQUEUE_H
#define UNIQUEPRIORITYQUEUE_H


#include <iostream>
#include <queue>
#include <set>
#include <unordered_set>

template<typename T>
class UniquePriorityQueue {
public:
    bool push(const T& val) {
        // Проверяем, не содержится ли уже значение в наборе
        if (uniqueElements.find(val) != uniqueElements.end()) {
            // Значение уже содержится в наборе, возвращаем false
            return false;
        }

        // Добавляем значение в очередь и набор
        priorityQueue.push(val);
        uniqueElements.insert(val);

        return true;
    }

    void pop() {
        if (!priorityQueue.empty()) {
            // Удаляем значение из набора
            uniqueElements.erase(priorityQueue.top());
            // Удаляем значение из очереди
            priorityQueue.pop();
        }
    }

    T top() {
        if (!priorityQueue.empty()) {
            return priorityQueue.top();
        }
        // Здесь вы должны обрабатывать ситуацию, когда очередь пуста.
        throw std::logic_error("Queue is empty.");
    }

    bool empty() {
        return priorityQueue.empty();
    }

    size_t size() {
        return priorityQueue.size();
    }

private:
    std::priority_queue<T> priorityQueue;
    std::unordered_set<T> uniqueElements;
};

#endif // UNIQUEPRIORITYQUEUE_H

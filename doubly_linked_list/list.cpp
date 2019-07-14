#include <iostream>

namespace custom {
    template<typename T>
    class list {
        struct node {
            T value;
            node *prev;
            node *next;
            node(T _val, node *_prev, node *_next) : value(_val), prev(_prev), next(_next) {}
        };
        node *head;
        node *tail;
        public:
            list() : head(NULL), tail(NULL) {}
            ~list();
            void push_front(T);
            void push_back(T);
            T pop_back();
            T pop_front();
            bool empty() const { return (!head || !tail); }
            void print();
    };

    template<typename T>
    list<T>::~list() {
        while(head) {
            node *temp = head;
            head = temp->next;
            delete temp;
        }
    }

    template<typename T>
    void list<T>::push_front(T value) {
        head = new node(value, NULL, head);
        if (head->next) {
            head->next->prev = head;
        }
        if (empty()) {
            tail = head;
        }
    }

    template<typename T>
    void list<T>::push_back(T value) {
        tail = new node(value, tail, NULL);
        if (tail->prev) {
            tail->prev->next = tail;
        }
        if (empty()) {
            head = tail;
        }
    }

    template<typename T>
    T list<T>::pop_front() {
        if (empty()) {
            throw("list is empty");
        }
        T value = head->value;
        if (head->next == NULL) {
            head = NULL;
            tail = NULL;
        } else {
            node *temp = head;
            head = head->next;
            delete temp;
        }
        return value;
    }

    template<typename T>
    T list<T>::pop_back() {
        if (empty()) {
            throw("list is empty");
        }
        T value = tail->value;
        if (tail->prev == NULL) {
            tail = NULL;
            head = NULL;
        } else {
            node *temp = tail;
            tail = tail->prev;
            delete temp;
        }
        return value;
    }

    template<typename T>
    void list<T>::print() {
        node *current = head;
        while (current) {
            std::cout << current->value << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
}

int main(void) {
    custom::list<int> list;
    list.push_back(9);
    list.print();
    return 0;
}
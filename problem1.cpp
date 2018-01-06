#include <iostream>
#include <utility>    //need for std::swap

template<typename T>
class Elem {
private:
    T key;

public:
    Elem<T>* next;

    explicit Elem (T key):
            key(key),
            next(nullptr) {
    }

    int getKey() {
        return key;
    }
};

template<typename T>
class List {
private:
    Elem<T>* first;
    Elem<T>* last;

public:
    List ():
        first(nullptr),
        last(nullptr) {}

    explicit List (const T& x) {
        first = new Elem<T>(x);
        last = first;
    }

    void add(const T& x) {
        Elem<T>* elem = new Elem<T>(x);
        add(elem);
    }

    void add(Elem<T>* elem) {
        elem->next = nullptr;
        if (!first) {
            first = elem;
            last = first;
        } else {
            last->next = elem;
            last = elem;
        }
    }

    void print() const {
        Elem<T>* tmp = first;
        while (tmp) {
            std::cout << tmp->getKey() << " ";
            tmp = tmp->next;
        }
        std::cout << std::endl;
    }

    void merge(const List& other) {
        List res;
        Elem<T> *iter1 = this->first;
        Elem<T> *iter2 = other.first;
        while (iter1 && iter2) {
            if (iter1->getKey() > iter2->getKey())
                std::swap(iter1, iter2);
            res.add(iter1->getKey());
            iter1 = iter1->next;
        }
        while(iter2) {
            res.add(iter2->getKey());
            iter2 = iter2->next;
        }
        std::swap(first, res.first);
    }

    void mergeSort() {
        if (first == last) return;
        Elem<T>* iter1 = first;
        Elem<T>* iter2 = first;
        List secondPart;

        while (iter2->next) {
            iter2 = iter2->next;
            Elem<T> *tmp = iter1->next;
            secondPart.add(iter1);
            iter1 = tmp;
            if (iter2->next) iter2 = iter2->next;
        }
        first = iter1;

        this->mergeSort();
        secondPart.mergeSort();
        merge(secondPart);
    }

    ~List() {
        while (first) {
            Elem<T>* next = first->next;
            delete first;
            first = next;
        }
    }
};

int main() {

    int n;
    std::cin >> n;
    List<int> list;

    for (int i = 0; i < n; i++) {
        int x;
        std::cin >> x;
        list.add(x);
    }

    list.mergeSort();
    list.print();
}

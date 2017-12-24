#include <iostream>
using namespace std;

class Elem {
private:
    int key;

public:
    Elem* next;

    explicit Elem (int key):
            key(key),
            next(nullptr) {
    }

    int getKey() {
        return key;
    }
};

class List {
private:
    Elem* first;

public:
    List ():first(nullptr) {}

    explicit List (int x) {
        first = new Elem(x);
    }

    void add(int x) {
        Elem* tmp = first;
        first = new Elem(x);
        first->next = tmp;
    }

    void add(Elem* elem) {
        if (first) {
            elem->next = first;
            first = elem;
        } else {
            first = elem;
            first->next = nullptr;
        }
    }

    void print() const {
        Elem* tmp = first;
        while (tmp) {
            cout << tmp->getKey() << " ";
            tmp = tmp->next;
        }
        cout << endl;
    }

    void mergeByOrder(const List& other) {
        List res;
        Elem* iter1 = this->first;
        Elem* iter2 = other.first;
        while (iter1 || iter2) {
            if (iter1 && iter2) {
                if (iter1->getKey() < iter2->getKey()) {
                    res.add(iter1->getKey());
                    iter1 = iter1->next;
                } else {
                    res.add(iter2->getKey());
                    iter2 = iter2->next;
                }
            } else if (iter1) {
                res.add(iter1->getKey());
                iter1 = iter1->next;
            } else {
                res.add(iter2->getKey());
                iter2 = iter2->next;
            }
        }
        res.reverse();
        swap(first, res.first);
    }

    void reverse() {
        List tmp;
        while (first) {
            Elem* next = first->next;
            tmp.add(first);
            first = next;
        }
        swap(first, tmp.first);
    }

    void mergeSort() {
        size_t length = 0;
        Elem* iter = first;
        while (iter) {
            length++;
            iter = iter->next;
        }
        if (length <= 1)
            return;
        List secondPart;
        for (size_t i = 0; 2 * i < length; ++i) {
            Elem* next = first->next;
            secondPart.add(first);
            first = next;
        }
        secondPart.mergeSort();
        mergeSort();
        mergeByOrder(secondPart);
    }

    ~List() {
        while (first) {
            Elem* next = first->next;
            delete first;
            first = next;
        }
    }
};

int main() {

    int n;
    cin >> n;
    List list;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        list.add(x);
    }

    list.mergeSort();
    list.print();
}

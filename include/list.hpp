#ifndef LIST_HPP
#define LIST_HPP

#include <stdexcept>

// Sablon (template) osztály, hogy bármilyen típust (T) tárolhassunk benne
template <typename T>
class List
{
private:
    // A lista belső építőeleme. A külvilág számára teljesen rejtett.
    struct Node
    {
        T data;
        Node *next;
        Node *prev;

        Node(const T &data, Node *next = nullptr, Node *prev = nullptr)
            : data(data), next(next), prev(prev) {}
    };

    Node *head;
    Node *tail;
    size_t count;

public:
    // ---------------------------------------------------------
    // ITERÁTOR OSZTÁLY
    // ---------------------------------------------------------
    class Iterator
    {
    private:
        Node *current;
        // A List osztály hozzáférhet a privát current mutatóhoz
        friend class List<T>;

    public:
        Iterator(Node *node = nullptr) : current(node) {}

        T &operator*()
        {
            return current->data;
        }

        Iterator &operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        bool operator==(const Iterator &other) const
        {
            return current == other.current;
        }

        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    class ConstIterator {
    private:
        const Node* current;
        friend class List<T>; 

    public:
        ConstIterator(const Node* node = nullptr) : current(node) {}

        const T& operator*() const {
            return current->data;
        }

        ConstIterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        bool operator==(const ConstIterator& other) const {
            return current == other.current;
        }

        bool operator!=(const ConstIterator& other) const {
            return current != other.current;
        }
    };

    // 1. Alapértelmezett konstruktor
    List() : head(nullptr), tail(nullptr), count(0) {}

    // 2. Destruktor
    ~List()
    {
        clear();
    }

    // 3. Másoló konstruktor (Deep copy)
    List(const List<T> &other) : head(nullptr), tail(nullptr), count(0)
    {
        for (Node *curr = other.head; curr != nullptr; curr = curr->next)
        {
            push_back(curr->data);
        }
    }

    // Értékadó operátor (Deep copy copy-and-swap idiómával is lehetne, itt a hagyományos utat követjük)
    List<T> &operator=(const List<T> &other)
    {
        if (this != &other)
        {
            clear();
            for (Node *curr = other.head; curr != nullptr; curr = curr->next)
            {
                push_back(curr->data);
            }
        }
        return *this;
    }

    // ---------------------------------------------------------
    // LISTA FUNKCIÓK
    // ---------------------------------------------------------

    void push_back(const T &item)
    {
        Node *newNode = new Node(item, nullptr, tail);
        if (tail)
        {
            tail->next = newNode;
        }
        else
        {
            head = newNode;
        }
        tail = newNode;
        count++;
    }

    void erase(Iterator it)
    {
        Node *toDelete = it.current;
        if (!toDelete)
            return;

        if (toDelete->prev)
        {
            toDelete->prev->next = toDelete->next;
        }
        else
        {
            head = toDelete->next; // Ha a legelső elemet töröljük
        }

        if (toDelete->next)
        {
            toDelete->next->prev = toDelete->prev;
        }
        else
        {
            tail = toDelete->prev; // Ha a legutolsó elemet töröljük
        }

        delete toDelete;
        count--;
    }

    void clear()
    {
        Node *curr = head;
        while (curr != nullptr)
        {
            Node *nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    size_t getSize() const
    {
        return count;
    }

    // Módosítható bejáráshoz
    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }

    // Csak olvasható bejáráshoz
    ConstIterator begin() const { return ConstIterator(head); }
    ConstIterator end() const { return ConstIterator(nullptr); }

    // Indexelő operátor kivételkezeléssel
    T &operator[](size_t index)
    {
        if (index >= count)
        {
            throw std::out_of_range("List index out of range!");
        }
        Node *curr = head;
        for (size_t i = 0; i < index; ++i)
        {
            curr = curr->next;
        }
        return curr->data;
    }

    // Konstans indexelő operátor (read-only eléréshez)
    const T &operator[](size_t index) const
    {
        if (index >= count)
        {
            throw std::out_of_range("List index out of range!");
        }
        Node *curr = head;
        for (size_t i = 0; i < index; ++i)
        {
            curr = curr->next;
        }
        return curr->data;
    }
};

#endif // LIST_HPP
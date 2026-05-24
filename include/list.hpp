#ifndef LIST_HPP
#define LIST_HPP

/**
 * @file list.hpp
 * @brief Minimális kétirányú láncolt lista template a projekthez.
 */

#include <stdexcept>

/**
 * @brief Kis, projektcélú kétirányú láncolt lista template.
 * @tparam T Tárolt érték típusa.
 */
template <typename T>
class List
{
private:
    /**
    * @brief Belső csomópont típus, kívülről nem látható.
     */
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
    /**
    * @brief Módosítható iterator a Listhez.
     */
    class Iterator
    {
    private:
        Node *current;
        /**
         * @brief Lehetővé teszi, hogy a List hozzáférjen az iterator belsejéhez.
         */
        friend class List<T>;

    public:
        /**
         * @brief Iterator létrehozása egy csomópontmutatóból.
         */
        Iterator(Node *node = nullptr) : current(node) {}

        /**
         * @brief Az iterator dereferenciálása.
         */
        T &operator*()
        {
            return current->data;
        }

        /**
         * @brief Átlép a következő csomópontra.
         */
        Iterator &operator++()
        {
            if (current)
                current = current->next;
            return *this;
        }

        /**
         * @brief Két iterator egyenlőségének vizsgálata.
         */
        bool operator==(const Iterator &other) const
        {
            return current == other.current;
        }

        /**
         * @brief Két iterator különbözőségének vizsgálata.
         */
        bool operator!=(const Iterator &other) const
        {
            return current != other.current;
        }
    };

    /**
    * @brief Csak olvasható iterator a Listhez.
     */
    class ConstIterator {
    private:
        const Node* current;
        /**
         * @brief Lehetővé teszi, hogy a List hozzáférjen az iterator belsejéhez.
         */
        friend class List<T>; 

    public:
        /**
         * @brief Konstans iterator létrehozása egy csomópontmutatóból.
         */
        ConstIterator(const Node* node = nullptr) : current(node) {}

        /**
         * @brief Az iterator dereferenciálása.
         */
        const T& operator*() const {
            return current->data;
        }

        /**
         * @brief Átlép a következő csomópontra.
         */
        ConstIterator& operator++() {
            if (current) current = current->next;
            return *this;
        }

        /**
         * @brief Két iterator egyenlőségének vizsgálata.
         */
        bool operator==(const ConstIterator& other) const {
            return current == other.current;
        }

        /**
         * @brief Két iterator különbözőségének vizsgálata.
         */
        bool operator!=(const ConstIterator& other) const {
            return current != other.current;
        }
    };

    /**
    * @brief Üres lista létrehozása.
     */
    List() : head(nullptr), tail(nullptr), count(0) {}

    /**
    * @brief Megsemmisíti a listát és felszabadít minden csomópontot.
     */
    ~List()
    {
        clear();
    }

    /**
    * @brief Másoló konstruktor, amely mély másolatot készít.
     */
    List(const List<T> &other) : head(nullptr), tail(nullptr), count(0)
    {
        for (Node *curr = other.head; curr != nullptr; curr = curr->next)
        {
            push_back(curr->data);
        }
    }

    /**
    * @brief Értékadó operátor, amely mély másolatot készít.
     */
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

    /**
    * @brief Elem hozzáfűzése a lista végére.
     */
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

    /**
    * @brief Törli az iterator által mutatott elemet.
     */
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

    /**
    * @brief Töröl minden elemet a listából.
     */
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

    /**
    * @brief Visszaadja a tárolt elemek számát.
     */
    size_t getSize() const
    {
        return count;
    }

    /**
    * @brief Iterator az első elemre.
     */
    Iterator begin() { return Iterator(head); }

    /**
    * @brief Az end iterator visszaadása.
     */
    Iterator end() { return Iterator(nullptr); }

    /**
    * @brief Konstans iterator az első elemre.
     */
    ConstIterator begin() const { return ConstIterator(head); }

    /**
    * @brief A konstans end iterator visszaadása.
     */
    ConstIterator end() const { return ConstIterator(nullptr); }

    /**
    * @brief Visszaadja az adott indexű elemet.
     */
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

    /**
    * @brief Visszaadja az adott indexű elemet.
     */
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
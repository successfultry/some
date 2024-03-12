#pragma once
template <typename T>
class List
{
public:

    List() {};
    List(size_t n, T value)
    {
        Shared_ptr<list_struct<T>> current;
        for (int i = 0; i < n; i++)
        {
            if (head.get() == nullptr)
            {
                Shared_ptr<list_struct<T>> tmp(new list_struct<T>);
                tmp->elem = value;
                head = tmp;
                current = tmp;
                tail = tmp;
            }

            else
            {
                Shared_ptr<list_struct<T>> tmp(new list_struct<T>);
                tmp->elem = value;
                current->next = tmp;
                tmp->prev = current;
                current = tmp;
                tail = tmp;
            }
            
        }
    

    };

    ~List()
    {
        while (head.get() != nullptr)
        {
            head = head->next;
            if (head.get() != nullptr)
                head->prev = nullptr;

        }
        tail = nullptr;

    }

    T& front()
    {
        return head->elem;

    }

    void insert(Iterator<T> position, const T& val)
    {
        Shared_ptr<list_struct<T>> tmp(new list_struct<T>);
        tmp->elem = val;

        Shared_ptr<list_struct<T>> node = position.get();

        if (tail.get() == nullptr)
        {
            head = tmp;
            tail = tmp;
        }

        else if (node.get() == tail->next.get())
        {
            tail->next = tmp;
            tmp->prev = tail;
            tail = tmp;
        }

        else
        {
            tmp->next = node;
            tmp->prev = node->prev;
            if (node->prev.get() != nullptr)
                node->prev->next = tmp;
            else
                head = tmp;
            node->prev = tmp;
        }

    }

    void erase(Iterator<T> position)
    {
        Shared_ptr<list_struct<T>> node = position.get();

        if (head.get() == nullptr)
            return;

        else if (node.get() == head.get())
        {
            head = head->next;
            head->prev = nullptr;
            if (tail.get() == node.get())
            {
                tail = head;
            }
        }

        else if (node.get() == tail.get())
        {
            tail = tail->prev;
            tail->next = nullptr;

        }

        else
        {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        }
    }

    Iterator<T> Begin()
    {
        return Iterator<T>(head);
    };

    Iterator<T> End()
    {
        if (tail.get() == nullptr)
            return  Iterator<T>(tail);
        return Iterator<T>(tail->next);
    };

private:

    Shared_ptr<list_struct<T>> head = nullptr;
    Shared_ptr<list_struct<T>> tail = nullptr;

};

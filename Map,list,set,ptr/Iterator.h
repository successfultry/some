#pragma once


template <typename T>
struct list_struct
{
    T elem;
    Shared_ptr<list_struct<T>> next = nullptr;
    Shared_ptr<list_struct<T>> prev = nullptr;
};

template<typename T>
class Iterator
{
public:
    Iterator(Shared_ptr<list_struct<T>> t = nullptr) : it(t) {};

    bool operator==(const Iterator& r)
    {
        return it.get() == r.it.get();
    }

    bool operator!=(const Iterator& r)
    {
        return !(*this == r);
    }


    Shared_ptr<list_struct<T>>& operator++()
    {
        if (it.get() != nullptr)
        {
            it = it->next;
        }
        return it;
    }

    Shared_ptr<list_struct<T>>& operator--()
    {
        if (it != nullptr)
        {
            it = it->prev;
        }

        return it;
    }

    T& operator*()
    {
        return it->elem;
    }

    Shared_ptr<list_struct<T>> get()
    {
        return it;
    }

private:
    Shared_ptr<list_struct<T>> it;

};

#pragma once

template <typename T>
class Shared_ptr //
{
public:
    Shared_ptr(T* p = nullptr)
    {
        ptr = new struct Obj;
        ptr->data = p;
        ptr->counter = 1;
    };

    ~Shared_ptr()
    {
 

        if (ptr != nullptr)
        {

            ptr->counter -= 1;

            if (ptr->counter == 0)
            {
                if (ptr->data != nullptr)
                {
                    delete ptr->data;
                }
                delete ptr;
            }

            //               

        }
    };

    Shared_ptr(const Shared_ptr& rptr)
    {
        ptr = rptr.ptr;
        rptr.ptr->counter++;
    }

    T& operator*() const
    {
        return *(ptr->data);
    }

    T* operator->() const
    {
        return ptr->data;
    }

    Shared_ptr& operator=(Shared_ptr&& rptr)
    {
        if (ptr != nullptr)
        {
            ptr->counter--;

            if (ptr->counter == 0)
            {
                if (ptr->data != nullptr)
                {
                    delete ptr->data;
                }
                if (ptr != nullptr)
                {
                    delete ptr;
                }
                ptr = nullptr;
            }
        }
        ptr = rptr.ptr;
        rptr.ptr->counter++;
        return *this;
     
    }

    Shared_ptr& operator=(const Shared_ptr& rptr)
    {

        if (ptr != nullptr)
        {
            ptr->counter--;

            if (ptr->counter == 0)
            {
                if (ptr->data != nullptr)
                {
                    delete ptr->data;
                }
                if (ptr != nullptr)
                {
                    delete ptr;
                }
                ptr = nullptr;
            }
        }
        ptr = rptr.ptr;
        rptr.ptr->counter++;
        return *this;
 
    }

    T& operator[](size_t i) const
    {
        return (ptr->data)[i];
    }

    void reset(T* p = nullptr)
    {
        if (ptr->data != nullptr)
            delete ptr->data;
        ptr->data = p;
    }

    void swap(Shared_ptr& rptr);

    T* get() const { return ptr->data; }

private:
    struct Obj
    {
        int counter = 0;
        T* data;

    };

    struct Obj* ptr;

};


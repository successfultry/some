#pragma once
#include <iostream>
template <typename T>
class Unique_ptr//<T[]>
 {
    public:
        Unique_ptr(T* p = nullptr) : ptr(p)
        {
           // std::cout << "Constructor" << std::endl;
        };

        ~Unique_ptr()
        {
            //std::cout << "Destructor" << std::endl;
            if (ptr != nullptr)
                delete[]ptr;
        };

        T& operator*() const
        {
            return *ptr;
        }

        T* operator->() const
        {
            return ptr;
        }

        Unique_ptr& operator=(Unique_ptr&& rptr)
        {
            ptr = rptr.ptr;
            rptr.ptr = nullptr;
        }

        Unique_ptr& operator=(const Unique_ptr& rptr) = delete;

        T& operator[](size_t i) const
        {
            return ptr[i];
        }

        T* release()
        {
            T* tmp_p;
            tmp_p = ptr;
            ptr = nullptr;
            return tmp_p;
        }

        void reset(T* p = nullptr)
        {
            delete[] ptr;
            ptr = p;
        }

        void swap(Unique_ptr& rptr)
        {
            Unique_ptr tmp;
            tmp.ptr = rptr.ptr;
            rptr.ptr = ptr;
            ptr = tmp.ptr;
            tmp.ptr = nullptr;
        }

        T* get() const { return ptr; }

    private:
        T* ptr;

 };




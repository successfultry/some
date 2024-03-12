#pragma once
#include <iostream>
template <typename T>
struct set_struct
{
    T elem;
    Shared_ptr<set_struct<T>> left = nullptr;
    Shared_ptr<set_struct<T>> right = nullptr;
};

template <typename T>
class Set
{
public:

    Set() {};

    bool Find(const T& val, Shared_ptr<set_struct<T>>& parent, Shared_ptr<set_struct<T>>& current)
    {
        while (current.get() != nullptr)
        {
            if (val == current->elem)
            {
                return true;
            }

            else if (val < current->elem)
            {
                parent = current;
                current = current->left;
            }

            else
            {
                parent = current;
                current = current->right;
            }
        }

        return false;

    }

    bool insert(const T& val)
    {
        bool exist = true;
        Shared_ptr<set_struct<T>> tmp(new set_struct<T>);
        tmp->elem = val;

        if (root.get() == nullptr)
        {
            root = tmp;
            return exist;
        }

        else
        {
            Shared_ptr<set_struct<T>> current = root;
            Shared_ptr<set_struct<T>> parent = current;

            exist = Find(val, parent, current);

            if (exist)
            {
                return exist;
            }

            if (tmp->elem < parent->elem)
                parent->left = tmp;
            else if (tmp->elem > parent->elem)
                parent->right = tmp;
            return exist;
        }
    }

    size_t erase(const T& val)
    {
        size_t counter = 0;
        Shared_ptr<set_struct<T>> current = root;
        Shared_ptr<set_struct<T>> parent = current;

        bool exist = Find(val, parent, current);

        if (exist)
        {
            Shared_ptr<set_struct<T>> tmp = current;
            Shared_ptr<set_struct<T>> par = tmp;
            if (tmp->right.get() != nullptr)
            {
            
                tmp = tmp->right;
                bool flag_left = false;
                while (tmp->left.get() != nullptr)
                {
                    par = tmp;
                    tmp = tmp->left;
                    flag_left = true;
                }

                current->elem = tmp->elem;

                if (flag_left)
                {
                    par->left = tmp->right;
                }
                else
                {
                    par->right = tmp->right;
                }
            }


            else if (tmp->left.get() != nullptr)
            {
                tmp = tmp->left;
                bool flag_right = false;
                while (tmp->right.get() != nullptr)
                {
                    par = tmp;
                    tmp = tmp->right;
                    flag_right = true;
                }

                current->elem = tmp->elem;

                if (flag_right)
                {
                    par->right = tmp->left;
                }
                else
                {
                    par->left = tmp->left;
                }

            }
            return ++counter;
        }
        return counter;
    }



    void PrintSet()
    {
        Print(root);
    }

private:
    Shared_ptr<set_struct<T>> root = nullptr;

private:
    void Print(const Shared_ptr<set_struct<T>>& p) const
    {
        if (p.get() != nullptr)
        {
            std::cout << p->elem << std::endl;
            Print(p->left);
            Print(p->right);
        }
    }
};

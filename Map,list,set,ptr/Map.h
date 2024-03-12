#pragma once
#include <iostream>
template <typename T,typename U>
struct map_struct
{
    T key_;
    U values_;
    Shared_ptr<map_struct<T, U>> left = nullptr;
    Shared_ptr<map_struct<T, U>> right = nullptr;
};

template <typename T, typename U>
class Map
{
public:

    Map() {};

    bool insert(const T& key, const U& value)
    {
        Shared_ptr<map_struct<T, U>> node;
        bool existed = try_to_get(key, node);
        if (!existed)
        {
            node->values_ = value;
        }
        return existed;
    }

    U& operator[](const T& key)
    {
        Shared_ptr<map_struct<T, U>> node;
        try_to_get(key, node);

        return node->values_;
    }

    

    size_t erase(const T& key)
    {
        size_t counter = 0;
        Shared_ptr<map_struct<T, U>> current = root;
        Shared_ptr<map_struct<T, U>> parent = current;

        bool exist = Find(key, parent, current);

        if (exist)
        {
            Shared_ptr<map_struct<T, U>> tmp = current;
            Shared_ptr<map_struct<T, U>> par = tmp;
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

                current->key_ = tmp->key_;
                current->values_ = tmp->values_;

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

                current->key_ = tmp->key_;
                current->values_ = tmp->values_;

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

    
    void PrintMap()
    {
        Print(root);
    }

private:
    Shared_ptr<map_struct<T,U>> root = nullptr;

private:
    void Print(const Shared_ptr<map_struct<T,U>>& p) const
    {
        if (p.get() != nullptr)
        {
            std::cout << p->key_ << ":" << p->values_ << std::endl;
            Print(p->left);
            Print(p->right);
        }
    }

    bool Find(const T& key, Shared_ptr<map_struct<T, U>>& parent, Shared_ptr<map_struct<T, U>>& current)
    {
        while (current.get() != nullptr)
        {
            if (key == current->key_)
            {
                return true;
            }

            else if (key < current->key_)
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


    bool try_to_get(const T& key, Shared_ptr<map_struct<T, U>>& node)
    {
        Shared_ptr<map_struct<T, U>> tmp(new map_struct<T, U>);
        tmp->key_ = key;
        node = tmp;

        if (root.get() == nullptr)
        {
            root = tmp;
            return false;
        }

        else
        {
            Shared_ptr<map_struct<T, U>> current = root;
            Shared_ptr<map_struct<T, U>> parent = current;

            bool existed = Find(key, parent, current);

            if (!existed)
            {
                if (tmp->key_ < parent->key_)
                    parent->left = tmp;
                else if (tmp->key_ > parent->key_)
                    parent->right = tmp;
                return existed;
            }

            node = current;
            return existed;
        }
    }
};


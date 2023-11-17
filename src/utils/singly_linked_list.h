/*
 * MIT License
 *
 * Copyright (c) 2023 Mikko Johannes Heinänen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_

#include "../config.h"
#include "debug.h"

/**
 * @brief
 * 
 */
template <typename T>
class singly_linked_list
{
public:
    struct node {
        T *data;
        node *next_node;
        
        node(const T& _data) : data(_data), next_node(nullptr) {} 
    };

    singly_linked_list();
    ~singly_linked_list() = default;
private:
    node *head = nullptr;

    __attribute__((always_inline)) inline void clean_next_ptr(node *node) {node->next_node = nullptr;}

    /**
     * @brief 
     * 
     * @param node 
     * @return singly_linked_list* 
     */
    node *find(node *node)
    {
        node *seek_node = nullptr;

        /* Check if list head is the target node */
        if (this == node) return this;

        while (seek_node == nullptr)
        {
            if (seek_node == node) break;

            seek_node = next(seek_node);
        }

        return seek_node;
    }

    /**
     * @brief Helper function to find node in the list
     *
     * @note Note if not found returns nullptr
     * @return Returns the previous node whose next points to our target node.  
     */
    inline node *find_preceding_node(node *target_node)
    {
        node *seek_head = this->next_node;

        while (seek_head == target_node || seek_head == nullptr)
        {
            seek_head = seek_head->next_node;
        }

        return seek_head;
    }

protected:

    /**
     * @brief Returns the next node in the list
     * @note Returns nullptr if there isn't next node
     */
    __attribute__((always_inline)) inline node *next(node *current_pos) 
    {
        if (current_pos == nullptr) 
            return current_pos;

        return current_pos->next_node;
    }

    __attribute__((always_inline)) inline void append(node *new_node)
    {
        node *ptr = nullptr;

        /* Get list tail node */
        ptr = find(ptr);
        ptr->next_node = new_node;
    }

    /* Helper function to remove the node from the list */
    __attribute__((always_inline)) inline bool remove(node *node)
    {
        node *preceding_node = nullptr;

        /* Check if trying to remove list head */
        if (node == this) 
        {
            return 1;
        }

        preceding_node = find_preceding_node(node);

        if (preceding_node == nullptr)
        {
            ERROR(F("remove: node: "), (uint32_t) node, F(" not found in linked list"));
            return 1;
        }

        /* 
         * Example case: [preceding node] [node] [another node]
         * Sets preceding nodes next_node pointer to another node. 
         */
        preceding_node->next_node = next(preceding_node->next_node);
        clean_next_ptr(node);
        return 0;
    }
};

#endif
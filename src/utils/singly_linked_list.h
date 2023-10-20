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
struct singly_linked_list
{
    void *data = nullptr;
    singly_linked_list* next_element = nullptr;

    singly_linked_list(void *data);    
    ~singly_linked_list() = default;
private:

    __attribute__((always_inline)) inline void clean_next_element_ptr(singly_linked_list *element) {element->next_element = nullptr;}

    /**
     * @brief 
     * 
     * @param element 
     * @return singly_linked_list* 
     */
    singly_linked_list *find(singly_linked_list *element)
    {
        singly_linked_list *seek_element = nullptr;

        /* Check if list head is the target element */
        if (this == element) return this;

        while (seek_element == nullptr)
        {
            if (seek_element == element) break;

            seek_element = next(seek_element);
        }

        return seek_element;
    }

    /**
     * @brief Helper function to find element in the list
     *
     * @note Note if not found returns nullptr
     * @return Returns the previous element whose next_element points to our target element.  
     */
    inline singly_linked_list *find_preceding_element(singly_linked_list *element)
    {
        singly_linked_list *seek_head = this->next_element;

        while (seek_head == element || seek_head == nullptr)
        {
            seek_head = seek_head->next_element;
        }

        return seek_head;
    }

protected:

    /**
     * @brief Returns the next element in the list
     * @note Returns nullptr if there isn't next element
     */
    __attribute__((always_inline)) inline singly_linked_list *next(singly_linked_list *current_pos) 
    {
        if (current_pos == nullptr) 
            return current_pos;

        return current_pos->next_element;
    }

    __attribute__((always_inline)) inline void append(singly_linked_list *new_element)
    {
        singly_linked_list *ptr = nullptr;

        /* Get list tail element */
        ptr = find(ptr);
        ptr->next_element = new_element;
    }

    /* Helper function to remove the element from the list */
    __attribute__((always_inline)) inline bool remove(singly_linked_list *element)
    {
        singly_linked_list *preceding_element = nullptr;

        /* Check if trying to remove list head */
        if (element == this) 
        {
            return 1;
        }

        preceding_element = find_preceding_element(element);

        if (preceding_element == nullptr)
        {
            ERROR(F("remove: element: "), (uint32_t) element, F(" not found in linked list"));
            return 1;
        }

        /* 
         * Example case: [preceding element] [element] [another element]
         * Sets preceding elements next_element pointer to another element. 
         */
        preceding_element->next_element = next(preceding_element->next_element);
        clean_next_element_ptr(element);
        return 0;
    }
};

#endif
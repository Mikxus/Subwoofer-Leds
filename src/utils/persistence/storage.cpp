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
#include "storage.h"

bool storage::validate_index(uint16_t &index)
{

    if (persistence_backend == 0)
    {
        ERROR(F("Operation failed: No storage persistence_backend"));
    }

    index += persistence_backend.m_min_pos;
    if (index > persistence_backend.m_max_pos)
    {
        ERROR(F("Operation out of storage range. \n\rindex: "),
              index - persistence_backend.m_min_pos,
              F("\n\rEnd range: "),
              persistence_backend.m_max_pos - persistence_backend.m_min_pos);
        return 0;
    }
    return 1;
}

uint8_t storage::read(uint16_t index)
{
    if (!validate_index(index))
        return 0;

    return persistence_backend.read(index);
}

void storage::write(uint16_t index, uint8_t input)
{
    if (!validate_index(index))
        return;

    persistence_backend.write(index, input);
}

void storage::update(uint16_t index, uint8_t input)
{
    if (!validate_index(index))
        return;

    persistence_backend.update(index, input);
}

storage settings_storage;
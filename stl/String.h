#pragma once

#include <stdio.h>

struct String
{
    String(): m_str(nullptr), m_size(0) {}

    String(char* str): m_str(str), m_size(sizeof(m_str)/sizeof(char)){

    }

    private:
        char* m_str;
        size_t m_size;
};

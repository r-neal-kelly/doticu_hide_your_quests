/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "intrinsic.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    class Quest_And_Label_t
    {
    public:
        static Bool_t   Has(Vector_t<Quest_And_Label_t>& vector, some<Quest_t*> quest);
        static void     Sort(Vector_t<Quest_And_Label_t>& vector);

    public:
        some<Quest_t*>  quest;
        String_t        label;

    public:
        Quest_And_Label_t(some<Quest_t*> quest);
        Quest_And_Label_t(const Quest_And_Label_t& other);
        Quest_And_Label_t(Quest_And_Label_t&& other) noexcept;
        Quest_And_Label_t& operator =(const Quest_And_Label_t& other);
        Quest_And_Label_t& operator =(Quest_And_Label_t&& other) noexcept;
        ~Quest_And_Label_t();
    };

}}

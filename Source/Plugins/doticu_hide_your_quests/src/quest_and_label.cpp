/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/player.h"
#include "doticu_skylib/quest.h"

#include "quest_and_label.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    Bool_t Quest_And_Label_t::Has(Vector_t<Quest_And_Label_t>& vector, some<Quest_t*> quest)
    {
        for (size_t idx = 0, end = vector.size(); idx < end; idx += 1) {
            if (vector[idx].quest == quest) {
                return true;
            }
        }
        return false;
    }

    void Quest_And_Label_t::Sort(Vector_t<Quest_And_Label_t>& vector)
    {
        vector.Sort(
            [](Quest_And_Label_t& a, Quest_And_Label_t& b)->Int_t
            {
                Bool_t a_is_finished = a.quest->Is_Completed_Or_Failed();
                Bool_t b_is_finished = b.quest->Is_Completed_Or_Failed();
                if (a_is_finished && !b_is_finished) {
                    return Comparator_e::IS_UNORDERED;
                } else if (!a_is_finished && b_is_finished) {
                    return Comparator_e::IS_ORDERED;
                } else {
                    Bool_t a_is_misc = a.quest->quest_type == Quest_Type_e::MISC;
                    Bool_t b_is_misc = b.quest->quest_type == Quest_Type_e::MISC;
                    if (a_is_misc && !b_is_misc) {
                        return Comparator_e::IS_UNORDERED;
                    } else if (!a_is_misc && b_is_misc) {
                        return Comparator_e::IS_ORDERED;
                    } else {
                        Comparator_e result = Form_t::Compare_Names(a.label, b.label);
                        if (result == Comparator_e::IS_EQUAL) {
                            return a.quest->form_id - b.quest->form_id;
                        } else {
                            return result;
                        }
                    }
                }
            }
        );
    }

    Quest_And_Label_t::Quest_And_Label_t(some<Quest_t*> quest) :
        quest(quest)
    {
        SKYLIB_ASSERT_SOME(this->quest);

        if (this->quest->quest_type == Quest_Type_e::MISC) {
            maybe<Player_Objective_t> objective = Player_t::Self()->Highest_Displayed_Player_Objective(this->quest);
            if (objective.Has_Value()) {
                label = objective().Parse_Display_Text();
            } else {
                label = this->quest->Any_Name();
            }
        } else {
            label = this->quest->Any_Name();
        }
    }

    Quest_And_Label_t::Quest_And_Label_t(const Quest_And_Label_t& other) :
        quest(other.quest), label(other.label)
    {
    }

    Quest_And_Label_t::Quest_And_Label_t(Quest_And_Label_t&& other) noexcept :
        quest(std::move(other.quest)), label(std::move(other.label))
    {
    }

    Quest_And_Label_t& Quest_And_Label_t::operator =(const Quest_And_Label_t& other)
    {
        if (this != std::addressof(other)) {
            this->quest = other.quest;
            this->label = other.label;
        }
        return *this;
    }

    Quest_And_Label_t& Quest_And_Label_t::operator =(Quest_And_Label_t&& other) noexcept
    {
        if (this != std::addressof(other)) {
            this->quest = std::move(other.quest);
            this->label = std::move(other.label);
        }
        return *this;
    }

    Quest_And_Label_t::~Quest_And_Label_t()
    {
    }

}}

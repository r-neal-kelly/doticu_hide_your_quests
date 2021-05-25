/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "consts.h"
#include "mcm.h"
#include "quest_and_label.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    template <typename T>
    inline void MCM_t::Build_Page(T& self, Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = self.MCM();

        self.Reset_Option_State();

        if (!is_refresh) {
            self.Reset_Items_State();
        }

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        const Vector_t<Quest_And_Label_t>& items = self.items_state.Items();
        size_t item_count = items.size();
        if (item_count > 0) {
            Int_t page_count = self.items_state.Page_Count();
            Int_t page_index = self.items_state.Page_Index();

            mcm->Title_Text(mcm->Page_Title(T::PAGE_NAME, item_count, page_index, page_count));

            if (page_count > 1) {
                self.option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "");
                self.option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "");
            } else {
                self.option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
                self.option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
            }

            mcm->Add_Header_Option("");
            mcm->Add_Header_Option("");

            Int_t idx = T::ITEMS_PER_PAGE * page_index;
            Int_t end = idx + T::ITEMS_PER_PAGE;
            if (end > item_count) {
                end = item_count;
            }
            for (; idx < end; idx += 1) {
                const Quest_And_Label_t& item = items[idx];
                mcm->Add_Toggle_Option(item.label, !mcm->Has_Hidden_Quest(item.quest));
            }
        } else {
            mcm->Title_Text(mcm->Page_Title(Const::String::$CURRENT, 0, 0, 1));

            self.option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
            self.option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);

            mcm->Add_Header_Option(Const::String::NO_QUESTS);
        }
    }

    template <typename T>
    inline void MCM_t::Handle_On_Option_Select(T& self, Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = self.MCM();

        if (option == self.option_state.previous) {
            mcm->Disable_Option(option);
            self.items_state.Previous_Page();
            mcm->Reset_Page();
        } else if (option == self.option_state.next) {
            mcm->Disable_Option(option);
            self.items_state.Next_Page();
            mcm->Reset_Page();
        } else {
            const Vector_t<Quest_And_Label_t> items = self.items_state.Items();
            size_t item_count = items.size();
            maybe<size_t> item_index = mcm->Item_Index(
                option,
                item_count,
                self.items_state.Page_Index(),
                T::HEADERS_PER_PAGE,
                T::ITEMS_PER_PAGE
            );
            if (item_index.Has_Value()) {
                some<Quest_t*> quest = items[item_index.Value()].quest;
                if (mcm->Has_Hidden_Quest(quest)) {
                    mcm->Remove_Hidden_Quest(quest);
                    mcm->Toggle(option, true);
                } else {
                    mcm->Add_Hidden_Quest(quest);
                    mcm->Toggle(option, false);
                }
                mcm->Enforce_Hidden_Objectives();
            }
        }
    }

}}

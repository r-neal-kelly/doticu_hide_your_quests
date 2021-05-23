/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/game.h"
#include "doticu_skylib/math.h"
#include "doticu_skylib/player.h"
#include "doticu_skylib/quest_objective.h"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_misc.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    MCM_Misc_t::Save_State_t::Save_State_t() :
        page_index(DEFAULT_PAGE_INDEX)
    {
    }

    MCM_Misc_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<Int_t>& MCM_Misc_t::Save_State_t::Page_Index()
    {
        DEFINE_VARIABLE_REFERENCE(Int_t, "page_index");
    }

    void MCM_Misc_t::Save_State_t::Read()
    {
        this->page_index = Page_Index();
    }

    void MCM_Misc_t::Save_State_t::Write()
    {
        Page_Index() = static_cast<Int_t>(this->page_index);
    }

    MCM_Misc_t::Option_State_t::Option_State_t() :
        previous(-1),
        next(-1)
    {
    }

    MCM_Misc_t::Option_State_t::~Option_State_t()
    {
    }

    MCM_Misc_t::Items_State_t::Items_State_t() :
        items(),
        do_update(true)
    {
    }

    MCM_Misc_t::Items_State_t::~Items_State_t()
    {
    }

    const Vector_t<Quest_And_Label_t>& MCM_Misc_t::Items_State_t::Items()
    {
        if (this->do_update) {
            this->do_update = false;

            this->items.clear();

            Vector_t<some<Quest_Objective_t*>> objectives;
            {
                Read_Locker_t locker(Game_t::Form_IDs_To_Forms_Lock());
                Player_t::Self()->Quest_Objectives(objectives, locker);
            }
            for (size_t idx = 0, end = objectives.size(); idx < end; idx += 1) {
                some<Quest_Objective_t*> objective = objectives[idx];
                maybe<Quest_t*> quest = objective->quest;
                if (quest && !Quest_And_Label_t::Has(this->items, quest()) && MCM_t::Is_Objective_Hidable(objective)) {
                    if (quest->quest_type == Quest_Type_e::MISC) {
                        this->items.push_back({ quest() });
                    }
                }
            }

            Quest_And_Label_t::Sort(this->items);
        }

        return this->items;
    }

    size_t MCM_Misc_t::Items_State_t::Page_Count()
    {
        return MCM_t::Page_Count(this->items.size(), ITEMS_PER_PAGE);
    }

    size_t MCM_Misc_t::Items_State_t::Page_Index()
    {
        return MCM_t::Page_Index(save_state.page_index, Page_Count());
    }

    size_t MCM_Misc_t::Items_State_t::Previous_Page()
    {
        return MCM_t::Previous_Page(save_state.page_index, Page_Count(), this->items.size());
    }

    size_t MCM_Misc_t::Items_State_t::Next_Page()
    {
        return MCM_t::Next_Page(save_state.page_index, Page_Count(), this->items.size());
    }

    MCM_Misc_t::Save_State_t    MCM_Misc_t::save_state;
    MCM_Misc_t::Option_State_t  MCM_Misc_t::option_state;
    MCM_Misc_t::Items_State_t   MCM_Misc_t::items_state;

    some<MCM_t*> MCM_Misc_t::MCM()
    {
        return MCM_t::Self();
    }

    some<MCM_Misc_t*> MCM_Misc_t::Self()
    {
        return Const::Quest::MCM();
    }

    String_t MCM_Misc_t::Class_Name()
    {
        DEFINE_CLASS_NAME("doticu_hide_your_quests_misc");
    }

    some<Virtual::Class_t*> MCM_Misc_t::Class()
    {
        DEFINE_CLASS();
    }

    some<Virtual::Object_t*> MCM_Misc_t::Object()
    {
        DEFINE_COMPONENT_OBJECT_METHOD(Self()());
    }

    void MCM_Misc_t::Reset_Save_State()
    {
        save_state.~Save_State_t();
        new (&save_state) Save_State_t;
    }

    void MCM_Misc_t::Reset_Option_State()
    {
        option_state.~Option_State_t();
        new (&option_state) Option_State_t;
    }

    void MCM_Misc_t::Reset_Items_State()
    {
        items_state.~Items_State_t();
        new (&items_state) Items_State_t;
    }

    void MCM_Misc_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        using type_name = MCM_Misc_t;

        SKYLIB_ASSERT_SOME(v_machine);

        String_t class_name = Class_Name();

        #define STATIC(STATIC_NAME_, WAITS_FOR_FRAME_, RETURN_TYPE_, STATIC_, ...)  \
        SKYLIB_M                                                                    \
            BIND_STATIC(v_machine, class_name, STATIC_NAME_, WAITS_FOR_FRAME_,      \
                        RETURN_TYPE_, STATIC_, __VA_ARGS__);                        \
        SKYLIB_W

        #undef STATIC

        #define METHOD(METHOD_NAME_, WAITS_FOR_FRAME_, RETURN_TYPE_, METHOD_, ...)          \
        SKYLIB_M                                                                            \
            BIND_METHOD(v_machine, class_name, type_name, METHOD_NAME_, WAITS_FOR_FRAME_,   \
                        RETURN_TYPE_, METHOD_, __VA_ARGS__);                                \
        SKYLIB_W

        #undef METHOD
    }

    void MCM_Misc_t::On_After_New_Game()
    {
        Reset_Save_State();
        Reset_Option_State();
        Reset_Items_State();
    }

    void MCM_Misc_t::On_Before_Save_Game()
    {
        save_state.Write();
    }

    void MCM_Misc_t::On_After_Save_Game()
    {
    }

    void MCM_Misc_t::On_Before_Load_Game()
    {
    }

    void MCM_Misc_t::On_After_Load_Game()
    {
        Reset_Save_State();
        Reset_Option_State();
        Reset_Items_State();

        save_state.Read();
    }

    void MCM_Misc_t::On_Update()
    {
    }

    void MCM_Misc_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
    }

    void MCM_Misc_t::On_Config_Open()
    {
    }

    void MCM_Misc_t::On_Config_Close()
    {
    }

    void MCM_Misc_t::On_Page_Open(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        Reset_Option_State();

        if (!is_refresh) {
            Reset_Items_State();
        }

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        const Vector_t<Quest_And_Label_t>& items = items_state.Items();
        size_t item_count = items.size();
        if (item_count > 0) {
            Int_t page_count = items_state.Page_Count();
            Int_t page_index = items_state.Page_Index();

            mcm->Title_Text(mcm->Page_Title(Const::String::$MISC, item_count, page_index, page_count));

            if (page_count > 1) {
                option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "");
                option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "");
            } else {
                option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
                option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
            }

            mcm->Add_Header_Option("");
            mcm->Add_Header_Option("");

            Int_t idx = ITEMS_PER_PAGE * page_index;
            Int_t end = idx + ITEMS_PER_PAGE;
            if (end > item_count) {
                end = item_count;
            }
            for (; idx < end; idx += 1) {
                const Quest_And_Label_t& item = items[idx];
                if (item.quest->Is_Completed_Or_Failed()) {
                    mcm->Add_Toggle_Option(item.label, !mcm->Has_Hidden_Objective(item.quest));
                } else {
                    mcm->Add_Toggle_Option(item.label, !mcm->Has_Hidden_Objective(item.quest));
                }
            }
        } else {
            mcm->Title_Text(mcm->Page_Title(Const::String::CURRENT, 0, 0, 1));

            option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
            option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);

            mcm->Add_Header_Option("");
            mcm->Add_Header_Option("");

            mcm->Add_Header_Option(Const::String::NO_QUESTS);
        }
    }

    void MCM_Misc_t::On_Option_Select(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.previous) {
            mcm->Disable_Option(option);
            items_state.Previous_Page();
            mcm->Reset_Page();
        } else if (option == option_state.next) {
            mcm->Disable_Option(option);
            items_state.Next_Page();
            mcm->Reset_Page();
        } else {
            const Vector_t<Quest_And_Label_t> items = items_state.Items();
            size_t item_count = items.size();
            maybe<size_t> item_index = mcm->Item_Index(
                option,
                item_count,
                items_state.Page_Index(),
                HEADERS_PER_PAGE,
                ITEMS_PER_PAGE
            );
            if (item_index.Has_Value()) {
                some<Quest_t*> quest = items[item_index.Value()].quest;
                if (mcm->Has_Hidden_Objective(quest)) {
                    mcm->Remove_Hidden_Objective(quest);
                    mcm->Toggle(option, true);
                } else {
                    mcm->Add_Hidden_Objective(quest);
                    mcm->Toggle(option, false);
                }
                mcm->Enforce_Hidden_Objectives();
            }
        }
    }

    void MCM_Misc_t::On_Option_Menu_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Misc_t::On_Option_Menu_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
    }

    void MCM_Misc_t::On_Option_Slider_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Misc_t::On_Option_Slider_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Float_t value)
    {
    }

    void MCM_Misc_t::On_Option_Input_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, String_t value)
    {
    }

    void MCM_Misc_t::On_Option_Keymap_Change(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
    }

    void MCM_Misc_t::On_Option_Default(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Misc_t::On_Option_Highlight(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {

    }

}}

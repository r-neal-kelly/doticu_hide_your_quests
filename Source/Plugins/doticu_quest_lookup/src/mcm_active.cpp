/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/player.h"
#include "doticu_skylib/quest_objective.h"
#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_active.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    MCM_Active_t::Save_State_t::Save_State_t() :
        current_view(DEFAULT_CURRENT_VIEW),

        list_current_page(DEFAULT_LIST_CURRENT_PAGE),

        item_current(none<Quest_t*>())
    {
    }

    MCM_Active_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<String_t>& MCM_Active_t::Save_State_t::Current_View()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "current_view");
    }

    Virtual::Variable_tt<Int_t>& MCM_Active_t::Save_State_t::List_Current_Page()
    {
        DEFINE_VARIABLE_REFERENCE(Int_t, "list_current_page");
    }

    Virtual::Variable_tt<maybe<Quest_t*>>& MCM_Active_t::Save_State_t::Item_Current()
    {
        DEFINE_VARIABLE_REFERENCE(maybe<Quest_t*>, "item_current");
    }

    void MCM_Active_t::Save_State_t::Read()
    {
        this->current_view = MCM_View_e::From_String(static_cast<String_t>(Current_View()));

        this->list_current_page = List_Current_Page();

        this->item_current = Item_Current();
    }

    void MCM_Active_t::Save_State_t::Write()
    {
        Current_View() = this->current_view().As_String();

        List_Current_Page() = this->list_current_page;

        Item_Current() = this->item_current;
    }

    MCM_Active_t::Option_State_t::Option_State_t() :
        filter(-1),
        options(-1),
        previous(-1),
        next(-1)
    {
    }

    MCM_Active_t::Option_State_t::~Option_State_t()
    {
    }

    MCM_Active_t::List_State_t::List_State_t() :
        items()
    {
    }

    MCM_Active_t::List_State_t::~List_State_t()
    {
    }

    Vector_t<some<Quest_t*>>& MCM_Active_t::List_State_t::Items()
    {
        this->items.clear();

        return this->items;
    }

    MCM_Active_t::Save_State_t      MCM_Active_t::save_state;
    MCM_Active_t::Option_State_t    MCM_Active_t::option_state;
    MCM_Active_t::List_State_t      MCM_Active_t::list_state;

    some<MCM_t*> MCM_Active_t::MCM()
    {
        return MCM_t::Self();
    }

    some<MCM_Active_t*> MCM_Active_t::Self()
    {
        return Const::Quest::MCM();
    }

    String_t MCM_Active_t::Class_Name()
    {
        DEFINE_CLASS_NAME("doticu_quest_lookup_mcm_active");
    }

    some<Virtual::Class_t*> MCM_Active_t::Class()
    {
        DEFINE_CLASS();
    }

    some<Virtual::Object_t*> MCM_Active_t::Object()
    {
        DEFINE_COMPONENT_OBJECT_METHOD(Self()());
    }

    void MCM_Active_t::Reset_Save_State()
    {
        save_state.~Save_State_t();
        new (&save_state) Save_State_t;
    }

    void MCM_Active_t::Reset_Option_State()
    {
        option_state.~Option_State_t();
        new (&option_state) Option_State_t;
    }

    void MCM_Active_t::Reset_List_State()
    {
        list_state.~List_State_t();
        new (&list_state) List_State_t;
    }

    some<MCM_View_e> MCM_Active_t::Current_View()
    {
        if (!save_state.current_view) {
            save_state.current_view = DEFAULT_CURRENT_VIEW;
        }
        return save_state.current_view();
    }

    void MCM_Active_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        using type_name = MCM_Active_t;

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

    void MCM_Active_t::On_After_New_Game()
    {
        Reset_Save_State();
    }

    void MCM_Active_t::On_Before_Save_Game()
    {
        save_state.Write();
    }

    void MCM_Active_t::On_After_Save_Game()
    {
    }

    void MCM_Active_t::On_Before_Load_Game()
    {
    }

    void MCM_Active_t::On_After_Load_Game()
    {
        Reset_Save_State();

        save_state.Read();
    }

    void MCM_Active_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
    }

    void MCM_Active_t::On_Config_Open()
    {
    }

    void MCM_Active_t::On_Config_Close()
    {
    }

    void MCM_Active_t::On_Page_Open(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        Reset_Option_State();

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)       On_Page_Open_List(std::move(latent_id), is_refresh);
        else if (current_view == MCM_View_e::ITEM)  On_Page_Open_Item(std::move(latent_id), is_refresh);
    }

    void MCM_Active_t::On_Option_Select(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Active_t::On_Option_Menu_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Active_t::On_Option_Menu_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
    }

    void MCM_Active_t::On_Option_Slider_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Active_t::On_Option_Slider_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Float_t value)
    {
    }

    void MCM_Active_t::On_Option_Input_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, String_t value)
    {
    }

    void MCM_Active_t::On_Option_Keymap_Change(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
    }

    void MCM_Active_t::On_Option_Default(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Active_t::On_Option_Highlight(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Active_t::On_Page_Open_List(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        Vector_t<some<Quest_t*>>& items = list_state.Items();

        //temp
        // we shall lock using the global form hash map and then we can do what we want to player structure.
        some<Player_t*> player = Player_t::Self();
        size_t alterable_count = 0;
        for (size_t idx = 0, end = player->objectives.Count(); idx < end; idx += 1) {
            Player_Objective_t& objective = player->objectives[idx];
            if (objective.objective) {
                // COMPLETED_AND_DISPLAYED's, FAILED_AND_DISPLAYED's, and DISPLAYED's two states naturally match.
                // this means we can just look at the actual objective's state to determine if this is one we can alter.
                // And of course, we should have one source of truth, but we can't depend on the player objectives
                // to tell us whether or not it is hidden, only that it is on the player and what deeper state it has.
                // we must keep the state of the shallow state ourselves, perhaps in a hash map, but a vector is probably good enough.
                // We need to be able to add and remove from our state to match the player objectives, if it no longer exists
                // or if it has a different deeper state, which we shall not change.
                // when do we update state? everytime we read from player?
                if (objective.objective->state == Quest_Objective_State_e::COMPLETED_AND_DISPLAYED ||
                    objective.objective->state == Quest_Objective_State_e::FAILED_AND_DISPLAYED ||
                    objective.objective->state == Quest_Objective_State_e::DISPLAYED) {
                    alterable_count += 1;
                    objective.Log(SKYLIB_TAB);
                    objective.state = Quest_Objective_State_e::DORMANT;
                    // for our state, we need to store quest, objective index, instance id, and whether or not it's hidden.
                    // the only one I question is instance id. maybe we should just operate on all of them in unison.
                }
            }
        }
        _MESSAGE("alterable player_objective_count: %u", alterable_count);
        // we'll use a vector. there's not enough elements to warrant all the trouble of using a hash map and filtering and sorting it.
        // I do think we should have the quest on the upper level and it's item view will have all of its objectives.

        mcm->Title_Text(Const::String::ACTIVE);
    }

    void MCM_Active_t::On_Page_Open_Item(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Title_Text("WIP");
    }

}}

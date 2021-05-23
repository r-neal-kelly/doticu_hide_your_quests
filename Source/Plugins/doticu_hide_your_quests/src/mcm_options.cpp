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
#include "mcm_options.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    MCM_Options_t::Save_State_t::Save_State_t() :
        show_new_objectives(DEFAULT_SHOW_NEW_OBJECTIVES),
        show_finished_quests(DEFAULT_SHOW_FINISHED_QUESTS)
    {
    }

    MCM_Options_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<Bool_t>& MCM_Options_t::Save_State_t::Show_New_Objectives()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "show_new_objectives");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Options_t::Save_State_t::Show_Finished_Quests()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "show_finished_quests");
    }

    void MCM_Options_t::Save_State_t::Read()
    {
        this->show_new_objectives = Show_New_Objectives();
        this->show_finished_quests = Show_Finished_Quests();
    }

    void MCM_Options_t::Save_State_t::Write()
    {
        Show_New_Objectives() = this->show_new_objectives;
        Show_Finished_Quests() = this->show_finished_quests;
    }

    MCM_Options_t::Option_State_t::Option_State_t() :
        reset(-1),
        show_new_objectives(-1),
        show_finished_quests(-1)
    {
    }

    MCM_Options_t::Option_State_t::~Option_State_t()
    {
    }

    MCM_Options_t::Save_State_t     MCM_Options_t::save_state;
    MCM_Options_t::Option_State_t   MCM_Options_t::option_state;

    some<MCM_t*> MCM_Options_t::MCM()
    {
        return MCM_t::Self();
    }

    some<MCM_Options_t*> MCM_Options_t::Self()
    {
        return Const::Quest::MCM();
    }

    String_t MCM_Options_t::Class_Name()
    {
        DEFINE_CLASS_NAME("doticu_hide_your_quests_options");
    }

    some<Virtual::Class_t*> MCM_Options_t::Class()
    {
        DEFINE_CLASS();
    }

    some<Virtual::Object_t*> MCM_Options_t::Object()
    {
        DEFINE_COMPONENT_OBJECT_METHOD(Self()());
    }

    void MCM_Options_t::Reset_Save_State()
    {
        save_state.~Save_State_t();
        new (&save_state) Save_State_t;
    }

    void MCM_Options_t::Reset_Option_State()
    {
        option_state.~Option_State_t();
        new (&option_state) Option_State_t;
    }

    Bool_t MCM_Options_t::Do_Show_New_Objectives()
    {
        return save_state.show_new_objectives;
    }

    void MCM_Options_t::Do_Show_New_Objectives(Bool_t value)
    {
        save_state.show_new_objectives = value;
    }

    Bool_t MCM_Options_t::Do_Show_Finished_Quests()
    {
        return save_state.show_finished_quests;
    }

    void MCM_Options_t::Do_Show_Finished_Quests(Bool_t value)
    {
        save_state.show_finished_quests = value;
    }

    void MCM_Options_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        using type_name = MCM_Options_t;

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

    void MCM_Options_t::On_After_New_Game()
    {
        Reset_Save_State();
        Reset_Option_State();
    }

    void MCM_Options_t::On_Before_Save_Game()
    {
        save_state.Write();
    }

    void MCM_Options_t::On_After_Save_Game()
    {
    }

    void MCM_Options_t::On_Before_Load_Game()
    {
    }

    void MCM_Options_t::On_After_Load_Game()
    {
        Reset_Save_State();
        Reset_Option_State();

        save_state.Read();
    }

    void MCM_Options_t::On_Update()
    {
    }

    void MCM_Options_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
    }

    void MCM_Options_t::On_Config_Open()
    {
    }

    void MCM_Options_t::On_Config_Close()
    {
    }

    void MCM_Options_t::On_Page_Open(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        Reset_Option_State();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        mcm->Title_Text(Const::String::OPTIONS);
        option_state.reset = mcm->Add_Text_Option(Const::String::CENTER_RESET, "");
        mcm->Add_Empty_Option();

        mcm->Add_Header_Option("");
        mcm->Add_Header_Option("");
        option_state.show_new_objectives = mcm->Add_Toggle_Option(Const::String::SHOW_NEW_OBJECTIVES, Do_Show_New_Objectives());
        option_state.show_finished_quests = mcm->Add_Toggle_Option(Const::String::SHOW_FINISHED_QUESTS, Do_Show_Finished_Quests());
    }

    void MCM_Options_t::On_Option_Select(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.reset) {
            mcm->Flicker_Option(option);
            Reset_Save_State();
            mcm->Reset_Page();

        } else if (option == option_state.show_new_objectives) {
            mcm->Toggle(option_state.show_new_objectives, save_state.show_new_objectives);
            mcm->Enforce_Hidden_Objectives();
        } else if (option == option_state.show_finished_quests) {
            mcm->Toggle(option_state.show_finished_quests, save_state.show_finished_quests);
            mcm->Enforce_Hidden_Objectives();
        }
    }

    void MCM_Options_t::On_Option_Menu_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Options_t::On_Option_Menu_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
    }

    void MCM_Options_t::On_Option_Slider_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Options_t::On_Option_Slider_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Float_t value)
    {
    }

    void MCM_Options_t::On_Option_Input_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, String_t value)
    {
    }

    void MCM_Options_t::On_Option_Keymap_Change(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
    }

    void MCM_Options_t::On_Option_Default(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Options_t::On_Option_Highlight(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

}}

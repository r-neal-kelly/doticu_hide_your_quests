/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_active.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    MCM_Active_t::Save_State_t::Save_State_t()
    {
    }

    MCM_Active_t::Save_State_t::~Save_State_t()
    {
    }

    void MCM_Active_t::Save_State_t::Read()
    {
    }

    void MCM_Active_t::Save_State_t::Write()
    {
    }

    maybe<MCM_Active_t::Save_State_t*> MCM_Active_t::save_state = none<MCM_Active_t::Save_State_t*>();

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

    Bool_t MCM_Active_t::Has_Save_State()
    {
        return save_state != none<Save_State_t*>();
    }

    void MCM_Active_t::Create_Save_State()
    {
        Delete_Save_State();
        save_state = new Save_State_t();
    }

    void MCM_Active_t::Delete_Save_State()
    {
        if (Has_Save_State()) {
            delete save_state();
        }
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
        Create_Save_State();
    }

    void MCM_Active_t::On_Before_Save_Game()
    {
        SKYLIB_ASSERT(Has_Save_State());

        save_state->Write();
    }

    void MCM_Active_t::On_After_Save_Game()
    {
    }

    void MCM_Active_t::On_Before_Load_Game()
    {
    }

    void MCM_Active_t::On_After_Load_Game()
    {
        Create_Save_State();

        save_state->Read();
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
        MCM()->Title_Text("hello, world");
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

}}

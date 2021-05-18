/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/quest.h"
#include "doticu_skylib/virtual_latent_id.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    class MCM_t;

    class MCM_Active_t :
        public Quest_t
    {
    public:
        class Save_State_t
        {
        public:
            Save_State_t();
            Save_State_t(const Save_State_t& other) = delete;
            Save_State_t(Save_State_t&& other) noexcept = delete;
            Save_State_t& operator =(const Save_State_t& other) = delete;
            Save_State_t& operator =(Save_State_t&& other) noexcept = delete;
            ~Save_State_t();

        public:
            void    Read();
            void    Write();
        };

    public:
        static maybe<Save_State_t*> save_state;

    public:
        static some<MCM_t*>             MCM();

        static some<MCM_Active_t*>      Self();
        static String_t                 Class_Name();
        static some<Virtual::Class_t*>  Class();
        static some<Virtual::Object_t*> Object();

    public:
        static Bool_t   Has_Save_State();
        static void     Create_Save_State();
        static void     Delete_Save_State();

    public:
        static void On_Register(some<Virtual::Machine_t*> v_machine);

        static void On_After_New_Game();
        static void On_Before_Save_Game();
        static void On_After_Save_Game();
        static void On_Before_Load_Game();
        static void On_After_Load_Game();

        static void On_Update_Version(const Version_t<u16> version_to_update);

    public:
        static void On_Config_Open();
        static void On_Config_Close();

        static void On_Page_Open(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh);
        static void On_Option_Select(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Open(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index);
        static void On_Option_Slider_Open(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Slider_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Float_t value);
        static void On_Option_Input_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, String_t value);
        static void On_Option_Keymap_Change(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t key, String_t conflict, String_t mod);
        static void On_Option_Default(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Highlight(Virtual::Latent_ID_t&& latent_id, Int_t option);
    };

}}

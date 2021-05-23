/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include <mutex>
#include <unordered_map>

#include "doticu_skylib/enum_logic_gate.h"
#include "doticu_skylib/quest.h"
#include "doticu_skylib/virtual_latent_id.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    class MCM_t;

    class MCM_Options_t :
        public Quest_t
    {
    public:
        static constexpr Bool_t DEFAULT_SHOW_NEW_OBJECTIVES     = false;
        static constexpr Bool_t DEFAULT_SHOW_FINISHED_QUESTS    = true;

    public:
        class Save_State_t
        {
        public:
            Bool_t  show_new_objectives;
            Bool_t  show_finished_quests;

        public:
            Save_State_t();
            Save_State_t(const Save_State_t& other) = delete;
            Save_State_t(Save_State_t&& other) noexcept = delete;
            Save_State_t& operator =(const Save_State_t& other) = delete;
            Save_State_t& operator =(Save_State_t&& other) noexcept = delete;
            ~Save_State_t();

        public:
            Virtual::Variable_tt<Bool_t>&   Show_New_Objectives();
            Virtual::Variable_tt<Bool_t>&   Show_Finished_Quests();

        public:
            void    Read();
            void    Write();
        };

        class Option_State_t
        {
        public:
            Int_t   reset;
            Int_t   show_new_objectives;
            Int_t   show_finished_quests;

        public:
            Option_State_t();
            Option_State_t(const Option_State_t& other) = delete;
            Option_State_t(Option_State_t&& other) noexcept = delete;
            Option_State_t& operator =(const Option_State_t& other) = delete;
            Option_State_t& operator =(Option_State_t&& other) noexcept = delete;
            ~Option_State_t();
        };

    public:
        static Save_State_t     save_state;
        static Option_State_t   option_state;

    public:
        static some<MCM_t*>             MCM();

        static some<MCM_Options_t*>     Self();
        static String_t                 Class_Name();
        static some<Virtual::Class_t*>  Class();
        static some<Virtual::Object_t*> Object();

    public:
        static void     Reset_Save_State();
        static void     Reset_Option_State();

        static Bool_t   Do_Show_New_Objectives();
        static void     Do_Show_New_Objectives(Bool_t value);
        static Bool_t   Do_Show_Finished_Quests();
        static void     Do_Show_Finished_Quests(Bool_t value);

    public:
        static void On_Register(some<Virtual::Machine_t*> v_machine);

        static void On_After_New_Game();
        static void On_Before_Save_Game();
        static void On_After_Save_Game();
        static void On_Before_Load_Game();
        static void On_After_Load_Game();
        static void On_Update();
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

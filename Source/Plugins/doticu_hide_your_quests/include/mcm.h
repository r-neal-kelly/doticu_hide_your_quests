/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include <mutex>
#include <unordered_map>

#include "doticu_skylib/virtual_stack_id.h"

#include "doticu_mcmlib/config_base.h"

#include "consts.h"
#include "intrinsic.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    class MCM_t :
        public doticu_mcmlib::Config_Base_t
    {
    public:
        static constexpr const char*    DEFAULT_CURRENT_PAGE    = Const::String::CURRENT;

    public:
        class Save_State_t
        {
        public:
            String_t                    current_page;

            Vector_t<maybe<Quest_t*>>   hidden_quests;
            Vector_t<Int_t>             hidden_instances;
            Vector_t<Int_t>             hidden_objectives;

        public:
            Save_State_t();
            Save_State_t(const Save_State_t& other) = delete;
            Save_State_t(Save_State_t&& other) noexcept = delete;
            Save_State_t& operator =(const Save_State_t& other) = delete;
            Save_State_t& operator =(Save_State_t&& other) noexcept = delete;
            ~Save_State_t();

        public:
            Virtual::Variable_tt<String_t>&                     Current_Page();
            Virtual::Variable_tt<Vector_t<maybe<Quest_t*>>>&    Hidden_Quests();
            Virtual::Variable_tt<Vector_t<Int_t>>&              Hidden_Instances();
            Virtual::Variable_tt<Vector_t<Int_t>>&              Hidden_Objectives();

        public:
            void    Read();
            void    Write();
        };

    public:
        static std::mutex                                           lock;

        static Save_State_t                                         save_state;

        static std::unordered_map<Quest_t*, std::tuple<u32, u16>>   hidden_quests;

    public:
        static some<MCM_t*>             Self();
        static String_t                 Class_Name();
        static some<Virtual::Class_t*>  Class();
        static some<Virtual::Object_t*> Object();

    public:
        static void Reset_Save_State();
        static void Reset_State();

    public:
        static Bool_t           Current_Page(String_t& result);

        static std::string      Page_Title(const char* title, Int_t item_count, Int_t page_index, Int_t page_count);
        static maybe<size_t>    Item_Index(Int_t option,
                                           Int_t item_count,
                                           Int_t page_index,
                                           Int_t headers_per_page,
                                           Int_t items_per_page);

        static Bool_t           Has_Hidden_Quest(some<Quest_t*> quest);
        static Bool_t           Has_Hidden_Objective(some<Quest_Objective_t*> objective);
        static void             Add_Hidden_Quest(some<Quest_t*> quest);
        static void             Remove_Hidden_Quest(some<Quest_t*> quest);

        static void             Enforce_Hidden_Objectives();

    public:
        static size_t   Page_Count(size_t item_count, size_t items_per_page);
        static size_t   Page_Index(size_t& page_index, size_t page_count);
        static size_t   Previous_Page(size_t& page_index, size_t page_count, size_t item_count);
        static size_t   Next_Page(size_t& page_index, size_t page_count, size_t item_count);

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
        void    Toggle(Int_t option, Bool_t& value);
        void    Toggle(Int_t option, Bool_t&& value);

    public:
        Bool_t  On_Config_Open(Virtual::Stack_ID_t stack_id);
        Bool_t  On_Config_Close(Virtual::Stack_ID_t stack_id);

        Bool_t  On_Page_Open(Virtual::Stack_ID_t stack_id, String_t page);
        Bool_t  On_Option_Select(Virtual::Stack_ID_t stack_id, Int_t option);
        Bool_t  On_Option_Menu_Open(Virtual::Stack_ID_t stack_id, Int_t option);
        Bool_t  On_Option_Menu_Accept(Virtual::Stack_ID_t stack_id, Int_t option, Int_t index);
        Bool_t  On_Option_Slider_Open(Virtual::Stack_ID_t stack_id, Int_t option);
        Bool_t  On_Option_Slider_Accept(Virtual::Stack_ID_t stack_id, Int_t option, Float_t value);
        Bool_t  On_Option_Input_Accept(Virtual::Stack_ID_t stack_id, Int_t option, String_t value);
        Bool_t  On_Option_Keymap_Change(Virtual::Stack_ID_t stack_id, Int_t option, Int_t key, String_t conflict, String_t mod);
        Bool_t  On_Option_Default(Virtual::Stack_ID_t stack_id, Int_t option);
        Bool_t  On_Option_Highlight(Virtual::Stack_ID_t stack_id, Int_t option);

    public:
        template <typename T>
        static void Build_Page(T& self, Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh);
        template <typename T>
        static void Handle_On_Option_Select(T& self, Virtual::Latent_ID_t&& latent_id, Int_t option);
    };

}}

/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/virtual_stack_id.h"

#include "doticu_mcmlib/config_base.h"

#include "consts.h"
#include "intrinsic.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    class MCM_t :
        public doticu_mcmlib::Config_Base_t
    {
    public:
        static constexpr const char*    DEFAULT_CURRENT_PAGE    = Const::String::JOURNAL_QUESTS;

    public:
        class Save_State_t
        {
        public:
            String_t    current_page;

        public:
            Save_State_t();
            Save_State_t(const Save_State_t& other) = delete;
            Save_State_t(Save_State_t&& other) noexcept = delete;
            Save_State_t& operator =(const Save_State_t& other) = delete;
            Save_State_t& operator =(Save_State_t&& other) noexcept = delete;
            ~Save_State_t();

        public:
            Virtual::Variable_tt<String_t>& Current_Page();

        public:
            void    Read();
            void    Write();
        };

    public:
        static Save_State_t save_state;

    public:
        static some<MCM_t*>             Self();
        static String_t                 Class_Name();
        static some<Virtual::Class_t*>  Class();
        static some<Virtual::Object_t*> Object();

    public:
        static void Reset_Save_State();

    public:
        static Bool_t           Current_Page(String_t& result);

        static std::string      Page_Title(const char* title, Int_t item_count, Int_t page_index, Int_t page_count);
        static std::string      Item_Title(const char* item_type, const char* item_name, Int_t item_index, Int_t item_count);

        static maybe<size_t>    Option_To_Item_Index(Int_t option,
                                                     Int_t item_count,
                                                     Int_t page_index,
                                                     Int_t headers_per_page,
                                                     Int_t items_per_page);
        static maybe<size_t>    Option_To_Item_Index(Int_t option,
                                                     Int_t option_begin,
                                                     Int_t item_count);

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
    };

}}

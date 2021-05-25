/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/quest.h"
#include "doticu_skylib/virtual_latent_id.h"

#include "consts.h"
#include "quest_and_label.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    class MCM_t;

    class MCM_Titled_t :
        public Quest_t
    {
    public:
        static constexpr const char*    PAGE_NAME           = Const::String::$TITLED;
        static constexpr Int_t          DEFAULT_PAGE_INDEX  = 0;
        static constexpr size_t         HEADERS_PER_PAGE    = 4;
        static constexpr size_t         ITEMS_PER_PAGE      = 20;

    public:
        class Save_State_t
        {
        public:
            size_t  page_index;

        public:
            Save_State_t();
            Save_State_t(const Save_State_t& other) = delete;
            Save_State_t(Save_State_t&& other) noexcept = delete;
            Save_State_t& operator =(const Save_State_t& other) = delete;
            Save_State_t& operator =(Save_State_t&& other) noexcept = delete;
            ~Save_State_t();

        public:
            Virtual::Variable_tt<Int_t>&    Page_Index();

        public:
            void    Read();
            void    Write();
        };

        class Option_State_t
        {
        public:
            Int_t   previous;
            Int_t   next;

        public:
            Option_State_t();
            Option_State_t(const Option_State_t& other) = delete;
            Option_State_t(Option_State_t&& other) noexcept = delete;
            Option_State_t& operator =(const Option_State_t& other) = delete;
            Option_State_t& operator =(Option_State_t&& other) noexcept = delete;
            ~Option_State_t();
        };

        class Items_State_t
        {
        public:
            Vector_t<Quest_And_Label_t> items;
            Bool_t                      do_update;

        public:
            Items_State_t();
            Items_State_t(const Items_State_t& other) = delete;
            Items_State_t(Items_State_t&& other) noexcept = delete;
            Items_State_t& operator =(const Items_State_t& other) = delete;
            Items_State_t& operator =(Items_State_t&& other) noexcept = delete;
            ~Items_State_t();

        public:
            const Vector_t<Quest_And_Label_t>&  Items();

            size_t                              Page_Count();
            size_t                              Page_Index();
            size_t                              Previous_Page();
            size_t                              Next_Page();
        };

    public:
        static Save_State_t     save_state;
        static Option_State_t   option_state;
        static Items_State_t    items_state;

    public:
        static some<MCM_t*>             MCM();

        static some<MCM_Titled_t*>      Self();
        static String_t                 Class_Name();
        static some<Virtual::Class_t*>  Class();
        static some<Virtual::Object_t*> Object();

    public:
        static void Reset_Save_State();
        static void Reset_Option_State();
        static void Reset_Items_State();

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

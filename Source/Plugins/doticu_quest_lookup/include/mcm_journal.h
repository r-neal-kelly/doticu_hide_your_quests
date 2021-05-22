/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include <unordered_map>

#include "doticu_skylib/enum_logic_gate.h"
#include "doticu_skylib/quest.h"
#include "doticu_skylib/virtual_latent_id.h"

#include "enum_mcm_journal_mode.h"
#include "enum_mcm_view.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    class MCM_t;

    class MCM_Journal_t :
        public Quest_t
    {
    public:
        static constexpr MCM_View_e::value_type         DEFAULT_CURRENT_VIEW                        = MCM_View_e::LIST;

        static constexpr Int_t                          DEFAULT_LIST_CURRENT_PAGE_INDEX             = 0;

        static constexpr Logic_Gate_e::value_type       DEFAULT_FILTER_MODE                         = Logic_Gate_e::OR;
        static constexpr Bool_t                         DEFAULT_FILTER_STATE_ACTIVE                 = false;
        static constexpr Bool_t                         DEFAULT_FILTER_STATE_COMPLETED              = false;
        static constexpr Bool_t                         DEFAULT_FILTER_STATE_CURRENT                = true;
        static constexpr Bool_t                         DEFAULT_FILTER_STATE_FAILED                 = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_CIVIL_WAR               = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_COMPANIONS              = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_DAEDRIC                 = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_DARK_BROTHERHOOD        = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_DAWNGUARD               = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_DRAGONBORN              = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_MAGES_GUILD             = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_MAIN                    = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_MISC                    = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_OTHER                   = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_SIDE                    = false;
        static constexpr Bool_t                         DEFAULT_FILTER_TYPE_THIEVES_GUILD           = false;

        static constexpr MCM_Journal_Mode_e::value_type DEFAULT_OPTIONS_MODE                        = MCM_Journal_Mode_e::HIDE;
        static constexpr Bool_t                         DEFAULT_OPTIONS_SHOW_NEW_OBJECTIVES         = false;
        static constexpr Bool_t                         DEFAULT_OPTIONS_SHOW_FINISHED_QUESTS        = true;

        static constexpr size_t                         LIST_HEADERS_PER_PAGE                       = 6;
        static constexpr size_t                         LIST_ITEMS_PER_PAGE                         = 18;

    public:
        class Save_State_t
        {
        public:
            maybe<MCM_View_e>           current_view;
            Vector_t<maybe<Quest_t*>>   hidden_quests;
            Vector_t<Int_t>             hidden_objectives;

            Int_t                       list_current_page_index;

            maybe<Logic_Gate_e>         filter_mode;
            Bool_t                      filter_state_active;
            Bool_t                      filter_state_completed;
            Bool_t                      filter_state_current;
            Bool_t                      filter_state_failed;
            Bool_t                      filter_type_civil_war;
            Bool_t                      filter_type_companions;
            Bool_t                      filter_type_daedric;
            Bool_t                      filter_type_dark_brotherhood;
            Bool_t                      filter_type_dawnguard;
            Bool_t                      filter_type_dragonborn;
            Bool_t                      filter_type_mages_guild;
            Bool_t                      filter_type_main;
            Bool_t                      filter_type_misc;
            Bool_t                      filter_type_other;
            Bool_t                      filter_type_side;
            Bool_t                      filter_type_thieves_guild;

            maybe<MCM_Journal_Mode_e>   options_mode;
            Bool_t                      options_show_new_objectives;
            Bool_t                      options_show_finished_quests;

            maybe<Quest_t*>             item_current;

        public:
            Save_State_t();
            Save_State_t(const Save_State_t& other) = delete;
            Save_State_t(Save_State_t&& other) noexcept = delete;
            Save_State_t& operator =(const Save_State_t& other) = delete;
            Save_State_t& operator =(Save_State_t&& other) noexcept = delete;
            ~Save_State_t();

        public:
            Virtual::Variable_tt<String_t>&                     Current_View();
            Virtual::Variable_tt<Vector_t<maybe<Quest_t*>>>&    Hidden_Quests();
            Virtual::Variable_tt<Vector_t<Int_t>>&              Hidden_Objectives();

            Virtual::Variable_tt<Int_t>&                        List_Current_Page_Index();

            Virtual::Variable_tt<String_t>&                     Filter_Mode();
            Virtual::Variable_tt<Bool_t>&                       Filter_State_Active();
            Virtual::Variable_tt<Bool_t>&                       Filter_State_Completed();
            Virtual::Variable_tt<Bool_t>&                       Filter_State_Current();
            Virtual::Variable_tt<Bool_t>&                       Filter_State_Failed();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Civil_War();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Companions();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Daedric();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Dark_Brotherhood();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Dawnguard();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Dragonborn();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Mages_Guild();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Main();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Misc();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Other();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Side();
            Virtual::Variable_tt<Bool_t>&                       Filter_Type_Thieves_Guild();

            Virtual::Variable_tt<String_t>&                     Options_Mode();
            Virtual::Variable_tt<Bool_t>&                       Options_Show_New_Objectives();
            Virtual::Variable_tt<Bool_t>&                       Options_Show_Finished_Quests();

            Virtual::Variable_tt<maybe<Quest_t*>>&              Item_Current();

        public:
            void    Read();
            void    Write();
        };

        class Option_State_t
        {
        public:
            Int_t   filter;
            Int_t   options;
            Int_t   previous;
            Int_t   next;
            Int_t   back;
            Int_t   reset;
            Int_t   mode;

            Int_t   state_active;
            Int_t   state_completed;
            Int_t   state_current;
            Int_t   state_failed;
            Int_t   type_civil_war;
            Int_t   type_companions;
            Int_t   type_daedric;
            Int_t   type_dark_brotherhood;
            Int_t   type_dawnguard;
            Int_t   type_dragonborn;
            Int_t   type_mages_guild;
            Int_t   type_main;
            Int_t   type_misc;
            Int_t   type_other;
            Int_t   type_side;
            Int_t   type_thieves_guild;

            Int_t   show_new_objectives;
            Int_t   show_finished_quests;

            Int_t   show_objectives;

        public:
            Option_State_t();
            Option_State_t(const Option_State_t& other) = delete;
            Option_State_t(Option_State_t&& other) noexcept = delete;
            Option_State_t& operator =(const Option_State_t& other) = delete;
            Option_State_t& operator =(Option_State_t&& other) noexcept = delete;
            ~Option_State_t();
        };

        class List_State_t
        {
        public:
            Vector_t<some<Quest_t*>>    quests;
            Bool_t                      do_update_quests;

        public:
            List_State_t();
            List_State_t(const List_State_t& other) = delete;
            List_State_t(List_State_t&& other) noexcept = delete;
            List_State_t& operator =(const List_State_t& other) = delete;
            List_State_t& operator =(List_State_t&& other) noexcept = delete;
            ~List_State_t();

        public:
            Int_t                       Page_Count(size_t item_count);
            Int_t                       Current_Page_Index(Int_t page_count);
            void                        Go_To_Previous_Page(size_t item_count);
            void                        Go_To_Next_Page(size_t item_count);
            void                        Go_To_Page(maybe<Quest_t*> item);

            Vector_t<some<Quest_t*>>&   Quests();
            size_t                      Quest_Count();
            void                        Queue_Quests_Update();
        };

        class Filter_State_t
        {
        public:
            Filter_State_t();
            Filter_State_t(const Filter_State_t& other) = delete;
            Filter_State_t(Filter_State_t&& other) noexcept = delete;
            Filter_State_t& operator =(const Filter_State_t& other) = delete;
            Filter_State_t& operator =(Filter_State_t&& other) noexcept = delete;
            ~Filter_State_t();

        public:
            some<Logic_Gate_e>  Mode();
            void                Mode(String_t name);
            Vector_t<String_t>  Mode_Names();

            Bool_t              OR_Flags(some<Quest_t*> quest, Vector_t<Quest_Flags_e>& flags);
            Bool_t              AND_Flags(some<Quest_t*> quest, Vector_t<Quest_Flags_e>& flags);

            void                Filter(Vector_t<some<Quest_Objective_t*>>& objectives);
            void                Reset();
        };

        class Options_State_t
        {
        public:
            Options_State_t();
            Options_State_t(const Options_State_t& other) = delete;
            Options_State_t(Options_State_t&& other) noexcept = delete;
            Options_State_t& operator =(const Options_State_t& other) = delete;
            Options_State_t& operator =(Options_State_t&& other) noexcept = delete;
            ~Options_State_t();

        public:
            some<MCM_Journal_Mode_e>    Mode();
            void                        Mode(String_t name);
            Vector_t<String_t>          Mode_Names();

            Bool_t                      Do_Show_New_Objectives();
            void                        Do_Show_New_Objectives(Bool_t value);
            Bool_t                      Do_Show_Finished_Quests();
            void                        Do_Show_Finished_Quests(Bool_t value);
        };

        class Item_State_t
        {
        public:
            Vector_t<Player_Objective_t>    objectives;
            Bool_t                          do_update_objectives;

        public:
            Item_State_t();
            Item_State_t(const Item_State_t& other) = delete;
            Item_State_t(Item_State_t&& other) noexcept = delete;
            Item_State_t& operator =(const Item_State_t& other) = delete;
            Item_State_t& operator =(Item_State_t&& other) noexcept = delete;
            ~Item_State_t();

        public:
            maybe<Quest_t*>                 Current();
            void                            Current(maybe<Quest_t*> item);
            maybe<Quest_t*>                 Go_To_Previous_Item();
            maybe<Quest_t*>                 Go_To_Next_Item();

            Vector_t<Player_Objective_t>&   Objectives();
            size_t                          Objective_Count();
            void                            Queue_Objectives_Update();
        };

    public:
        static Save_State_t                         save_state;
        static Option_State_t                       option_state;
        static List_State_t                         list_state;
        static Filter_State_t                       filter_state;
        static Options_State_t                      options_state;
        static Item_State_t                         item_state;

        static std::unordered_map<Quest_t*, u16>    hidden_quests;

    public:
        static some<MCM_t*>             MCM();

        static some<MCM_Journal_t*>     Self();
        static String_t                 Class_Name();
        static some<Virtual::Class_t*>  Class();
        static some<Virtual::Object_t*> Object();

    public:
        static void Reset_Save_State();
        static void Reset_Option_State();
        static void Reset_List_State();
        static void Reset_Filter_State();
        static void Reset_Options_State();
        static void Reset_Item_State();

    public:
        static some<MCM_View_e>             Current_View();
        static void                         Current_View(some<MCM_View_e> view);

        static Bool_t                       Is_Objective_Hidable(some<Quest_Objective_t*> objective);
        static Bool_t                       Has_Hidden_Objective(some<Quest_Objective_t*> objective);
        static Bool_t                       Has_Hidden_Objective(some<Quest_t*> quest);
        static void                         Add_Hidden_Objective(some<Quest_Objective_t*> objective);
        static void                         Add_Hidden_Objective(some<Quest_t*> quest);
        static void                         Remove_Hidden_Objective(some<Quest_Objective_t*> objective);
        static void                         Remove_Hidden_Objective(some<Quest_t*> quest);

        static void                         Enforce_Objectives();

        static maybe<Player_Objective_t>    Highest_Objective(some<Quest_t*> quest);

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

        static void On_Page_Open_List(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh);
        static void On_Option_Select_List(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Open_List(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Accept_List(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index);

        static void On_Page_Open_Filter(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh);
        static void On_Option_Select_Filter(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Open_Filter(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Accept_Filter(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index);

        static void On_Page_Open_Options(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh);
        static void On_Option_Select_Options(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Open_Options(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Accept_Options(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index);

        static void On_Page_Open_Item(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh);
        static void On_Option_Select_Item(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Open_Item(Virtual::Latent_ID_t&& latent_id, Int_t option);
        static void On_Option_Menu_Accept_Item(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index);
    };

}}

/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/game.h"
#include "doticu_skylib/player.h"
#include "doticu_skylib/player_objective.h"
#include "doticu_skylib/quest_objective.h"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/virtual_latent_id.h"
#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_current.h"
#include "mcm_finished.h"
#include "mcm_misc.h"
#include "mcm_options.h"
#include "mcm_titled.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    MCM_t::Save_State_t::Save_State_t() :
        current_page(DEFAULT_CURRENT_PAGE),

        hidden_quests(0),
        hidden_instances(0),
        hidden_objectives(0)
    {
    }

    MCM_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<String_t>& MCM_t::Save_State_t::Current_Page()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "current_page");
    }

    Virtual::Variable_tt<Vector_t<maybe<Quest_t*>>>& MCM_t::Save_State_t::Hidden_Quests()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<maybe<Quest_t*>>, "hidden_quests");
    }

    Virtual::Variable_tt<Vector_t<Int_t>>& MCM_t::Save_State_t::Hidden_Instances()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "hidden_instances");
    }

    Virtual::Variable_tt<Vector_t<Int_t>>& MCM_t::Save_State_t::Hidden_Objectives()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "hidden_objectives");
    }

    void MCM_t::Save_State_t::Read()
    {
        this->current_page = Current_Page();

        this->hidden_quests = Hidden_Quests();
        this->hidden_instances = Hidden_Instances();
        this->hidden_objectives = Hidden_Objectives();

        size_t hidden_quest_count = this->hidden_quests.size();
        this->hidden_instances.resize(hidden_quest_count, 0);
        this->hidden_objectives.resize(hidden_quest_count, 0);
    }

    void MCM_t::Save_State_t::Write()
    {
        Current_Page() = this->current_page;

        Hidden_Quests() = this->hidden_quests;
        Hidden_Instances() = this->hidden_instances;
        Hidden_Objectives() = this->hidden_objectives;
    }

    std::mutex                                          MCM_t::lock;

    MCM_t::Save_State_t                                 MCM_t::save_state;

    std::unordered_map<Quest_t*, std::tuple<u32, u16>>  MCM_t::hidden_quests;

    some<MCM_t*> MCM_t::Self()
    {
        return Const::Quest::MCM();
    }

    String_t MCM_t::Class_Name()
    {
        DEFINE_CLASS_NAME("doticu_hide_your_quests");
    }

    some<Virtual::Class_t*> MCM_t::Class()
    {
        DEFINE_CLASS();
    }

    some<Virtual::Object_t*> MCM_t::Object()
    {
        DEFINE_COMPONENT_OBJECT_METHOD(Self()());
    }

    void MCM_t::Reset_Save_State()
    {
        save_state.~Save_State_t();
        new (&save_state) Save_State_t;
    }

    void MCM_t::Reset_State()
    {
        hidden_quests.clear();
    }

    Bool_t MCM_t::Current_Page(String_t& result)
    {
        if (!save_state.current_page) {
            save_state.current_page = DEFAULT_CURRENT_PAGE;
        }

        if (!result) {
            result = save_state.current_page;
            return false;
        } else {
            if (result != save_state.current_page) {
                save_state.current_page = result;
                return false;
            } else {
                return true;
            }
        }
    }

    std::string MCM_t::Page_Title(const char* title, Int_t item_count, Int_t page_index, Int_t page_count)
    {
        std::string items =
            std::string(title) + ": " +
            std::to_string(item_count);

        std::string pages =
            std::string("Page") + ": " +
            std::to_string(page_index + 1) + "/" +
            std::to_string(page_count);

        return items + "               " + pages;
    }

    maybe<size_t> MCM_t::Item_Index(Int_t option,
                                    Int_t item_count,
                                    Int_t page_index,
                                    Int_t headers_per_page,
                                    Int_t items_per_page)
    {
        Int_t relative_idx = doticu_mcmlib::Option_t(option).position - headers_per_page;
        if (relative_idx > -1 && relative_idx < items_per_page) {
            Int_t absolute_idx = page_index * items_per_page + relative_idx;
            if (absolute_idx > -1 && absolute_idx < item_count) {
                return absolute_idx;
            } else {
                return none<size_t>();
            }
        } else {
            return none<size_t>();
        }
    }

    Bool_t MCM_t::Has_Hidden_Quest(some<Quest_t*> quest)
    {
        SKYLIB_ASSERT_SOME(quest);

        if (hidden_quests.count(quest()) > 0) {
            if (MCM_Options_t::Do_Show_New_Objectives()) {
                maybe<Player_Objective_t> objective = Player_t::Self()->Highest_Displayed_Player_Objective(quest);
                if (objective.Has_Value()) {
                    std::tuple<u32, u16>& tuple = hidden_quests[quest()];
                    return std::get<0>(tuple) >= objective().instance_id && std::get<1>(tuple) >= objective().objective->index;
                } else {
                    return true;
                }
            } else {
                return true;
            }
        } else {
            return false;
        }
    }

    Bool_t MCM_t::Has_Hidden_Objective(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);
        SKYLIB_ASSERT_SOME(objective->quest);

        if (hidden_quests.count(objective->quest()) > 0) {
            if (MCM_Options_t::Do_Show_New_Objectives()) {
                std::tuple<u32, u16>& tuple = hidden_quests[objective->quest()];
                return std::get<0>(tuple) >= objective->quest->current_instance_id && std::get<1>(tuple) >= objective->index;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }

    void MCM_t::Add_Hidden_Quest(some<Quest_t*> quest)
    {
        SKYLIB_ASSERT_SOME(quest);

        maybe<Player_Objective_t> objective = Player_t::Self()->Highest_Displayed_Player_Objective(quest);
        if (objective.Has_Value()) {
            std::tuple<u32, u16>& tuple = hidden_quests[quest()];
            std::get<0>(tuple) = objective().instance_id;
            std::get<1>(tuple) = objective().objective->index;
        } else {
            std::tuple<u32, u16>& tuple = hidden_quests[quest()];
            std::get<0>(tuple) = quest->current_instance_id;
            std::get<1>(tuple) = 0;
        }
    }

    void MCM_t::Remove_Hidden_Quest(some<Quest_t*> quest)
    {
        SKYLIB_ASSERT_SOME(quest);

        hidden_quests.erase(quest());
    }

    void MCM_t::Enforce_Hidden_Objectives()
    {
        class Iterator_t :
            public Iterator_i<some<Player_Objective_t*>>
        {
        public:
            virtual Iterator_e operator ()(some<Player_Objective_t*> it) override
            {
                if (it->objective) {
                    if (it->objective->Is_Displayed()) {
                        if (!MCM_Options_t::Do_Show_Finished_Quests() && it->objective->quest->Is_Completed_Or_Failed()) {
                            it->state = Quest_Objective_State_e::DORMANT;
                        } else if (Has_Hidden_Objective(it->objective())) {
                            it->state = Quest_Objective_State_e::DORMANT;
                        } else {
                            if (it->instance_id == it->objective->quest->current_instance_id) {
                                it->state = it->objective->state;
                            } else {
                                it->state = Quest_Objective_State_e::DORMANT;
                            }
                        }
                    } else {
                        it->state = it->objective->state;
                    }
                }
                return Iterator_e::CONTINUE;
            }
        };

        Iterator_t iterator;
        Read_Locker_t locker(Game_t::Form_IDs_To_Forms_Lock());
        Player_t::Self()->Iterate_Player_Objectives(iterator, locker);
    }

    size_t MCM_t::Page_Count(size_t item_count, size_t items_per_page)
    {
        return static_cast<size_t>(ceilf(
            static_cast<Float_t>(item_count) / static_cast<Float_t>(items_per_page)
        ));
    }

    size_t MCM_t::Page_Index(size_t& page_index, size_t page_count)
    {
        if (page_index >= page_count) {
            page_index = page_count - 1;
        }
        return page_index;
    }

    size_t MCM_t::Previous_Page(size_t& page_index, size_t page_count, size_t item_count)
    {
        if (item_count > 0) {
            if (page_index == 0) {
                page_index = page_count - 1;
            } else {
                page_index -= 1;
            }
        } else {
            page_index = 0;
        }
        return page_index;
    }

    size_t MCM_t::Next_Page(size_t& page_index, size_t page_count, size_t item_count)
    {
        if (item_count > 0) {
            if (page_index == page_count - 1) {
                page_index = 0;
            } else {
                page_index += 1;
            }
        } else {
            page_index = 0;
        }
        return page_index;
    }

    void MCM_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        using type_name = MCM_t;

        std::lock_guard<std::mutex> locker(lock);

        SKYLIB_ASSERT_SOME(v_machine);

        Config_Base_t::Register_Me(v_machine);

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

        METHOD("OnConfigOpen", true, void, On_Config_Open);
        METHOD("OnConfigClose", true, void, On_Config_Close);
        METHOD("OnPageReset", true, void, On_Page_Open, String_t);
        METHOD("OnOptionSelect", true, void, On_Option_Select, Int_t);
        METHOD("OnOptionMenuOpen", true, void, On_Option_Menu_Open, Int_t);
        METHOD("OnOptionMenuAccept", true, void, On_Option_Menu_Accept, Int_t, Int_t);
        METHOD("OnOptionSliderOpen", true, void, On_Option_Slider_Open, Int_t);
        METHOD("OnOptionSliderAccept", true, void, On_Option_Slider_Accept, Int_t, Float_t);
        METHOD("OnOptionInputAccept", true, void, On_Option_Input_Accept, Int_t, String_t);
        METHOD("OnOptionKeymapChange", true, void, On_Option_Keymap_Change, Int_t, Int_t, String_t, String_t);
        METHOD("OnOptionDefault", true, void, On_Option_Default, Int_t);
        METHOD("OnOptionHighlight", true, void, On_Option_Highlight, Int_t);

        #undef METHOD

        MCM_Current_t::On_Register(v_machine);
        MCM_Finished_t::On_Register(v_machine);
        MCM_Titled_t::On_Register(v_machine);
        MCM_Misc_t::On_Register(v_machine);
        MCM_Options_t::On_Register(v_machine);

        SKYLIB_LOG("- Registered all MCM functions.");
    }

    void MCM_t::On_After_New_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        Reset_Save_State();
        Reset_State();

        MCM_Current_t::On_After_New_Game();
        MCM_Finished_t::On_After_New_Game();
        MCM_Titled_t::On_After_New_Game();
        MCM_Misc_t::On_After_New_Game();
        MCM_Options_t::On_After_New_Game();
    }

    void MCM_t::On_Before_Save_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        const size_t hidden_quest_count = save_state.hidden_quests.size();
        save_state.hidden_quests.reserve(hidden_quest_count);
        save_state.hidden_instances.reserve(hidden_quest_count);
        save_state.hidden_objectives.reserve(hidden_quest_count);
        save_state.hidden_quests.clear();
        save_state.hidden_instances.clear();
        save_state.hidden_objectives.clear();

        for (auto it = hidden_quests.begin(), end = hidden_quests.end(); it != end; ++it) {
            maybe<Quest_t*> quest = it->first;
            std::tuple<u32, u16> tuple = it->second;
            if (quest) {
                save_state.hidden_quests.push_back(quest);
                save_state.hidden_instances.push_back(std::get<0>(tuple));
                save_state.hidden_objectives.push_back(std::get<1>(tuple));
            }
        }

        save_state.Write();

        MCM_Current_t::On_Before_Save_Game();
        MCM_Finished_t::On_Before_Save_Game();
        MCM_Titled_t::On_Before_Save_Game();
        MCM_Misc_t::On_Before_Save_Game();
        MCM_Options_t::On_Before_Save_Game();
    }

    void MCM_t::On_After_Save_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        MCM_Current_t::On_After_Save_Game();
        MCM_Finished_t::On_After_Save_Game();
        MCM_Titled_t::On_After_Save_Game();
        MCM_Misc_t::On_After_Save_Game();
        MCM_Options_t::On_After_Save_Game();
    }

    void MCM_t::On_Before_Load_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        MCM_Current_t::On_Before_Load_Game();
        MCM_Finished_t::On_Before_Load_Game();
        MCM_Titled_t::On_Before_Load_Game();
        MCM_Misc_t::On_Before_Load_Game();
        MCM_Options_t::On_Before_Load_Game();
    }

    void MCM_t::On_After_Load_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        Reset_Save_State();
        Reset_State();

        save_state.Read();

        const size_t count = save_state.hidden_quests.size();
        hidden_quests.reserve(count);
        hidden_quests.clear();

        for (size_t idx = 0, end = count; idx < end; idx += 1) {
            maybe<Quest_t*> quest = save_state.hidden_quests[idx];
            if (quest) {
                maybe<Quest_Objective_t*> objective = quest->Objective(save_state.hidden_objectives[idx]);
                if (objective) {
                    hidden_quests[quest()] = std::make_tuple(save_state.hidden_instances[idx], objective->index);
                } else {
                    hidden_quests[quest()] = std::make_tuple(save_state.hidden_instances[idx], 0);
                }
            }
        }

        Enforce_Hidden_Objectives();

        MCM_Current_t::On_After_Load_Game();
        MCM_Finished_t::On_After_Load_Game();
        MCM_Titled_t::On_After_Load_Game();
        MCM_Misc_t::On_After_Load_Game();
        MCM_Options_t::On_After_Load_Game();
    }

    void MCM_t::On_Update()
    {
        std::lock_guard<std::mutex> locker(lock);

        if (!UI_t::Is_Menu_Open("Journal Menu")) {
            Enforce_Hidden_Objectives();
        }

        MCM_Current_t::On_Update();
        MCM_Finished_t::On_Update();
        MCM_Titled_t::On_Update();
        MCM_Misc_t::On_Update();
        MCM_Options_t::On_Update();
    }

    void MCM_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
        std::lock_guard<std::mutex> locker(lock);

        MCM_Current_t::On_Update_Version(version_to_update);
        MCM_Finished_t::On_Update_Version(version_to_update);
        MCM_Titled_t::On_Update_Version(version_to_update);
        MCM_Misc_t::On_Update_Version(version_to_update);
        MCM_Options_t::On_Update_Version(version_to_update);
    }

    void MCM_t::Toggle(Int_t option, Bool_t& value)
    {
        Flicker_Option(option);
        value = !value;
        Toggle_Option_Value(option, value, true);
    }

    void MCM_t::Toggle(Int_t option, Bool_t&& value)
    {
        Flicker_Option(option);
        Toggle_Option_Value(option, value, true);
    }

    Bool_t MCM_t::On_Config_Open(Virtual::Stack_ID_t stack_id)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        Vector_t<String_t> pages;
        pages.reserve(5);
        pages.push_back(Const::String::CURRENT);
        pages.push_back(Const::String::FINISHED);
        pages.push_back(Const::String::TITLED);
        pages.push_back(Const::String::MISC);
        pages.push_back(Const::String::OPTIONS);
        Pages() = pages;

        MCM_Current_t::On_Config_Open();
        MCM_Finished_t::On_Config_Open();
        MCM_Titled_t::On_Config_Open();
        MCM_Misc_t::On_Config_Open();
        MCM_Options_t::On_Config_Open();

        return true;
    }

    Bool_t MCM_t::On_Config_Close(Virtual::Stack_ID_t stack_id)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        MCM_Current_t::On_Config_Close();
        MCM_Finished_t::On_Config_Close();
        MCM_Titled_t::On_Config_Close();
        MCM_Misc_t::On_Config_Close();
        MCM_Options_t::On_Config_Close();

        return true;
    }

    Bool_t MCM_t::On_Page_Open(Virtual::Stack_ID_t stack_id, String_t page)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        Bool_t is_refresh = Current_Page(page);

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Page_Open(std::move(latent_id), is_refresh);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Page_Open(std::move(latent_id), is_refresh);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Page_Open(std::move(latent_id), is_refresh);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Page_Open(std::move(latent_id), is_refresh);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Page_Open(std::move(latent_id), is_refresh);
        else                                        MCM_Current_t::On_Page_Open(std::move(latent_id), is_refresh);

        return true;
    }

    Bool_t MCM_t::On_Option_Select(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Select(std::move(latent_id), option);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Select(std::move(latent_id), option);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Select(std::move(latent_id), option);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Select(std::move(latent_id), option);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Select(std::move(latent_id), option);
        else                                        MCM_Current_t::On_Option_Select(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Menu_Open(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Menu_Open(std::move(latent_id), option);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Menu_Open(std::move(latent_id), option);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Menu_Open(std::move(latent_id), option);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Menu_Open(std::move(latent_id), option);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Menu_Open(std::move(latent_id), option);
        else                                        MCM_Current_t::On_Option_Menu_Open(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Menu_Accept(Virtual::Stack_ID_t stack_id, Int_t option, Int_t index)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Menu_Accept(std::move(latent_id), option, index);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Menu_Accept(std::move(latent_id), option, index);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Menu_Accept(std::move(latent_id), option, index);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Menu_Accept(std::move(latent_id), option, index);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Menu_Accept(std::move(latent_id), option, index);
        else                                        MCM_Current_t::On_Option_Menu_Accept(std::move(latent_id), option, index);

        return true;
    }

    Bool_t MCM_t::On_Option_Slider_Open(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Slider_Open(std::move(latent_id), option);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Slider_Open(std::move(latent_id), option);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Slider_Open(std::move(latent_id), option);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Slider_Open(std::move(latent_id), option);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Slider_Open(std::move(latent_id), option);
        else                                        MCM_Current_t::On_Option_Slider_Open(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Slider_Accept(Virtual::Stack_ID_t stack_id, Int_t option, Float_t value)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Slider_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Slider_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Slider_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Slider_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Slider_Accept(std::move(latent_id), option, value);
        else                                        MCM_Current_t::On_Option_Slider_Accept(std::move(latent_id), option, value);

        return true;
    }

    Bool_t MCM_t::On_Option_Input_Accept(Virtual::Stack_ID_t stack_id, Int_t option, String_t value)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Input_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Input_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Input_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Input_Accept(std::move(latent_id), option, value);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Input_Accept(std::move(latent_id), option, value);
        else                                        MCM_Current_t::On_Option_Input_Accept(std::move(latent_id), option, value);

        return true;
    }

    Bool_t MCM_t::On_Option_Keymap_Change(Virtual::Stack_ID_t stack_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);
        else                                        MCM_Current_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);

        return true;
    }

    Bool_t MCM_t::On_Option_Default(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Default(std::move(latent_id), option);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Default(std::move(latent_id), option);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Default(std::move(latent_id), option);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Default(std::move(latent_id), option);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Default(std::move(latent_id), option);
        else                                        MCM_Current_t::On_Option_Default(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Highlight(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::CURRENT)         MCM_Current_t::On_Option_Highlight(std::move(latent_id), option);
        else if (page == Const::String::FINISHED)   MCM_Finished_t::On_Option_Highlight(std::move(latent_id), option);
        else if (page == Const::String::TITLED)     MCM_Titled_t::On_Option_Highlight(std::move(latent_id), option);
        else if (page == Const::String::MISC)       MCM_Misc_t::On_Option_Highlight(std::move(latent_id), option);
        else if (page == Const::String::OPTIONS)    MCM_Options_t::On_Option_Highlight(std::move(latent_id), option);
        else                                        MCM_Current_t::On_Option_Highlight(std::move(latent_id), option);

        return true;
    }

}}

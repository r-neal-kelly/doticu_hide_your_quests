/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/virtual_latent_id.h"
#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_journal.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    MCM_t::Save_State_t::Save_State_t() :
        current_page(DEFAULT_CURRENT_PAGE)
    {
    }

    MCM_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<String_t>& MCM_t::Save_State_t::Current_Page()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "current_page");
    }

    void MCM_t::Save_State_t::Read()
    {
        this->current_page = Current_Page();
    }

    void MCM_t::Save_State_t::Write()
    {
        Current_Page() = this->current_page;
    }

    MCM_t::Save_State_t MCM_t::save_state;

    some<MCM_t*> MCM_t::Self()
    {
        return Const::Quest::MCM();
    }

    String_t MCM_t::Class_Name()
    {
        DEFINE_CLASS_NAME("doticu_quest_lookup_mcm");
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

    std::string MCM_t::Item_Title(const char* item_type, const char* item_name, Int_t item_index, Int_t item_count)
    {
        std::string item =
            std::string(item_type) + ": " +
            item_name;

        std::string items =
            std::string("Item") + ": " +
            std::to_string(item_index + 1) + "/" +
            std::to_string(item_count);

        return item + "               " + items;
    }

    maybe<size_t> MCM_t::Option_To_Item_Index(Int_t option,
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

    maybe<size_t> MCM_t::Option_To_Item_Index(Int_t option,
                                              Int_t option_begin,
                                              Int_t item_count)
    {
        Int_t begin = doticu_mcmlib::Option_t(option_begin).position;
        Int_t index = doticu_mcmlib::Option_t(option).position - begin;
        if (index > -1) {
            Int_t end = index + item_count;
            if (end < 0 || end > 128) {
                end = 128;
            }
            if (index < end) {
                return index;
            } else {
                return none<size_t>();
            }
        } else {
            return none<size_t>();
        }
    }

    void MCM_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        using type_name = MCM_t;

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

        MCM_Journal_t::On_Register(v_machine);

        SKYLIB_LOG("- Registered all MCM functions.");
    }

    void MCM_t::On_After_New_Game()
    {
        Reset_Save_State();

        MCM_Journal_t::On_After_New_Game();
    }

    void MCM_t::On_Before_Save_Game()
    {
        save_state.Write();

        MCM_Journal_t::On_Before_Save_Game();
    }

    void MCM_t::On_After_Save_Game()
    {
        MCM_Journal_t::On_After_Save_Game();
    }

    void MCM_t::On_Before_Load_Game()
    {
        MCM_Journal_t::On_Before_Load_Game();
    }

    void MCM_t::On_After_Load_Game()
    {
        Reset_Save_State();

        save_state.Read();

        MCM_Journal_t::On_After_Load_Game();
    }

    void MCM_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
        MCM_Journal_t::On_Update_Version(version_to_update);
    }

    Bool_t MCM_t::On_Config_Open(Virtual::Stack_ID_t stack_id)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        Vector_t<String_t> pages;
        pages.reserve(1);
        pages.push_back(Const::String::JOURNAL_QUESTS);
        Pages() = pages;

        MCM_Journal_t::On_Config_Open();

        return true;
    }

    Bool_t MCM_t::On_Config_Close(Virtual::Stack_ID_t stack_id)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        MCM_Journal_t::On_Config_Close();

        return true;
    }

    Bool_t MCM_t::On_Page_Open(Virtual::Stack_ID_t stack_id, String_t page)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        Bool_t is_refresh = Current_Page(page);

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Page_Open(std::move(latent_id), is_refresh);
        else                                        MCM_Journal_t::On_Page_Open(std::move(latent_id), is_refresh);

        return true;
    }

    Bool_t MCM_t::On_Option_Select(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Select(std::move(latent_id), option);
        else                                        MCM_Journal_t::On_Option_Select(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Menu_Open(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Menu_Open(std::move(latent_id), option);
        else                                        MCM_Journal_t::On_Option_Menu_Open(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Menu_Accept(Virtual::Stack_ID_t stack_id, Int_t option, Int_t index)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Menu_Accept(std::move(latent_id), option, index);
        else                                        MCM_Journal_t::On_Option_Menu_Accept(std::move(latent_id), option, index);

        return true;
    }

    Bool_t MCM_t::On_Option_Slider_Open(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Slider_Open(std::move(latent_id), option);
        else                                        MCM_Journal_t::On_Option_Slider_Open(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Slider_Accept(Virtual::Stack_ID_t stack_id, Int_t option, Float_t value)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Slider_Accept(std::move(latent_id), option, value);
        else                                        MCM_Journal_t::On_Option_Slider_Accept(std::move(latent_id), option, value);

        return true;
    }

    Bool_t MCM_t::On_Option_Input_Accept(Virtual::Stack_ID_t stack_id, Int_t option, String_t value)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Input_Accept(std::move(latent_id), option, value);
        else                                        MCM_Journal_t::On_Option_Input_Accept(std::move(latent_id), option, value);

        return true;
    }

    Bool_t MCM_t::On_Option_Keymap_Change(Virtual::Stack_ID_t stack_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);
        else                                        MCM_Journal_t::On_Option_Keymap_Change(std::move(latent_id), option, key, conflict, mod);

        return true;
    }

    Bool_t MCM_t::On_Option_Default(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Default(std::move(latent_id), option);
        else                                        MCM_Journal_t::On_Option_Default(std::move(latent_id), option);

        return true;
    }

    Bool_t MCM_t::On_Option_Highlight(Virtual::Stack_ID_t stack_id, Int_t option)
    {
        Virtual::Latent_ID_t latent_id(stack_id);

        String_t page = save_state.current_page;

        if (page == Const::String::JOURNAL_QUESTS)  MCM_Journal_t::On_Option_Highlight(std::move(latent_id), option);
        else                                        MCM_Journal_t::On_Option_Highlight(std::move(latent_id), option);

        return true;
    }

}}

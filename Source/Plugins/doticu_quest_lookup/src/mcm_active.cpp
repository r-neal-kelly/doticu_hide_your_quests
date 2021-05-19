/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/game.h"
#include "doticu_skylib/math.h"
#include "doticu_skylib/player.h"
#include "doticu_skylib/quest_objective.h"
#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_active.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    MCM_Active_t::Save_State_t::Save_State_t() :
        current_view(DEFAULT_CURRENT_VIEW),

        list_current_page_index(DEFAULT_LIST_CURRENT_PAGE_INDEX),
        list_objective_quests(),
        list_hidden_objectives(),

        item_current(none<Quest_t*>())
    {
    }

    MCM_Active_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<String_t>& MCM_Active_t::Save_State_t::Current_View()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "current_view");
    }

    Virtual::Variable_tt<Int_t>& MCM_Active_t::Save_State_t::List_Current_Page_Index()
    {
        DEFINE_VARIABLE_REFERENCE(Int_t, "list_current_page_index");
    }

    Virtual::Variable_tt<Vector_t<Int_t>>& MCM_Active_t::Save_State_t::List_Hidden_Objectives()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "list_hidden_objectives");
    }

    Virtual::Variable_tt<Vector_t<maybe<Quest_t*>>>& MCM_Active_t::Save_State_t::List_Objective_Quests()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<maybe<Quest_t*>>, "list_objective_quests");
    }

    Virtual::Variable_tt<maybe<Quest_t*>>& MCM_Active_t::Save_State_t::Item_Current()
    {
        DEFINE_VARIABLE_REFERENCE(maybe<Quest_t*>, "item_current");
    }

    void MCM_Active_t::Save_State_t::Read()
    {
        this->current_view = MCM_View_e::From_String(static_cast<String_t>(Current_View()));

        this->list_current_page_index = List_Current_Page_Index();
        this->list_hidden_objectives = List_Hidden_Objectives();
        this->list_objective_quests = List_Objective_Quests();

        this->item_current = Item_Current();

        {
            size_t max = Math_t::Max(this->list_objective_quests.size(), this->list_hidden_objectives.size());
            this->list_hidden_objectives.resize(max, -1);
            this->list_objective_quests.resize(max, none<Quest_t*>());
        }
    }

    void MCM_Active_t::Save_State_t::Write()
    {
        Current_View() = this->current_view().As_String();

        List_Current_Page_Index() = this->list_current_page_index;
        List_Hidden_Objectives() = this->list_hidden_objectives;
        List_Objective_Quests() = this->list_objective_quests;

        Item_Current() = this->item_current;
    }

    MCM_Active_t::Option_State_t::Option_State_t() :
        filter(-1),
        options(-1),
        previous(-1),
        next(-1)
    {
    }

    MCM_Active_t::Option_State_t::~Option_State_t()
    {
    }

    MCM_Active_t::List_State_t::List_State_t() :
        hidden_objectives()
    {
    }

    MCM_Active_t::List_State_t::~List_State_t()
    {
    }

    void MCM_Active_t::List_State_t::Read()
    {
        const size_t hidden_objective_count = save_state.list_hidden_objectives.size();
        this->hidden_objectives.reserve(hidden_objective_count);
        this->hidden_objectives.clear();

        for (size_t idx = 0, end = hidden_objective_count; idx < end; idx += 1) {
            maybe<Quest_t*> quest = save_state.list_objective_quests[idx];
            if (quest) {
                maybe<Quest_Objective_t*> quest_objective = quest->Objective(save_state.list_hidden_objectives[idx]);
                if (quest_objective) {
                    this->hidden_objectives.insert(quest_objective());
                }
            }
        }
    }

    void MCM_Active_t::List_State_t::Write()
    {
        const size_t hidden_objective_count = this->hidden_objectives.size();
        save_state.list_hidden_objectives.reserve(hidden_objective_count);
        save_state.list_objective_quests.reserve(hidden_objective_count);
        save_state.list_hidden_objectives.clear();
        save_state.list_objective_quests.clear();

        for (auto it = this->hidden_objectives.begin(), end = this->hidden_objectives.end(); it != end; ++it) {
            maybe<Quest_Objective_t*> quest_objective = *it;
            if (quest_objective && quest_objective->quest) {
                save_state.list_hidden_objectives.push_back(quest_objective->index);
                save_state.list_objective_quests.push_back(quest_objective->quest);
            }
        }
    }

    Int_t MCM_Active_t::List_State_t::Page_Count(size_t quest_count)
    {
        return static_cast<Int_t>(ceilf(
            static_cast<Float_t>(quest_count) / static_cast<Float_t>(LIST_ITEMS_PER_PAGE)
        ));
    }

    Int_t MCM_Active_t::List_State_t::Current_Page_Index(Int_t page_count)
    {
        if (save_state.list_current_page_index < 0) {
            save_state.list_current_page_index = 0;
        } else if (save_state.list_current_page_index >= page_count) {
            save_state.list_current_page_index = page_count - 1;
        }
        return save_state.list_current_page_index;
    }

    Vector_t<some<Quest_t*>> MCM_Active_t::List_State_t::Quests()
    {
        Vector_t<some<Quest_t*>> results;

        Vector_t<some<Quest_Objective_t*>> quest_objectives = Player_t::Self()->Quest_Objectives();
        for (size_t idx = 0, end = quest_objectives.size(); idx < end; idx += 1) {
            some<Quest_Objective_t*> quest_objective = quest_objectives[idx];
            if (quest_objective->quest && !results.Has(quest_objective->quest)) {
                results.push_back(quest_objective->quest);
            }
        }

        // we need to filter this
        
        // we need to sort this

        return results;
    }

    Vector_t<some<Quest_Objective_t*>> MCM_Active_t::List_State_t::Objectives(some<Quest_t*> quest)
    {
        class Filter_t :
            public Filter_i<some<Quest_Objective_t*>>
        {
        public:
            List_State_t&   self;
            some<Quest_t*>  quest;

        public:
            Filter_t(List_State_t& self, some<Quest_t*> quest) :
                self(self), quest(quest)
            {
            }

        public:
            virtual Bool_t operator ()(some<Quest_Objective_t*> quest_objective) override
            {
                return quest_objective->quest == this->quest && self.Is_Objective_Hidable(quest_objective);
            }
        };

        SKYLIB_ASSERT_SOME(quest);

        Filter_t filter(*this, quest);
        Vector_t<some<Quest_Objective_t*>> objectives = Player_t::Self()->Quest_Objectives(filter);

        // we need to filter this
        
        // we need to sort this

        return objectives;
    }

    Bool_t MCM_Active_t::List_State_t::Is_Objective_Hidable(some<Quest_Objective_t*> objective)
    {
        return
            objective->state == Quest_Objective_State_e::COMPLETED_AND_DISPLAYED ||
            objective->state == Quest_Objective_State_e::FAILED_AND_DISPLAYED ||
            objective->state == Quest_Objective_State_e::DISPLAYED;
    }

    Bool_t MCM_Active_t::List_State_t::Is_Objective_Shown(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);

        return this->hidden_objectives.count(objective()) < 1;
    }

    Bool_t MCM_Active_t::List_State_t::Is_Objective_Hidden(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);

        return this->hidden_objectives.count(objective()) > 0;
    }

    void MCM_Active_t::List_State_t::Show_Objective(some<Quest_Objective_t*> objective)
    {
        this->hidden_objectives.erase(objective());
    }

    void MCM_Active_t::List_State_t::Hide_Objective(some<Quest_Objective_t*> objective)
    {
        this->hidden_objectives.insert(objective());
    }

    void MCM_Active_t::List_State_t::Enforce_Objectives()
    {
        class Iterator_t :
            public Iterator_i<some<Player_Objective_t*>>
        {
        public:
            List_State_t& self;

        public:
            Iterator_t(List_State_t& self) :
                self(self)
            {
            }

        public:
            virtual Iterator_e operator ()(some<Player_Objective_t*> player_objective) override
            {
                if (player_objective->objective) {
                    if (self.Is_Objective_Hidable(player_objective->objective()) &&
                        self.Is_Objective_Hidden(player_objective->objective())) {
                        player_objective->state = Quest_Objective_State_e::DORMANT;
                    } else {
                        player_objective->state = player_objective->objective->state;
                    }
                }
                return Iterator_e::CONTINUE;
            }
        };

        Iterator_t iterator(*this);
        Player_t::Self()->Iterate_Player_Objectives(iterator);
    }

    MCM_Active_t::Save_State_t      MCM_Active_t::save_state;
    MCM_Active_t::Option_State_t    MCM_Active_t::option_state;
    MCM_Active_t::List_State_t      MCM_Active_t::list_state;

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

    void MCM_Active_t::Reset_Save_State()
    {
        save_state.~Save_State_t();
        new (&save_state) Save_State_t;
    }

    void MCM_Active_t::Reset_Option_State()
    {
        option_state.~Option_State_t();
        new (&option_state) Option_State_t;
    }

    void MCM_Active_t::Reset_List_State()
    {
        list_state.~List_State_t();
        new (&list_state) List_State_t;
    }

    some<MCM_View_e> MCM_Active_t::Current_View()
    {
        if (!save_state.current_view) {
            save_state.current_view = DEFAULT_CURRENT_VIEW;
        }
        return save_state.current_view();
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
        Reset_Save_State();
        Reset_Option_State();
        Reset_List_State();
    }

    void MCM_Active_t::On_Before_Save_Game()
    {
        list_state.Write();
        save_state.Write();
    }

    void MCM_Active_t::On_After_Save_Game()
    {
    }

    void MCM_Active_t::On_Before_Load_Game()
    {
    }

    void MCM_Active_t::On_After_Load_Game()
    {
        Reset_Save_State();
        Reset_Option_State();
        Reset_List_State();

        save_state.Read();
        list_state.Read();
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
        Reset_Option_State();

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)       On_Page_Open_List(std::move(latent_id), is_refresh);
        else if (current_view == MCM_View_e::ITEM)  On_Page_Open_Item(std::move(latent_id), is_refresh);
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

    void MCM_Active_t::On_Page_Open_List(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        Vector_t<some<Quest_t*>> quests = list_state.Quests();
        size_t quest_count = quests.size();
        if (quest_count > 0) {
            Int_t page_count = list_state.Page_Count(quest_count);
            Int_t page_index = list_state.Current_Page_Index(page_count);

            mcm->Title_Text(mcm->Page_Title(Const::String::$ACTIVE_QUESTS, quest_count, page_index, page_count));

            option_state.filter = mcm->Add_Empty_Option();//Filter_Option() = mcm->Add_Text_Option(Main_t::CENTER_FILTER, Main_t::_NONE_);
            option_state.options = mcm->Add_Empty_Option();//Options_Option() = mcm->Add_Text_Option(Main_t::CENTER_OPTIONS, Main_t::_NONE_);
            if (page_count > 1) {
                option_state.previous = mcm->Add_Empty_Option();//mcm->Add_Text_Option(Main_t::CENTER_GO_TO_PREVIOUS_PAGE, Main_t::_NONE_);
                option_state.next = mcm->Add_Empty_Option();//mcm->Add_Text_Option(Main_t::CENTER_GO_TO_NEXT_PAGE, Main_t::_NONE_);
            } else {
                option_state.previous = mcm->Add_Empty_Option();//mcm->Add_Text_Option(Main_t::CENTER_GO_TO_PREVIOUS_PAGE, Main_t::_NONE_, Flag_e::DISABLE);
                option_state.next = mcm->Add_Empty_Option();//mcm->Add_Text_Option(Main_t::CENTER_GO_TO_NEXT_PAGE, Main_t::_NONE_, Flag_e::DISABLE);
            }

            mcm->Add_Header_Option("");
            mcm->Add_Header_Option("");

            Int_t idx = LIST_ITEMS_PER_PAGE * page_index;
            Int_t end = idx + LIST_ITEMS_PER_PAGE;
            if (end > quest_count) {
                end = quest_count;
            }
            for (; idx < end; idx += 1) {
                some<Quest_t*> quest = quests[idx];
                mcm->Add_Text_Option(quest->Any_Name(), "...");
            }
        } else {
            mcm->Title_Text(mcm->Page_Title(Const::String::ACTIVE_QUESTS, 0, 0, 1));

            /*
            Filter_Option() = mcm->Add_Text_Option(Main_t::CENTER_FILTER, Main_t::_NONE_);
            Options_Option() = mcm->Add_Text_Option(Main_t::CENTER_OPTIONS, Main_t::_NONE_);
            Previous_Page_Option() = mcm->Add_Text_Option(Main_t::CENTER_GO_TO_PREVIOUS_PAGE, Main_t::_NONE_, Flag_e::DISABLE);
            Next_Page_Option() = mcm->Add_Text_Option(Main_t::CENTER_GO_TO_NEXT_PAGE, Main_t::_NONE_, Flag_e::DISABLE);

            mcm->Add_Header_Option(Main_t::NO_LOADED_REFERENCES);
            */
        }
    }

    void MCM_Active_t::On_Page_Open_Item(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Title_Text("WIP");
    }

}}

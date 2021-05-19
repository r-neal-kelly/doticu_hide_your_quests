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
#include "mcm_journal.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    MCM_Journal_t::Save_State_t::Save_State_t() :
        current_view(DEFAULT_CURRENT_VIEW),

        list_current_page_index(DEFAULT_LIST_CURRENT_PAGE_INDEX),
        list_objective_quests(),
        list_hidden_objectives(),

        item_current(none<Quest_t*>())
    {
    }

    MCM_Journal_t::Save_State_t::~Save_State_t()
    {
    }

    Virtual::Variable_tt<String_t>& MCM_Journal_t::Save_State_t::Current_View()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "current_view");
    }

    Virtual::Variable_tt<Int_t>& MCM_Journal_t::Save_State_t::List_Current_Page_Index()
    {
        DEFINE_VARIABLE_REFERENCE(Int_t, "list_current_page_index");
    }

    Virtual::Variable_tt<Vector_t<Int_t>>& MCM_Journal_t::Save_State_t::List_Hidden_Objectives()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "list_hidden_objectives");
    }

    Virtual::Variable_tt<Vector_t<maybe<Quest_t*>>>& MCM_Journal_t::Save_State_t::List_Objective_Quests()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<maybe<Quest_t*>>, "list_objective_quests");
    }

    Virtual::Variable_tt<maybe<Quest_t*>>& MCM_Journal_t::Save_State_t::Item_Current()
    {
        DEFINE_VARIABLE_REFERENCE(maybe<Quest_t*>, "item_current");
    }

    void MCM_Journal_t::Save_State_t::Read()
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

    void MCM_Journal_t::Save_State_t::Write()
    {
        Current_View() = this->current_view().As_String();

        List_Current_Page_Index() = this->list_current_page_index;
        List_Hidden_Objectives() = this->list_hidden_objectives;
        List_Objective_Quests() = this->list_objective_quests;

        Item_Current() = this->item_current;
    }

    MCM_Journal_t::Option_State_t::Option_State_t() :
        filter(-1),
        options(-1),
        previous(-1),
        next(-1),
        back(-1)
    {
    }

    MCM_Journal_t::Option_State_t::~Option_State_t()
    {
    }

    MCM_Journal_t::List_State_t::List_State_t() :
        hidden_objectives(),

        quests(),
        do_update_quests(true)
    {
    }

    MCM_Journal_t::List_State_t::~List_State_t()
    {
    }

    void MCM_Journal_t::List_State_t::Read()
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

    void MCM_Journal_t::List_State_t::Write()
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

    Int_t MCM_Journal_t::List_State_t::Page_Count(size_t item_count)
    {
        return static_cast<Int_t>(ceilf(
            static_cast<Float_t>(item_count) / static_cast<Float_t>(LIST_ITEMS_PER_PAGE)
        ));
    }

    Int_t MCM_Journal_t::List_State_t::Current_Page_Index(Int_t page_count)
    {
        if (save_state.list_current_page_index < 0) {
            save_state.list_current_page_index = 0;
        } else if (save_state.list_current_page_index >= page_count) {
            save_state.list_current_page_index = page_count - 1;
        }
        return save_state.list_current_page_index;
    }

    void MCM_Journal_t::List_State_t::Go_To_Previous_Page(size_t item_count)
    {
        if (item_count > 0) {
            Int_t page_count = Page_Count(item_count);
            Int_t page_index = Current_Page_Index(page_count);
            if (page_index == 0) {
                save_state.list_current_page_index = page_count - 1;
            } else {
                save_state.list_current_page_index = page_index - 1;
            }
        }
    }

    void MCM_Journal_t::List_State_t::Go_To_Next_Page(size_t item_count)
    {
        if (item_count > 0) {
            Int_t page_count = Page_Count(item_count);
            Int_t page_index = Current_Page_Index(page_count);
            if (page_index == page_count - 1) {
                save_state.list_current_page_index = 0;
            } else {
                save_state.list_current_page_index = page_index + 1;
            }
        }
    }

    void MCM_Journal_t::List_State_t::Go_To_Page(maybe<Quest_t*> item)
    {
        if (item) {
            maybe<size_t> maybe_idx = this->quests.Index_Of(item());
            if (maybe_idx.Has_Value()) {
                save_state.list_current_page_index = maybe_idx.Value() / LIST_ITEMS_PER_PAGE;
            }
        }
    }

    Vector_t<some<Quest_t*>>& MCM_Journal_t::List_State_t::Quests()
    {
        if (this->do_update_quests) {
            this->do_update_quests = false;

            this->quests.clear();

            Vector_t<some<Quest_Objective_t*>> quest_objectives = Player_t::Self()->Quest_Objectives();
            for (size_t idx = 0, end = quest_objectives.size(); idx < end; idx += 1) {
                some<Quest_Objective_t*> quest_objective = quest_objectives[idx];
                if (Is_Objective_Hidable(quest_objective) && quest_objective->quest && !this->quests.Has(quest_objective->quest)) {
                    this->quests.push_back(quest_objective->quest);
                }
            }

            // we need to filter this

            this->quests.Sort(
                [](some<Quest_t*>& quest_a, some<Quest_t*>& quest_b)->Int_t
                {
                    Comparator_e result = Form_t::Compare_Names(
                        quest_a->Any_Name(),
                        quest_b->Any_Name()
                    );
                    if (result == Comparator_e::IS_EQUAL) {
                        return quest_a->form_id - quest_b->form_id;
                    } else {
                        return result;
                    }
                }
            );
        }

        return this->quests;
    }

    size_t MCM_Journal_t::List_State_t::Quest_Count()
    {
        return Quests().size();
    }

    void MCM_Journal_t::List_State_t::Queue_Quests_Update()
    {
        this->do_update_quests = true;
    }

    Vector_t<some<Quest_Objective_t*>> MCM_Journal_t::List_State_t::Objectives(some<Quest_t*> quest)
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

        objectives.Sort(
            [](some<Quest_Objective_t*>& objective_a, some<Quest_Objective_t*>& objective_b)->Int_t
            {
                Comparator_e result = Form_t::Compare_Names(
                    objective_a->display_text,
                    objective_b->display_text
                );
                if (result == Comparator_e::IS_EQUAL) {
                    return objective_a->index - objective_b->index;
                } else {
                    return result;
                }
            }
        );

        return objectives;
    }

    Bool_t MCM_Journal_t::List_State_t::Is_Objective_Hidable(some<Quest_Objective_t*> objective)
    {
        return
            objective->state == Quest_Objective_State_e::COMPLETED_AND_DISPLAYED ||
            objective->state == Quest_Objective_State_e::FAILED_AND_DISPLAYED ||
            objective->state == Quest_Objective_State_e::DISPLAYED;
    }

    Bool_t MCM_Journal_t::List_State_t::Is_Objective_Shown(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);

        return this->hidden_objectives.count(objective()) < 1;
    }

    Bool_t MCM_Journal_t::List_State_t::Is_Objective_Hidden(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);

        return this->hidden_objectives.count(objective()) > 0;
    }

    void MCM_Journal_t::List_State_t::Show_Objective(some<Quest_Objective_t*> objective)
    {
        this->hidden_objectives.erase(objective());
    }

    void MCM_Journal_t::List_State_t::Hide_Objective(some<Quest_Objective_t*> objective)
    {
        this->hidden_objectives.insert(objective());
    }

    void MCM_Journal_t::List_State_t::Enforce_Objectives()
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

    MCM_Journal_t::Item_State_t::Item_State_t() :
        objectives(),
        do_update_objectives(true)
    {
    }

    MCM_Journal_t::Item_State_t::~Item_State_t()
    {
    }

    maybe<Quest_t*> MCM_Journal_t::Item_State_t::Current()
    {
        return save_state.item_current;
    }

    void MCM_Journal_t::Item_State_t::Current(maybe<Quest_t*> item)
    {
        save_state.item_current = item;
    }

    maybe<Quest_t*> MCM_Journal_t::Item_State_t::Go_To_Previous_Item()
    {
        maybe<Quest_t*> current_item = Current();
        if (current_item) {
            Vector_t<some<Quest_t*>> items = list_state.Quests();
            maybe<size_t> maybe_idx = items.Index_Of(Current());
            if (maybe_idx.Has_Value()) {
                size_t idx = maybe_idx.Value();
                maybe<Quest_t*> result;
                if (idx == 0) {
                    result = items[items.size() - 1];
                } else {
                    result = items[idx - 1];
                }
                Current(result);
                return result;
            } else {
                return none<Quest_t*>();
            }
        } else {
            return none<Quest_t*>();
        }
    }

    maybe<Quest_t*> MCM_Journal_t::Item_State_t::Go_To_Next_Item()
    {
        maybe<Quest_t*> current_item = Current();
        if (current_item) {
            Vector_t<some<Quest_t*>> items = list_state.Quests();
            maybe<size_t> maybe_idx = items.Index_Of(Current());
            if (maybe_idx.Has_Value()) {
                size_t idx = maybe_idx.Value();
                maybe<Quest_t*> result;
                if (idx == items.size() - 1) {
                    result = items[0];
                } else {
                    result = items[idx + 1];
                }
                Current(result);
                return result;
            } else {
                return none<Quest_t*>();
            }
        } else {
            return none<Quest_t*>();
        }
    }

    Vector_t<some<Quest_Objective_t*>>& MCM_Journal_t::Item_State_t::Objectives()
    {
        if (this->do_update_objectives) {
            this->do_update_objectives = false;

            this->objectives.clear();

            maybe<Quest_t*> current_item = Current();
            if (current_item) {
                this->objectives = list_state.Objectives(current_item());
            }
        }

        return this->objectives;
    }

    size_t MCM_Journal_t::Item_State_t::Objective_Count()
    {
        return Objectives().size();
    }

    void MCM_Journal_t::Item_State_t::Queue_Objectives_Update()
    {
        this->do_update_objectives = true;
    }

    MCM_Journal_t::Save_State_t     MCM_Journal_t::save_state;
    MCM_Journal_t::Option_State_t   MCM_Journal_t::option_state;
    MCM_Journal_t::List_State_t     MCM_Journal_t::list_state;
    MCM_Journal_t::Item_State_t     MCM_Journal_t::item_state;

    some<MCM_t*> MCM_Journal_t::MCM()
    {
        return MCM_t::Self();
    }

    some<MCM_Journal_t*> MCM_Journal_t::Self()
    {
        return Const::Quest::MCM();
    }

    String_t MCM_Journal_t::Class_Name()
    {
        DEFINE_CLASS_NAME("doticu_quest_lookup_mcm_journal");
    }

    some<Virtual::Class_t*> MCM_Journal_t::Class()
    {
        DEFINE_CLASS();
    }

    some<Virtual::Object_t*> MCM_Journal_t::Object()
    {
        DEFINE_COMPONENT_OBJECT_METHOD(Self()());
    }

    void MCM_Journal_t::Reset_Save_State()
    {
        save_state.~Save_State_t();
        new (&save_state) Save_State_t;
    }

    void MCM_Journal_t::Reset_Option_State()
    {
        option_state.~Option_State_t();
        new (&option_state) Option_State_t;
    }

    void MCM_Journal_t::Reset_List_State()
    {
        list_state.~List_State_t();
        new (&list_state) List_State_t;
    }

    void MCM_Journal_t::Reset_Item_State()
    {
        item_state.~Item_State_t();
        new (&item_state) Item_State_t;
    }

    some<MCM_View_e> MCM_Journal_t::Current_View()
    {
        if (!save_state.current_view) {
            save_state.current_view = DEFAULT_CURRENT_VIEW;
        }
        return save_state.current_view();
    }

    void MCM_Journal_t::Current_View(some<MCM_View_e> view)
    {
        SKYLIB_ASSERT_SOME(view);

        save_state.current_view = view;
    }

    void MCM_Journal_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        using type_name = MCM_Journal_t;

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

    void MCM_Journal_t::On_After_New_Game()
    {
        Reset_Save_State();
        Reset_Option_State();
        Reset_List_State();
        Reset_Item_State();
    }

    void MCM_Journal_t::On_Before_Save_Game()
    {
        list_state.Write();
        save_state.Write();
    }

    void MCM_Journal_t::On_After_Save_Game()
    {
    }

    void MCM_Journal_t::On_Before_Load_Game()
    {
    }

    void MCM_Journal_t::On_After_Load_Game()
    {
        Reset_Save_State();
        Reset_Option_State();
        Reset_List_State();
        Reset_Item_State();

        save_state.Read();
        list_state.Read();

        list_state.Enforce_Objectives();
    }

    void MCM_Journal_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
    }

    void MCM_Journal_t::On_Config_Open()
    {
        list_state.Queue_Quests_Update();
        item_state.Queue_Objectives_Update();
    }

    void MCM_Journal_t::On_Config_Close()
    {
    }

    void MCM_Journal_t::On_Page_Open(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        Reset_Option_State();

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)       On_Page_Open_List(std::move(latent_id), is_refresh);
        else if (current_view == MCM_View_e::ITEM)  On_Page_Open_Item(std::move(latent_id), is_refresh);
    }

    void MCM_Journal_t::On_Option_Select(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)       On_Option_Select_List(std::move(latent_id), option);
        else if (current_view == MCM_View_e::ITEM)  On_Option_Select_Item(std::move(latent_id), option);
    }

    void MCM_Journal_t::On_Option_Menu_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Journal_t::On_Option_Menu_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
    }

    void MCM_Journal_t::On_Option_Slider_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Journal_t::On_Option_Slider_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Float_t value)
    {
    }

    void MCM_Journal_t::On_Option_Input_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, String_t value)
    {
    }

    void MCM_Journal_t::On_Option_Keymap_Change(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
    }

    void MCM_Journal_t::On_Option_Default(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Journal_t::On_Option_Highlight(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Journal_t::On_Page_Open_List(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        if (!is_refresh) {
            list_state.Queue_Quests_Update();
        }

        Vector_t<some<Quest_t*>>& quests = list_state.Quests();
        size_t quest_count = quests.size();
        if (quest_count > 0) {
            Int_t page_count = list_state.Page_Count(quest_count);
            Int_t page_index = list_state.Current_Page_Index(page_count);

            mcm->Title_Text(mcm->Page_Title(Const::String::$JOURNAL_QUESTS, quest_count, page_index, page_count));

            option_state.filter = mcm->Add_Text_Option(Const::String::CENTER_FILTER, "", doticu_mcmlib::Flag_e::DISABLE);
            option_state.options = mcm->Add_Text_Option(Const::String::CENTER_OPTIONS, "", doticu_mcmlib::Flag_e::DISABLE);
            if (page_count > 1) {
                option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "");
                option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "");
            } else {
                option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
                option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
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
            mcm->Title_Text(mcm->Page_Title(Const::String::$JOURNAL_QUESTS, 0, 0, 1));

            option_state.filter = mcm->Add_Text_Option(Const::String::CENTER_FILTER, "");
            option_state.options = mcm->Add_Text_Option(Const::String::CENTER_OPTIONS, "");
            option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);
            option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_PAGE, "", doticu_mcmlib::Flag_e::DISABLE);

            mcm->Add_Header_Option(Const::String::NO_QUESTS);
        }
    }

    void MCM_Journal_t::On_Option_Select_List(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.previous) {
            mcm->Disable_Option(option);
            list_state.Go_To_Previous_Page(list_state.Quests().size());
            mcm->Reset_Page();
        } else if (option == option_state.next) {
            mcm->Disable_Option(option);
            list_state.Go_To_Next_Page(list_state.Quests().size());
            mcm->Reset_Page();
        } else {
            Vector_t<some<Quest_t*>> quests = list_state.Quests();
            size_t quest_count = quests.size();
            maybe<size_t> quest_index = mcm->Option_To_Item_Index(
                option,
                quest_count,
                list_state.Current_Page_Index(list_state.Page_Count(quest_count)),
                LIST_HEADERS_PER_PAGE,
                LIST_ITEMS_PER_PAGE
            );
            if (quest_index.Has_Value()) {
                some<Quest_t*> quest = quests[quest_index.Value()];
                mcm->Disable_Option(option);
                item_state.Current(quest);
                Current_View(MCM_View_e::ITEM);
                mcm->Reset_Page();
            }
        }
    }

    void MCM_Journal_t::On_Page_Open_Item(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        maybe<Quest_t*> item = item_state.Current();
        if (item) {
            Vector_t<some<Quest_t*>> items = list_state.Quests();
            maybe<size_t> item_index = items.Index_Of(item());
            if (item_index.Has_Value()) {
                size_t item_count = items.size();
                mcm->Title_Text(mcm->Item_Title(Const::String::$JOURNAL_QUEST, item->Any_Name(), item_index(), item_count));

                option_state.back = mcm->Add_Text_Option(Const::String::CENTER_BACK, "");
                mcm->Add_Empty_Option();
                if (item_count > 1) {
                    option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_QUEST, "");
                    option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_QUEST, "");
                } else {
                    option_state.previous = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_PREVIOUS_QUEST, "", doticu_mcmlib::Flag_e::DISABLE);
                    option_state.next = mcm->Add_Text_Option(Const::String::CENTER_GO_TO_NEXT_QUEST, "", doticu_mcmlib::Flag_e::DISABLE);
                }

                mcm->Add_Header_Option("");
                mcm->Add_Header_Option("");

                {
                    item_state.Queue_Objectives_Update();
                    Vector_t<some<Quest_Objective_t*>>& objectives = item_state.Objectives();
                    size_t objective_count = objectives.size();
                    if (objective_count > 0) {
                        mcm->Add_Text_Option(mcm->Add_Font("_______________________", "", "#80", "27"), Const::String::OBJECTIVES);
                        option_state.show_objectives =
                            mcm->Add_Text_Option(mcm->Add_Font("_______________________", "", "#80", "27"), "");
                        for (size_t idx = 0, end = objectives.size(); idx < end && mcm->Can_Add_Options(1); idx += 1) {
                            some<Quest_Objective_t*> objective = objectives[idx];
                            mcm->Add_Toggle_Option(objective->display_text, list_state.Is_Objective_Shown(objective));
                        }
                    } else {
                        mcm->Add_Text_Option(mcm->Add_Font("_______________________", "", "#80", "27"), Const::String::OBJECTIVES, doticu_mcmlib::Flag_e::DISABLE);
                        option_state.show_objectives =
                            mcm->Add_Text_Option(mcm->Add_Font("_______________________", "", "#80", "27"), "", doticu_mcmlib::Flag_e::DISABLE);
                    }
                }
            } else {
                list_state.Queue_Quests_Update();
                Current_View(MCM_View_e::LIST);
                mcm->Reset_Page();
            }
        } else {
            list_state.Queue_Quests_Update();
            Current_View(MCM_View_e::LIST);
            mcm->Reset_Page();
        }
    }

    void MCM_Journal_t::On_Option_Select_Item(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.back) {
            mcm->Disable_Option(option);
            list_state.Queue_Quests_Update();
            list_state.Go_To_Page(item_state.Current());
            Current_View(MCM_View_e::LIST);
            mcm->Reset_Page();
        } else if (option == option_state.previous) {
            mcm->Disable_Option(option);
            item_state.Go_To_Previous_Item();
            mcm->Reset_Page();
        } else if (option == option_state.next) {
            mcm->Disable_Option(option);
            item_state.Go_To_Next_Item();
            mcm->Reset_Page();
        } else {
            maybe<size_t> index = none<size_t>();
            if (index = mcm->Option_To_Item_Index(option, option_state.show_objectives + 1, item_state.Objective_Count()),
                index.Has_Value()) {
                some<Quest_Objective_t*> objective = item_state.Objectives()[index()];
                if (list_state.Is_Objective_Shown(objective)) {
                    list_state.Hide_Objective(objective);
                } else {
                    list_state.Show_Objective(objective);
                }
                list_state.Enforce_Objectives();
                mcm->Reset_Page();
            }
        }
    }

}}

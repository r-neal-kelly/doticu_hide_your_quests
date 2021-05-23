/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/game.h"
#include "doticu_skylib/math.h"
#include "doticu_skylib/player.h"
#include "doticu_skylib/quest_objective.h"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/virtual_macros.h"

#include "consts.h"
#include "mcm.h"
#include "mcm_journal.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    Quest_And_Label_t::Quest_And_Label_t(some<Quest_t*> quest, Bool_t do_misc_objective) :
        quest(quest)
    {
        SKYLIB_ASSERT_SOME(this->quest);

        if (do_misc_objective) {
            if (this->quest->quest_type == Quest_Type_e::MISC) {
                maybe<Player_Objective_t> objective = Player_t::Self()->Highest_Player_Objective(this->quest);
                if (objective.Has_Value()) {
                    label = objective().Parse_Display_Text();
                } else {
                    label = this->quest->Any_Name();
                }
            } else {
                label = this->quest->Any_Name();
            }
        } else {
            label = this->quest->Any_Name();
        }
    }

    Quest_And_Label_t::Quest_And_Label_t(const Quest_And_Label_t& other) :
        quest(other.quest), label(other.label)
    {
    }

    Quest_And_Label_t::Quest_And_Label_t(Quest_And_Label_t&& other) noexcept :
        quest(std::move(other.quest)), label(std::move(other.label))
    {
    }

    Quest_And_Label_t& Quest_And_Label_t::operator =(const Quest_And_Label_t& other)
    {
        if (this != std::addressof(other)) {
            this->quest = other.quest;
            this->label = other.label;
        }
        return *this;
    }

    Quest_And_Label_t& Quest_And_Label_t::operator =(Quest_And_Label_t&& other) noexcept
    {
        if (this != std::addressof(other)) {
            this->quest = std::move(other.quest);
            this->label = std::move(other.label);
        }
        return *this;
    }

    Quest_And_Label_t::~Quest_And_Label_t()
    {
    }

    MCM_Journal_t::Save_State_t::Save_State_t() :
        current_view(DEFAULT_CURRENT_VIEW),
        hidden_quests(),
        hidden_objectives(),

        list_current_page_index(DEFAULT_LIST_CURRENT_PAGE_INDEX),

        filter_mode(DEFAULT_FILTER_MODE),
        filter_state_active(DEFAULT_FILTER_STATE_ACTIVE),
        filter_state_completed(DEFAULT_FILTER_STATE_COMPLETED),
        filter_state_current(DEFAULT_FILTER_STATE_CURRENT),
        filter_state_failed(DEFAULT_FILTER_STATE_FAILED),
        filter_type_civil_war(DEFAULT_FILTER_TYPE_CIVIL_WAR),
        filter_type_companions(DEFAULT_FILTER_TYPE_COMPANIONS),
        filter_type_daedric(DEFAULT_FILTER_TYPE_DAEDRIC),
        filter_type_dark_brotherhood(DEFAULT_FILTER_TYPE_DARK_BROTHERHOOD),
        filter_type_dawnguard(DEFAULT_FILTER_TYPE_DAWNGUARD),
        filter_type_dragonborn(DEFAULT_FILTER_TYPE_DRAGONBORN),
        filter_type_mages_guild(DEFAULT_FILTER_TYPE_MAGES_GUILD),
        filter_type_main(DEFAULT_FILTER_TYPE_MAIN),
        filter_type_misc(DEFAULT_FILTER_TYPE_MISC),
        filter_type_other(DEFAULT_FILTER_TYPE_OTHER),
        filter_type_side(DEFAULT_FILTER_TYPE_SIDE),
        filter_type_thieves_guild(DEFAULT_FILTER_TYPE_THIEVES_GUILD),

        options_mode(DEFAULT_OPTIONS_MODE),
        options_show_new_objectives(DEFAULT_OPTIONS_SHOW_NEW_OBJECTIVES),
        options_show_finished_quests(DEFAULT_OPTIONS_SHOW_FINISHED_QUESTS),

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

    Virtual::Variable_tt<Vector_t<maybe<Quest_t*>>>& MCM_Journal_t::Save_State_t::Hidden_Quests()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<maybe<Quest_t*>>, "hidden_quests");
    }

    Virtual::Variable_tt<Vector_t<Int_t>>& MCM_Journal_t::Save_State_t::Hidden_Objectives()
    {
        DEFINE_VARIABLE_REFERENCE(Vector_t<Int_t>, "hidden_objectives");
    }

    Virtual::Variable_tt<Int_t>& MCM_Journal_t::Save_State_t::List_Current_Page_Index()
    {
        DEFINE_VARIABLE_REFERENCE(Int_t, "list_current_page_index");
    }

    Virtual::Variable_tt<String_t>& MCM_Journal_t::Save_State_t::Filter_Mode()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "filter_mode");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_State_Active()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_state_active");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_State_Completed()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_state_completed");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_State_Current()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_state_current");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_State_Failed()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_state_failed");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Civil_War()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_civil_war");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Companions()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_companions");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Daedric()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_daedric");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Dark_Brotherhood()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_dark_brotherhood");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Dawnguard()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_dawnguard");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Dragonborn()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_dragonborn");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Mages_Guild()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_mages_guild");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Main()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_main");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Misc()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_misc");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Other()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_other");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Side()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_side");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Filter_Type_Thieves_Guild()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "filter_type_thieves_guild");
    }

    Virtual::Variable_tt<String_t>& MCM_Journal_t::Save_State_t::Options_Mode()
    {
        DEFINE_VARIABLE_REFERENCE(String_t, "options_mode");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Options_Show_New_Objectives()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "options_show_new_objectives");
    }

    Virtual::Variable_tt<Bool_t>& MCM_Journal_t::Save_State_t::Options_Show_Finished_Quests()
    {
        DEFINE_VARIABLE_REFERENCE(Bool_t, "options_show_finished_quests");
    }

    Virtual::Variable_tt<maybe<Quest_t*>>& MCM_Journal_t::Save_State_t::Item_Current()
    {
        DEFINE_VARIABLE_REFERENCE(maybe<Quest_t*>, "item_current");
    }

    void MCM_Journal_t::Save_State_t::Read()
    {
        this->current_view = MCM_View_e::From_String(static_cast<String_t>(Current_View()));
        this->hidden_quests = Hidden_Quests();
        this->hidden_objectives = Hidden_Objectives();

        this->list_current_page_index = List_Current_Page_Index();

        this->filter_mode = Filter_Mode().String();
        this->filter_state_active = Filter_State_Active();
        this->filter_state_completed = Filter_State_Completed();
        this->filter_state_current = Filter_State_Current();
        this->filter_state_failed = Filter_State_Failed();
        this->filter_type_civil_war = Filter_Type_Civil_War();
        this->filter_type_companions = Filter_Type_Companions();
        this->filter_type_daedric = Filter_Type_Daedric();
        this->filter_type_dark_brotherhood = Filter_Type_Dark_Brotherhood();
        this->filter_type_dawnguard = Filter_Type_Dawnguard();
        this->filter_type_dragonborn = Filter_Type_Dragonborn();
        this->filter_type_mages_guild = Filter_Type_Mages_Guild();
        this->filter_type_main = Filter_Type_Main();
        this->filter_type_misc = Filter_Type_Misc();
        this->filter_type_other = Filter_Type_Other();
        this->filter_type_side = Filter_Type_Side();
        this->filter_type_thieves_guild = Filter_Type_Thieves_Guild();

        this->options_mode = Options_Mode().String();
        this->options_show_new_objectives = Options_Show_New_Objectives();
        this->options_show_finished_quests = Options_Show_Finished_Quests();

        this->item_current = Item_Current();

        this->hidden_objectives.resize(this->hidden_quests.size(), -1);
    }

    void MCM_Journal_t::Save_State_t::Write()
    {
        Current_View() = this->current_view().As_String();
        Hidden_Quests() = this->hidden_quests;
        Hidden_Objectives() = this->hidden_objectives;

        List_Current_Page_Index() = this->list_current_page_index;

        Filter_Mode() = this->filter_mode().As_String();
        Filter_State_Active() = this->filter_state_active;
        Filter_State_Completed() = this->filter_state_completed;
        Filter_State_Current() = this->filter_state_current;
        Filter_State_Failed() = this->filter_state_failed;
        Filter_Type_Civil_War() = this->filter_type_civil_war;
        Filter_Type_Companions() = this->filter_type_companions;
        Filter_Type_Daedric() = this->filter_type_daedric;
        Filter_Type_Dark_Brotherhood() = this->filter_type_dark_brotherhood;
        Filter_Type_Dawnguard() = this->filter_type_dawnguard;
        Filter_Type_Dragonborn() = this->filter_type_dragonborn;
        Filter_Type_Mages_Guild() = this->filter_type_mages_guild;
        Filter_Type_Main() = this->filter_type_main;
        Filter_Type_Misc() = this->filter_type_misc;
        Filter_Type_Other() = this->filter_type_other;
        Filter_Type_Side() = this->filter_type_side;
        Filter_Type_Thieves_Guild() = this->filter_type_thieves_guild;

        Options_Mode() = this->options_mode().As_String();
        Options_Show_New_Objectives() = this->options_show_new_objectives;
        Options_Show_Finished_Quests() = this->options_show_finished_quests;

        Item_Current() = this->item_current;
    }

    MCM_Journal_t::Option_State_t::Option_State_t() :
        filter(-1),
        options(-1),
        previous(-1),
        next(-1),
        back(-1),
        reset(-1),
        mode(-1),

        state_active(-1),
        state_completed(-1),
        state_current(-1),
        state_failed(-1),
        type_civil_war(-1),
        type_companions(-1),
        type_daedric(-1),
        type_dark_brotherhood(-1),
        type_dawnguard(-1),
        type_dragonborn(-1),
        type_mages_guild(-1),
        type_main(-1),
        type_misc(-1),
        type_other(-1),
        type_side(-1),
        type_thieves_guild(-1),

        show_new_objectives(-1),
        show_finished_quests(-1),

        show_objectives(-1)
    {
    }

    MCM_Journal_t::Option_State_t::~Option_State_t()
    {
    }

    MCM_Journal_t::List_State_t::List_State_t() :
        quests(),
        do_update_quests(true)
    {
    }

    MCM_Journal_t::List_State_t::~List_State_t()
    {
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
            maybe<size_t> maybe_idx = Index_Of_Quest(item());
            if (maybe_idx.Has_Value()) {
                save_state.list_current_page_index = maybe_idx.Value() / LIST_ITEMS_PER_PAGE;
            }
        }
    }

    Vector_t<Quest_And_Label_t>& MCM_Journal_t::List_State_t::Quests()
    {
        if (this->do_update_quests) {
            this->do_update_quests = false;

            this->quests.clear();

            Vector_t<some<Quest_Objective_t*>> objectives;
            {
                Read_Locker_t locker(Game_t::Form_IDs_To_Forms_Lock());
                Player_t::Self()->Quest_Objectives(objectives, locker);
            }
            filter_state.Filter(objectives);

            Bool_t should_do_misc_objective = options_state.Mode() == MCM_Journal_Mode_e::HIDE;
            for (size_t idx = 0, end = objectives.size(); idx < end; idx += 1) {
                some<Quest_Objective_t*> objective = objectives[idx];
                maybe<Quest_t*> quest = objective->quest;
                if (quest && !Has_Quest(quest()) && Is_Objective_Hidable(objective)) {
                    this->quests.push_back({ quest(), should_do_misc_objective });
                }
            }

            this->quests.Sort(
                [](Quest_And_Label_t& a, Quest_And_Label_t& b)->Int_t
                {
                    Bool_t a_is_finished = a.quest->Is_Completed_Or_Failed();
                    Bool_t b_is_finished = b.quest->Is_Completed_Or_Failed();
                    if (a_is_finished && !b_is_finished) {
                        return Comparator_e::IS_UNORDERED;
                    } else if (!a_is_finished && b_is_finished) {
                        return Comparator_e::IS_ORDERED;
                    } else {
                        Bool_t a_is_misc = a.quest->quest_type == Quest_Type_e::MISC;
                        Bool_t b_is_misc = b.quest->quest_type == Quest_Type_e::MISC;
                        if (a_is_misc && !b_is_misc) {
                            return Comparator_e::IS_UNORDERED;
                        } else if (!a_is_misc && b_is_misc) {
                            return Comparator_e::IS_ORDERED;
                        } else {
                            Comparator_e result = Form_t::Compare_Names(a.label, b.label);
                            if (result == Comparator_e::IS_EQUAL) {
                                return a.quest->form_id - b.quest->form_id;
                            } else {
                                return result;
                            }
                        }
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

    Bool_t MCM_Journal_t::List_State_t::Has_Quest(some<Quest_t*> quest)
    {
        Vector_t<Quest_And_Label_t>& quests = Quests();
        for (size_t idx = 0, end = quests.size(); idx < end; idx += 1) {
            if (quests[idx].quest == quest) {
                return true;
            }
        }
        return false;
    }

    maybe<size_t> MCM_Journal_t::List_State_t::Index_Of_Quest(some<Quest_t*> quest)
    {
        Vector_t<Quest_And_Label_t>& quests = Quests();
        for (size_t idx = 0, end = quests.size(); idx < end; idx += 1) {
            if (quests[idx].quest == quest) {
                return idx;
            }
        }
        return none<size_t>();
    }

    MCM_Journal_t::Filter_State_t::Filter_State_t()
    {
    }

    MCM_Journal_t::Filter_State_t::~Filter_State_t()
    {
    }

    some<Logic_Gate_e> MCM_Journal_t::Filter_State_t::Mode()
    {
        if (!save_state.filter_mode) {
            save_state.filter_mode = DEFAULT_FILTER_MODE;
        }
        return save_state.filter_mode();
    }

    void MCM_Journal_t::Filter_State_t::Mode(String_t name)
    {
        maybe<Logic_Gate_e> mode = name;
        if (mode) {
            save_state.filter_mode = mode;
        }
    }

    Vector_t<String_t> MCM_Journal_t::Filter_State_t::Mode_Names()
    {
        Vector_t<String_t> results;

        results.push_back(Logic_Gate_e(Logic_Gate_e::OR).As_String());
        results.push_back(Logic_Gate_e(Logic_Gate_e::AND).As_String());

        return results;
    }

    Bool_t MCM_Journal_t::Filter_State_t::OR_Flags(some<Quest_t*> quest, Vector_t<Quest_Flags_e>& flags)
    {
        for (size_t idx = 0, end = flags.size(); idx < end; idx += 1) {
            if (quest->quest_flags.Is_Flagged(flags[idx])) {
                return true;
            }
        }
        return false;
    }

    Bool_t MCM_Journal_t::Filter_State_t::AND_Flags(some<Quest_t*> quest, Vector_t<Quest_Flags_e>& flags)
    {
        for (size_t idx = 0, end = flags.size(); idx < end; idx += 1) {
            if (!quest->quest_flags.Is_Flagged(flags[idx])) {
                return false;
            }
        }
        return true;
    }

    void MCM_Journal_t::Filter_State_t::Filter(Vector_t<some<Quest_Objective_t*>>& objectives)
    {
        Vector_t<some<Quest_Objective_State_e>> objective_states;
        objective_states.reserve(3);
        if (save_state.filter_state_completed)  objective_states.push_back(Quest_Objective_State_e::COMPLETED_AND_DISPLAYED);
        if (save_state.filter_state_current)    objective_states.push_back(Quest_Objective_State_e::DISPLAYED);
        if (save_state.filter_state_failed)     objective_states.push_back(Quest_Objective_State_e::FAILED_AND_DISPLAYED);

        Vector_t<Quest_Flags_e> quest_flags;
        quest_flags.reserve(1);
        if (save_state.filter_state_active) quest_flags.push_back(Quest_Flags_e::IS_ACTIVE);

        Vector_t<some<Quest_Type_e>> quest_types;
        quest_types.reserve(12);
        if (save_state.filter_type_civil_war)           quest_types.push_back(Quest_Type_e::CIVIL_WAR);
        if (save_state.filter_type_companions)          quest_types.push_back(Quest_Type_e::COMPANIONS);
        if (save_state.filter_type_daedric)             quest_types.push_back(Quest_Type_e::DAEDRIC);
        if (save_state.filter_type_dark_brotherhood)    quest_types.push_back(Quest_Type_e::DARK_BROTHERHOOD);
        if (save_state.filter_type_dawnguard)           quest_types.push_back(Quest_Type_e::DAWNGUARD);
        if (save_state.filter_type_dragonborn)          quest_types.push_back(Quest_Type_e::DRAGONBORN);
        if (save_state.filter_type_mages_guild)         quest_types.push_back(Quest_Type_e::MAGES_GUILD);
        if (save_state.filter_type_main)                quest_types.push_back(Quest_Type_e::MAIN);
        if (save_state.filter_type_misc)                quest_types.push_back(Quest_Type_e::MISC);
        if (save_state.filter_type_other)               quest_types.push_back(Quest_Type_e::OTHER);
        if (save_state.filter_type_side)                quest_types.push_back(Quest_Type_e::SIDE);
        if (save_state.filter_type_thieves_guild)       quest_types.push_back(Quest_Type_e::THIEVES_GUILD);

        some<Logic_Gate_e> mode = Mode();
        if (mode == Logic_Gate_e::OR) {
            for (size_t idx = 0; idx < objectives.size();) {
                some<Quest_Objective_t*> objective = objectives[idx];
                maybe<Quest_t*> quest = objective->quest;
                if (quest) {
                    if (objective_states.Has(objective->state) ||
                        quest_types.Has(quest->quest_type()) ||
                        OR_Flags(quest(), quest_flags)) {
                        idx += 1;
                    } else {
                        objectives.Unstable_Remove(idx);
                    }
                } else {
                    objectives.Unstable_Remove(idx);
                }
            }
        } else if (mode == Logic_Gate_e::AND) {
            for (size_t idx = 0; idx < objectives.size();) {
                some<Quest_Objective_t*> objective = objectives[idx];
                maybe<Quest_t*> quest = objective->quest;
                if (quest) {
                    if ((!objective_states.size() || objective_states.Has(objective->state)) &&
                        (!quest_types.size() || quest_types.Has(quest->quest_type())) &&
                        (!quest_flags.size() || AND_Flags(quest(), quest_flags))) {
                        idx += 1;
                    } else {
                        objectives.Unstable_Remove(idx);
                    }
                } else {
                    objectives.Unstable_Remove(idx);
                }
            }
        }
    }

    void MCM_Journal_t::Filter_State_t::Reset()
    {
        some<Logic_Gate_e> mode = Mode();
        if (mode == Logic_Gate_e::OR) {

        } else if (mode == Logic_Gate_e::AND) {

        }
    }

    MCM_Journal_t::Options_State_t::Options_State_t()
    {
    }

    MCM_Journal_t::Options_State_t::~Options_State_t()
    {
    }

    some<MCM_Journal_Mode_e> MCM_Journal_t::Options_State_t::Mode()
    {
        if (!save_state.options_mode) {
            save_state.options_mode = DEFAULT_OPTIONS_MODE;
        }
        return save_state.options_mode();
    }

    void MCM_Journal_t::Options_State_t::Mode(String_t name)
    {
        maybe<MCM_Journal_Mode_e> mode = name;
        if (mode) {
            save_state.options_mode = mode;
        }
    }

    Vector_t<String_t> MCM_Journal_t::Options_State_t::Mode_Names()
    {
        Vector_t<String_t> results;

        results.push_back(MCM_Journal_Mode_e(MCM_Journal_Mode_e::LOOKUP).As_String());
        results.push_back(MCM_Journal_Mode_e(MCM_Journal_Mode_e::HIDE).As_String());

        return results;
    }

    Bool_t MCM_Journal_t::Options_State_t::Do_Show_New_Objectives()
    {
        return save_state.options_show_new_objectives;
    }

    void MCM_Journal_t::Options_State_t::Do_Show_New_Objectives(Bool_t value)
    {
        save_state.options_show_new_objectives = value;
    }

    Bool_t MCM_Journal_t::Options_State_t::Do_Show_Finished_Quests()
    {
        return save_state.options_show_finished_quests;
    }

    void MCM_Journal_t::Options_State_t::Do_Show_Finished_Quests(Bool_t value)
    {
        save_state.options_show_finished_quests = value;
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
            maybe<size_t> maybe_idx = list_state.Index_Of_Quest(current_item());
            if (maybe_idx.Has_Value()) {
                Vector_t<Quest_And_Label_t> items = list_state.Quests();
                size_t idx = maybe_idx.Value();
                maybe<Quest_t*> result;
                if (idx == 0) {
                    result = items[items.size() - 1].quest;
                } else {
                    result = items[idx - 1].quest;
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
            maybe<size_t> maybe_idx = list_state.Index_Of_Quest(current_item());
            if (maybe_idx.Has_Value()) {
                Vector_t<Quest_And_Label_t> items = list_state.Quests();
                size_t idx = maybe_idx.Value();
                maybe<Quest_t*> result;
                if (idx == items.size() - 1) {
                    result = items[0].quest;
                } else {
                    result = items[idx + 1].quest;
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

    Vector_t<Player_Objective_t>& MCM_Journal_t::Item_State_t::Objectives()
    {
        if (this->do_update_objectives) {
            this->do_update_objectives = false;

            this->objectives.clear();

            maybe<Quest_t*> quest = Current();
            if (quest) {
                class Iterator_t :
                    public Iterator_i<some<Player_Objective_t*>>
                {
                public:
                    some<Quest_t*> quest;

                public:
                    Iterator_t(some<Quest_t*> quest) :
                        quest(quest)
                    {
                    }

                public:
                    virtual Iterator_e operator ()(some<Player_Objective_t*> it) override
                    {
                        if (it->objective && it->objective->quest == this->quest && !item_state.objectives.Has(*it)) {
                            if (Is_Objective_Hidable(it->objective())) {
                                item_state.objectives.push_back(*it);
                            }
                        }
                        return Iterator_e::CONTINUE;
                    }
                };

                Iterator_t iterator(quest());
                Read_Locker_t locker(Game_t::Form_IDs_To_Forms_Lock());
                Player_t::Self()->Iterate_Player_Objectives(iterator, locker);

                this->objectives.Sort(
                    [](Player_Objective_t& objective_a, Player_Objective_t& objective_b)->Int_t
                    {
                        Comparator_e result = objective_b.instance_id - objective_a.instance_id;
                        if (result == Comparator_e::IS_EQUAL) {
                            result = objective_b.objective->index - objective_a.objective->index;
                        }
                        return result;
                    }
                );
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

    std::mutex                          MCM_Journal_t::lock;
    MCM_Journal_t::Save_State_t         MCM_Journal_t::save_state;
    MCM_Journal_t::Option_State_t       MCM_Journal_t::option_state;
    MCM_Journal_t::List_State_t         MCM_Journal_t::list_state;
    MCM_Journal_t::Filter_State_t       MCM_Journal_t::filter_state;
    MCM_Journal_t::Options_State_t      MCM_Journal_t::options_state;
    MCM_Journal_t::Item_State_t         MCM_Journal_t::item_state;

    std::unordered_map<Quest_t*, u16>   MCM_Journal_t::hidden_quests;

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

    void MCM_Journal_t::Reset_Filter_State()
    {
        filter_state.~Filter_State_t();
        new (&filter_state) Filter_State_t;
    }

    void MCM_Journal_t::Reset_Options_State()
    {
        options_state.~Options_State_t();
        new (&options_state) Options_State_t;
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

    Bool_t MCM_Journal_t::Is_Objective_Hidable(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);
        SKYLIB_ASSERT_SOME(objective->quest);

        if (objective->quest->quest_type == Quest_Type_e::MISC) {
            return objective->state == Quest_Objective_State_e::DISPLAYED;
        } else if (objective->quest->Is_Displayed_In_HUD()) {
            return
                objective->state == Quest_Objective_State_e::COMPLETED_AND_DISPLAYED ||
                objective->state == Quest_Objective_State_e::FAILED_AND_DISPLAYED ||
                objective->state == Quest_Objective_State_e::DISPLAYED;
        } else {
            return false;
        }
    }

    Bool_t MCM_Journal_t::Has_Hidden_Objective(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);
        SKYLIB_ASSERT_SOME(objective->quest);

        if (hidden_quests.count(objective->quest()) > 0) {
            if (options_state.Do_Show_New_Objectives()) {
                return hidden_quests[objective->quest()] >= objective->index;
            } else {
                return true;
            }
        } else {
            return false;
        }
    }

    Bool_t MCM_Journal_t::Has_Hidden_Objective(some<Quest_t*> quest)
    {
        SKYLIB_ASSERT_SOME(quest);

        if (hidden_quests.count(quest()) > 0) {
            if (options_state.Do_Show_New_Objectives()) {
                maybe<Player_Objective_t> objective = Highest_Objective(quest);
                if (objective.Has_Value()) {
                    return hidden_quests[quest()] >= objective().objective->index;
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

    void MCM_Journal_t::Add_Hidden_Objective(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);
        SKYLIB_ASSERT_SOME(objective->quest);

        if (options_state.Do_Show_New_Objectives()) {
            hidden_quests[objective->quest()] = objective->index;
        } else {
            Add_Hidden_Objective(objective->quest());
        }
    }

    void MCM_Journal_t::Add_Hidden_Objective(some<Quest_t*> quest)
    {
        SKYLIB_ASSERT_SOME(quest);

        maybe<Player_Objective_t> objective = Highest_Objective(quest);
        if (objective.Has_Value()) {
            hidden_quests[quest()] = objective().objective->index;
        } else {
            hidden_quests[quest()] = 0;
        }
    }

    void MCM_Journal_t::Remove_Hidden_Objective(some<Quest_Objective_t*> objective)
    {
        SKYLIB_ASSERT_SOME(objective);
        SKYLIB_ASSERT_SOME(objective->quest);

        if (options_state.Do_Show_New_Objectives()) {
            maybe<Quest_Objective_Index_t> lowest_index = objective->quest->Lowest_Objective_Index();
            SKYLIB_ASSERT_SOME(lowest_index.Has_Value());

            if (objective->index > lowest_index.Value()) {
                hidden_quests[objective->quest()] = objective->index - 1;
            } else {
                hidden_quests.erase(objective->quest());
            }
        } else {
            Remove_Hidden_Objective(objective->quest());
        }
    }

    void MCM_Journal_t::Remove_Hidden_Objective(some<Quest_t*> quest)
    {
        SKYLIB_ASSERT_SOME(quest);

        hidden_quests.erase(quest());
    }

    void MCM_Journal_t::Enforce_Objectives()
    {
        class Iterator_t :
            public Iterator_i<some<Player_Objective_t*>>
        {
        public:
            virtual Iterator_e operator ()(some<Player_Objective_t*> it) override
            {
                if (it->objective) {
                    if (it->objective->quest && Is_Objective_Hidable(it->objective())) {
                        if (!options_state.Do_Show_Finished_Quests() && it->objective->quest->Is_Completed_Or_Failed()) {
                            it->state = Quest_Objective_State_e::DORMANT;
                        } else if (Has_Hidden_Objective(it->objective())) {
                            it->state = Quest_Objective_State_e::DORMANT;
                        } else {
                            it->state = it->objective->state;
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

    maybe<Player_Objective_t> MCM_Journal_t::Highest_Objective(some<Quest_t*> quest)
    {
        Read_Locker_t locker(Game_t::Form_IDs_To_Forms_Lock());
        return Player_t::Self()->Highest_Player_Objective(quest, locker);
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
        std::lock_guard<std::mutex> locker(lock);

        Reset_Save_State();
        Reset_Option_State();
        Reset_List_State();
        Reset_Filter_State();
        Reset_Options_State();
        Reset_Item_State();
    }

    void MCM_Journal_t::On_Before_Save_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        const size_t count = save_state.hidden_quests.size();
        save_state.hidden_quests.reserve(count);
        save_state.hidden_objectives.reserve(count);
        save_state.hidden_quests.clear();
        save_state.hidden_objectives.clear();

        for (auto it = hidden_quests.begin(), end = hidden_quests.end(); it != end; ++it) {
            maybe<Quest_t*> quest = it->first;
            u16 objective_index = it->second;
            if (quest) {
                save_state.hidden_quests.push_back(quest);
                save_state.hidden_objectives.push_back(objective_index);
            }
        }

        save_state.Write();
    }

    void MCM_Journal_t::On_After_Save_Game()
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Before_Load_Game()
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_After_Load_Game()
    {
        std::lock_guard<std::mutex> locker(lock);

        Reset_Save_State();
        Reset_Option_State();
        Reset_List_State();
        Reset_Filter_State();
        Reset_Options_State();
        Reset_Item_State();

        save_state.Read();

        const size_t count = save_state.hidden_quests.size();
        hidden_quests.reserve(count);
        hidden_quests.clear();

        for (size_t idx = 0, end = count; idx < end; idx += 1) {
            maybe<Quest_t*> quest = save_state.hidden_quests[idx];
            if (quest) {
                maybe<Quest_Objective_t*> objective = quest->Objective(save_state.hidden_objectives[idx]);
                if (objective) {
                    hidden_quests[quest()] = objective->index;
                } else {
                    hidden_quests[quest()] = 0;
                }
            }
        }

        Enforce_Objectives();
    }

    void MCM_Journal_t::On_Update()
    {
        if (!UI_t::Is_Menu_Open("Journal Menu")) {
            std::lock_guard<std::mutex> locker(lock);

            Enforce_Objectives();
        }
    }

    void MCM_Journal_t::On_Update_Version(const Version_t<u16> version_to_update)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Config_Open()
    {
        std::lock_guard<std::mutex> locker(lock);

        list_state.Queue_Quests_Update();
        item_state.Queue_Objectives_Update();
    }

    void MCM_Journal_t::On_Config_Close()
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Page_Open(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        std::lock_guard<std::mutex> locker(lock);

        Reset_Option_State();

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)           On_Page_Open_List(std::move(latent_id), is_refresh);
        else if (current_view == MCM_View_e::FILTER)    On_Page_Open_Filter(std::move(latent_id), is_refresh);
        else if (current_view == MCM_View_e::OPTIONS)   On_Page_Open_Options(std::move(latent_id), is_refresh);
        else if (current_view == MCM_View_e::ITEM)      On_Page_Open_Item(std::move(latent_id), is_refresh);
    }

    void MCM_Journal_t::On_Option_Select(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)           On_Option_Select_List(std::move(latent_id), option);
        else if (current_view == MCM_View_e::FILTER)    On_Option_Select_Filter(std::move(latent_id), option);
        else if (current_view == MCM_View_e::OPTIONS)   On_Option_Select_Options(std::move(latent_id), option);
        else if (current_view == MCM_View_e::ITEM)      On_Option_Select_Item(std::move(latent_id), option);
    }

    void MCM_Journal_t::On_Option_Menu_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)           On_Option_Menu_Open_List(std::move(latent_id), option);
        else if (current_view == MCM_View_e::FILTER)    On_Option_Menu_Open_Filter(std::move(latent_id), option);
        else if (current_view == MCM_View_e::OPTIONS)   On_Option_Menu_Open_Options(std::move(latent_id), option);
        else if (current_view == MCM_View_e::ITEM)      On_Option_Menu_Open_Item(std::move(latent_id), option);
    }

    void MCM_Journal_t::On_Option_Menu_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
        std::lock_guard<std::mutex> locker(lock);

        some<MCM_View_e> current_view = Current_View();
        if (current_view == MCM_View_e::LIST)           On_Option_Menu_Accept_List(std::move(latent_id), option, index);
        else if (current_view == MCM_View_e::FILTER)    On_Option_Menu_Accept_Filter(std::move(latent_id), option, index);
        else if (current_view == MCM_View_e::OPTIONS)   On_Option_Menu_Accept_Options(std::move(latent_id), option, index);
        else if (current_view == MCM_View_e::ITEM)      On_Option_Menu_Accept_Item(std::move(latent_id), option, index);
    }

    void MCM_Journal_t::On_Option_Slider_Open(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Option_Slider_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, Float_t value)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Option_Input_Accept(Virtual::Latent_ID_t&& latent_id, Int_t option, String_t value)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Option_Keymap_Change(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t key, String_t conflict, String_t mod)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Option_Default(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Option_Highlight(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        std::lock_guard<std::mutex> locker(lock);
    }

    void MCM_Journal_t::On_Page_Open_List(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        if (!is_refresh) {
            list_state.Queue_Quests_Update();
        }

        Vector_t<Quest_And_Label_t>& quests = list_state.Quests();
        size_t quest_count = quests.size();
        if (quest_count > 0) {
            Int_t page_count = list_state.Page_Count(quest_count);
            Int_t page_index = list_state.Current_Page_Index(page_count);

            mcm->Title_Text(mcm->Page_Title(Const::String::$JOURNAL_QUESTS, quest_count, page_index, page_count));

            option_state.filter = mcm->Add_Text_Option(Const::String::CENTER_FILTER, "");
            option_state.options = mcm->Add_Text_Option(Const::String::CENTER_OPTIONS, "");
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

            if (options_state.Mode() == MCM_Journal_Mode_e::LOOKUP) {
                for (; idx < end; idx += 1) {
                    Quest_And_Label_t& item = quests[idx];
                    if (item.quest->Is_Completed_Or_Failed()) {
                        mcm->Add_Text_Option(mcm->Add_Font(quests[idx].label, Const::String::COLOR_BLUE), "...");
                    } else {
                        mcm->Add_Text_Option(mcm->Add_Font(quests[idx].label, Const::String::COLOR_WHITE), "...");
                    }
                }
            } else if (options_state.Mode() == MCM_Journal_Mode_e::HIDE) {
                for (; idx < end; idx += 1) {
                    Quest_And_Label_t& item = quests[idx];
                    if (item.quest->Is_Completed_Or_Failed()) {
                        mcm->Add_Toggle_Option(mcm->Add_Font(quests[idx].label, Const::String::COLOR_BLUE),
                                               !Has_Hidden_Objective(item.quest));
                    } else {
                        mcm->Add_Toggle_Option(mcm->Add_Font(quests[idx].label, Const::String::COLOR_WHITE),
                                               !Has_Hidden_Objective(item.quest));
                    }
                }
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

        if (option == option_state.filter) {
            mcm->Disable_Option(option);
            Current_View(MCM_View_e::FILTER);
            mcm->Reset_Page();
        } else if (option == option_state.options) {
            mcm->Disable_Option(option);
            Current_View(MCM_View_e::OPTIONS);
            mcm->Reset_Page();
        } else if (option == option_state.previous) {
            mcm->Disable_Option(option);
            list_state.Go_To_Previous_Page(list_state.Quests().size());
            mcm->Reset_Page();
        } else if (option == option_state.next) {
            mcm->Disable_Option(option);
            list_state.Go_To_Next_Page(list_state.Quests().size());
            mcm->Reset_Page();
        } else {
            Vector_t<Quest_And_Label_t> quests = list_state.Quests();
            size_t quest_count = quests.size();
            maybe<size_t> quest_index = mcm->Option_To_Item_Index(
                option,
                quest_count,
                list_state.Current_Page_Index(list_state.Page_Count(quest_count)),
                LIST_HEADERS_PER_PAGE,
                LIST_ITEMS_PER_PAGE
            );
            if (quest_index.Has_Value()) {
                some<Quest_t*> quest = quests[quest_index.Value()].quest;
                if (options_state.Mode() == MCM_Journal_Mode_e::LOOKUP) {
                    mcm->Disable_Option(option);
                    item_state.Current(quest);
                    Current_View(MCM_View_e::ITEM);
                    mcm->Reset_Page();
                } else if (options_state.Mode() == MCM_Journal_Mode_e::HIDE) {
                    if (Has_Hidden_Objective(quest)) {
                        Remove_Hidden_Objective(quest);
                        mcm->Toggle(option, true);
                    } else {
                        Add_Hidden_Objective(quest);
                        mcm->Toggle(option, false);
                    }
                    Enforce_Objectives();
                }
            }
        }
    }

    void MCM_Journal_t::On_Option_Menu_Open_List(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Journal_t::On_Option_Menu_Accept_List(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
    }

    void MCM_Journal_t::On_Page_Open_Filter(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        mcm->Title_Text(Const::String::JOURNAL_FILTER);
        option_state.back = mcm->Add_Text_Option(Const::String::CENTER_BACK, "");
        option_state.reset = mcm->Add_Text_Option(Const::String::CENTER_RESET, "");

        mcm->Add_Header_Option("");
        mcm->Add_Header_Option("");
        option_state.mode = mcm->Add_Menu_Option(Const::String::MODE, filter_state.Mode()().As_String());
        mcm->Add_Empty_Option();

        mcm->Add_Header_Option(Const::String::STATE);
        mcm->Add_Header_Option("");
        option_state.state_active = mcm->Add_Toggle_Option(Const::String::ACTIVE, save_state.filter_state_active);
        option_state.state_completed = mcm->Add_Toggle_Option(Const::String::COMPLETED, save_state.filter_state_completed);
        option_state.state_current = mcm->Add_Toggle_Option(Const::String::CURRENT, save_state.filter_state_current);
        option_state.state_failed = mcm->Add_Toggle_Option(Const::String::FAILED, save_state.filter_state_failed);

        mcm->Add_Header_Option(Const::String::TYPE);
        mcm->Add_Header_Option("");
        option_state.type_civil_war = mcm->Add_Toggle_Option(Const::String::CIVIL_WAR, save_state.filter_type_civil_war);
        option_state.type_companions = mcm->Add_Toggle_Option(Const::String::COMPANIONS, save_state.filter_type_companions);
        option_state.type_daedric = mcm->Add_Toggle_Option(Const::String::DAEDRIC, save_state.filter_type_daedric);
        option_state.type_dark_brotherhood = mcm->Add_Toggle_Option(Const::String::DARK_BROTHERHOOD, save_state.filter_type_dark_brotherhood);
        option_state.type_dawnguard = mcm->Add_Toggle_Option(Const::String::DAWNGUARD, save_state.filter_type_dawnguard);
        option_state.type_dragonborn = mcm->Add_Toggle_Option(Const::String::DRAGONBORN, save_state.filter_type_dragonborn);
        option_state.type_mages_guild = mcm->Add_Toggle_Option(Const::String::MAGES_GUILD, save_state.filter_type_mages_guild);
        option_state.type_main = mcm->Add_Toggle_Option(Const::String::MAIN, save_state.filter_type_main);
        option_state.type_misc = mcm->Add_Toggle_Option(Const::String::MISC, save_state.filter_type_misc);
        option_state.type_other = mcm->Add_Toggle_Option(Const::String::OTHER, save_state.filter_type_other);
        option_state.type_side = mcm->Add_Toggle_Option(Const::String::SIDE, save_state.filter_type_side);
        option_state.type_thieves_guild = mcm->Add_Toggle_Option(Const::String::THIEVES_GUILD, save_state.filter_type_thieves_guild);
    }

    void MCM_Journal_t::On_Option_Select_Filter(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.back) {
            mcm->Disable_Option(option);
            list_state.Queue_Quests_Update();
            list_state.Go_To_Page(item_state.Current());
            Current_View(MCM_View_e::LIST);
            mcm->Reset_Page();

        } else if (option == option_state.state_active) {
            mcm->Toggle(option_state.state_active, save_state.filter_state_active);
        } else if (option == option_state.state_completed) {
            mcm->Toggle(option_state.state_completed, save_state.filter_state_completed);
        } else if (option == option_state.state_current) {
            mcm->Toggle(option_state.state_current, save_state.filter_state_current);
        } else if (option == option_state.state_failed) {
            mcm->Toggle(option_state.state_failed, save_state.filter_state_failed);

        } else if (option == option_state.type_civil_war) {
            mcm->Toggle(option_state.type_civil_war, save_state.filter_type_civil_war);
        } else if (option == option_state.type_companions) {
            mcm->Toggle(option_state.type_companions, save_state.filter_type_companions);
        } else if (option == option_state.type_daedric) {
            mcm->Toggle(option_state.type_daedric, save_state.filter_type_daedric);
        } else if (option == option_state.type_dark_brotherhood) {
            mcm->Toggle(option_state.type_dark_brotherhood, save_state.filter_type_dark_brotherhood);
        } else if (option == option_state.type_dawnguard) {
            mcm->Toggle(option_state.type_dawnguard, save_state.filter_type_dawnguard);
        } else if (option == option_state.type_dragonborn) {
            mcm->Toggle(option_state.type_dragonborn, save_state.filter_type_dragonborn);
        } else if (option == option_state.type_mages_guild) {
            mcm->Toggle(option_state.type_mages_guild, save_state.filter_type_mages_guild);
        } else if (option == option_state.type_main) {
            mcm->Toggle(option_state.type_main, save_state.filter_type_main);
        } else if (option == option_state.type_misc) {
            mcm->Toggle(option_state.type_misc, save_state.filter_type_misc);
        } else if (option == option_state.type_other) {
            mcm->Toggle(option_state.type_other, save_state.filter_type_other);
        } else if (option == option_state.type_side) {
            mcm->Toggle(option_state.type_side, save_state.filter_type_side);
        } else if (option == option_state.type_thieves_guild) {
            mcm->Toggle(option_state.type_thieves_guild, save_state.filter_type_thieves_guild);
        }
    }

    void MCM_Journal_t::On_Option_Menu_Open_Filter(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.mode) {
            mcm->Flicker_Option(option);
            mcm->Menu_Dialog_Values(filter_state.Mode_Names());
            mcm->Menu_Dialog_Default(0);
        }
    }

    void MCM_Journal_t::On_Option_Menu_Accept_Filter(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
        some<MCM_t*> mcm = MCM();

        if (index > -1) {
            if (option == option_state.mode) {
                Vector_t<String_t> values = filter_state.Mode_Names();
                if (index < values.size()) {
                    filter_state.Mode(values[index]);
                    mcm->Menu_Option_Value(option_state.mode, filter_state.Mode()().As_String(), true);
                }
            }
        }
    }

    void MCM_Journal_t::On_Page_Open_Options(Virtual::Latent_ID_t&& latent_id, Bool_t is_refresh)
    {
        some<MCM_t*> mcm = MCM();

        mcm->Current_Cursor_Position() = 0;
        mcm->Current_Cursor_Mode() = doticu_mcmlib::Cursor_e::LEFT_TO_RIGHT;

        mcm->Title_Text(Const::String::JOURNAL_OPTIONS);
        option_state.back = mcm->Add_Text_Option(Const::String::CENTER_BACK, "");
        option_state.reset = mcm->Add_Text_Option(Const::String::CENTER_RESET, "");

        mcm->Add_Header_Option("");
        mcm->Add_Header_Option("");
        option_state.mode = mcm->Add_Menu_Option(Const::String::MODE, options_state.Mode()().As_String());
        mcm->Add_Empty_Option();
        option_state.show_new_objectives = mcm->Add_Toggle_Option(Const::String::SHOW_NEW_OBJECTIVES, save_state.options_show_new_objectives);
        option_state.show_finished_quests = mcm->Add_Toggle_Option(Const::String::SHOW_FINISHED_QUESTS, save_state.options_show_finished_quests);
    }

    void MCM_Journal_t::On_Option_Select_Options(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.back) {
            mcm->Disable_Option(option);
            list_state.Queue_Quests_Update();
            list_state.Go_To_Page(item_state.Current());
            Current_View(MCM_View_e::LIST);
            mcm->Reset_Page();

        } else if (option == option_state.show_new_objectives) {
            mcm->Toggle(option_state.show_new_objectives, save_state.options_show_new_objectives);
            Enforce_Objectives();
        } else if (option == option_state.show_finished_quests) {
            mcm->Toggle(option_state.show_finished_quests, save_state.options_show_finished_quests);
            Enforce_Objectives();
        }
    }

    void MCM_Journal_t::On_Option_Menu_Open_Options(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
        some<MCM_t*> mcm = MCM();

        if (option == option_state.mode) {
            mcm->Flicker_Option(option);
            mcm->Menu_Dialog_Values(options_state.Mode_Names());
            mcm->Menu_Dialog_Default(1);
        }
    }

    void MCM_Journal_t::On_Option_Menu_Accept_Options(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
        some<MCM_t*> mcm = MCM();

        if (index > -1) {
            if (option == option_state.mode) {
                Vector_t<String_t> values = options_state.Mode_Names();
                if (index < values.size()) {
                    options_state.Mode(values[index]);
                    mcm->Menu_Option_Value(option_state.mode, options_state.Mode()().As_String(), true);
                }
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
            maybe<size_t> item_index = list_state.Index_Of_Quest(item());
            if (item_index.Has_Value()) {
                Vector_t<Quest_And_Label_t> items = list_state.Quests();
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
                    Vector_t<Player_Objective_t>& objectives = item_state.Objectives();
                    size_t objective_count = objectives.size();
                    if (objective_count > 0) {
                        mcm->Add_Text_Option(mcm->Add_Font("_______________________", "", "#80", "27"), Const::String::OBJECTIVES);
                        option_state.show_objectives =
                            mcm->Add_Text_Option(mcm->Add_Font("_______________________", "", "#80", "27"), "");
                        for (size_t idx = 0, end = objectives.size(); idx < end && mcm->Can_Add_Options(1); idx += 1) {
                            Player_Objective_t& objective = objectives[idx];
                            mcm->Add_Toggle_Option(objective.Parse_Display_Text(), !Has_Hidden_Objective(objective.objective()));
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
            maybe<size_t> index = mcm->Option_To_Item_Index(
                option,
                option_state.show_objectives + 1,
                item_state.Objective_Count()
            );
            if (index.Has_Value()) {
                mcm->Disable_Option(option);
                Player_Objective_t& objective = item_state.Objectives()[index()];
                if (Has_Hidden_Objective(objective.objective())) {
                    Remove_Hidden_Objective(objective.objective());
                } else {
                    Add_Hidden_Objective(objective.objective());
                }
                Enforce_Objectives();
                mcm->Reset_Page();
            }
        }
    }

    void MCM_Journal_t::On_Option_Menu_Open_Item(Virtual::Latent_ID_t&& latent_id, Int_t option)
    {
    }

    void MCM_Journal_t::On_Option_Menu_Accept_Item(Virtual::Latent_ID_t&& latent_id, Int_t option, Int_t index)
    {
    }

}}

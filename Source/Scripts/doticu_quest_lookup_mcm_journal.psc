; Copyright © 2020 r-neal-kelly, aka doticu

Scriptname doticu_quest_lookup_mcm_journal extends Quest

string      current_view                        = ""
Quest[]     hidden_quests                       = none
int[]       hidden_objectives                   = none

int         list_current_page_index             = -1

string      filter_mode                         = ""
bool        filter_state_active                 = false
bool        filter_state_completed              = false
bool        filter_state_current                = false
bool        filter_state_failed                 = false
bool        filter_type_civil_war               = false
bool        filter_type_companions              = false
bool        filter_type_daedric                 = false
bool        filter_type_dark_brotherhood        = false
bool        filter_type_dawnguard               = false
bool        filter_type_dragonborn              = false
bool        filter_type_mages_guild             = false
bool        filter_type_main                    = false
bool        filter_type_misc                    = false
bool        filter_type_other                   = false
bool        filter_type_side                    = false
bool        filter_type_thieves_guild           = false

string      options_mode                        = ""
bool        options_show_new_objectives         = false
bool        options_show_finished_quests        = false

Quest       item_current                        = none

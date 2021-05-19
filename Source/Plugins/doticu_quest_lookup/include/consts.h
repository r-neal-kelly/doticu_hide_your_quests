/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "intrinsic.h"

namespace doticu_skylib { namespace doticu_quest_lookup { namespace Const {

    namespace Global {

        some<Global_t*> Is_Installed();
        some<Global_t*> Version_Major();
        some<Global_t*> Version_Minor();
        some<Global_t*> Version_Patch();

    }

    namespace Mod {

        maybe<Mod_t*>   Self();

    }

    namespace Quest {

        some<Quest_t*>  MCM();

    }

    namespace String {

        static constexpr const char*    JOURNAL_QUESTS              = "$dquestl_journal_quests";
        static constexpr const char*    $JOURNAL_QUEST              = "Journal Quest";
        static constexpr const char*    $JOURNAL_QUESTS             = "Journal Quests";

        static constexpr const char*    OBJECTIVES                  = "$dquestl_objectives";

        static constexpr const char*    NO_QUESTS                   = "$dquestl_no_quests";

        static constexpr const char*    CENTER_FILTER               = "$dquestl_center_filter";
        static constexpr const char*    CENTER_OPTIONS              = "$dquestl_center_options";
        static constexpr const char*    CENTER_GO_TO_PREVIOUS_PAGE  = "$dquestl_center_go_to_previous_page";
        static constexpr const char*    CENTER_GO_TO_NEXT_PAGE      = "$dquestl_center_go_to_next_page";
        static constexpr const char*    CENTER_GO_TO_PREVIOUS_QUEST = "$dquestl_center_go_to_previous_quest";
        static constexpr const char*    CENTER_GO_TO_NEXT_QUEST     = "$dquestl_center_go_to_next_quest";
        static constexpr const char*    CENTER_BACK                 = "$dquestl_center_back";
        static constexpr const char*    CENTER_CLEAR                = "$dquestl_center_clear";
        static constexpr const char*    CENTER_RESET                = "$dquestl_center_reset";

        static constexpr const char*    THANK_YOU_FOR_PLAYING       = "$dquestl_thank_you_for_playing";

        static constexpr const char*    QUESTS_ARE_NOT_RUNNING_NEW  = "$dquestl_quests_are_not_running_new";
        static constexpr const char*    QUESTS_ARE_NOT_RUNNING_LOAD = "$dquestl_quests_are_not_running_load";

    }

    namespace Version {

        const Version_t<u16>    Self();

    }

}}}

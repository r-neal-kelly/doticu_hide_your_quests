/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "intrinsic.h"

namespace doticu_skylib { namespace doticu_hide_your_quests { namespace Const {

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

        static constexpr const char*    CURRENT                     = "$doticu_hyq_current";
        static constexpr const char*    $CURRENT                    = "Current";
        static constexpr const char*    FINISHED                    = "$doticu_hyq_finished";
        static constexpr const char*    $FINISHED                   = "Finished";
        static constexpr const char*    MISC                        = "$doticu_hyq_misc";
        static constexpr const char*    $MISC                       = "Miscellaneous";
        static constexpr const char*    OPTIONS                     = "$doticu_hyq_options";

        static constexpr const char*    NO_QUESTS                   = "$doticu_hyq_no_quests";

        static constexpr const char*    SHOW_NEW_OBJECTIVES         = "$doticu_hyq_show_new_objectives";
        static constexpr const char*    SHOW_FINISHED_QUESTS        = "$doticu_hyq_show_finished_quests";

        static constexpr const char*    CENTER_GO_TO_PREVIOUS_PAGE  = "$doticu_hyq_center_go_to_previous_page";
        static constexpr const char*    CENTER_GO_TO_NEXT_PAGE      = "$doticu_hyq_center_go_to_next_page";
        static constexpr const char*    CENTER_RESET                = "$doticu_hyq_center_reset";

        static constexpr const char*    THANK_YOU_FOR_PLAYING       = "$doticu_hyq_thank_you_for_playing";

        static constexpr const char*    QUESTS_ARE_NOT_RUNNING_NEW  = "$doticu_hyq_quests_are_not_running_new";
        static constexpr const char*    QUESTS_ARE_NOT_RUNNING_LOAD = "$doticu_hyq_quests_are_not_running_load";

    }

    namespace Version {

        const Version_t<u16>    Self();

    }

}}}

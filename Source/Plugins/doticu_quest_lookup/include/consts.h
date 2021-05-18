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

        static const char* ACTIVE                       = "$dquestl_active";

        static const char* THANK_YOU_FOR_PLAYING        = "$dquestl_thank_you_for_playing";

        static const char* QUESTS_ARE_NOT_RUNNING_NEW   = "$dquestl_quests_are_not_running_new";
        static const char* QUESTS_ARE_NOT_RUNNING_LOAD  = "$dquestl_quests_are_not_running_load";

    }

    namespace Version {

        const Version_t<u16>    Self();

    }

}}}

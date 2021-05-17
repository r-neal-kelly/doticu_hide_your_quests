/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/game_macros.h"
#include "doticu_skylib/global.h"
#include "doticu_skylib/mod.h"
#include "doticu_skylib/quest.h"
#include "doticu_skylib/version.h"

#include "consts.h"

namespace doticu_skylib { namespace doticu_quest_lookup { namespace Const { namespace Global {

    some<Global_t*> Is_Installed()  { DEFINE_FORM(Mod::Self(),  Global_t,   0x801); }
    some<Global_t*> Version_Major() { DEFINE_FORM(Mod::Self(),  Global_t,   0x802); }
    some<Global_t*> Version_Minor() { DEFINE_FORM(Mod::Self(),  Global_t,   0x803); }
    some<Global_t*> Version_Patch() { DEFINE_FORM(Mod::Self(),  Global_t,   0x804); }

}}}}

namespace doticu_skylib { namespace doticu_quest_lookup { namespace Const { namespace Mod {

    maybe<Mod_t*>   Self()  { DEFINE_MOD("doticu_quest_lookup.esp"); }

}}}}

namespace doticu_skylib { namespace doticu_quest_lookup { namespace Const { namespace Quest {

    some<Quest_t*>  MCM()   { DEFINE_FORM(Mod::Self(),  Quest_t,    0x800); }

}}}}

namespace doticu_skylib { namespace doticu_quest_lookup { namespace Const { namespace Version {

    const Version_t<u16>    Self()  { return Version_t<u16>(1, 0, 0); }

}}}}

/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/global.inl"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/virtual_utility.h"

#include "consts.h"
#include "mcm.h"
#include "plugin.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    Plugin_t::Plugin_t() :
        SKSE_Plugin_t("doticu_quest_lookup",
                      Version_t<u16>(1, 5, 97),
                      Operator_e::EQUAL_TO,
                      Version_t<u16>(2, 0, 17),
                      Operator_e::GREATER_THAN_OR_EQUAL_TO)
    {
        SKYLIB_LOG("doticu_quest_lookup:");
        SKYLIB_LOG("");
    }

    Plugin_t::~Plugin_t()
    {
    }

    Bool_t Plugin_t::On_Register(some<Virtual::Machine_t*> v_machine)
    {
        MCM_t::On_Register(v_machine);

        SKYLIB_LOG("- Registered all functions.");

        return true;
    }

    void Plugin_t::On_After_Load_Data()
    {
    }

    void Plugin_t::On_After_New_Game()
    {
        if (Is_Active() && !Is_Installed()) {
            if (Are_Quests_Running()) {
                Const::Global::Is_Installed()->Bool(true);

                const Version_t<u16>& version = Const::Version::Self();
                Const::Global::Version_Major()->As<u16>(version.major);
                Const::Global::Version_Minor()->As<u16>(version.minor);
                Const::Global::Version_Patch()->As<u16>(version.patch);

                MCM_t::On_After_New_Game();

                class Wait_Callback :
                    public Callback_i<>
                {
                public:
                    virtual void operator ()() override
                    {
                        UI_t::Create_Notification(Const::String::THANK_YOU_FOR_PLAYING, none<Virtual::Callback_i*>());
                    }
                };
                some<Wait_Callback*> wait_callback = new Wait_Callback();
                Virtual::Utility_t::Wait_Out_Of_Menu(1.0f, wait_callback());
                (*wait_callback)();
            }
        }
    }

    void Plugin_t::On_Before_Save_Game()
    {
        if (Is_Active() && Is_Installed() && Are_Quests_Running()) {
            MCM_t::On_Before_Save_Game();
        }
    }

    void Plugin_t::On_After_Save_Game()
    {
        if (Is_Active() && Is_Installed() && Are_Quests_Running()) {
            MCM_t::On_After_Save_Game();
        }
    }

    void Plugin_t::On_Before_Load_Game(some<const char*> file_path, u32 file_path_length)
    {
        if (Is_Active() && Is_Installed() && Are_Quests_Running()) {
            MCM_t::On_Before_Load_Game();
        }
    }

    void Plugin_t::On_After_Load_Game(Bool_t did_load_successfully)
    {
        if (did_load_successfully) {
            if (Is_Active()) {
                if (Is_Installed()) {
                    if (Are_Quests_Running()) {
                        MCM_t::On_After_Load_Game();
                    }
                } else {
                    On_After_New_Game();
                }
            }
        }
    }

    void Plugin_t::On_Before_Delete_Game(some<const char*> file_path, u32 file_path_length)
    {
    }

    void Plugin_t::On_Update(u32 time_stamp)
    {
    }

    Bool_t Plugin_t::Is_Active()
    {
        return Const::Mod::Self() != none<Mod_t*>();
    }

    Bool_t Plugin_t::Is_Installed()
    {
        return Const::Global::Is_Installed()->Bool();
    }

    Bool_t Plugin_t::Are_Quests_Running()
    {
        Vector_t<some<Quest_t*>> quests;
        for (size_t idx = 0, end = quests.size(); idx < end; idx += 1) {
            if (!quests[idx]->Is_Enabled()) {
                return false;
            }
        }
        return true;
    }

    Vector_t<some<Quest_t*>> Plugin_t::Quests()
    {
        Vector_t<some<Quest_t*>> quests;
        quests.reserve(1);

        quests.push_back(Const::Quest::MCM());

        return quests;
    }

    Plugin_t plugin;

}}

SKYLIB_EXPORT_SKSE_PLUGIN(doticu_skylib::doticu_quest_lookup::plugin);

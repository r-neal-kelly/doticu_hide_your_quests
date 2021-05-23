/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#include "doticu_skylib/global.inl"
#include "doticu_skylib/ui.h"
#include "doticu_skylib/virtual_utility.h"

#include "consts.h"
#include "mcm.h"
#include "plugin.h"

namespace doticu_skylib { namespace doticu_hide_your_quests {

    Plugin_t::Plugin_t() :
        SKSE_Plugin_t("doticu_hide_your_quests",
                      Version_t<u16>(1, 5, 97),
                      Operator_e::EQUAL_TO,
                      Version_t<u16>(2, 0, 17),
                      Operator_e::GREATER_THAN_OR_EQUAL_TO)
    {
        SKYLIB_LOG("doticu_hide_your_quests:");
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
        Start_Updating(std::chrono::milliseconds(1000));
    }

    void Plugin_t::On_After_New_Game()
    {
        if (Is_Active() && !Is_Installed()) {
            if (Is_Quest_Running()) {
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
            } else {
                UI_t::Create_Message_Box(Const::String::QUESTS_ARE_NOT_RUNNING_NEW, none<Virtual::Callback_i*>());
            }
        }
    }

    void Plugin_t::On_Before_Save_Game(some<const char*> file_path, u32 file_path_length)
    {
        if (Is_Active() && Is_Installed() && Is_Quest_Running()) {
            MCM_t::On_Before_Save_Game();
        }
    }

    void Plugin_t::On_After_Save_Game()
    {
        if (Is_Active() && Is_Installed() && Is_Quest_Running()) {
            MCM_t::On_After_Save_Game();
        }
    }

    void Plugin_t::On_Before_Load_Game(some<const char*> file_path, u32 file_path_length)
    {
        if (Is_Active() && Is_Installed() && Is_Quest_Running()) {
            MCM_t::On_Before_Load_Game();
        }
    }

    void Plugin_t::On_After_Load_Game(Bool_t did_load_successfully)
    {
        if (did_load_successfully) {
            if (Is_Active()) {
                if (Is_Installed()) {
                    if (Is_Quest_Running()) {
                        const Version_t<u16>& current = Const::Version::Self();
                        const Version_t<u16> saved(Const::Global::Version_Major()->As<u16>(),
                                                   Const::Global::Version_Minor()->As<u16>(),
                                                   Const::Global::Version_Patch()->As<u16>());
                        if (saved < current) {
                            Const::Global::Version_Major()->As<u16>(current.major);
                            Const::Global::Version_Minor()->As<u16>(current.minor);
                            Const::Global::Version_Patch()->As<u16>(current.patch);

                            MCM_t::On_Update_Version(saved);

                            UI_t::Create_Notification("Quest Lookup: Updated to version " +
                                                      std::to_string(current.major) + "." +
                                                      std::to_string(current.minor) + "." +
                                                      std::to_string(current.patch),
                                                      none<Virtual::Callback_i*>());
                        }

                        MCM_t::On_After_Load_Game();
                    } else {
                        UI_t::Create_Message_Box(Const::String::QUESTS_ARE_NOT_RUNNING_LOAD, none<Virtual::Callback_i*>());
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
        if (Is_Active() && Is_Installed() && Is_Quest_Running()) {
            MCM_t::On_Update();
        }
    }

    Bool_t Plugin_t::Is_Active()
    {
        return Const::Mod::Self() != none<Mod_t*>();
    }

    Bool_t Plugin_t::Is_Installed()
    {
        return Const::Global::Is_Installed()->Bool();
    }

    Bool_t Plugin_t::Is_Quest_Running()
    {
        return Const::Quest::MCM()->Is_Enabled();
    }

    Plugin_t plugin;

}}

SKYLIB_EXPORT_SKSE_PLUGIN(doticu_skylib::doticu_hide_your_quests::plugin);

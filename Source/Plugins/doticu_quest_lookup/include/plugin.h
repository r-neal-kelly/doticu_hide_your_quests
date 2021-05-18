/*
    Copyright © 2020 r-neal-kelly, aka doticu
*/

#pragma once

#include "doticu_skylib/skse_plugin.h"

#include "intrinsic.h"

namespace doticu_skylib { namespace doticu_quest_lookup {

    class Plugin_t :
        public SKSE_Plugin_t
    {
    protected:
        static Vector_t<some<Quest_t*>> quests;

    public:
        Plugin_t();

    public:
        virtual         ~Plugin_t();

        virtual Bool_t  On_Register(some<Virtual::Machine_t*> v_machine) override;

        virtual void    On_After_Load_Data() override;
        virtual void    On_After_New_Game() override;
        virtual void    On_Before_Save_Game() override;
        virtual void    On_After_Save_Game() override;
        virtual void    On_Before_Load_Game(some<const char*> file_path, u32 file_path_length) override;
        virtual void    On_After_Load_Game(Bool_t did_load_successfully) override;
        virtual void    On_Before_Delete_Game(some<const char*> file_path, u32 file_path_length) override;
        virtual void    On_Update(u32 time_stamp) override;

    public:
        Bool_t  Is_Active();
        Bool_t  Is_Installed();
        Bool_t  Are_Quests_Running();
    };

    extern Plugin_t plugin;

}}

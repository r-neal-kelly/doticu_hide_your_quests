; Copyright © 2020 r-neal-kelly, aka doticu

Scriptname doticu_hide_your_quests extends SKI_ConfigBase

string  current_page        = ""
Quest[] hidden_quests       = none
int[]   hidden_instances    = none
int[]   hidden_objectives   = none

event OnConfigOpen() native
event OnConfigClose() native
event OnPageReset(string page) native
event OnOptionSelect(int option) native
event OnOptionMenuOpen(int option) native
event OnOptionMenuAccept(int option, int index) native
event OnOptionSliderOpen(int option) native
event OnOptionSliderAccept(int option, float value) native
event OnOptionInputAccept(int option, string value) native
event OnOptionKeymapChange(int option, int key, string conflict, string mod) native
event OnOptionDefault(int option) native
event OnOptionHighlight(int option) native

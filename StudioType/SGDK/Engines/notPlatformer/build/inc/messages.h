#pragma once

#include <genesis.h>

extern void printMessageNoStop(char* str, bool vertSide, u16 windowTiles);
extern void printMessageBase(char* str, Image* img_msg_sign, u8 msg_sign_pal);
extern void printMessage(char* str);
extern void printMessageName(char* str, char* name);
extern void printMessageNameSnd4(char* str, char* name, u8* snd4_sound_data, bool snd4Loop);
extern void printMessageNamePCM(char* str, char* name, u8 pcm_id);
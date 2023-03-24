#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/enums.h"
#include "../inc/global.h"
#include "../inc/player.h"
#include "../inc/customScripts.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"
#include "../res/sounds.h"
#include "../res/music.h"
#include "../res/images.h"

u16 levelNum = 0;
Level* curLvlData;
EntityAll* curEntityAll;
MessagePack* curMessagePacks;

u16 palette_full[64];

const Vect2D_s16 const Position_arr_Level_0[] = {{29, 146}, };
const Trigger const Trigger_arr_Level_0[] = {{TRUE, {310, 7}, {0, 0, 10, 200}, 0, 1, 0, 0, 1, }, {TRUE, {177, 158}, {0, 0, 55, 55}, TRIGGER_TYPE_hurt, 0, 0, 0, 1, }, {TRUE, {113, 166}, {0, 0, 55, 55}, TRIGGER_TYPE_hurt, 0, 0, 0, 1, }, {TRUE, {77, 162}, {0, 0, 55, 55}, TRIGGER_TYPE_hurt, 0, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_0[] = {{0, TRUE, {310, 7}, {FIX32(310), FIX32(7)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[0],0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {3, TRUE, {177, 158}, {FIX32(177), FIX32(158)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[1],1,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, {3, TRUE, {113, 166}, {FIX32(113), FIX32(166)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[2],2,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, {3, TRUE, {77, 162}, {FIX32(77), FIX32(162)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[3],3,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_0[] = {10, 10, NULL, 4, &EntityMerged_arr_Level_0, 4, &Trigger_arr_Level_0, };
const Message const msgArr_Level_0_msgPack_0[] = {};
const MessagePack const MessagePack_Level_0[] = {};
const Level const lvl_Level_0 = {NULL, &map_01, NULL, &tileset_01, NULL, &pal_01, NULL, NULL, &Position_arr_Level_0, 1, collisionMap, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_1[] = {{15, 170}, };
const Trigger const Trigger_arr_Level_1[] = {{TRUE, {301, 7}, {0, 0, 20, 200}, 0, 2, 0, 0, 1, }, {TRUE, {189, 175}, {0, 21, 128, 11}, 0, 0, 0, 0, 1, }, {TRUE, {119, 46}, {0, 0, 56, 64}, 0, 1, 0, 0, 1, }, {TRUE, {196, 105}, {0, 35, 55, 23}, 0, 0, 0, 0, 1, }, {TRUE, {42, 26}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {55, 106}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {86, 106}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {127, 107}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {163, 105}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {207, 104}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {282, 139}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {86, 178}, {0, 21, 128, 11}, 0, 0, 0, 0, 1, }, {TRUE, {144, 151}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {196, 181}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, {TRUE, {256, 174}, {0, 0, 20, 25}, 0, 1, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_1[] = {{0, TRUE, {301, 7}, {FIX32(301), FIX32(7)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[0],0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {2, TRUE, {189, 175}, {FIX32(189), FIX32(175)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[1],1,NULL,NULL, NULL, &spr_Car, 1, 1, 0, 1, 1, }, {4, TRUE, {119, 46}, {FIX32(119), FIX32(46)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[2],2,NULL,NULL, NULL, &spr_lady, 1, 1, 0, 1, 1, }, {3, TRUE, {196, 105}, {FIX32(196), FIX32(105)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[3],3,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, {1, TRUE, {42, 26}, {FIX32(42), FIX32(26)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[4],4,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {55, 106}, {FIX32(55), FIX32(106)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[5],5,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {86, 106}, {FIX32(86), FIX32(106)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[6],6,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {127, 107}, {FIX32(127), FIX32(107)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[7],7,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {163, 105}, {FIX32(163), FIX32(105)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[8],8,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {207, 104}, {FIX32(207), FIX32(104)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[9],9,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {282, 139}, {FIX32(282), FIX32(139)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[10],10,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {2, TRUE, {86, 178}, {FIX32(86), FIX32(178)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[11],11,NULL,NULL, NULL, &spr_Car, 1, 1, 0, 1, 1, }, {1, TRUE, {126, 176}, {FIX32(126), FIX32(176)}, {0, 0}, {32, 32}, FALSE, NULL,12,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {144, 151}, {FIX32(144), FIX32(151)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[12],12,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {196, 181}, {FIX32(196), FIX32(181)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[13],13,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, {1, TRUE, {256, 174}, {FIX32(256), FIX32(174)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[14],14,NULL,NULL, NULL, &spr_coin, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_1[] = {10, 10, NULL, 16, &EntityMerged_arr_Level_1, 15, &Trigger_arr_Level_1, };
const Message const msgArr_Level_1_msgPack_0[] = {};
const MessagePack const MessagePack_Level_1[] = {};
const Level const lvl_Level_1 = {NULL, &map_02, NULL, &tileset_02, NULL, &pal_02, NULL, NULL, &Position_arr_Level_1, 1, collisionMap1, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_2[] = {{5, 172}, };
const Trigger const Trigger_arr_Level_2[] = {{TRUE, {301, 4}, {0, 0, 20, 200}, 0, 3, 0, 0, 1, }, {TRUE, {230, 160}, {0, 0, 56, 64}, 0, 1, 0, 0, 1, }, {TRUE, {100, 157}, {0, 35, 55, 23}, 0, 0, 0, 0, 1, }, {TRUE, {181, 133}, {0, 0, 8, 8}, 0, 1, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_2[] = {{0, TRUE, {301, 4}, {FIX32(301), FIX32(4)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[0],0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {4, TRUE, {230, 160}, {FIX32(230), FIX32(160)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[1],1,NULL,NULL, NULL, &spr_lady, 1, 1, 0, 1, 1, }, {3, TRUE, {100, 157}, {FIX32(100), FIX32(157)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[2],2,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, {5, TRUE, {181, 133}, {FIX32(181), FIX32(133)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[3],3,NULL,NULL, NULL, &spr_MacePickup2, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_2[] = {10, 10, NULL, 4, &EntityMerged_arr_Level_2, 4, &Trigger_arr_Level_2, };
const Message const msgArr_Level_2_msgPack_0[] = {};
const MessagePack const MessagePack_Level_2[] = {};
const Level const lvl_Level_2 = {NULL, &map_03, NULL, &tileset_03, NULL, &pal_03, NULL, NULL, &Position_arr_Level_2, 1, collisionMap2, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_3[] = {{13, 175}, };
const Trigger const Trigger_arr_Level_3[] = {{TRUE, {300, 4}, {0, 0, 20, 200}, 0, 4, 0, 0, 1, }, {TRUE, {151, 175}, {0, 21, 128, 11}, 0, 1, 0, 0, 1, }, {TRUE, {101, 108}, {0, 35, 55, 23}, 0, 0, 0, 0, 1, }, {TRUE, {251, 43}, {0, 35, 55, 23}, 0, 0, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_3[] = {{0, TRUE, {300, 4}, {FIX32(300), FIX32(4)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[0],0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {2, TRUE, {151, 175}, {FIX32(151), FIX32(175)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[1],1,NULL,NULL, NULL, &spr_Car, 1, 1, 0, 1, 1, }, {3, TRUE, {101, 108}, {FIX32(101), FIX32(108)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[2],2,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, {3, TRUE, {251, 43}, {FIX32(251), FIX32(43)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[3],3,NULL,NULL, NULL, &spr_octopus, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_3[] = {10, 10, NULL, 4, &EntityMerged_arr_Level_3, 4, &Trigger_arr_Level_3, };
const Message const msgArr_Level_3_msgPack_0[] = {};
const MessagePack const MessagePack_Level_3[] = {};
const Level const lvl_Level_3 = {NULL, &map_04, NULL, &tileset_04, NULL, &pal_04, NULL, NULL, &Position_arr_Level_3, 1, collisionMap3, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_4[] = {{8, 174}, };
const Trigger const Trigger_arr_Level_4[] = {};
const EntityMerged const EntityMerged_arr_Level_4[] = {{0, TRUE, {298, 2}, {FIX32(298), FIX32(2)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {0, TRUE, {129, 61}, {FIX32(129), FIX32(61)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, &spr_enemy01, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_4[] = {10, 10, NULL, 2, &EntityMerged_arr_Level_4, 0, &Trigger_arr_Level_4, };
const Message const msgArr_Level_4_msgPack_0[] = {};
const MessagePack const MessagePack_Level_4[] = {};
const Level const lvl_Level_4 = {NULL, &map_05, NULL, &tileset_05, NULL, &pal_05, NULL, NULL, &Position_arr_Level_4, 1, collisionMap4, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_5[] = {{10, 172}, };
const Trigger const Trigger_arr_Level_5[] = {};
const EntityMerged const EntityMerged_arr_Level_5[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {0, TRUE, {87, 1}, {FIX32(87), FIX32(1)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {0, TRUE, {229, 39}, {FIX32(229), FIX32(39)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, &spr_enemy02, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_5[] = {10, 10, NULL, 3, &EntityMerged_arr_Level_5, 0, &Trigger_arr_Level_5, };
const Message const msgArr_Level_5_msgPack_0[] = {};
const MessagePack const MessagePack_Level_5[] = {};
const Level const lvl_Level_5 = {NULL, &map_06, NULL, &tileset_06, NULL, &pal_06, NULL, NULL, &Position_arr_Level_5, 1, collisionMap5, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_6[] = {{7, 177}, };
const Trigger const Trigger_arr_Level_6[] = {};
const EntityMerged const EntityMerged_arr_Level_6[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, {0, TRUE, {141, 49}, {FIX32(141), FIX32(49)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, &spr_enemy01, 1, 1, 0, 1, 1, }, {0, TRUE, {200, 47}, {FIX32(200), FIX32(47)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, &spr_noSpr, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_6[] = {10, 10, NULL, 3, &EntityMerged_arr_Level_6, 0, &Trigger_arr_Level_6, };
const Message const msgArr_Level_6_msgPack_0[] = {};
const MessagePack const MessagePack_Level_6[] = {};
const Level const lvl_Level_6 = {NULL, &map_07, NULL, &tileset_07, NULL, &pal_07, NULL, NULL, &Position_arr_Level_6, 1, collisionMap6, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_7[] = {{10, 172}, };
const Trigger const Trigger_arr_Level_7[] = {};
const EntityMerged const EntityMerged_arr_Level_7[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_7[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_7, 0, &Trigger_arr_Level_7, };
const Message const msgArr_Level_7_msgPack_0[] = {};
const MessagePack const MessagePack_Level_7[] = {};
const Level const lvl_Level_7 = {NULL, &map_08, NULL, &tileset_08, NULL, &pal_08, NULL, NULL, &Position_arr_Level_7, 1, collisionMap7, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_8[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_8[] = {};
const EntityMerged const EntityMerged_arr_Level_8[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_8[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_8, 0, &Trigger_arr_Level_8, };
const Message const msgArr_Level_8_msgPack_0[] = {};
const MessagePack const MessagePack_Level_8[] = {};
const Level const lvl_Level_8 = {NULL, &map_09, NULL, &tileset_09, NULL, &pal_09, NULL, NULL, &Position_arr_Level_8, 1, collisionMap8, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_9[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_9[] = {};
const EntityMerged const EntityMerged_arr_Level_9[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_9[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_9, 0, &Trigger_arr_Level_9, };
const Message const msgArr_Level_9_msgPack_0[] = {};
const MessagePack const MessagePack_Level_9[] = {};
const Level const lvl_Level_9 = {NULL, &map_10, NULL, &tileset_10, NULL, &pal_10, NULL, NULL, &Position_arr_Level_9, 1, collisionMap9, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_10[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_10[] = {};
const EntityMerged const EntityMerged_arr_Level_10[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_10[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_10, 0, &Trigger_arr_Level_10, };
const Message const msgArr_Level_10_msgPack_0[] = {};
const MessagePack const MessagePack_Level_10[] = {};
const Level const lvl_Level_10 = {NULL, &map_11, NULL, &tileset_11, NULL, &pal_11, NULL, NULL, &Position_arr_Level_10, 1, collisionMap10, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_11[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_11[] = {};
const EntityMerged const EntityMerged_arr_Level_11[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_11[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_11, 0, &Trigger_arr_Level_11, };
const Message const msgArr_Level_11_msgPack_0[] = {};
const MessagePack const MessagePack_Level_11[] = {};
const Level const lvl_Level_11 = {NULL, &map_12, NULL, &tileset_12, NULL, &pal_12, NULL, NULL, &Position_arr_Level_11, 1, collisionMap11, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_12[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_12[] = {};
const EntityMerged const EntityMerged_arr_Level_12[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_12[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_12, 0, &Trigger_arr_Level_12, };
const Message const msgArr_Level_12_msgPack_0[] = {};
const MessagePack const MessagePack_Level_12[] = {};
const Level const lvl_Level_12 = {NULL, &map_13, NULL, &tileset_13, NULL, &pal_13, NULL, NULL, &Position_arr_Level_12, 1, collisionMap12, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_13[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_13[] = {};
const EntityMerged const EntityMerged_arr_Level_13[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_13[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_13, 0, &Trigger_arr_Level_13, };
const Message const msgArr_Level_13_msgPack_0[] = {};
const MessagePack const MessagePack_Level_13[] = {};
const Level const lvl_Level_13 = {NULL, &map_14, NULL, &tileset_14, NULL, &pal_14, NULL, NULL, &Position_arr_Level_13, 1, collisionMap13, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_14[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_14[] = {};
const EntityMerged const EntityMerged_arr_Level_14[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_14[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_14, 0, &Trigger_arr_Level_14, };
const Message const msgArr_Level_14_msgPack_0[] = {};
const MessagePack const MessagePack_Level_14[] = {};
const Level const lvl_Level_14 = {NULL, &map_15, NULL, &tileset_15, NULL, &pal_15, NULL, NULL, &Position_arr_Level_14, 1, collisionMap14, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_15[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_15[] = {};
const EntityMerged const EntityMerged_arr_Level_15[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_15[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_15, 0, &Trigger_arr_Level_15, };
const Message const msgArr_Level_15_msgPack_0[] = {};
const MessagePack const MessagePack_Level_15[] = {};
const Level const lvl_Level_15 = {NULL, &map_16, NULL, &tileset_16, NULL, &pal_16, NULL, NULL, &Position_arr_Level_15, 1, collisionMap15, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_16[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_16[] = {};
const EntityMerged const EntityMerged_arr_Level_16[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_16[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_16, 0, &Trigger_arr_Level_16, };
const Message const msgArr_Level_16_msgPack_0[] = {};
const MessagePack const MessagePack_Level_16[] = {};
const Level const lvl_Level_16 = {NULL, &map_17, NULL, &tileset_17, NULL, &pal_17, NULL, NULL, &Position_arr_Level_16, 1, collisionMap16, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_17[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_17[] = {};
const EntityMerged const EntityMerged_arr_Level_17[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_17[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_17, 0, &Trigger_arr_Level_17, };
const Message const msgArr_Level_17_msgPack_0[] = {};
const MessagePack const MessagePack_Level_17[] = {};
const Level const lvl_Level_17 = {NULL, &map_18, NULL, &tileset_18, NULL, &pal_18, NULL, NULL, &Position_arr_Level_17, 1, collisionMap17, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_18[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_18[] = {};
const EntityMerged const EntityMerged_arr_Level_18[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_18[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_18, 0, &Trigger_arr_Level_18, };
const Message const msgArr_Level_18_msgPack_0[] = {};
const MessagePack const MessagePack_Level_18[] = {};
const Level const lvl_Level_18 = {NULL, &map_19, NULL, &tileset_19, NULL, &pal_19, NULL, NULL, &Position_arr_Level_18, 1, collisionMap18, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_19[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_19[] = {};
const EntityMerged const EntityMerged_arr_Level_19[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_19[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_19, 0, &Trigger_arr_Level_19, };
const Message const msgArr_Level_19_msgPack_0[] = {};
const MessagePack const MessagePack_Level_19[] = {};
const Level const lvl_Level_19 = {NULL, &map_20, NULL, &tileset_20, NULL, &pal_20, NULL, NULL, &Position_arr_Level_19, 1, collisionMap19, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_20[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_20[] = {};
const EntityMerged const EntityMerged_arr_Level_20[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_20[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_20, 0, &Trigger_arr_Level_20, };
const Message const msgArr_Level_20_msgPack_0[] = {};
const MessagePack const MessagePack_Level_20[] = {};
const Level const lvl_Level_20 = {NULL, &map_20, NULL, &tileset_20, NULL, &pal_20, NULL, NULL, &Position_arr_Level_20, 1, collisionMap20, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_21[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_21[] = {};
const EntityMerged const EntityMerged_arr_Level_21[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_21[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_21, 0, &Trigger_arr_Level_21, };
const Message const msgArr_Level_21_msgPack_0[] = {};
const MessagePack const MessagePack_Level_21[] = {};
const Level const lvl_Level_21 = {NULL, &map_22, NULL, &tileset_22, NULL, &pal_22, NULL, NULL, &Position_arr_Level_21, 1, collisionMap21, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_22[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_22[] = {};
const EntityMerged const EntityMerged_arr_Level_22[] = {{0, TRUE, {5, 221}, {FIX32(5), FIX32(221)}, {0, 0}, {32, 32}, FALSE, NULL,0,NULL,NULL, NULL, NULL, 1, 1, 0, 1, 1, }, };
const EntityAll const EntityAll_arr_Level_22[] = {10, 10, NULL, 1, &EntityMerged_arr_Level_22, 0, &Trigger_arr_Level_22, };
const Message const msgArr_Level_22_msgPack_0[] = {};
const MessagePack const MessagePack_Level_22[] = {};
const Level const lvl_Level_22 = {NULL, &map_23, NULL, &tileset_23, NULL, &pal_23, NULL, NULL, &Position_arr_Level_22, 1, collisionMap22, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_23[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_23[] = {};
const EntityMerged const EntityMerged_arr_Level_23[] = {};
const EntityAll const EntityAll_arr_Level_23[] = {10, 10, NULL, 0, &EntityMerged_arr_Level_23, 0, &Trigger_arr_Level_23, };
const Message const msgArr_Level_23_msgPack_0[] = {};
const MessagePack const MessagePack_Level_23[] = {};
const Level const lvl_Level_23 = {NULL, &map_24, NULL, &tileset_24, NULL, &pal_24, NULL, NULL, &Position_arr_Level_23, 1, collisionMap23, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const Vect2D_s16 const Position_arr_Level_24[] = {{25, 103}, };
const Trigger const Trigger_arr_Level_24[] = {};
const EntityMerged const EntityMerged_arr_Level_24[] = {};
const EntityAll const EntityAll_arr_Level_24[] = {10, 10, NULL, 0, &EntityMerged_arr_Level_24, 0, &Trigger_arr_Level_24, };
const Message const msgArr_Level_24_msgPack_0[] = {};
const MessagePack const MessagePack_Level_24[] = {};
const Level const lvl_Level_24 = {NULL, &map_25, NULL, &tileset_25, NULL, &pal_25, NULL, NULL, &Position_arr_Level_24, 1, collisionMap24, {320, 224}, {40, 28}, NULL, NULL, emptyScript, NULL};
const LevelFull const LevelFull_arr[] = {{&lvl_Level_0, &EntityAll_arr_Level_0, &MessagePack_Level_0}, {&lvl_Level_1, &EntityAll_arr_Level_1, &MessagePack_Level_1}, {&lvl_Level_2, &EntityAll_arr_Level_2, &MessagePack_Level_2}, {&lvl_Level_3, &EntityAll_arr_Level_3, &MessagePack_Level_3}, {&lvl_Level_4, &EntityAll_arr_Level_4, &MessagePack_Level_4}, {&lvl_Level_5, &EntityAll_arr_Level_5, &MessagePack_Level_5}, {&lvl_Level_6, &EntityAll_arr_Level_6, &MessagePack_Level_6}, {&lvl_Level_7, &EntityAll_arr_Level_7, &MessagePack_Level_7}, {&lvl_Level_8, &EntityAll_arr_Level_8, &MessagePack_Level_8}, {&lvl_Level_9, &EntityAll_arr_Level_9, &MessagePack_Level_9}, {&lvl_Level_10, &EntityAll_arr_Level_10, &MessagePack_Level_10}, {&lvl_Level_11, &EntityAll_arr_Level_11, &MessagePack_Level_11}, {&lvl_Level_12, &EntityAll_arr_Level_12, &MessagePack_Level_12}, {&lvl_Level_13, &EntityAll_arr_Level_13, &MessagePack_Level_13}, {&lvl_Level_14, &EntityAll_arr_Level_14, &MessagePack_Level_14}, {&lvl_Level_15, &EntityAll_arr_Level_15, &MessagePack_Level_15}, {&lvl_Level_16, &EntityAll_arr_Level_16, &MessagePack_Level_16}, {&lvl_Level_17, &EntityAll_arr_Level_17, &MessagePack_Level_17}, {&lvl_Level_18, &EntityAll_arr_Level_18, &MessagePack_Level_18}, {&lvl_Level_19, &EntityAll_arr_Level_19, &MessagePack_Level_19}, {&lvl_Level_20, &EntityAll_arr_Level_20, &MessagePack_Level_20}, {&lvl_Level_21, &EntityAll_arr_Level_21, &MessagePack_Level_21}, {&lvl_Level_22, &EntityAll_arr_Level_22, &MessagePack_Level_22}, {&lvl_Level_23, &EntityAll_arr_Level_23, &MessagePack_Level_23}, {&lvl_Level_24, &EntityAll_arr_Level_24, &MessagePack_Level_24}, };


Vect2D_s16 getLevelPos(u16 posNum) {
	return LevelFull_arr[levelNum].lvl->posArr[posNum];
}

void updateRoomSize(Level* lvl){
	roomSize = newAABB(0, lvl->sizeinPx.x, 0, lvl->sizeinPx.y);
}

u16 getTileValue(s16 x, s16 y) {
	if (x >= curLvlData->sizeinTiles.x || x < 0 || y < 0 || y >= curLvlData->sizeinTiles.y)
		return 0;

	//If the position is inside the collision map return the value of the tile from it
	return curLvlData->collArr[x+(y*curLvlData->sizeinTiles.x)];
}


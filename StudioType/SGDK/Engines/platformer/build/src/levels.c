#include "../inc/levels.h"

#include "../inc/maps.h"
#include "../inc/global.h"
#include "../inc/player.h"

#include "../res/resources.h"
#include "../res/gfx.h"
#include "../res/sprites.h"


u16 levelNum = 0;
Level* curLvlData;
EntityAll* curEntityAll;

const Trigger const Trigger_arr_Level_0[] = {{TRUE, {310, 7}, {0, 0, 10, 200}, 0, 1, 1, }, {TRUE, {281, 104}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {148, 157}, {0, 35, 55, 23}, 2, 0, 1, }, {TRUE, {231, 127}, {0, 0, 20, 25}, 1, 1, 1, }, };
const EntityMerged const EntityMerged_arr_Level_0[] = {{0, TRUE, {310, 7}, {FIX32(310), FIX32(7)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[0],0,NULL,NULL, 1, 1, 0, }, {1, TRUE, {281, 104}, {FIX32(281), FIX32(104)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[1],1,NULL,&spr_coin, 1, 1, 0, }, {3, TRUE, {148, 157}, {FIX32(148), FIX32(157)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[2],2,NULL,&spr_octopus, 1, 1, 0, }, {1, TRUE, {231, 127}, {FIX32(231), FIX32(127)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_0[3],3,NULL,&spr_coin, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_0[] = {4, &EntityMerged_arr_Level_0, 4, &Trigger_arr_Level_0, };
const Level const lvl_Level_0 = {NULL, &Level_0_bgb_map, NULL, &Level_0_bgb_tileset, NULL, &Level_0_bgb_pal, {26, 170}, collisionMap, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_1[] = {{TRUE, {301, 7}, {0, 0, 20, 200}, 0, 2, 1, }, {TRUE, {189, 175}, {0, 21, 128, 11}, 2, 0, 1, }, {TRUE, {119, 46}, {0, 0, 56, 64}, 1, 1, 1, }, {TRUE, {191, 106}, {0, 35, 55, 23}, 2, 0, 1, }, {TRUE, {42, 26}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {55, 106}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {86, 106}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {127, 107}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {163, 105}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {202, 105}, {0, 0, 20, 25}, 1, 1, 1, }, {TRUE, {282, 139}, {0, 0, 20, 25}, 1, 1, 1, }, };
const EntityMerged const EntityMerged_arr_Level_1[] = {{0, TRUE, {301, 7}, {FIX32(301), FIX32(7)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[0],0,NULL,NULL, 1, 1, 0, }, {2, TRUE, {189, 175}, {FIX32(189), FIX32(175)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[1],1,NULL,&spr_Car, 1, 1, 0, }, {4, TRUE, {119, 46}, {FIX32(119), FIX32(46)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[2],2,NULL,&spr_lady, 1, 1, 0, }, {3, TRUE, {191, 106}, {FIX32(191), FIX32(106)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[3],3,NULL,&spr_octopus, 1, 1, 0, }, {1, TRUE, {42, 26}, {FIX32(42), FIX32(26)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[4],4,NULL,&spr_coin, 1, 1, 0, }, {1, TRUE, {55, 106}, {FIX32(55), FIX32(106)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[5],5,NULL,&spr_coin, 1, 1, 0, }, {1, TRUE, {86, 106}, {FIX32(86), FIX32(106)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[6],6,NULL,&spr_coin, 1, 1, 0, }, {1, TRUE, {127, 107}, {FIX32(127), FIX32(107)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[7],7,NULL,&spr_coin, 1, 1, 0, }, {1, TRUE, {163, 105}, {FIX32(163), FIX32(105)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[8],8,NULL,&spr_coin, 1, 1, 0, }, {1, TRUE, {202, 105}, {FIX32(202), FIX32(105)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[9],9,NULL,&spr_coin, 1, 1, 0, }, {1, TRUE, {282, 139}, {FIX32(282), FIX32(139)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_1[10],10,NULL,&spr_coin, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_1[] = {11, &EntityMerged_arr_Level_1, 11, &Trigger_arr_Level_1, };
const Level const lvl_Level_1 = {NULL, &Level_1_bgb_map, NULL, &Level_1_bgb_tileset, NULL, &Level_1_bgb_pal, {5, 189}, collisionMap1, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_2[] = {{TRUE, {301, 4}, {0, 0, 20, 200}, 0, 3, 1, }, {TRUE, {230, 160}, {0, 0, 56, 64}, 1, 1, 1, }, {TRUE, {100, 157}, {0, 35, 55, 23}, 2, 0, 1, }, {TRUE, {181, 133}, {0, 0, 8, 8}, 1, 1, 1, }, };
const EntityMerged const EntityMerged_arr_Level_2[] = {{0, TRUE, {301, 4}, {FIX32(301), FIX32(4)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[0],0,NULL,NULL, 1, 1, 0, }, {4, TRUE, {230, 160}, {FIX32(230), FIX32(160)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[1],1,NULL,&spr_lady, 1, 1, 0, }, {3, TRUE, {100, 157}, {FIX32(100), FIX32(157)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[2],2,NULL,&spr_octopus, 1, 1, 0, }, {5, TRUE, {181, 133}, {FIX32(181), FIX32(133)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_2[3],3,NULL,&spr_MacePickup2, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_2[] = {4, &EntityMerged_arr_Level_2, 4, &Trigger_arr_Level_2, };
const Level const lvl_Level_2 = {NULL, &Level_2_bgb_map, NULL, &Level_2_bgb_tileset, NULL, &Level_2_bgb_pal, {12, 173}, collisionMap2, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_3[] = {{TRUE, {300, 4}, {0, 0, 20, 200}, 0, 4, 1, }, {TRUE, {151, 175}, {0, 21, 128, 11}, 2, 1, 1, }, {TRUE, {101, 108}, {0, 35, 55, 23}, 2, 0, 1, }, {TRUE, {251, 43}, {0, 35, 55, 23}, 2, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_3[] = {{0, TRUE, {300, 4}, {FIX32(300), FIX32(4)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[0],0,NULL,NULL, 1, 1, 0, }, {2, TRUE, {151, 175}, {FIX32(151), FIX32(175)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[1],1,NULL,&spr_Car, 1, 1, 0, }, {3, TRUE, {101, 108}, {FIX32(101), FIX32(108)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[2],2,NULL,&spr_octopus, 1, 1, 0, }, {3, TRUE, {251, 43}, {FIX32(251), FIX32(43)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_3[3],3,NULL,&spr_octopus, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_3[] = {4, &EntityMerged_arr_Level_3, 4, &Trigger_arr_Level_3, };
const Level const lvl_Level_3 = {NULL, &Level_3_bgb_map, NULL, &Level_3_bgb_tileset, NULL, &Level_3_bgb_pal, {11, 178}, collisionMap3, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_4[] = {{TRUE, {298, 2}, {0, 0, 20, 200}, 0, 5, 1, }, {TRUE, {129, 61}, {0, 0, 8, 8}, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_4[] = {{0, TRUE, {298, 2}, {FIX32(298), FIX32(2)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_4[0],0,NULL,NULL, 1, 1, 0, }, {0, TRUE, {129, 61}, {FIX32(129), FIX32(61)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_4[1],1,NULL,&spr_enemy01, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_4[] = {2, &EntityMerged_arr_Level_4, 2, &Trigger_arr_Level_4, };
const Level const lvl_Level_4 = {NULL, &Level_4_bgb_map, NULL, &Level_4_bgb_tileset, NULL, &Level_4_bgb_pal, {13, 181}, collisionMap4, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_5[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 6, 1, }, {TRUE, {87, 1}, {0, 0, 100, 20}, 0, 22, 1, }, {TRUE, {229, 39}, {0, 0, 8, 8}, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_5[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_5[0],0,NULL,NULL, 1, 1, 0, }, {0, TRUE, {87, 1}, {FIX32(87), FIX32(1)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_5[1],1,NULL,NULL, 1, 1, 0, }, {0, TRUE, {229, 39}, {FIX32(229), FIX32(39)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_5[2],2,NULL,&spr_enemy02, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_5[] = {3, &EntityMerged_arr_Level_5, 3, &Trigger_arr_Level_5, };
const Level const lvl_Level_5 = {NULL, &Level_5_bgb_map, NULL, &Level_5_bgb_tileset, NULL, &Level_5_bgb_pal, {133, 71}, collisionMap5, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_6[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 7, 1, }, {TRUE, {141, 49}, {0, 0, 8, 8}, 0, 0, 1, }, {TRUE, {200, 47}, {0, 0, 8, 8}, 0, 0, 1, }, };
const EntityMerged const EntityMerged_arr_Level_6[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_6[0],0,NULL,NULL, 1, 1, 0, }, {0, TRUE, {141, 49}, {FIX32(141), FIX32(49)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_6[1],1,NULL,&spr_enemy01, 1, 1, 0, }, {0, TRUE, {200, 47}, {FIX32(200), FIX32(47)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_6[2],2,NULL,&spr_noSpr, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_6[] = {3, &EntityMerged_arr_Level_6, 3, &Trigger_arr_Level_6, };
const Level const lvl_Level_6 = {NULL, &Level_6_bgb_map, NULL, &Level_6_bgb_tileset, NULL, &Level_6_bgb_pal, {10, 186}, collisionMap6, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_7[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 8, 1, }, };
const EntityMerged const EntityMerged_arr_Level_7[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_7[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_7[] = {1, &EntityMerged_arr_Level_7, 1, &Trigger_arr_Level_7, };
const Level const lvl_Level_7 = {NULL, &Level_7_bgb_map, NULL, &Level_7_bgb_tileset, NULL, &Level_7_bgb_pal, {11, 181}, collisionMap7, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_8[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 9, 1, }, };
const EntityMerged const EntityMerged_arr_Level_8[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_8[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_8[] = {1, &EntityMerged_arr_Level_8, 1, &Trigger_arr_Level_8, };
const Level const lvl_Level_8 = {NULL, &Level_8_bgb_map, NULL, &Level_8_bgb_tileset, NULL, &Level_8_bgb_pal, {11, 173}, collisionMap8, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_9[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 10, 1, }, };
const EntityMerged const EntityMerged_arr_Level_9[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_9[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_9[] = {1, &EntityMerged_arr_Level_9, 1, &Trigger_arr_Level_9, };
const Level const lvl_Level_9 = {NULL, &Level_9_bgb_map, NULL, &Level_9_bgb_tileset, NULL, &Level_9_bgb_pal, {8, 184}, collisionMap9, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_10[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 11, 1, }, };
const EntityMerged const EntityMerged_arr_Level_10[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_10[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_10[] = {1, &EntityMerged_arr_Level_10, 1, &Trigger_arr_Level_10, };
const Level const lvl_Level_10 = {NULL, &Level_10_bgb_map, NULL, &Level_10_bgb_tileset, NULL, &Level_10_bgb_pal, {0, 0}, collisionMap10, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_11[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 12, 1, }, };
const EntityMerged const EntityMerged_arr_Level_11[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_11[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_11[] = {1, &EntityMerged_arr_Level_11, 1, &Trigger_arr_Level_11, };
const Level const lvl_Level_11 = {NULL, &Level_11_bgb_map, NULL, &Level_11_bgb_tileset, NULL, &Level_11_bgb_pal, {10, 179}, collisionMap11, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_12[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 13, 1, }, };
const EntityMerged const EntityMerged_arr_Level_12[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_12[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_12[] = {1, &EntityMerged_arr_Level_12, 1, &Trigger_arr_Level_12, };
const Level const lvl_Level_12 = {NULL, &Level_12_bgb_map, NULL, &Level_12_bgb_tileset, NULL, &Level_12_bgb_pal, {6, 174}, collisionMap12, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_13[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 14, 1, }, };
const EntityMerged const EntityMerged_arr_Level_13[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_13[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_13[] = {1, &EntityMerged_arr_Level_13, 1, &Trigger_arr_Level_13, };
const Level const lvl_Level_13 = {NULL, &Level_13_bgb_map, NULL, &Level_13_bgb_tileset, NULL, &Level_13_bgb_pal, {0, 167}, collisionMap13, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_14[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 15, 1, }, };
const EntityMerged const EntityMerged_arr_Level_14[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_14[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_14[] = {1, &EntityMerged_arr_Level_14, 1, &Trigger_arr_Level_14, };
const Level const lvl_Level_14 = {NULL, &Level_14_bgb_map, NULL, &Level_14_bgb_tileset, NULL, &Level_14_bgb_pal, {0, 179}, collisionMap14, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_15[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 16, 1, }, };
const EntityMerged const EntityMerged_arr_Level_15[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_15[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_15[] = {1, &EntityMerged_arr_Level_15, 1, &Trigger_arr_Level_15, };
const Level const lvl_Level_15 = {NULL, &Level_15_bgb_map, NULL, &Level_15_bgb_tileset, NULL, &Level_15_bgb_pal, {0, 176}, collisionMap15, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_16[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 17, 1, }, };
const EntityMerged const EntityMerged_arr_Level_16[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_16[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_16[] = {1, &EntityMerged_arr_Level_16, 1, &Trigger_arr_Level_16, };
const Level const lvl_Level_16 = {NULL, &Level_16_bgb_map, NULL, &Level_16_bgb_tileset, NULL, &Level_16_bgb_pal, {0, 178}, collisionMap16, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_17[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 18, 1, }, };
const EntityMerged const EntityMerged_arr_Level_17[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_17[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_17[] = {1, &EntityMerged_arr_Level_17, 1, &Trigger_arr_Level_17, };
const Level const lvl_Level_17 = {NULL, &Level_17_bgb_map, NULL, &Level_17_bgb_tileset, NULL, &Level_17_bgb_pal, {0, 180}, collisionMap17, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_18[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 19, 1, }, };
const EntityMerged const EntityMerged_arr_Level_18[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_18[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_18[] = {1, &EntityMerged_arr_Level_18, 1, &Trigger_arr_Level_18, };
const Level const lvl_Level_18 = {NULL, &Level_18_bgb_map, NULL, &Level_18_bgb_tileset, NULL, &Level_18_bgb_pal, {0, 171}, collisionMap18, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_19[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 20, 1, }, };
const EntityMerged const EntityMerged_arr_Level_19[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_19[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_19[] = {1, &EntityMerged_arr_Level_19, 1, &Trigger_arr_Level_19, };
const Level const lvl_Level_19 = {NULL, &Level_19_bgb_map, NULL, &Level_19_bgb_tileset, NULL, &Level_19_bgb_pal, {0, 175}, collisionMap19, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_20[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 21, 1, }, };
const EntityMerged const EntityMerged_arr_Level_20[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_20[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_20[] = {1, &EntityMerged_arr_Level_20, 1, &Trigger_arr_Level_20, };
const Level const lvl_Level_20 = {NULL, &Level_20_bgb_map, NULL, &Level_20_bgb_tileset, NULL, &Level_20_bgb_pal, {0, 184}, collisionMap20, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_21[] = {{TRUE, {299, 6}, {0, 0, 20, 200}, 0, 22, 1, }, };
const EntityMerged const EntityMerged_arr_Level_21[] = {{0, TRUE, {299, 6}, {FIX32(299), FIX32(6)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_21[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_21[] = {1, &EntityMerged_arr_Level_21, 1, &Trigger_arr_Level_21, };
const Level const lvl_Level_21 = {NULL, &Level_21_bgb_map, NULL, &Level_21_bgb_tileset, NULL, &Level_21_bgb_pal, {0, 174}, collisionMap21, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_22[] = {{TRUE, {5, 221}, {0, 0, 310, 10}, 0, 5, 1, }, };
const EntityMerged const EntityMerged_arr_Level_22[] = {{0, TRUE, {5, 221}, {FIX32(5), FIX32(221)}, {0, 0}, {32, 32}, FALSE, &Trigger_arr_Level_22[0],0,NULL,NULL, 1, 1, 0, }, };
const EntityAll const EntityAll_arr_Level_22[] = {1, &EntityMerged_arr_Level_22, 1, &Trigger_arr_Level_22, };
const Level const lvl_Level_22 = {NULL, &Level_22_bgb_map, NULL, &Level_22_bgb_tileset, NULL, &Level_22_bgb_pal, {133, 189}, collisionMap22, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_23[] = {};
const EntityMerged const EntityMerged_arr_Level_23[] = {};
const EntityAll const EntityAll_arr_Level_23[] = {0, &EntityMerged_arr_Level_23, 0, &Trigger_arr_Level_23, };
const Level const lvl_Level_23 = {NULL, &Level_23_bgb_map, NULL, &Level_23_bgb_tileset, NULL, &Level_23_bgb_pal, {166, 184}, collisionMap23, {320, 224}, {40, 28}};
const Trigger const Trigger_arr_Level_24[] = {};
const EntityMerged const EntityMerged_arr_Level_24[] = {};
const EntityAll const EntityAll_arr_Level_24[] = {0, &EntityMerged_arr_Level_24, 0, &Trigger_arr_Level_24, };
const Level const lvl_Level_24 = {NULL, &Level_24_bgb_map, NULL, &Level_24_bgb_tileset, NULL, &Level_24_bgb_pal, {55, 176}, collisionMap24, {320, 224}, {40, 28}};
const LevelFull const LevelFull_arr[] = {{&lvl_Level_0, &EntityAll_arr_Level_0}, {&lvl_Level_1, &EntityAll_arr_Level_1}, {&lvl_Level_2, &EntityAll_arr_Level_2}, {&lvl_Level_3, &EntityAll_arr_Level_3}, {&lvl_Level_4, &EntityAll_arr_Level_4}, {&lvl_Level_5, &EntityAll_arr_Level_5}, {&lvl_Level_6, &EntityAll_arr_Level_6}, {&lvl_Level_7, &EntityAll_arr_Level_7}, {&lvl_Level_8, &EntityAll_arr_Level_8}, {&lvl_Level_9, &EntityAll_arr_Level_9}, {&lvl_Level_10, &EntityAll_arr_Level_10}, {&lvl_Level_11, &EntityAll_arr_Level_11}, {&lvl_Level_12, &EntityAll_arr_Level_12}, {&lvl_Level_13, &EntityAll_arr_Level_13}, {&lvl_Level_14, &EntityAll_arr_Level_14}, {&lvl_Level_15, &EntityAll_arr_Level_15}, {&lvl_Level_16, &EntityAll_arr_Level_16}, {&lvl_Level_17, &EntityAll_arr_Level_17}, {&lvl_Level_18, &EntityAll_arr_Level_18}, {&lvl_Level_19, &EntityAll_arr_Level_19}, {&lvl_Level_20, &EntityAll_arr_Level_20}, {&lvl_Level_21, &EntityAll_arr_Level_21}, {&lvl_Level_22, &EntityAll_arr_Level_22}, {&lvl_Level_23, &EntityAll_arr_Level_23}, {&lvl_Level_24, &EntityAll_arr_Level_24}, };


Vect2D_s16 getLevelStartPos() {
	return curLvlData->startPos;
}

void updateRoomSize(Level* lvl){
	roomSize = newAABB(0, lvl->sizeinPx.x, 0, lvl->sizeinPx.y);
}

void loadLevel(u16 levelNum) {
	//Reset everything
	VDP_resetScreen();
	VDP_setPlaneSize(64,32,TRUE);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	SPR_reset();

	//Deallocate prev entityData to avoid memory leak
	MEM_free(curEntityAll->EntityMerged_arr);
	MEM_free(curEntityAll->Trigger_arr);
	MEM_free(curEntityAll);

	MEM_free(bga);
	MEM_free(bgb);

	VDPTilesFilled = TILE_USER_INDEX;
	
	//playerBody.globalPosition = getLevelStartPos();
	playerInit(); //janky solution, but who cares if it works... Me.. I will fix that later.
	curLvlData = LevelFull_arr[levelNum].lvl;

	//duplicate entityAll
	curEntityAll = MEM_alloc(sizeof(EntityAll));
	memcpy(curEntityAll, LevelFull_arr[levelNum].entityAll_arr, sizeof(EntityAll));

	//duplicate entityMerged_arr for entityAll
	curEntityAll->EntityMerged_arr = MEM_alloc(sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);
	memcpy(curEntityAll->EntityMerged_arr, LevelFull_arr[levelNum].entityAll_arr->EntityMerged_arr, sizeof(EntityMerged)*LevelFull_arr[levelNum].entityAll_arr->EntityMerged_size);

	//duplicate Trigger_arr for entityAll
	curEntityAll->Trigger_arr = MEM_alloc(sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);
	memcpy(curEntityAll->Trigger_arr, LevelFull_arr[levelNum].entityAll_arr->Trigger_arr, sizeof(Trigger)*LevelFull_arr[levelNum].entityAll_arr->Trigger_size);

	//update trigger reference in entityMerged_arr
	for(u16 i=0 ; i < curEntityAll->EntityMerged_size; i++){
		curEntityAll->EntityMerged_arr[i].trigger =  &curEntityAll->Trigger_arr[curEntityAll->EntityMerged_arr[i].triggerInd];
	}
	
	
	if(curLvlData->foregroundTileset != NULL){
		PAL_setPalette(PAL1, curLvlData->foregroundPallete->data, DMA);
		VDP_loadTileSet(curLvlData->foregroundTileset, VDPTilesFilled, DMA);
		bga = MAP_create(curLvlData->foregroundMap, BG_A, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->foregroundTileset->numTile;
	} else {
		bga = NULL;
	}
	
	if(curLvlData->backgroundTileset != NULL){
		PAL_setPalette(PAL0, curLvlData->backgroundPallete->data, DMA);
		VDP_loadTileSet(curLvlData->backgroundTileset, VDPTilesFilled, DMA);
		bgb = MAP_create(curLvlData->backgroundMap, BG_B, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, VDPTilesFilled));
		VDPTilesFilled += curLvlData->backgroundTileset->numTile;
	} else {
		bgb = NULL;
	}
	

	//Update the number of tiles filled in order to avoid overlaping them when loading more
	VDPTilesFilled += level_tileset.numTile;

	updateRoomSize(curLvlData);

	//Start play the level's song
	XGM_startPlay(song);
}

u16 getTileValue(s16 x, s16 y) {
	if (x >= curLvlData->sizeinTiles.x || x < 0 || y < 0 || y >= curLvlData->sizeinTiles.y)
		return 0;

	//If the position is inside the collision map return the value of the tile from it
	return curLvlData->collArr[x+(y*curLvlData->sizeinTiles.x)];
}


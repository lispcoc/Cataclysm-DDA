#include "itype.h"
#include "game.h"
#include "setvector.h"
#include <fstream>

// Armor colors
#define C_SHOES  c_blue
#define C_PANTS  c_brown
#define C_BODY   c_yellow
#define C_TORSO  c_ltred
#define C_ARMS   c_blue
#define C_GLOVES c_ltblue
#define C_MOUTH  c_white
#define C_EYES   c_cyan
#define C_HAT    c_dkgray
#define C_STORE  c_green
#define C_DECOR  c_ltgreen

// Special function for setting melee techniques
#define TECH(id, t) itypes[id]->techniques = t

std::vector<std::string> unreal_itype_ids;
std::vector<std::string> martial_arts_itype_ids;
std::vector<std::string> artifact_itype_ids;
std::vector<std::string> standard_itype_ids;
std::vector<std::string> pseudo_itype_ids;

// GENERAL GUIDELINES
// When adding a new item, you MUST REMEMBER to insert it in the itype_id enum
//  at the top of itype.h!
//  Additionally, you should check mapitemsdef.cpp and insert the new item in
//  any appropriate lists.
void game::init_itypes ()
{
// First, the null object.  NOT REALLY AN OBJECT AT ALL.  More of a concept.
 itypes["null"]=
  new itype("null", 0, 0, "none", "", '#', c_white, MNULL, MNULL, PNULL, 0, 0, 0, 0, 0, 0);
// Corpse - a special item
 itypes["corpse"]=
  new itype("corpse", 0, 0, "corpse", "A dead body.", '%', c_white, MNULL, MNULL, PNULL, 0, 0,
            0, 0, 1, 0);
// Fire - only appears in crafting recipes
 itypes["fire"]=
  new itype("fire", 0, 0, "nearby fire",
            "Some fire - if you are reading this it's a bug! (itypdef:fire)",
            '$', c_red, MNULL, MNULL, PNULL, 0, 0, 0, 0, 0, 0);
// Integrated toolset - ditto
 itypes["toolset"]=
  new itype("toolset", 0, 0, "integrated toolset",
            "A fake item. If you are reading this it's a bug! (itypdef:toolset)",
            '$', c_red, MNULL, MNULL, PNULL, 0, 0, 0, 0, 0, 0);
// For smoking crack or meth
 itypes["apparatus"]=
  new itype("apparatus", 0, 0, "something to smoke that from, and a lighter",
            "A fake item. If you are reading this it's a bug! (itypdef:apparatus)",
            '$', c_red, MNULL, MNULL, PNULL, 0, 0, 0, 0, 0, 0);

// Drinks
// Stim should be -8 to 8.
// quench MAY be less than zero--salt water and liquor make you thirstier.
// Thirst goes up by 1 every 5 minutes; so a quench of 12 lasts for 1 hour

// Any foods with a nutrition of lower than 5 will never prompt a 'You are full, force yourself to eat that?' message

#define DRINK(id, name,rarity,price,color,container,quench,nutr,spoils,stim,\
healthy,addict,charges,fun,use_func,addict_func,des, item_flags) \
	itypes[id] = new it_comest(id,rarity,price,name,des,'~',\
color,MNULL,LIQUID,2,1,0,0,0,item_flags,quench,nutr,spoils,stim,healthy,addict,charges,\
fun,container,"null",use_func,addict_func,"DRINK");

#define FOOD(id, name, rarity,price,color,mat1,container,volume,weight,quench,\
nutr,spoils,stim,healthy,addict,charges,fun,use_func,addict_func,des, item_flags) \
itypes[id]=new it_comest(id,rarity,price,name,des,'%',\
color,mat1,SOLID,volume,weight,0,0,0,item_flags, quench,nutr,spoils,stim,healthy,addict,charges,\
fun,container,"null",use_func,addict_func,"FOOD");
// FOOD

// MEDS
#define MED(id, name,rarity,price,color,tool,mat,stim,healthy,addict,\
charges,fun,use_func,addict_func,des) \
itypes[id]=new it_comest(id,rarity,price,name,des,'!',\
color,mat,SOLID,1,1,0,0,0,0,0,0,0,stim,healthy,addict,charges,\
fun,"null",tool,use_func,addict_func,"MED");

/*MED("grack", "Grack Cocaine",      8,420, c_white,        "apparatus",
        POWDER,  200, -2, 80,  4, 50,&iuse::grack,       ADD_CRACK, "\
Grack Cocaine, the strongest substance known to the multiverse\n\
this potent substance is refined from the sweat of the legendary\n\
gracken");
*/

// MELEE WEAPONS
// Only use secondary material if it will have a major impact.
// dam is a somewhat rigid bonus--anything above 30, tops, is ridiculous
// cut is even MORE rigid, and should be kept lower still
// to_hit (affects chances of hitting) should be kept small, -5 to +5
// Note that do-nothing objects (e.g. superglue) belong here too!
#define MELEE(id, name,rarity,price,sym,color,mat1,mat2,volume,wgt,dam,cut,to_hit,\
              flags, des)\
itypes[id]=new itype(id,rarity,price,name,des,sym,\
color,mat1,mat2,SOLID,volume,wgt,dam,cut,to_hit,flags);

//    NAME		RAR PRC SYM COLOR	MAT1	MAT2
MELEE("rope_6", "rope - 6 ft",	 4, 45, ',', c_yellow,	WOOD,	MNULL,
//	VOL WGT DAM CUT HIT FLAGS
	 2,  4,  1,  0,  1, mfb(IF_WRAP), "\
A short piece of nylon rope. Too small to be of much use.");

MELEE("chain", "steel chain",	20, 80, '/', c_cyan,	STEEL,	MNULL,
	 4,  8, 12,  0,  2, mfb(IF_WRAP), "\
A heavy steel chain. Useful as a weapon, or for crafting. It has a chance\n\
to wrap around your target, allowing for a bonus unarmed attack.");
TECH("chain", mfb(TEC_GRAB) );

MELEE("hose", "rubber hose",	15, 80, ',', c_green,	PLASTIC,MNULL,
	 3,  2,  4,  0,  3, mfb(IF_WRAP), "\
A flexible rubber hose. Useful for crafting.");

MELEE("broom", "broom",		30, 24, '/', c_blue,	PLASTIC,MNULL,
	10,  8,  6,  0,  1, 0, "\
A long-handled broom. Makes a terrible weapon unless you're chasing cats.");
TECH("broom", mfb(TEC_WBLOCK_1) );

MELEE("hammer_sledge", "sledge hammer",	 6, 120,'/', c_brown,	WOOD,	IRON,
	18, 38, 40,  0,  0, 0, "\
A large, heavy hammer. Makes a good melee weapon for the very strong, but is\n\
nearly useless in the hands of the weak.");
TECH("hammer_sledge", mfb(TEC_BRUTAL)|mfb(TEC_WIDE) );

MELEE("nailboard", "nail board",	 5,  80,'/', c_ltred,	WOOD,	MNULL,
	 6,  6, 16,  6,  1, mfb(IF_STAB), "\
A long piece of wood with several nails through one end; essentially a simple\n\
mace. Makes a great melee weapon.");
TECH("nailboard", mfb(TEC_WBLOCK_1) );

MELEE("nailbat", "nail bat",	60, 160,'/', c_ltred,	WOOD,	MNULL,
	12, 10, 28,  6,  3, mfb(IF_STAB), "\
A baseball bat with several nails driven through it, an excellent melee weapon.");
TECH("nailbat", mfb(TEC_WBLOCK_1) );

MELEE("2x4", "two by four", 	60,  80,'/', c_ltred,	WOOD,	MNULL,
	 6,  6, 14,  0,  1, 0, "\
A plank of wood. Makes a decent melee weapon, and can be used to board up\n\
doors and windows if you have a hammer and nails.");
TECH("2x4", mfb(TEC_WBLOCK_1) );

MELEE("muffler", "muffler",	30,  30,'/', c_ltgray,	IRON,	MNULL,
	20, 20, 19,  0, -3, 0, "\
A muffler from a car. Very unwieldy as a weapon. Useful in a few crafting\n\
recipes.");
TECH("muffler", mfb(TEC_WBLOCK_2) );

MELEE("pipe", "pipe",		20,  75,'/', c_dkgray,	STEEL,	MNULL,
	 4, 10, 13,  0,  3, 0, "\
A steel pipe, makes a good melee weapon. Useful in a few crafting recipes.");
TECH("pipe", mfb(TEC_WBLOCK_1) );

MELEE("bat", "baseball bat",	60, 160,'/', c_ltred,	WOOD,	MNULL,
	12, 10, 28,  0,  3, 0, "\
A sturdy wood bat. Makes a great melee weapon.");
TECH("bat", mfb(TEC_WBLOCK_1) );

MELEE("bat_metal", "aluminium bat",	60, 160,'/', c_ltred,	STEEL,	MNULL,
	10, 6, 24,  0,  3, 0, "\
An aluminium baseball bat, smaller and lighter than a wooden bat\n\
and a little less damaging as a result.");
TECH("bat_metal", mfb(TEC_WBLOCK_1) );

MELEE("spear_wood", "wood spear",	 5,  40,'/', c_ltred,	WOOD,	MNULL,
	 5,  3,  4, 18,  1, mfb(IF_SPEAR), "\
A stout pole with an improvised grip and a fire-hardened point.");
TECH("spear_wood", mfb(TEC_WBLOCK_1) | mfb(TEC_RAPID) );

MELEE("baton", "expandable baton",8, 175,'/', c_blue,	STEEL,	MNULL,
	 1,  4, 12,  0,  2, 0, "\
A telescoping baton that collapses for easy storage. Makes an excellent\n\
melee weapon.");
TECH("baton", mfb(TEC_WBLOCK_1) );

MELEE("bee_sting", "bee sting",	 5,  70,',', c_dkgray,	FLESH,	MNULL,
	 1,  0,  0, 18, -1, mfb(IF_SPEAR), "\
A six-inch stinger from a giant bee. Makes a good melee weapon.");
TECH("bee_sting", mfb(TEC_PRECISE) );

MELEE("wasp_sting", "wasp sting",	 5,  90,',', c_dkgray,	FLESH,	MNULL,
	 1,  0,  0, 22, -1, mfb(IF_SPEAR), "\
A six-inch stinger from a giant wasp. Makes a good melee weapon.");
TECH("wasp_sting", mfb(TEC_PRECISE) );

//    NAME		RAR PRC SYM COLOR	MAT1	MAT2
MELEE("cane", "walking cane",   10, 160,'/', c_ltred,	WOOD,	MNULL,
//	VOL WGT DAM CUT HIT FLAGS
	  8,  7, 10,  0,  2, 0, "\
Handicapped or not, you always walk in style. Consisting of a metal\n\
headpiece and a wooden body, this makes a great bashing weapon in a pinch.");
TECH("cane", mfb(TEC_WBLOCK_1) );

MELEE("mace", "mace",		20,1000,'/',c_dkgray,	IRON,	WOOD,
	10, 18, 36,  0,  1, 0, "\
A medieval weapon consisting of a wood handle with a heavy iron end. It\n\
is heavy and slow, but its crushing damage is devastating.");
TECH("mace", mfb(TEC_SWEEP) );

MELEE("morningstar", "morningstar",	10,1200,'/',c_dkgray, 	IRON,	WOOD,
	11, 20, 32,  4,  1, mfb(IF_SPEAR), "\
A medieval weapon consisting of a wood handle with a heavy, spiked iron\n\
ball on the end. It deals devastating crushing damage, with a small\n\
amount of piercing to boot.");
TECH("morningstar", mfb(TEC_SWEEP) );

//    NAME		RAR PRC SYM COLOR	MAT1	MAT2
MELEE("pool_cue", "pool cue",	 4, 80,'/', c_red,	WOOD,	MNULL,
//	VOL WGT DAM CUT HIT FLAGS
	14,  5, 12,  0,  3, 0, "\
A hard-wood stick designed for hitting colorful balls around a felt\n\
table. Truly, the coolest of sports.");
TECH("pool_cue", mfb(TEC_WBLOCK_1) );

MELEE("spike", "spike",           0, 0,';',  c_cyan,     STEEL,  MNULL,
	 2,  2,  2, 10, -2, mfb(IF_STAB),"\
A large and slightly misshapen spike, could do some damage\n\
mounted on a vehicle.");

MELEE("javelin", "wooden javelin",	 5,  40,'/', c_ltred,	WOOD,	MNULL,
//	VOL WGT DAM CUT HIT FLAGS
	 5,  3,  6,  22,  2, mfb(IF_SPEAR), "\
A wooden spear, honed to a sharper point and fire hardened\n\
for toughness. The grip area has also be carved and covered\n\
for better grip.");
TECH("javelin", mfb(TEC_WBLOCK_1) | mfb(TEC_RAPID) );

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("frame", "steel frame",  20, 55, ']', c_cyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    60, 240, 20, 0, -5,  0, "\
A large frame made of steel. Useful for crafting.");
TECH("frame", mfb(TEC_DEF_DISARM) );

#define VAR_VEH_PART(id, name,rarity,price,sym,color,mat1,mat2,volume,wgt,dam,cut,to_hit,\
              flags, bigmin, bigmax, bigaspect, des)\
itypes[id]=new it_var_veh_part(id,rarity,price,name,des,sym,\
color,mat1,mat2,volume,wgt,dam,cut,to_hit,flags, bigmin, bigmax, bigaspect)

//"wheel", "wheel_wide", "wheel_bicycle", "wheel_motorbike", "wheel_small",
//           NAME     RAR PRC  SYM COLOR        MAT1    MAT2
VAR_VEH_PART("wheel", "wheel", 10, 100, ']', c_dkgray,  STEEL,   PLASTIC,
//  VOL WGT DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX  BIGNESS_ASPECT
    40,  140, 12,  0,  -1, 0,       13,         20,  BIGNESS_WHEEL_DIAMETER,  "\
A car wheel");
//           NAME         RAR PRC  SYM COLOR        MAT1    MAT2
VAR_VEH_PART("wheel_wide", "wide wheel", 4, 340, ']', c_dkgray,  STEEL,   PLASTIC,
//  VOL WGT  DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX   ASPECT
    70,  260, 17,  0,  -1, 0,       17,         36,  BIGNESS_WHEEL_DIAMETER,  "\
A wide wheel. \\o/ This wide.");
//           NAME            RAR  PRC  SYM COLOR        MAT1    MAT2
VAR_VEH_PART("wheel_bicycle", "bicycle wheel", 18, 40,  ']', c_dkgray,  STEEL,   PLASTIC,
//  VOL WGT  DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX    ASPECT
    28,  45,  8,  0,  -1, 0,       9,         18,  BIGNESS_WHEEL_DIAMETER,  "\
A bicycle wheel");
//           NAME              RAR  PRC   SYM COLOR        MAT1    MAT2
VAR_VEH_PART("wheel_motorbike", "motorbike wheel", 13, 140,  ']', c_dkgray,  STEEL,   PLASTIC,
//  VOL WGT  DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX    ASPECT
    33,  85,  10,  0,  -1, 0,       9,         14,  BIGNESS_WHEEL_DIAMETER,  "\
A motorbike wheel");
//           NAME              RAR  PRC   SYM COLOR        MAT1    MAT2
VAR_VEH_PART("wheel_small", "small wheel",    5, 140,  ']', c_dkgray,  STEEL,   PLASTIC,
//  VOL WGT  DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX    ASPECT
    9,  42,  10,  0,  -1, 0,       6,         14,   BIGNESS_WHEEL_DIAMETER,  "\
A pretty small wheel. Probably from one of those segway things.\
It is not very menacing.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("seat", "seat",  8, 250, '0', c_red,  PLASTIC,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    30,  80,  4,  0,  -4, 0, "\
A soft car seat covered with leather.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("vehicle_controls", "vehicle controls",  3, 400, '$', c_ltcyan,  PLASTIC,   STEEL,
//  VOL WGT DAM CUT HIT FLAGS
    12,  30,  2,  0,  -4, 0, "\
A set of various vehicle controls. Useful for crafting.");

//                                 NAME           RAR PRC SYM COLOR        MAT1    MAT2
VAR_VEH_PART("1cyl_combustion", "1-cylinder engine",  3, 100, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS 0BIGNESS_MIN BIGNESS_MAX   ASPECT
    6,  70,  4,  0,  -1, 0,       28,         75,   BIGNESS_ENGINE_DISPLACEMENT, "\
A single-cylinder 4-stroke combustion engine.");

//                              NAME           RAR PRC SYM COLOR        MAT1    MAT2
VAR_VEH_PART("v2_combustion", "V-twin engine",  2, 100, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX ASPECT
    6,  70,  4,  0,  -1, 0,       65,        260, BIGNESS_ENGINE_DISPLACEMENT, "\
A 2-cylinder 4-stroke combustion engine.");

//                                NAME           RAR PRC SYM COLOR        MAT1    MAT2
VAR_VEH_PART("i4_combustion", "Inline-4 engine",  6, 150, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX ASPECT
    6,  160,  8,  0,  -2, 0,       220,       350, BIGNESS_ENGINE_DISPLACEMENT, "\
A small, yet powerful 4-cylinder combustion engine.");

//                          NAME           RAR PRC SYM COLOR        MAT1    MAT2
VAR_VEH_PART("v6_combustion", "V6 engine",  3, 180, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX ASPECT
    14,  400,  12,  0,  -3, 0,    250,        520, BIGNESS_ENGINE_DISPLACEMENT, "\
A powerful 6-cylinder combustion engine.");

//                          NAME           RAR PRC SYM COLOR        MAT1    MAT2
VAR_VEH_PART("v8_combustion", "V8 engine",  2, 250, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS BIGNESS_MIN BIGNESS_MAX ASPECT
    25,  600,  15,  0,  -5, 0,    380,     700, BIGNESS_ENGINE_DISPLACEMENT, "\
A large and very powerful 8-cylinder combustion engine.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("motor", "electric motor",  2,120, ',', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    6,  80,  4,  0,  0, 0, "\
A powerful electric motor. Useful for crafting.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("motor_large", "large electric motor",  1,220, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    15,  650,  9,  0,  -3, 0, "\
A large and very powerful electric motor. Useful for crafting.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("plasma_engine", "plasma engine",  1, 900, ':', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    12,  350,  7,  0,  -2, 0, "\
High technology engine, working on hydrgen fuel.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("foot_crank", "foot crank",  10, 90, ':', c_ltgray,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    2,  10,  10,  0,  -1, 0, "\
The pedal and gear assembly from a bicycle.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("metal_tank", "metal tank",  10, 40, '}', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    18,  25,  3,  0,  -2, 0, "\
A metal tank for holding liquids. Useful for crafting.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("storage_battery", "storage battery",  6, 80, ':', c_ltcyan,  IRON,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    8,  220,  6,  0,  -2, 0, "\
A large storage battery. Useful for crafting.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("minireactor", "minireactor",  1, 900, ':', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    6,  250,  11,  0,  -4, 0, "\
A small portable plutonium reactor. Handle with great care!");

//      NAME          RAR PRC SYM COLOR        MAT1    MAT2
MELEE("solar_panel", "solar panel",  3, 900, ']', c_yellow,  GLASS,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    12,  4,  1,  0,  -4, 0, "\
Electronic device that can convert solar radiation into electric\n\
power. Useful for crafting.");

MELEE("solar_cell", "solar cell", 5, 50, ';', c_yellow, GLASS, MNULL,
      1, 0,  1,  0,  -4, 0, "\
A small electronic device that can convert solar radiation into\n\
electric power. Useful for crafting.");

MELEE("sheet_metal", "sheet metal",  30, 60, ']', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    4,  20,  5,  0,  -2, 0, "\
A thin sheet of metal.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("steel_plate", "steel plating",  30, 120, ']', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    12,  600,  6,  0,  -1, 0, "\
A piece of armor plating made of steel.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("alloy_plate", "superalloy plating",  10, 185, ']', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    12,  350,  6,  0,  -1, 0, "\
A piece of armor plating made of sturdy superalloy.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("spiked_plate", "spiked plating",  15, 185, ']', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    14,  600,  6,  3,  -1, 0, "\
A piece of armor plating made of steel. It is covered by menacing\n\
spikes.");

//      NAME           RAR PRC SYM COLOR        MAT1    MAT2
MELEE("hard_plate", "hard plating",  30, 160, ']', c_ltcyan,  STEEL,   MNULL,
//  VOL WGT DAM CUT HIT FLAGS
    12,  1800,  6,  0,  -1, 0, "\
A piece of very thick armor plating made of steel.");

MELEE("kitchen_unit", "RV kitchen unit", 20, 400, '&', c_ltcyan, STEEL, MNULL,
    80, 900, 0, 0, -2, 0, "\
A vehicle mountable electric range and sink unit with integrated\n\
tool storage for cooking utensils.");

// ARMOR
#define ARMOR(id, name, rarity,price,color,mat1,mat2,volume,wgt,dam,to_hit,\
encumber,dmg_resist,cut_resist,env,warmth,storage,covers,des,item_flags)\
itypes[id]=new it_armor(id, rarity,price,name,des,'[',\
color,mat1,mat2,volume,wgt,dam,0,to_hit,item_flags,covers,encumber,dmg_resist,cut_resist,\
env,warmth,storage)

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("socks", "socks",	70, 100,C_SHOES,	COTTON,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  1, -5,  0,  0,  0,  0,  0,  10,  0,	mfb(bp_feet), "\
Socks. Put 'em on your feet.", 0);

ARMOR("socks_wool", "wool socks",		30, 120,C_SHOES,	WOOL,	MNULL,
    2,  1, -5,  0,  0,  0,  0,  0,  20,  0,	mfb(bp_feet), "\
Warm socks made of wool.", 0);

ARMOR("sneakers", "sneakers",	80, 100,C_SHOES,	LEATHER,	MNULL,
    5,  4, -2,  0,  0,  0,  2,  0,  20,  0,	mfb(bp_feet), "\
Guaranteed to make you run faster and jump higher!", mfb(IF_VARSIZE));

ARMOR("boots", "boots",		70, 120,C_SHOES,	LEATHER,	MNULL,
    7,  6,  1, -1,  1,  1,  4,  2,  50,  0,	mfb(bp_feet), "\
Tough leather boots. Very durable.", mfb(IF_VARSIZE));

ARMOR("boots_fur", "fur boots",		70, 120,C_SHOES,	LEATHER,	FUR,
    7,  6,  1, -1,  1,  1,  4,  2,  70,  0,	mfb(bp_feet), "\
Boots lined with fur for warmth.", mfb(IF_VARSIZE));

ARMOR("boots_steel", "steeltoed boots",50, 135,C_SHOES,	LEATHER,	STEEL,
    7,  9,  4, -1,  1,  4,  4,  3,  50,  0,	mfb(bp_feet), "\
Leather boots with a steel toe. Extremely durable.", mfb(IF_VARSIZE));

ARMOR("boots_winter", "winter boots",	60, 140,C_SHOES,	WOOL,	PLASTIC,
    8,  7,  0, -1,  2,  0,  2,  1,  90,  0,	mfb(bp_feet), "\
Cumbersome boots designed for warmth.", mfb(IF_VARSIZE));

ARMOR("mocassins", "mocassins",	 5,  80,C_SHOES,	FUR,	LEATHER,
    2,  1, -3,  0,  0,  0,  1,  0,  40,  0,	mfb(bp_feet), "\
Simple shoes made from animal pelts.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("flip_flops", "flip-flops",	35,  25,C_SHOES,	PLASTIC,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  1, -4, -2,  3,  0,  0,  0,  0,  0,	mfb(bp_feet), "\
Simple sandals. Very difficult to run in.", mfb(IF_VARSIZE));

ARMOR("dress_shoes", "dress shoes",	50,  45,C_SHOES,	LEATHER,	MNULL,
    5,  3,  1,  1,  1,  0,  3,  0,  10,  0,	mfb(bp_feet), "\
Fancy patent leather shoes. Not designed for running in.", mfb(IF_VARSIZE));

ARMOR("heels", "heels",		50,  50,C_SHOES,	LEATHER,	MNULL,
    4,  2,  6, -2,  4,  0,  0,  0,  0,  0,	mfb(bp_feet), "\
A pair of high heels. Difficult to even walk in.", mfb(IF_VARSIZE));

ARMOR("boots_chitin", "chitinous boots",50, 135,C_SHOES,	LEATHER,	STEEL,
    7,  9,  4, -1,  1,  4,  4,  3,  50,  0,	mfb(bp_feet), "\
Boots made from the exoskeletons of insects. Light and durable.", 0);

ARMOR("shorts", "shorts",		70, 180,C_PANTS,	COTTON,		MNULL,
    4,  2, -4,  1,  0,  0,  1,  0,  0,  4,	mfb(bp_legs), "\
A pair of khaki shorts.", mfb(IF_VARSIZE));

ARMOR("shorts_cargo", "cargo shorts",		50, 180,C_PANTS,	COTTON,		MNULL,
    4,  2, -4,  1,  0,  0,  1,  0,  0,  8,	mfb(bp_legs), "\
A pair of shorts lined with pockets, offering decent storage.", mfb(IF_VARSIZE));

ARMOR("jeans", "jeans",		90, 180,C_PANTS,	COTTON,		MNULL,
    5,  4, -4,  1,  1,  0,  1,  0,  10,  2,	mfb(bp_legs), "\
A pair of blue jeans with two deep pockets.", mfb(IF_VARSIZE));

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("pants", "pants",		75, 185,C_PANTS,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    5,  5, -4,  1,  1,  0,  1,  0,  20,  4,	mfb(bp_legs), "\
A pair of khaki pants. Slightly warmer than jeans.", mfb(IF_VARSIZE));

ARMOR("pants_leather", "leather pants",	60, 210,C_PANTS,	LEATHER,	MNULL,
    6,  8, -2,  1,  2,  1,  7,  0,  50,  2,	mfb(bp_legs), "\
A pair of black leather pants. Very tough, but cumbersome and without much\n\
storage.", mfb(IF_VARSIZE));

ARMOR("pants_cargo", "cargo pants",	70, 280,C_PANTS,	COTTON,		MNULL,
    6,  6, -3,  0,  1,  0,  2,  0,  20, 12,	mfb(bp_legs), "\
A pair of pants lined with pockets, offering lots of storage.", mfb(IF_VARSIZE));

ARMOR("pants_army", "army pants",	30, 315,C_PANTS,	COTTON,		MNULL,
    6,  7, -2,  0,  1,  0,  3,  0,  40, 14,	mfb(bp_legs), "\
A tough pair of pants lined with pockets. Favored by the military.", mfb(IF_VARSIZE));

ARMOR("pants_ski", "ski pants",	60, 300,C_PANTS,	COTTON,		MNULL,
    10,  6, -3,  0,  2,  2,  0,  3,  80, 4,	mfb(bp_legs), "\
A pair of pants meant for alpine skiing.", mfb(IF_VARSIZE));

ARMOR("pants_fur", "fur pants",	60, 300,C_PANTS,	COTTON,		FUR,
    10,  6, -3,  1,  2,  2,  0,  3,  80, 4,	mfb(bp_legs), "\
A hefty pair of fur-lined pants.", mfb(IF_VARSIZE));

ARMOR("long_underpants", "long underwear",	40, 200,C_PANTS,	COTTON,		MNULL,
    4,  2, -3,  0,  0,  0,  0,  0,  30,  0,	mfb(bp_legs), "\
A pair of long underwear that help to maintain body temperature.", mfb(IF_VARSIZE));

ARMOR("skirt", "skirt",		75, 120,C_PANTS,	COTTON,		MNULL,
    2,  2, -5,  0, -1,  0,  0,  0,  0,  1,	mfb(bp_legs), "\
A short, breezy cotton skirt. Easy to move in, but only has a single small\n\
pocket.", mfb(IF_VARSIZE));

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("jumpsuit", "jumpsuit",	20, 200,C_BODY,		COTTON,		PLASTIC,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    6,  6, -3, -3,  0,  0,  1,  0,  10, 8,	mfb(bp_legs)|mfb(bp_torso), "\
A thin, short-sleeved jumpsuit; similar to those\n\
worn by prisoners. Provides decent storage and is\n\
not very encumbering.", mfb(IF_VARSIZE));

ARMOR("wolfsuit", "wolf suit", 4, 200, C_BODY,  COTTON,     MNULL,
    18,  19, -3, -3, 1,  3,  7,  2,  50,  4,
    mfb(bp_legs)|mfb(bp_torso)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_head)|mfb(bp_feet)|mfb(bp_mouth)|mfb(bp_eyes), "\
A full body fursuit in the form of an anthropomorphic wolf. It is quite encumbering\n\
and has little storage but is very warm.", 0);

ARMOR("dress", "dress",		70, 180,C_BODY,		COTTON,		MNULL,
    8,  6, -5, -5,  3,  0,  1,  0,  20,  0,	mfb(bp_legs)|mfb(bp_torso), "\
A long cotton dress. Difficult to move in and lacks any storage space.", mfb(IF_VARSIZE));

ARMOR("armor_chitin", "chitinous armor", 1,1200,C_BODY,		FLESH,		MNULL,
   70, 10,  2, -5,  2,  8, 14,  0,  10,  0,	mfb(bp_legs)|mfb(bp_torso), "\
Leg and body armor made from the exoskeletons of insects. Light and durable.", 0);

ARMOR("suit", "suit",		60, 180,C_BODY,		COTTON,		MNULL,
   10,  7, -5, -5,  1,  0,  1,  0,  25,  10,	mfb(bp_legs)|mfb(bp_torso)|mfb(bp_arms), "\
A full-body cotton suit. Makes the apocalypse a truly gentlemanly\n\
experience.", mfb(IF_VARSIZE));

ARMOR("hazmat_suit", "hazmat suit",	10,1000,C_BODY,		PLASTIC,	MNULL,
   20, 8, -5,  -8,  4,  0,  0, 10,  20, 12,	mfb(bp_legs)|mfb(bp_torso)|mfb(bp_arms), "\
A hazardous materials suit. Though quite bulky and cumbersome, wearing it\n\
will provide excellent protection against ambient radiation.", mfb(IF_VARSIZE));

ARMOR("armor_plate", "plate mail",	 2, 700,C_BODY,		IRON,		MNULL,
   70,140,  8, -5,  5, 16, 20,  0,  20,  0,	mfb(bp_torso)|mfb(bp_legs)|mfb(bp_arms), "\
An extremely heavy ornamental suit of armor.", mfb(IF_VARSIZE));

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("tshirt", "t shirt",	80,  80,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    3,  2, -5,  0,  1,  0,  0,  0,  10,  0,	mfb(bp_torso), "\
A short-sleeved cotton shirt.", mfb(IF_VARSIZE));

ARMOR("polo_shirt", "polo shirt",	65,  95,C_TORSO,	COTTON,		MNULL,
    3,  2, -5,  0,  1,  0,  1,  0,  20,  0,	mfb(bp_torso), "\
A short-sleeved cotton shirt, slightly thicker than a t-shirt.", mfb(IF_VARSIZE));

ARMOR("dress_shirt", "dress shirt",	60, 115,C_TORSO,	COTTON,		MNULL,
    3,  3, -5,  0,  1,  0,  1,  0,  10,  1,	mfb(bp_torso)|mfb(bp_arms), "\
A white button-down shirt with long sleeves. Looks professional!", mfb(IF_VARSIZE));

ARMOR("tank_top", "tank top",	50,  75,C_TORSO,	COTTON,		MNULL,
    1,  1, -5,  0,  0,  0,  0,  0,  0,  0,	mfb(bp_torso), "\
A sleeveless cotton shirt. Very easy to move in.", mfb(IF_VARSIZE));

ARMOR("sweatshirt", "sweatshirt",	75, 110,C_TORSO,	COTTON,		MNULL,
    9,  5, -5,  0,  1,  1,  2,  0,  30,  0,	mfb(bp_torso)|mfb(bp_arms), "\
A thick cotton shirt. Provides warmth and a bit of padding.", mfb(IF_VARSIZE));

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("sweater", "sweater",	75, 105,C_TORSO,	WOOL,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    8,  5, -5,  0,  1,  1,  0,  0,  40,  0,	mfb(bp_torso)|mfb(bp_arms), "\
A wool shirt. Provides warmth.", mfb(IF_VARSIZE));

ARMOR("hoodie", "hoodie",		65, 130,C_TORSO,	COTTON,		MNULL,
   10,  5, -5,  0,  1,  1,  1,  0,  30,  9,	mfb(bp_torso)|mfb(bp_arms), "\
A sweatshirt with a hood and a \"kangaroo pocket\" in front for storage.", mfb(IF_VARSIZE));

ARMOR("under_armor", "under armor", 20, 200,C_TORSO,	COTTON,		MNULL,
   2,  2, -5,  0, 0,  0,  0,  0,  20,  0,	mfb(bp_torso), "\
Sports wear that clings to your chest to maintain body temperature.", mfb(IF_VARSIZE));

ARMOR("jacket_light", "light jacket",	50, 105,C_TORSO,	COTTON,		MNULL,
    6,  4, -5,  0,  1,  0,  2,  0,  20,  4,	mfb(bp_torso)|mfb(bp_arms), "\
A thin cotton jacket. Good for brisk weather.", mfb(IF_VARSIZE));

ARMOR("jacket_jean", "jean jacket",	35, 120,C_TORSO,	COTTON,		MNULL,
    7,  5, -3,  0,  1,  0,  4,  0,  20,  3,	mfb(bp_torso)|mfb(bp_arms), "\
A jacket made from denim. Provides decent protection from cuts.", mfb(IF_VARSIZE));

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("blazer", "blazer",		35, 120,C_TORSO,	WOOL,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  6, -4,  0,  2,  0,  3,  0,  30,  2,	mfb(bp_torso)|mfb(bp_arms), "\
A professional-looking wool blazer. Quite cumbersome.", mfb(IF_VARSIZE));

ARMOR("jacket_leather", "leather jacket",	30, 150,C_TORSO,	LEATHER,	MNULL,
   14, 14, -2,  1,  2,  1,  9,  1,  40,  4,	mfb(bp_torso)|mfb(bp_arms), "\
A jacket made from thick leather. Cumbersome, but offers excellent\n\
protection from cuts.", mfb(IF_VARSIZE));

ARMOR("kevlar", "kevlar vest",	30, 800,C_TORSO,	KEVLAR,		MNULL,
   24, 24,  6, -3,  2,  4, 22,  0,  20,  4,	mfb(bp_torso), "\
A heavy bulletproof vest. The best protection from cuts and bullets.", mfb(IF_VARSIZE));

ARMOR("coat_rain", "rain coat",	50, 100,C_TORSO,	COTTON,	PLASTIC,
    9,  8, -4,  0,  2,  0,  3,  1,  20,  7,	mfb(bp_torso)|mfb(bp_arms), "\
A plastic coat with two very large pockets. Provides protection from rain.", mfb(IF_VARSIZE));

ARMOR("poncho", "wool poncho",	15, 120,C_TORSO,	WOOL,		MNULL,
    7,  3, -5, -1,  0,  1,  2,  1,  35,  0,	mfb(bp_torso), "\
A simple wool garment worn over the torso. Provides a bit of protection.", mfb(IF_VARSIZE));

//     NAME		RARE	COLOR		MAT1		MAT2
ARMOR("trenchcoat", "trenchcoat",	25, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  6, -5, -1,  1,  0,  1,  1,  20, 24,	mfb(bp_torso)|mfb(bp_arms), "\
A thin cotton trenchcoat, lined with pockets. Great for storage.", mfb(IF_VARSIZE));

//     NAME		RARE	COLOR		MAT1		MAT2
ARMOR("trenchcoat_leather", "leather trenchcoat",	25, 225,C_TORSO,	LEATHER,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   16,  10, -5, -1,  2,  1,  9,  1,  50, 24,	mfb(bp_torso)|mfb(bp_arms), "\
A thick leather trenchcoat, lined with pockets. Great for storage.", mfb(IF_VARSIZE));

ARMOR("trenchcoat_fur", "fur trenchcoat",	25, 225,C_TORSO,	FUR,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   16,  10, -5, -1,  2,  1,  9,  1,  50, 24,	mfb(bp_torso)|mfb(bp_arms), "\
A thick fur trenchcoat, lined with pockets. Great for storage.", mfb(IF_VARSIZE));

ARMOR("coat_winter", "winter coat",	50, 160,C_TORSO,	COTTON,		MNULL,
   12,  6, -5, -2,  3,  3,  1,  1,  70, 12,	mfb(bp_torso)|mfb(bp_arms), "\
A padded coat with deep pockets. Very warm.", mfb(IF_VARSIZE));

ARMOR("coat_fur", "fur coat",	 5, 550,C_TORSO,	FUR,		LEATHER,
   18, 12, -5, -5,  2,  4,  2,  2, 80,  4,	mfb(bp_torso)|mfb(bp_arms), "\
A fur coat with a couple small pockets. Extremely warm.", mfb(IF_VARSIZE));

ARMOR("peacoat", "peacoat",	30, 180,C_TORSO,	COTTON,		MNULL,
   16, 10, -4, -3,  2,  1,  2,  0,  70, 10,	mfb(bp_torso)|mfb(bp_arms), "\
A heavy cotton coat. Cumbersome, but warm and with deep pockets.", mfb(IF_VARSIZE));

ARMOR("vest", "utility vest",	15, 200,C_TORSO,	COTTON,		MNULL,
    4,  3, -3,  0,  0,  0,  1,  0,  5, 14,	mfb(bp_torso), "\
A light vest covered in pockets and straps for storage.", 0);

ARMOR("beltrig", "belt rig",	10, 200,C_TORSO,	COTTON,		MNULL,
    4,  4, -3,  0,  0,  0,  1,  0,  5, 18,	mfb(bp_torso), "\
A light vest covered in webbing, pockets and straps.\n\
This variety is favoured by the military.", 0);

ARMOR("coat_lab", "lab coat",	20, 155,C_TORSO,	COTTON,		MNULL,
   11,  7, -3, -2,  1,  1,  2,  0,  10, 14,	mfb(bp_torso)|mfb(bp_arms), "\
A long white coat with several large pockets.", 0);

// arm guards
//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("armguard_soft", "soft arm sleeves",	40,  65,C_ARMS,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    0,  0, -5,  1,  0,  1,  1,  1,  30,  0,	mfb(bp_arms), "\
A pair of soft neoprene arm sleeves, often used in contact sports.", 0);

ARMOR("armguard_hard", "hard arm guards",	20,  130,C_ARMS,	COTTON,		PLASTIC,
    1,  0, -5,  1,  1,  2,  2,  1,  20,  0,	mfb(bp_arms), "\
A pair of neoprene arm sleeves covered with molded plastic sheaths.", 0);

ARMOR("armguard_chitin", "chitin arm guards",	10,  200,C_ARMS,	FLESH,		MNULL,
    2,  0, -5,  1,  1,  3,  3,  2,  10,  0,	mfb(bp_arms), "\
A pair of arm guards made from the exoskeletons of insects. Light and durable.", 0);

ARMOR("armguard_metal", "metal arm guards",	10,  400,C_ARMS,	IRON,		MNULL,
    1,  1, -5,  1,  1,  4,  4,  1,  0,  0,	mfb(bp_arms), "\
A pair of arm guards hammered out from metal. Very stylish.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("gloves_liner", "glove liners",	25,  100,C_GLOVES,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    0,  0, -5,  1,  0,  0,  0,  0,  10,  0,	mfb(bp_hands), "\
A pair of thin cotton gloves. Often used as a liner beneath other gloves.", mfb(IF_VARSIZE));

ARMOR("gloves_light", "light gloves",	35,  65,C_GLOVES,	COTTON,		MNULL,
    1,  0, -5,  1,  1,  0,  0,  0,  30,  0,	mfb(bp_hands), "\
A pair of cotton gloves.", 0);

ARMOR("mittens", "mittens",	30,  40,C_GLOVES,	WOOL,		MNULL,
    2,  0, -5,  1,  8,  0,  1,  0,  90,  0,	mfb(bp_hands), "\
A pair of warm mittens. They are extremely cumbersome.", 0);

ARMOR("gloves_fur", "fur gloves",	30,  40,C_GLOVES,	FUR,		MNULL,
    2,  0, -5,  1,  3,  0,  1,  0,  70,  0,	mfb(bp_hands), "\
A pair of warm fur gloves. They are somewhat cumbersome.", 0);

ARMOR("gloves_wool", "wool gloves",	33,  50,C_GLOVES,	WOOL,		MNULL,
    1,  0, -5,  1,  3,  0,  1,  0,  60,  0,	mfb(bp_hands), "\
A thick pair of wool gloves. Cumbersome but warm.", 0);

ARMOR("gloves_winter", "winter gloves",	40,  65,C_GLOVES,	COTTON,		MNULL,
    2,  0, -5,  1,  5,  1,  1,  0,  70,  0,	mfb(bp_hands), "\
A pair of padded gloves. Cumbersome but warm.", 0);

ARMOR("gloves_leather", "leather gloves",	45,  85,C_GLOVES,	LEATHER,	MNULL,
    1,  1, -3,  2,  1,  0,  3,  0,  40,  0,	mfb(bp_hands), "\
A thin pair of leather gloves. Good for doing manual labor.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("gloves_fingerless", "fingerless gloves",20,90,C_GLOVES,	LEATHER,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  0, -3,  2,  0,  0,  2,  0,  5,  0,	mfb(bp_hands), "\
A pair of leather gloves with no fingers, allowing greater manual dexterity.", 0);

ARMOR("gloves_rubber", "rubber gloves",	20,  30,C_GLOVES,	PLASTIC,	MNULL,
    1,  1, -3,  2,  3,  0,  1,  2,  5,  0,	mfb(bp_hands), "\
A pair of rubber gloves, often used while cleaning with caustic materials.", 0);

ARMOR("gloves_medical", "medical gloves",	70,  10,C_GLOVES,	PLASTIC,	MNULL,
    0,  0, -5,  1,  0,  0,  0,  1,  0,  0,	mfb(bp_hands), "\
A pair of thin latex gloves, designed to limit the spread of disease.", 0);

ARMOR("fire_gauntlets", "fire gauntlets",	 5,  95,C_GLOVES,	LEATHER,	MNULL,
    3,  5, -2,  2,  6,  1,  2,  5,  40,  0,	mfb(bp_hands), "\
A heavy pair of leather gloves, used by firefighters for heat protection.", 0);

ARMOR("gauntlets_chitin", "chitinous gauntlets", 1, 380,C_GLOVES,		FLESH,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   4,   1,  2, -2,  1,  5, 7,   4,  20,  0,	mfb(bp_hands), "\
Gauntlets made from the exoskeletons of insects. Very light and durable.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("mask_dust", "dust mask",	65,  20,C_MOUTH,	COTTON,		IRON,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    0,  0, -5, -3,  1,  0,  0,  2,  10,  0,	mfb(bp_mouth), "\
A simple piece of cotton that straps over the mouth. Provides a small amount\n\
of protection from air-borne illness and dust.", 0);

ARMOR("bandana", "bandana",	35,  28,C_MOUTH,	COTTON, 	MNULL,
    1,  0, -4, -1,  0,  0,  0,  1,  30,  0,	mfb(bp_mouth), "\
A cotton bandana, worn over the mouth for warmth and minor protection from\n\
dust and other contaminants.", 0);

ARMOR("scarf", "scarf",		45,  40,C_MOUTH,	WOOL,   	MNULL,
    2,  3, -5, -3,  1,  1,  0,  2,  60,  0,	mfb(bp_mouth), "\
A long wool scarf, worn over the mouth for warmth.", 0);

ARMOR("scarf_fur", "scarf",		45,  40,C_MOUTH,	FUR,   	MNULL,
    2,  3, -5, -3,  1,  1,  0,  2,  60,  0,	mfb(bp_mouth), "\
A long wool scarf, worn over the mouth for warmth.", 0);

ARMOR("mask_filter", "filter mask",	30,  80,C_MOUTH,	PLASTIC,	MNULL,
    3,  6,  1,  1,  2,  1,  1,  7,  20,  0,	mfb(bp_mouth), "\
A mask that straps over your mouth and nose and filters air. Protects from\n\
smoke, dust, and other contaminants quite well.", 0);

ARMOR("mask_gas", "gas mask",	10, 240,C_MOUTH,	PLASTIC,	MNULL,
    6,  8,  0, -3,  4,  1,  2, 16,  40,  0,	mfb(bp_mouth)|mfb(bp_eyes), "\
A full gas mask that covers the face and eyes. Provides excellent protection\n\
from smoke, teargas, and other contaminants.", 0);

// Eyewear - Encumberment is its effect on your eyesight.
// Environment is the defense to your eyes from noxious fumes etc.


//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("glasses_eye", "eyeglasses",	90, 150,C_EYES,		GLASS,		PLASTIC,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  0, -3, -2,  0,  0,  1,  1,  0,  0,	mfb(bp_eyes), "\
A pair of glasses for the near-sighted. Useless for anyone else.", 0);

ARMOR("glasses_reading", "reading glasses",90,  80,C_EYES,		GLASS,		PLASTIC,
    1,  0, -3, -2,  0,  0,  1,  1,  0,  0,	mfb(bp_eyes), "\
A pair of glasses for the far-sighted. Useless for anyone else.", 0);

ARMOR("glasses_safety", "safety glasses", 40, 100,C_EYES,		PLASTIC,	MNULL,
    1,  0, -5, -2,  0,  2,  4,  1,  0,  0,	mfb(bp_eyes), "\
A pair of plastic glasses, used in workshops, sports, chemistry labs, and\n\
many other places. Provides great protection from damage.", 0);

ARMOR("goggles_swim", "swim goggles",	50, 110,C_EYES,		PLASTIC,	MNULL,
    1,  0, -5, -2,  2,  1,  2,  4,  10,  0,	mfb(bp_eyes), "\
A small pair of goggles. Distorts vision above water, but allows you to see\n\
much further under water.", 0);

ARMOR("goggles_ski", "ski goggles",	30, 175,C_EYES,		PLASTIC,	MNULL,
    2,  1, -4, -2,  1,  1,  2,  6,  60,  0,	mfb(bp_eyes), "\
A large pair of goggles that completely seal off your eyes. Excellent\n\
protection from environmental dangers.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("goggles_welding", "welding goggles", 70, 240,C_EYES,		GLASS,  	STEEL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    2,  4, -1, -3,  6,  2,  5,  6,  10,  0,	mfb(bp_eyes), "\
A dark pair of goggles. They make seeing very difficult, but protect you\n\
from bright flashes.", 0);

ARMOR("goggles_nv", "light amp goggles",1,920,C_EYES,		STEEL,		GLASS,
    3,  6,  1, -2,  2,  2,  3,  6,  20,  0,	mfb(bp_eyes), "\
A pair of goggles that amplify ambient light, allowing you to see in the\n\
dark.  You must be carrying a powered-on unified power supply, or UPS, to use\n\
them.", 0);

ARMOR("glasses_monocle", "monocle",	 2, 200,C_EYES,		GLASS,		PLASTIC,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  0, -3, -2,  0,  0,  1,  1,  0,  0,	mfb(bp_eyes), "\
An essential article of the gentleman's apparel. Also negates near-sight.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("sunglasses", "sunglasses",	90, 150,C_EYES,		GLASS,		PLASTIC,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  0, -3, -2,  0,  0,  1,  1,  0,  0,	mfb(bp_eyes), "\
A pair of sunglasses, good for keeping the glare out of your eyes.", 0);

// Headwear encumberment should ONLY be 0 if it's ok to wear with another
// Headwear environmental protection (ENV) drops through to eyes

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("hat_ball", "baseball cap",	30,  35,C_HAT,		COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    2,  1, -5,  0,  0,  0,  0,  2,  10,  0,	mfb(bp_head), "\
A Red Sox cap. It provides a little bit of warmth.", 0);

ARMOR("hat_boonie", "boonie hat",	10,  55,C_HAT,		COTTON,	MNULL,
    2,  1, -5,  0,  0,  0,  1,  2,  20,  0,	mfb(bp_head), "\
Also called a \"bucket hat.\" Often used in the military.", 0);

ARMOR("hat_cotton", "cotton hat",	45,  40,C_HAT,		COTTON,		MNULL,
    2,  1, -5,  0,  0,  0,  0,  0,  30,  0,	mfb(bp_head), "\
A snug-fitting cotton hat. Quite warm.", 0);

ARMOR("hat_knit", "knit hat",	25,  50,C_HAT,		WOOL,		MNULL,
    2,  1, -5,  0,  0,  1,  0,  0,  40,  0,	mfb(bp_head), "\
A snug-fitting wool hat. Very warm.", 0);

ARMOR("hat_hunting", "hunting cap",	20,  80,C_HAT,		WOOL,		MNULL,
    3,  2, -5,  0,  0,  0,  1,  2,  60,  0,	mfb(bp_head), "\
A red plaid hunting cap with ear flaps. Notably warm.", 0);

ARMOR("hat_fur", "fur hat",	15, 120,C_HAT,		FUR,		LEATHER,
    4,  2, -5,  0,  1,  2,  2,  0,  80,  0,	mfb(bp_head), "\
A hat made from the pelts of animals. Extremely warm.", 0);

ARMOR("balclava", "balaclava",	15, 100,C_HAT,		COTTON,		MNULL,
    4,  2, -5,  0,  0,  0,  0,  0,  30,  0,	mfb(bp_head)|mfb(bp_mouth), "\
A warm covering that protects the head and face from cold.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("hat_hard", "hard hat",	50, 125,C_HAT,		PLASTIC,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    8,  4,  6,  0,  1,  4,  5,  0,  10,  0,	mfb(bp_head), "\
A hard plastic hat worn in constructions sites. Excellent protection from\n\
cuts and percussion.", 0);
TECH("hat_hard", mfb(TEC_WBLOCK_1) );

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("pickelhaube", "pickelhaube",	50, 240,C_HAT,		IRON,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    8,  8,  10,  1,  1,  10,  10,  0,  10,  0,	mfb(bp_head), "\
A spiked helmet once worn by German military officers. The spike is very sharp.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("beret", "beret",	50, 125,C_HAT,		COTTON,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    2,  1,  -5,  0,  0,  0,  0,  0,  20,  0,	mfb(bp_head), "\
A soft cotton hat commonly worn by armed forces and existentialists.", 0);

ARMOR("helmet_bike", "bike helmet",	35, 140,C_HAT,		PLASTIC,	MNULL,
   12,  2,  4,  0,  1,  8,  2,  0,  20,  0,	mfb(bp_head), "\
A thick foam helmet. Designed to protect against concussion.", 0);
TECH("hat_hard", mfb(TEC_WBLOCK_1) );

ARMOR("helmet_skid", "skid lid",	30, 190,C_HAT,		PLASTIC,	IRON,
   10,  5,  8,  0,  2,  6, 16,  0,  10,  0,	mfb(bp_head), "\
A small metal helmet that covers the head and protects against cuts and\n\
percussion.", 0);
TECH("helmet_skid", mfb(TEC_WBLOCK_1) );

ARMOR("helmet_ball", "baseball helmet",45, 195,C_HAT,		PLASTIC,	IRON,
   14,  6,  7, -1,  2, 10, 10,  1,  15,  0,	mfb(bp_head), "\
A hard plastic helmet that covers the head and ears. Designed to protect\n\
against a baseball to the head.", 0);
TECH("helmet_ball", mfb(TEC_WBLOCK_1) );

ARMOR("helmet_army", "army helmet",	40, 480,C_HAT,		PLASTIC,	IRON,
   16,  8, 10, -1,  2, 12, 28,  0,  25,  0,	mfb(bp_head), "\
A heavy helmet that provides excellent protection from all sorts of damage.", 0);
TECH("helmet_army", mfb(TEC_WBLOCK_1) );

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("helmet_riot", "riot helmet",	25, 420,C_HAT,		PLASTIC,	IRON,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   20,  7,  8, -1,  2,  6, 28,  2,  20,  0,	mfb(bp_head)|mfb(bp_eyes)|
						mfb(bp_mouth), "\
A helmet with a plastic shield that covers your entire face.", 0);
TECH("helmet_riot", mfb(TEC_WBLOCK_1) );

ARMOR("helmet_motor", "motorcycle helmet",40,325,C_HAT,		PLASTIC,	IRON,
   24,  8,  7, -1,  3,  8, 20,  1,  30,  0,	mfb(bp_head)|mfb(bp_mouth), "\
A helmet with covers for your head and chin, leaving space in-between for you\n\
to wear goggles.", 0);
TECH("helmet_motor", mfb(TEC_WBLOCK_1) );

ARMOR("helmet_chitin", "chitinous helmet", 1, 380,C_HAT,		FLESH,		MNULL,
   22,  1,  2, -2,  1, 10, 14,  4,  20,  0,	mfb(bp_head)|mfb(bp_eyes)|
						mfb(bp_mouth), "\
A helmet made from the exoskeletons of insects. Covers the entire head; very\n\
light and durable.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("helmet_plate", "great helm",	  1,400,C_HAT,		IRON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    20, 15, 10,  0,  4, 10, 15,  1,  10,  0,	mfb(bp_head)|mfb(bp_eyes)|
						mfb(bp_mouth), "\
A medieval helmet that provides excellent protection to the entire head, at\n\
the cost of great encumbrance.", 0);
TECH("helmet_plate", mfb(TEC_WBLOCK_1) );

ARMOR("tophat", "top hat",	10,  55,C_HAT,		COTTON,	MNULL,
    2,  1, -5,  0,  0,  0,  1,  1,  10,  0,	mfb(bp_head), "\
The only hat for a gentleman. Look exquisite while laughing in the face\n\
of danger!", 0);

ARMOR("backpack", "backpack",	38, 210,C_STORE,	COTTON,	PLASTIC,
   10,  2, -4,  0,  1,  0,  0,  0,  0, 40,	mfb(bp_torso), "\
A small backpack, good storage for a little encumbrance.", 0);

ARMOR("rucksack", "military rucksack",	20, 210,C_STORE,	PLASTIC,	MNULL,
   14,  3, -4,  0,  2,  0,  0,  0,  0, 80,	mfb(bp_torso), "\
A huge military rucksack, provides a lot of storage.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("purse", "purse",		40,  75,C_STORE,	LEATHER,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  3,  2,  2,  1,  0,  0,  0,  0, 20,	mfb(bp_torso), "\
A bit cumbersome to wear, but provides some storage", 0);

ARMOR("mbag", "messenger bag",	20, 110,C_STORE,	COTTON,	PLASTIC,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    8,  2,  1,  1,  0,  0,  0,  0,  0, 20,	mfb(bp_torso), "\
Light and easy to wear, but doesn't offer much storage.", 0);

ARMOR("fanny", "fanny pack", 	10, 100,C_STORE,	COTTON,	PLASTIC,
    3,  1,  1,  2,  0,  0,  0,  0,  0,  6,	0, "\
Provides a bit of extra storage without encumbering you at all.", 0);

ARMOR("holster", "holster",	 8,  90,C_STORE,	LEATHER,	MNULL,
    2,  2,  2, -1,  0,  0,  0,  0,  0,  3,	0, "\
Provides a bit of extra storage without encumbering you at all.", 0);

//     NAME		RAR PRC	COLOR		MAT1		MAT2
ARMOR("bootstrap", "bootstrap",	 3,  80,C_STORE, 	LEATHER,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    1,  1, -1, -1,  0,  0,  0,  0,  1,  2,	mfb(bp_legs), "\
A small holster worn on the ankle.", 0);

ARMOR("ragpouch", "pouch",	20, 110,C_STORE,	COTTON,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    5,  2,  1,  1,  1,  0,  0,  0,  0, 12,	mfb(bp_torso), "\
A makeshift bag, cobbled together from rags. Really gets in the way, but\n\
provides a decent amount of storage.", 0);

ARMOR("leather_pouch", "leather pouch",	20, 110,C_STORE,	LEATHER,	MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
    5,  2,  1,  1,  0,  0,  0,  0,  0, 12,	mfb(bp_torso), "\
A bag stitched together from leather scraps. Doesn't hold an awful lot\n\
but is easy to wear.", 0);

ARMOR("ring", "gold ring",	12, 600,C_DECOR,	SILVER,		MNULL,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	0,	"\
A flashy gold ring. You can wear it if you like, but it won't provide\n\
any effects.", 0);

ARMOR("necklace", "silver necklace",14, 500,C_DECOR,	SILVER,		MNULL,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,	0,	"\
A nice silver necklace. You can wear it if you like, but it won't provide\n\
any effects.", 0);

// Over the body clothing

//     NAME		RARE	COLOR		MAT1		MAT2
ARMOR("blanket", "blanket",	20, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  3, -5, -1,  5,  0,  1,  1,  60, 0,
   mfb(bp_torso)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_legs)|mfb(bp_feet), "\
Hiding under here will not protect you from the monsters.", 0);

ARMOR("fur_blanket", "fur blanket",	20, 225,C_TORSO,	COTTON,		FUR,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  10, -5, -1,  5,  0,  1,  1,  80, 0,
   mfb(bp_torso)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_legs)|mfb(bp_feet), "\
A heavy fur blanket that covers most of your body.", 0);

ARMOR("emer_blanket", "emergency blanket",	20, 225,C_TORSO,	WOOL,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   4,  2, -5, -1,  5,  0,  1,  1,  50, 0,
   mfb(bp_torso)|mfb(bp_arms)|mfb(bp_legs), "\
A compact wool blanket that covers your most important body parts.", 0);

ARMOR("sleeping_bag", "sleeping bag",	10, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  5, -5, -1,  5,  0,  1,  1,  80, 0,
   mfb(bp_torso)|mfb(bp_head)|mfb(bp_mouth)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_legs)|mfb(bp_feet), "\
A large sleeping bag that covers you head to toe.", 0);

ARMOR("sleeping_bag_fur", "fur sleeping bag",	10, 225,C_TORSO,	COTTON,		FUR,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  5, -5, -1,  5,  0,  1,  1,  100, 0,
   mfb(bp_torso)|mfb(bp_head)|mfb(bp_mouth)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_legs)|mfb(bp_feet), "\
A large sleeping bag lined with fur. Who needs a tent?", 0);

ARMOR("house_coat", "house coat",	25, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  6, -5, -1,  1,  0,  1,  1,  40, 6,
   mfb(bp_torso)|mfb(bp_arms)|mfb(bp_legs), "\
Makes you wish you had running water to take a shower.", mfb(IF_VARSIZE));

ARMOR("snuggie", "snuggie",	5, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  6, -5, -1,  2,  0,  1,  1,  50, 0,
   mfb(bp_torso)|mfb(bp_arms)|mfb(bp_legs), "\
Perfect for reading all those books you stole.", mfb(IF_VARSIZE));

ARMOR("cloak", "cloak",	5, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  15, -5, -1,  1,  2,  6,  3,  40, 0,
   mfb(bp_torso)|mfb(bp_head)|mfb(bp_arms)|mfb(bp_legs), "\
A heavy cloak that is thrown over your body.", mfb(IF_VARSIZE));

ARMOR("cloak_fur", "cloak",	5, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  15, -5, -1,  1,  2,  6,  3,  70, 0,
   mfb(bp_torso)|mfb(bp_head)|mfb(bp_arms)|mfb(bp_legs), "\
A heavy fur cloak that is thrown over your body.", mfb(IF_VARSIZE));

ARMOR("cloak_leather", "cloak",	5, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  25, -5, -1,  1,  2,  12,  3,  40, 0,
   mfb(bp_torso)|mfb(bp_head)|mfb(bp_arms)|mfb(bp_legs), "\
A heavy leather cloak that is thrown over your body. Provides decent protection", mfb(IF_VARSIZE));

ARMOR("jedi_cloak", "jedi cloak",	1, 225,C_TORSO,	COTTON,		MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   10,  14, -5, -1,  0,  5,  10,  5,  50, 0,
   mfb(bp_torso)|mfb(bp_head)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_legs), "\
Stylish cloak.", mfb(IF_VARSIZE));

#define POWER_ARMOR(id, name,rarity,price,color,mat1,mat2,volume,wgt,dam,to_hit,\
encumber,dmg_resist,cut_resist,env,warmth,storage,covers,des)\
itypes[id] = new it_armor(id,rarity,price,name,des,'[',\
  color,mat1,mat2,volume,wgt,dam,0,to_hit,0,covers,encumber,dmg_resist,cut_resist,\
  env,warmth,storage,true)

POWER_ARMOR("power_armor_basic", "basic power armor", 5, 1000, C_BODY, STEEL, MNULL,
// VOL WGT DAM HIT ENC RES CUT ENV WRM STO	COVERS
   40, 60, 1, 1, 5, 32, 50, 10, 90, 0, mfb(bp_torso)|mfb(bp_arms)|mfb(bp_hands)|mfb(bp_legs)|mfb(bp_feet), "\
A heavy suit of basic power armor, offering very good protection against attacks, but hard to move in.");

POWER_ARMOR("power_armor_helmet_basic", "basic power armor helmet", 6, 500, C_HAT, STEEL, MNULL,
   10, 18, 1, 1, 5, 32, 50, 10, 90, 0, mfb(bp_head)|mfb(bp_eyes)|mfb(bp_mouth), "\
A basic helmet, designed for use with power armor. Offers excellent protection from both attacks and environmental hazards.");

POWER_ARMOR("power_armor_frame", "power armor hauling frame", 4, 1000, C_STORE, STEEL, MNULL,
    8, 12, 1, 1, 4, 0, 0, 0, 0, 120, 0, "\
A heavy duty hauling frame designed to interface with power armor.");

// AMMUNITION
// Material should be the wrapper--even though shot is made of iron, because
//   it can survive a dip in water and be okay, its material here is PLASTIC.
// dmg is damage done, in an average hit.  Note that the average human has
//   80 health.  Headshots do 8x damage; vital hits do 2x-4x; glances do 0x-1x.
// Weight and price is per 100 rounds.
// AP is a reduction in the armor of the target.
// Accuracy is in quarter-degrees, and measures the maximum this ammo will
//   contribute to the angle of difference.  HIGH ACC IS BAD.
// Recoil is cumulative between shots.  4 recoil = 1 accuracy.
// IMPORTANT: If adding a new AT_*** ammotype, add it to the ammo_name function
//   at the end of this file.
#define AMMO(id, name,rarity,price,ammo_type,color,mat,volume,wgt,dmg,AP,range,\
accuracy,recoil,count,des,effects) \
itypes[id]=new it_ammo(id,rarity,price,name,des,'=',\
color,mat,SOLID,volume,wgt,1,0,0,effects,ammo_type,dmg,AP,accuracy,recoil,range,count);

// FUEL
// Fuel is just a special type of ammo; liquid
#define FUEL(id, name,rarity,price,ammo_type,color,dmg,AP,range,accuracy,recoil,\
             count,des,effects) \
itypes[id]=new it_ammo(id,rarity,price,name,des,'~',\
color,MNULL,LIQUID,1,1,0,0,0,effects,ammo_type,dmg,AP,accuracy,recoil,range,count)

// GUNS
// ammo_type matches one of the ammo_types above.
// dmg is ADDED to the damage of the corresponding ammo.  +/-, should be small.
// aim affects chances of hitting; low for handguns, hi for rifles, etc, small.
// Durability is rated 1-10; 10 near perfect, 1 it breaks every few shots
// Burst is the # of rounds fired, 0 if no burst ability.
// clip is how many shots we get before reloading.

#define GUN(id,name,rarity,price,color,mat1,mat2,skill,ammo,volume,wgt,melee_dam,\
to_hit,dmg,range,accuracy,recoil,durability,burst,clip,reload_time,des,flags) \
itypes[id]=new it_gun(id,rarity,price,name,des,'(',\
color,mat1,mat2,volume,wgt,melee_dam,0,to_hit,flags,skill,ammo,dmg,range,accuracy,\
recoil,durability,burst,clip,reload_time)

// GUN MODS
// Accuracy is inverted from guns; high values are a bonus, low values a penalty
// The clip modification is a percentage of the original clip size.
// The final variable is a bitfield of acceptable ammo types.  Using 0 means
//   that any ammo type is acceptable (so long as the mod works on the class of
//   gun)
#define GUNMOD(id, name, rare, value, color, mat1, mat2, volume, weight, meleedam,\
               meleecut, meleehit, acc, damage, loudness, clip, recoil, burst,\
               newtype, pistol, shotgun, smg, rifle, a_a_t, des, flags)\
  itypes[id]=new it_gunmod(id, rare, value, name, des, ':',\
                            color, mat1, mat2, volume, weight, meleedam,\
                            meleecut, meleehit, flags, acc, damage, loudness,\
                            clip, recoil, burst, newtype, a_a_t, pistol,\
                            shotgun, smg, rifle)


// BOOKS
// Try to keep colors consistant among types of books.
// TYPE is the skill type required to read, or trained via reading; see skill.h
// LEV is the skill level you can be brought to by this book; if your skill is
//  already at LEV or higher, you may enjoy the book but won't learn anything.
// REQ is the skill level required to read this book, at all. If you lack the
//  required skill level, you'll waste 10 (?) turns then quit.
// FUN is the fun had by reading;
// INT is an intelligence requirement.
// TIME is the time, in minutes (10 turns), taken to gain the fun/skill bonus.
#define BOOK(id, name,rarity,price,color,mat1,mat2,volume,wgt,melee_dam,to_hit,\
type,level,req,fun,intel,time,des) \
itypes[id]=new it_book(id,rarity,price,name,des,'?',\
color,mat1,mat2,volume,wgt,melee_dam,0,to_hit,0,type,level,req,fun,intel,time)
//	NAME			RAR PRC	COLOR		MAT1	MAT2

// CONTAINERS
// These are containers you hold in your hand--ones you wear are _armor_!
// These only have two attributes; contains, which is the volume it holds, and
//  the flags. There are only three flags, con_rigid, con_wtight and con_seals.
// con_rigid is used if the item's total volume is constant.
//  Otherwise, its volume is calculated as VOL + volume of the contents
// con_wtight is used if you can store liquids in this container
// con_seals is used if it seals--this has many implications
//  * Won't spill
//  * Can be used as an icebox
//  * Others??
#define CONT(id, name,rarity,price,color,mat1,mat2,volume,wgt,melee_dam,to_hit,\
contains,flags,des) \
itypes[id]=new it_container(id,rarity,price,name,des,\
')',color,mat1,mat2,volume,wgt,melee_dam,0,to_hit,0,contains,flags)
// NAME		RAR PRC	COLOR		MAT1	MAT2

/* TOOLS
 * MAX is the maximum number of charges help.
 * DEF is the default number of charges--items will be generated with this
 *  many charges.
 * USE is how many charges are used up when 'a'pplying the object.
 * SEC is how many turns will pass before a charge is drained if the item is
 *  active; generally only used in the "<whatever> (on)" forms
 * FUEL is the type of charge the tool uses; set to AT_NULL if the item is
 *  unable to be recharged.
 * REVERT is the item type that the tool will revert to once its charges are
 *  drained
 * FUNCTION is a function called when the tool is 'a'pplied, or called once per
 *  turn if the tool is active.  The same function can be used for both.  See
 *  iuse.h and iuse.cpp for functions.
 */
#define TOOL(id, name,rarity,price,sym,color,mat1,mat2,volume,wgt,melee_dam,\
melee_cut,to_hit,max_charge,def_charge,charge_per_use,charge_per_sec,fuel,\
revert,func,flags,des) \
itypes[id]=new it_tool(id,rarity,price,name,des,sym,\
color,mat1,mat2,SOLID,volume,wgt,melee_dam,melee_cut,to_hit,flags,max_charge,\
def_charge,charge_per_use,charge_per_sec,fuel,revert,func)


//	NAME      	 RAR  PRC  COLOR     MAT1   MAT2     VOL WGT DAM CUT HIT
GUNMOD("suppressor", "suppressor",	 15,  480, c_dkgray, STEEL, PLASTIC,  2,  1,  3,  0,  2,
//	ACC DAM NOI CLP REC BST NEWTYPE,	PISTOL	SHOT	SMG	RIFLE
	-1, -4,-25,  0,  0,  0,	AT_NULL,	true,	false,	true,	true,
	0, "\
Using a suppressor is almost an imperative in zombie-infested regions. Gunfire\n\
is very noisy, and will attract predators. Its only drawback is a reduced\n\
muzzle velocity, resulting in less accuracy and damage.",
0);

GUNMOD("grip", "enhanced grip",  12, 280, c_brown,  STEEL, PLASTIC,   1,  1,  0,  0, -1,
	 2,  0,  0,  0, -2,  0, AT_NULL,	false,	true,	true,	true,
	0, "\
A grip placed forward on the barrel allows for greater control and accuracy.\n\
Aside from increased weight, there are no drawbacks.",
0);

GUNMOD("barrel_big", "barrel extension",10,400,  c_ltgray, STEEL, MNULL,    4,  1,  5,  0,  2,
	 6,  1,  0,  0,  5,  0,	AT_NULL,	false,	false,	true,	true,
	0, "\
A longer barrel increases the muzzle velocity of a firearm, contributing to\n\
both accuracy and damage.  However, the longer barrel tends to vibrate after\n\
firing, greatly increasing recoil.",
0);

//	NAME      	 RAR  PRC  COLOR     MAT1   MAT2     VOL WGT DAM CUT HIT
GUNMOD("barrel_small", "shortened barrel", 6, 320, c_ltgray, STEEL, MNULL,    1,  1, -2,  0, -1,
//	ACC DAM NOI CLP REC BST NEWTYPE,	PISTOL	SHOT	SMG	RIFLE
	-5,  0,  2,  0, -6,  0, AT_NULL,	false,	false,	true,	true,
	0, "\
A shortened barrel results in markedly reduced accuracy, and a minor increase\n\
in noise, but also reduces recoil greatly as a result of the improved\n\
managability of the firearm.",
0);

GUNMOD("barrel_rifled", "rifled barrel",    5, 220, c_ltgray, STEEL, MNULL,    2,  1,  3,  0,  1,
	10,-20,  0,  0,  0, -1, AT_NULL,	false,	true,	false,	false,
	0, "\
Rifling a shotgun barrel is mainly done in order to improve its accuracy when\n\
firing slugs. The rifling makes the gun less suitable for shot, however.",
0);

GUNMOD("clip", "extended magazine",	  8,  560, c_ltgray, STEEL, PLASTIC,  1,  1, -2,  0, -1,
	-1,  0,  0, 50,  0,  0, AT_NULL,	true,	true,	true,	true,
	0, "\
Increases the ammunition capacity of your firearm by 50%, but the added bulk\n\
reduces accuracy slightly.",
0);

//	NAME      	 RAR  PRC  COLOR     MAT1   MAT2     VOL WGT DAM CUT HIT
GUNMOD("clip2", "double magazine",	   4, 720, c_ltgray, STEEL, PLASTIC,  2,  2,  0,  0,  0,
//	ACC DAM NOI CLP REC BST NEWTYPE,	PISTOL	SHOT	SMG	RIFLE
	-2,  0,  0,100,  2,  0, AT_NULL,	false,	true,	true,	true,
	0, "\
Completely doubles the ammunition capacity of your firearm, but the added\n\
bulk reduces accuracy and increases recoil.",
0);

GUNMOD("spare_mag", "spare magazine",	   15, 200, c_ltgray, STEEL, PLASTIC,  1,  1,  0,  0,  0,
	0,  0,  0,  0,  0,  0, AT_NULL,	true,	true,	true,	true,
	0, "\
A spare magazine you can keep on hand to make reloads faster, but must itself\n\
 be reloaded before it can be used again.",
0);

GUNMOD("stablizer", "gyroscopic stablizer",4,680,c_blue,  STEEL, PLASTIC,  3,  2,  0,  0, -3,
	 2, -2,  0,-10, -8,  0, AT_NULL,	false,	false,	true,	true,
	0, "\
An advanced unit that straps onto the side of your firearm and reduces\n\
vibration, greatly reducing recoil and increasing accuracy.  However, it also\n\
takes up space in the magazine slot, reducing ammo capacity.",
0);

GUNMOD("blowback", "rapid blowback",   3, 700, c_red,    STEEL, PLASTIC,  0,  1,  0,  0,  0,
	-3,  0,  4,  0,  0,  6, AT_NULL,	false,	false,	true,	true,
	0, "\
An improved blowback mechanism makes your firearm's automatic fire faster, at\n\
the cost of reduced accuracy and increased noise.",
0);

//	NAME      	 RAR  PRC  COLOR     MAT1   MAT2     VOL WGT DAM CUT HIT
GUNMOD("autofire", "auto-fire mechanism",2,650,c_red,    STEEL, PLASTIC,  1,  2,  2,  0, -1,
//	ACC DAM NOI CLP REC BST NEWTYPE,	PISTOL	SHOT	SMG	RIFLE
	-2,  0,  2,  0,  2,  3, AT_NULL,	true,	false,	false,	false,
	(mfb(AT_22)|mfb(AT_9MM)|mfb(AT_38)|mfb(AT_40)|mfb(AT_57)|mfb(AT_46)), "\
A simple mechanism that converts a pistol to a fully-automatic weapon, with\n\
a burst size of three rounds. However, it reduces accuracy, and increases\n\
noise and recoil.",
0);

GUNMOD("retool_9mm", "9mm caliber retool",3,420, c_green,  STEEL, MNULL,    1,  1,  0,  0, -1,
	-1,  0,  0,  0,  0,  0, AT_9MM,		true,	false,	true,	false,
	(mfb(AT_38)|mfb(AT_40)|mfb(AT_44)|mfb(AT_45)), "\
Replacing several key parts of a .38, .40, .44 or .45 firearm converts it to\n\
a 9mm firearm.  The conversion results in a slight reduction in accuracy.",
0);

GUNMOD("retool_22", ".22 caliber retool",2,320, c_green,  STEEL, MNULL,    1,  1, -2,  0, -1,
	-1,  0,  0,  0,  0,  0, AT_22,		true,	false,	true,	true,
	(mfb(AT_9MM)|mfb(AT_38)|mfb(AT_40)|mfb(AT_57)|mfb(AT_46)|mfb(AT_762)|
	 mfb(AT_223)), "\
Replacing several key parts of a 9mm, .38, .40, 5.7mm, 4.6mm, 7.62mm or .223\n\
firearm converts it to a .22 firearm. The conversion results in a slight\n\
reduction in accuracy.",
0);

//	NAME      	 RAR  PRC  COLOR     MAT1   MAT2     VOL WGT DAM CUT HIT
GUNMOD("retool_57", "5.7mm caliber retool",1,460,c_green, STEEL, MNULL,    1,  1, -3,  0, -1,
//	ACC DAM NOI CLP REC BST NEWTYPE,	PISTOL	SHOT	SMG	RIFLE
	-1,  0,  0,  0,  0,  0, AT_57,		true,	false,	true,	false,
	(mfb(AT_22)|mfb(AT_9MM)|mfb(AT_38)), "\
FN Hestal sells a conversion kit, used to convert .22, 9mm, or .38 firearms\n\
to their proprietary 5.7x28mm, a round designed for accuracy and armor\n\
penetration.",
0);

GUNMOD("retool_46", "4.6mm caliber retool",1,460,c_green, STEEL, MNULL,    1,  1, -3,  0, -1,
	-1,  0,  0,  0,  0,  0, AT_46,		true,	false,	true,	false,
	(mfb(AT_22)|mfb(AT_9MM)|mfb(AT_38)), "\
Heckler and Koch sells a conversion kit, used to convert .22, 9mm, or .38\n\
firearms to their proprietary 4.6x30mm, a round designed for accuracy and\n\
armor penetration.",
0);

//	NAME      	RAR  PRC  COLOR     MAT1   MAT2      VOL WGT DAM CUT HIT
GUNMOD("retool_308", ".308 caliber retool",2,520,c_green, STEEL, MNULL,     2,  1,  4,  0,  1,
//	ACC DAM NOI CLP REC BST NEWTYPE		PISTOL	SHOT	SMG	RIFLE
	-2,  0,  0,-20,  0,  0, AT_308,		false,	true,	false,	true,
	(mfb(AT_SHOT)|mfb(AT_762)|mfb(AT_223)|mfb(AT_3006)), "\
This kit is used to convert a shotgun or 7.62mm, .223 or .30-06 rifle to the\n\
popular and powerful .308 caliber. The conversion results in reduced ammo\n\
capacity and a slight reduction in accuracy.",
0);

GUNMOD("retool_223", ".223 caliber retool",2,500,c_green, STEEL, MNULL,     2,  1,  4,  0,  1,
	-2,  0,  0,-10,  0,  0, AT_223,		false,	true,	false,	true,
	(mfb(AT_SHOT)|mfb(AT_762)|mfb(AT_3006)|mfb(AT_308)), "\
This kit is used to convert a shotgun or 7.62mm, .30-06, or .308 rifle to the\n\
popular, accurate, and damaging .223 caliber. The conversion results in\n\
slight reductions in both accuracy and ammo capacity.",
0);

//	NAME      	RAR  PRC  COLOR     MAT1   MAT2      VOL WGT DAM CUT HIT
GUNMOD("conversion_battle", "battle rifle conversion",1,680,c_magenta,STEEL,MNULL, 4,  3,  6,  0, -1,
//	ACC DAM NOI CLP REC BST NEWTYPE		PISTOL	SHOT	SMG	RIFLE
	-6,  6,  4, 20,  4,  4, AT_NULL,	false,	false,	false,	true,
	0, "\
This is a complete conversion kit, designed to turn a rifle into a powerful\n\
battle rifle. It reduces accuracy, and increases noise and recoil, but also\n\
increases damage, ammo capacity, and fire rate.",
0);

GUNMOD("conversion_sniper", "sniper conversion",1,660, c_green,  STEEL, MNULL,     1,  2,  0,  0, -1,
	10,  8,  3,-15,  0,-99, AT_NULL,	false,	false,	false,	true,
	0, "\
This is a complete conversion kit, designed to turn a rifle into a deadly\n\
sniper rifle. It decreases ammo capacity, and removes any automatic fire\n\
capabilities, but also increases accuracy and damage.",
0);

GUNMOD("m203", "M203",		2,650,	c_ltred, STEEL,	MNULL,        2,  1,  2,  0, -1,
	-2,  0,  0,  1,  0, 0, AT_40MM,		false,	false,	false,	true,
	0, "\
The M203 was originally designed for use with M16 variants but today can be\n\
attached to almost any rifle. It allows a single 40mm grenade to be loaded\n\
and fired.",
mfb(IF_MODE_AUX));

//	NAME      	RAR  PRC  COLOR     MAT1   MAT2      VOL WGT DAM CUT HIT
GUNMOD("bayonet", "bayonet",	 6, 400, c_ltcyan, STEEL, MNULL,       2,  2,  0, 16, -3,
//	ACC DAM NOI CLP REC BST NEWTYPE		PISTOL	SHOT	SMG	RIFLE
	  0,  0,  0,  0,  3,  0, AT_NULL,	false,	true,	true,	true,
	0, "\
A bayonet is a stabbing weapon that can be attached to the front of a\n\
shotgun, sub-machinegun or rifle, allowing a melee attack to deal\n\
piercing damage. The added length increases recoil slightly.",
mfb(IF_STAB));

GUNMOD("u_shotgun", "underslung shotgun", 2,650,  c_ltred, STEEL, MNULL,        2,  1,  2,  0, -1,
        -2,  0,  0,  2,  0, 0, AT_SHOT,         false,  false,  false,  true,
        0, "\
A miniaturized shotgun with 2 barrels, which can be mounted under the\n\
barrel of many rifles. It allows two shotgun shells to be loaded and fired.",
mfb(IF_MODE_AUX));

GUNMOD("gun_crossbow", "rail-mounted crossbow", 2, 500,  c_ltred, STEEL, WOOD,      2,  1,  2,  0, -1,
        0,  0,  0,  1,  0, 0, AT_BOLT,         false,  true,  false,  true,
        0, "\
A kit to attach a pair of crossbow arms and a firing rail to\n\
the barrel of a long firearm. It allows crossbow bolts to be fired.",
mfb(IF_MODE_AUX)|mfb(IF_STR_RELOAD));

BOOK("mag_porn", "Playboy",			20,  30,c_pink,		PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    1,  1, -3,  1,	NULL,	 0,  0,  1,  0,  10, "\
You can read it for the articles. Or not.");

BOOK("mag_tv", "US Weekly",		40,  40,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"speech",	 1,  0,  1,  3,  8, "\
Weekly news about a bunch of famous people who're all (un)dead now.");

BOOK("mag_news", "TIME magazine",		35,  40,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	NULL,	 0,  0,  2,  7,  10, "\
Current events concerning a bunch of people who're all (un)dead now.");

BOOK("mag_cars", "Top Gear magazine",	40,  45,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"driving",	 1,  0,  1,  2,  8, "\
Lots of articles about cars and driving techniques.");

BOOK("mag_cooking", "Bon Appetit",		30,  45,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"cooking",	 1,  0,  1,  5,  8, "\
Exciting recipes and restaurant reviews. Full of handy tips about cooking.");

BOOK("mag_carpentry", "Birdhouse Monthly",       30,  45,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"carpentry",	 1,  0,  1,  5,  8, "\
A riveting periodical all about birdhouses and their construction.");

BOOK("mag_guns", "Guns n Ammo",		20,  48,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"gun",		 1,  0,  1,  2,  7, "\
Reviews of firearms, and various useful tips about their use.");

BOOK("mag_archery", "Archery for Kids",		20,  48, c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"archery",		 1,  0,  1,  2,  7, "\
Will you be able to place the arrow right into bull's eye?\n\
It is not that easy, but once you know how it's done,\n\
you will have a lot of fun with archery.");

BOOK("mag_gaming", "Computer Gaming",			20,  30,c_pink,		PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    1,  1, -3,  1,	NULL,	 0,  0,  2,  7,  8, "\
Reviews of recently released computer games and previews\n\
of upcoming titles.");

BOOK("mag_comic", "comic book",			20,  30,c_pink,		PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    1,  1, -3,  1,	NULL,	 0,  0,  2,  0,  7, "\
A super-hero comic.");

BOOK("mag_firstaid", "Paramedics",		15,  48, c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"firstaid",		 1,  0,  1,  1,  8, "\
An educational magazine for EMTs.");

BOOK("mag_dodge", "Dance Dance Dance!",		20,  48,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"dodge",		 1,  0,  1,  2,  8, "\
Learn the moves of the trendiest dances right now.");

BOOK("mag_throwing", "Diskobolus",		20,  48,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"throw",		 1,  0,  1,  1,  8, "\
A biannual magazine devoted to discus throw.");

BOOK("mag_swimming", "Swim Planet",		20,  48,c_pink,		PAPER,	MNULL,
    1,  1, -3,  1,	"swimming",		 1,  0,  1,  1,  8, "\
The world's leading resource about aquatic sports.");

BOOK("novel_romance", "romance novel",		30,  55,c_ltblue,	PAPER,	MNULL,
    4,  1, -2,  0,	NULL,	 0,  0,  2,  4, 15, "\
Drama and mild smut.");

BOOK("novel_spy", "spy novel",		28,  55,c_ltblue,	PAPER,	MNULL,
    4,  1, -2,  0,	NULL,	 0,  0,  3,  5, 18, "\
A tale of intrigue and espionage amongst Nazis, no, Commies, no, Iraqis!");

//	NAME			RAR PRC	COLOR		MAT1	MAT2
BOOK("novel_scifi", "scifi novel",		20,  55,c_ltblue,	PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    3,  1, -3,  0,	NULL,	 0,  0,  3,  6, 20, "\
Aliens, ray guns, and space ships.");

BOOK("novel_drama", "drama novel",		40,  55,c_ltblue,	PAPER,	MNULL,
    4,  1, -2,  0,	NULL,	 0,  0,  4,  7, 25, "\
A real book for real adults.");

BOOK("novel_fantasy", "fantasy novel",		20,  55,c_ltblue,	PAPER,	MNULL,
    4,  1, -2,  0,	NULL,	 0,  0,  4,  7, 20, "\
Basic Sword & Sorcery.");

BOOK("novel_mystery", "mystery novel",		25,  55,c_ltblue,	PAPER,	MNULL,
    4,  1, -2,  0,	NULL,	 0,  0,  4,  7, 18, "\
A detective investigates an unusual murder in a secluded location.");

BOOK("novel_horror", "horror novel",		18,  55,c_ltblue,	PAPER,	MNULL,
    4,  1, -2,  0,	NULL,	 0,  0,  1,  7, 18, "\
Maybe not the best reading material considering the situation.");

BOOK("manual_brawl", "101 Wrestling Moves",	30, 180,c_green,	PAPER,	MNULL,
    2,  1, -4,  0, 	"unarmed",	 3,  0,  0,  3, 15, "\
It seems to be a wrestling manual, poorly photocopied and released on spiral-\n\
bound paper. Still, there are lots of useful tips for unarmed combat.");

BOOK("manual_knives", "Spetsnaz Knife Techniques",12,200,c_green,	PAPER,	MNULL,
    1,  1, -5,  0,	"cutting",	 4,  1,  0,  4, 18, "\
A classic Soviet text on the art of attacking with a blade.");

//	NAME			RAR PRC	COLOR		MAT1	MAT2
BOOK("manual_mechanics", "Under the Hood",		35, 190,c_green,	PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    3,  1, -3,  0,	"mechanics",	 3,  0,  0,  5, 18, "\
An advanced mechanics manual, covering all sorts of topics.");

BOOK("manual_survival", "Pitching a Tent",20,200,c_green,  PAPER,  MNULL,
// VOL WGT DAM HIT      TYPE            LEV REQ FUN INT TIME
    3,  1,  -3, 0,      "survival",    3,   0,  0,  4,  18,"\
A guide detailing the basics of woodsmanship and outdoor survival.");

BOOK("manual_speech", "Self-Esteem for Dummies",	50, 160,c_green,	PAPER,	MNULL,
    3,  1, -3,  0,	"speech",	 3,  0,  0,  5, 20, "\
Full of useful tips for showing confidence in your speech.");

BOOK("manual_business", "How to Succeed in Business",40,180,c_green,	PAPER,	MNULL,
    3,  1, -3,  0,	"barter",	 3,  0, -1,  6, 25, "\
Useful if you want to get a good deal when purchasing goods.");

BOOK("manual_first_aid", "The Big Book of First Aid",40,200,c_green,	PAPER,	MNULL,
    5,  2, -2,  0,	"firstaid",	 3,  0,  0,  7, 20, "\
It's big and heavy, but full of great information about first aid.");

BOOK("manual_computers", "How to Browse the Web",	20, 170,c_green,	PAPER,	MNULL,
    3,  1, -3,  0,	"computer",	 2,  0,  0,  5, 15, "\
Very beginner-level information about computers.");

//	NAME			RAR PRC	COLOR		MAT1	MAT2
BOOK("cookbook", "Cooking on a Budget",	35, 160,c_green,	PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    4,  1, -2,  0,	"cooking",	 3,  0,  0,  4, 10, "\
A nice cook book that goes beyond recipes and into the chemistry of food.");

BOOK("cookbook_human", "To Serve Man", 1, 400, c_green, PAPER, MNULL,
    4, 1, -2, 0, "cooking", 4, 2, -5, 4, 10, "\
It's... it's a cookbook!");

BOOK("cookbook_italian", "Cucina Italiana", 25, 200, c_green, PAPER, MNULL,
    4, 1, -2, 0, "cooking", 4, 3, 0, 6, 10, "\
This cookbook is written in Italian, but handily illustrated with\n\
step by step photo instructions.");

BOOK("manual_electronics", "What's a Transistor?",	20, 200,c_green,	PAPER,	MNULL,
    3,  1, -3,  0,	"electronics",	 3,  0,  0,  7, 20, "\
A basic manual of electronics and circuit design.");

BOOK("manual_tailor", "Sew What?  Clothing!",	15, 190,c_green,	PAPER,	MNULL,
    3,  1, -3,  0,	"tailor",	 3,  0,  0,  4, 18, "\
A colorful book about tailoring.");

BOOK("manual_traps", "How to Trap Anything",	12, 240,c_green,	PAPER,	MNULL,
    2,  1, -3,  0,	"traps",	 4,  0,  0,  4, 20, "\
A worn manual that describes how to set and disarm a wide variety of traps.");

//	NAME			RAR PRC	COLOR		MAT1	MAT2
BOOK("manual_carpentry", "Building for Beginners",  10, 220,c_green,	PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    2,  1, -3,  0,	"carpentry",	 3,  0,  0,  5, 16, "\
A large, paperback book detailing several beginner's projects in\n\
construction.");

BOOK("textbook_computers", "Computer Science 301",	 8, 500,c_blue,		PAPER,	MNULL,
    7,  4,  5,  1,	"computer",	 5,  2, -2, 11, 35, "\
A college textbook on computer science.");

BOOK("textbook_electronics", "Advanced Electronics",	 6, 520,c_blue,		PAPER,	MNULL,
    7,  5,  5,  1,	"electronics",	 5,  2, -1, 11, 35, "\
A college textbook on circuit design.");

BOOK("textbook_business", "Advanced Economics",	12, 480,c_blue,		PAPER,	MNULL,
    7,  4,  5,  1,	"barter",	 5,  3, -1,  9, 30, "\
A college textbook on economics.");

BOOK("textbook_mechanics", "Mechanical Mastery",12,495,c_blue,PAPER,MNULL,
    6,  3,  4,  1,      "mechanics",   6,   3, -1,  6,  30,"\
An advanced guide on mechanics and welding, covering topics like\n\
\"Grinding off rust\" and \"Making cursive E\'s\".");

//	NAME			RAR PRC	COLOR		MAT1	MAT2
BOOK("textbook_chemistry", "Chemistry Textbook",	11, 495,c_blue,		PAPER,	MNULL,
// VOL WGT DAM HIT	TYPE		LEV REQ FUN INT TIME
    8,  6,  5,  1,	"cooking",	 6,  3, -1, 12, 35, "\
A college textbook on chemistry.");

BOOK("textbook_carpentry", "Engineering 301",		 6, 550,c_blue,		PAPER,	MNULL,
    6,  3,  4,  1,	"carpentry",	 6,  3, -1,  8, 30, "\
A textbook on civil engineering and construction.");

BOOK("SICP", "SICP",			 3, 780,c_blue,		PAPER,	MNULL,
    6,  5,  6,  0,	"computer",	 8,  4, -1, 13, 50, "\
A classic text, \"The Structure and Interpretation of Computer Programs.\"\n\
Written with examples in LISP, but applicable to any language.");

BOOK("textbook_robots", "Robots for Fun & Profit",  1, 920,c_blue,		PAPER,	MNULL,
    8,  8,  8,  1,	"electronics",	10,  5, -1, 14, 55, "\
A rare book on the design of robots, with lots of helpful step-by-step guides."
);

CONT("bag_plastic", "plastic bag",	50,  1,	c_ltgray,	PLASTIC,MNULL,
// VOL WGT DAM HIT	VOL	FLAGS
    1,  0, -8, -4,	24,	0, "\
A small, open plastic bag. Essentially trash.");

CONT("bottle_plastic", "plastic bottle",	70,  8,	c_ltcyan,	PLASTIC,MNULL,
    2,  0, -8,  1,	 2,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A resealable plastic bottle, holds 500mls of liquid.");

CONT("bottle_glass", "glass bottle",	70, 12,	c_cyan,		GLASS,	MNULL,
    3,  2,  8,  1,	 3,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A resealable glass bottle, holds 750mls of liquid.");

CONT("can_drink", "aluminum can",	70,  1,	c_ltblue,	STEEL,	MNULL,
    1,  0,  0,  0,	 1,	mfb(con_rigid)|mfb(con_wtight), "\
An aluminum can, like what soda comes in.");

CONT("can_food", "tin can",		65,  2,	c_blue,		IRON,	MNULL,
    1,  0, -1,  1,	 1,	mfb(con_rigid)|mfb(con_wtight), "\
A tin can, like what beans come in.");

CONT("box_small", "sm. cardboard box",50, 0,	c_brown,	PAPER,	MNULL,
    4,  0, -5,  1,	 4,	mfb(con_rigid), "\
A small cardboard box. No bigger than a foot in any dimension.");

CONT("canteen", "plastic canteen",	20,  1000,	c_green,	PLASTIC,MNULL,
    6,  2, -8,  1,	 6,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A large military-style water canteen, with a 1.5 liter capacity and strap.");

CONT("jerrycan", "plastic jerrycan",	10,  2500,	c_green,	PLASTIC,MNULL,
    40,  4, -2,  -2,	 40,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A bulky plastic jerrycan, meant to carry fuel, but can carry other liquids\n\
in a pinch. It has a capacity of 10 liters.");

CONT("jug_plastic", "gallon jug",	10,  2500,	c_ltcyan,	PLASTIC,MNULL,
    10,  2, -8,  1,	 10,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A standard plastic jug used for household cleaning chemicals.");

CONT("flask_glass", "glass flask",	10,  2500,	c_ltcyan,	GLASS,MNULL,
    1,  0, 8,  1,	 1,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A 250 ml laboratory conical flask, with a rubber bung.");

CONT("waterskin", "waterskin",   0,  0, c_brown, LEATHER, MNULL,
// VOL WGT DAM HIT	VOL	FLAGS
    6, 4,  -8, -5,   6, mfb(con_wtight)|mfb(con_seals), "\
A watertight leather bag, can hold 1.5 liters of water.");

CONT("jerrycan_big", "steel jerrycan", 20, 5000, c_green, STEEL, MNULL,
    100, 7, -3, -3, 100, mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A steel jerrycan, meant to carry fuel, but can carry other liquds\n\
in a pinch. It has a capacity of 25 liters.");

CONT("keg", "aluminum keg", 20, 6000, c_ltcyan, STEEL, MNULL,
    200, 12, -4, -4, 200, mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A reusable aluminum keg, used for shipping beer.\n\
It has a capcity of 50 liters.");

CONT("jar_glass", "glass jar",	50,  2500,	c_ltcyan,	GLASS,MNULL,
    1,  1, 8,  1,	 1,	mfb(con_rigid)|mfb(con_wtight)|mfb(con_seals),"\
A half-litre glass jar with a metal screw top lid, used for canning.");

TOOL("jack", "jack",		30, 86, ';', c_ltgray,	IRON,	MNULL,
//	VOL WGT DAM CUT HIT FLAGS
	 5,  10, 11,  0,  2, 0, 0, 0, 0, AT_NULL, "null", &iuse::none, 0, "\
A common hydraulic jack, used when changing tires.");

// BIONICS
// These are the modules used to install new bionics in the player.  They're
// very simple and straightforward; a difficulty, followed by a NULL-terminated
// list of options.
#define BIO(id, name, rarity, price, color, difficulty, des) \
itypes[id]=new it_bionic(id, rarity,price,name,des,':',\
color, STEEL, PLASTIC, 10, 18, 8, 0, 0, 0, difficulty)

#define BIO_SINGLE(id,rarity,price,color,difficulty) \
     BIO(id, std::string("CBM: ")+bionics[id]->name, rarity,price,color,difficulty, \
           word_rewrap(bionics[id]->description, 50)) \

//  Name                     RAR PRICE    COLOR   DIFFICULTY
BIO("bio_power_storage", "CBM: Power Storage",	24, 3800,	c_green,	 1, "\
Compact Bionics Module that upgrades your power capacity by 4 units. Having\n\
at least one of these is a prerequisite to using powered bionics. You will\n\
also need a power supply, found in another CBM."); // This is a special case, which increases power capacity by 4

 BIO("bio_power_storage_mkII", "CBM: Power Storage Mk. II", 8, 10000, c_green, 1, "\
Compact Bionics Module developed at DoubleTech Industries as a replacement\n\
for the highly sucessful CBM: Power Storage. Increases you power capacity\n\
by 10 units."); // This is another special case, increases power capacity by 10 units

// SOFTWARE
#define SOFTWARE(id, name, price, swtype, power, description) \
itypes[id]=new it_software(id, 0, price, name, description,\
	' ', c_white, MNULL, MNULL, 0, 0, 0, 0, 0, 0, swtype, power)

//Macguffins
#define MACGUFFIN(id, name, price, sym, color, mat1, mat2, volume, wgt, dam, cut,\
                  to_hit, readable, function, description) \
itypes[id]=new it_macguffin(id, 0, price, name, description,\
	sym, color, mat1, mat2, volume, wgt, dam, cut, to_hit, 0, readable,\
	function)

// BIONIC IMPLANTS
// Sometimes a bionic needs to set you up with a dummy weapon, or something
// similar.  For the sake of clarity, no matter what the type of item, place
// them all here.

// power sources
BIO_SINGLE("bio_solar", 2, 3500, c_yellow, 4);
BIO_SINGLE("bio_batteries", 5, 800, c_yellow, 4);
BIO_SINGLE("bio_metabolics", 4, 700, c_yellow, 4);
BIO_SINGLE("bio_furnace", 2, 4500, c_yellow, 4);
BIO_SINGLE("bio_ethanol", 6, 1200, c_yellow, 4);
BIO_SINGLE("bio_torsionratchet", 2, 3800, c_yellow, 4);
// utilities
BIO_SINGLE("bio_tools", 3, 8000, c_ltgray, 6);
BIO_SINGLE("bio_storage", 3, 4000, c_ltgray, 7);
BIO_SINGLE("bio_flashlight", 8, 200, c_ltgray, 2);
BIO_SINGLE("bio_lighter", 6, 1300, c_ltgray, 4);
BIO_SINGLE("bio_magnet", 5, 2000, c_ltgray, 2);
// neurological
BIO_SINGLE("bio_memory", 2, 10000, c_pink, 9);
BIO_SINGLE("bio_painkiller", 4, 2000, c_pink, 4);
BIO_SINGLE("bio_alarm", 7, 250, c_pink, 1);
// sensory
BIO_SINGLE("bio_ears", 2, 5000, c_ltblue, 6);
BIO_SINGLE("bio_eye_enhancer", 2, 8000, c_ltblue, 11);
BIO_SINGLE("bio_night_vision", 2, 9000, c_ltblue, 11);
BIO_SINGLE("bio_infrared", 4, 4500, c_ltblue, 6);
BIO_SINGLE("bio_scent_vision", 4, 4500, c_ltblue, 8);
// aquatic
BIO_SINGLE("bio_membrane", 3, 4500, c_blue, 6);
BIO_SINGLE("bio_gills", 3, 4500, c_blue, 6);
// combat augs
BIO_SINGLE("bio_targeting", 2, 6500, c_red, 5);
BIO_SINGLE("bio_ground_sonar", 3, 4500, c_red, 5);
// hazmat
BIO_SINGLE("bio_purifier", 3, 4500, c_ltgreen, 4);
BIO_SINGLE("bio_climate", 4, 3500, c_ltgreen, 3);
BIO_SINGLE("bio_heatsink", 4, 3500, c_ltgreen, 3);
BIO_SINGLE("bio_blood_filter", 4, 3500, c_ltgreen, 3);
// nutritional
BIO_SINGLE("bio_recycler", 2, 8500, c_green, 6);
BIO_SINGLE("bio_digestion", 3, 5500, c_green, 6);
BIO_SINGLE("bio_evap", 3, 5500, c_green, 4);
BIO_SINGLE("bio_water_extractor", 3, 5500, c_green, 5);
// was: desert survival (all dupes)
// melee:
BIO_SINGLE("bio_shock", 2, 5500, c_red, 5);
BIO_SINGLE("bio_heat_absorb", 2, 5500, c_red, 5);
BIO_SINGLE("bio_claws", 2, 5500, c_red, 5);
// armor:
BIO_SINGLE("bio_carbon", 3, 7500, c_cyan, 9);
BIO_SINGLE("bio_armor_head", 3, 3500, c_cyan, 5);
BIO_SINGLE("bio_armor_torso", 3, 3500, c_cyan, 4);
BIO_SINGLE("bio_armor_arms", 3, 3500, c_cyan, 3);
BIO_SINGLE("bio_armor_legs", 3, 3500, c_cyan, 3);
// espionage
BIO_SINGLE("bio_face_mask", 1, 8500, c_magenta, 5);
BIO_SINGLE("bio_scent_mask", 1, 8500, c_magenta, 5);
BIO_SINGLE("bio_cloak", 1, 8500, c_magenta, 5);
BIO_SINGLE("bio_fingerhack", 1, 3500, c_magenta, 2);
// defensive
BIO_SINGLE("bio_ads", 1, 9500, c_ltblue, 7);
BIO_SINGLE("bio_ods", 1, 9500, c_ltblue, 7);
// medical
BIO_SINGLE("bio_nanobots", 3, 9500, c_ltred, 6);
BIO_SINGLE("bio_blood_anal", 3, 3200, c_ltred, 2);
// construction
BIO_SINGLE("bio_resonator", 2, 12000, c_dkgray, 11);
BIO_SINGLE("bio_hydraulics", 3, 4000, c_dkgray, 6);
// super soldier
BIO_SINGLE("bio_time_freeze", 1, 14000, c_white, 11);
BIO_SINGLE("bio_teleport", 1, 7000, c_white, 7);
// ranged combat
BIO_SINGLE("bio_blaster", 13, 2200, c_red, 3);
BIO_SINGLE("bio_laser", 2, 7200, c_red, 5);
BIO_SINGLE("bio_emp", 2, 7200, c_red, 5);
// power armor
BIO_SINGLE("bio_power_armor_interface", 20, 1200, c_yellow, 1);

SOFTWARE("software_useless", "misc software", 300, SW_USELESS, 0, "\
A miscellaneous piece of hobby software. Probably useless.");

SOFTWARE("software_hacking", "hackPRO", 800, SW_HACKING, 2, "\
A piece of hacking software.");

SOFTWARE("software_medical", "MediSoft", 600, SW_MEDICAL, 2, "\
A piece of medical software.");

SOFTWARE("software_math", "MatheMAX", 500, SW_SCIENCE, 3, "\
A piece of mathematical software.");

SOFTWARE("software_blood_data", "infection data", 200, SW_DATA, 5, "\
Medical data on zombie blood.");

MACGUFFIN("note", "note", 0, '?', c_white, PAPER, MNULL, 1, 0, 0, 0, 0,
	true, &iuse::mcg_note, "\
A hand-written paper note.");

MELEE("poppy_flower", "poppy flower",   1, 400,',', c_magenta,	VEGGY,	MNULL,
	 1,  0, -8,  0, -3, 0, "\
A poppy stalk with some petals.");

MELEE("poppy_bud", "a poppy bud",   1, 400,',', c_magenta,	VEGGY,	MNULL,
	 1,  0, -8,  0, -3, 0, "\
Contains some substances commonly produced by mutated poppy flower");

#define STATIONARY(id, name, rarity, price, category, description) \
itypes[id] = new it_stationary(id, rarity, price, name, description,\
',', c_white, PAPER, MNULL, 0, 0, 0, 0, 0, 0, category)

STATIONARY("flyer", "flyer", 5, 1, "flier", "A scrap of paper.");

// Finally, add all the keys from the map to a vector of all possible items
for(std::map<std::string,itype*>::iterator iter = itypes.begin(); iter != itypes.end(); ++iter){
    if(iter->first == "null" || iter->first == "corpse" || iter->first == "toolset" || iter->first == "fire" || iter->first == "apparatus"){
        pseudo_itype_ids.push_back(iter->first);
    } else {
        standard_itype_ids.push_back(iter->first);
    }
}

//    NAME		RARE SYM COLOR		MAT1	MAT2
MELEE("bio_claws_weapon", "adamantite claws",0,0,'{', c_pink,	STEEL,	MNULL,
//	VOL WGT DAM CUT HIT
	 2,  0,  8, 16,  4,
 mfb(IF_STAB)|mfb(IF_UNARMED_WEAPON)|mfb(IF_NO_UNWIELD), "\
Short and sharp claws made from a high-tech metal.");

//  NAME		RARE  TYPE	COLOR		MAT
AMMO("bio_fusion_ammo", "Fusion blast",	 0,0, AT_FUSION,c_dkgray,	MNULL,
//	VOL WGT DMG  AP RNG ACC REC COUNT
	 0,  0, 40,  0, 10,  1,  0,  5, "", mfb(AMMO_INCENDIARY));

//  NAME		RARE	COLOR		MAT1	MAT2
GUN("bio_blaster_gun", "fusion blaster",	 0,0,c_magenta,	STEEL,	PLASTIC,
//	SKILL		AMMO	   VOL WGT MDG HIT DMG RNG ACC REC DUR BST CLIP REL
	"rifle",	AT_FUSION, 12,  0,  0,  0,  0,  0,  4,  0, 10,  0,  1, 500,
"",0);







// Unarmed Styles
#define STYLE(id, name, dam, description, ...) \
itypes[id]=new it_style(id, 0, 0, name, description, '$', \
                              c_white, MNULL, MNULL, 0, 0, dam, 0, 0, 0); \
 setvector(&((static_cast<it_style*>(itypes[id])))->moves, __VA_ARGS__, NULL); \
itypes[id]->item_flags |= mfb(IF_UNARMED_WEAPON); \
martial_arts_itype_ids.push_back(id)

STYLE("style_karate", "karate", 2, "\
Karate is a popular martial art, originating from Japan. It focuses on\n\
rapid, precise attacks, blocks, and fluid movement. A successful hit allows\n\
you an extra dodge and two extra blocks on the following round.",

"quickly punch", TEC_RAPID, 0,
"block", TEC_BLOCK, 2,
"karate chop", TEC_PRECISE, 4
);

STYLE("style_aikido", "aikido", 0, "\
Aikido is a Japanese martial art focused on self-defense, while minimizing\n\
injury to the attacker. It uses defense throws and disarms. Damage done\n\
while using this technique is halved, but pain inflicted is doubled.",

"feint at", TEC_FEINT, 2,
"throw", TEC_DEF_THROW, 2,
"disarm", TEC_DISARM, 3,
"disarm", TEC_DEF_DISARM, 4
);

STYLE("style_judo", "judo", 0, "\
Judo is a martial art that focuses on grabs and throws, both defensive and\n\
offensive. It also focuses on recovering from throws; while using judo, you\n\
will not lose any turns to being thrown or knocked down.",

"grab", TEC_GRAB, 2,
"throw", TEC_THROW, 3,
"throw", TEC_DEF_THROW, 4
);

STYLE("style_tai_chi", "tai chi", 0, "\
Though tai chi is often seen as a form of mental and physical exercise, it is\n\
a legitimate martial art, focused on self-defense. Its ability to absorb the\n\
force of an attack makes your Perception decrease damage further on a block.",

"block", TEC_BLOCK, 1,
"disarm", TEC_DEF_DISARM, 3,
"strike", TEC_PRECISE, 4
);

STYLE("style_capoeira", "capoeira", 1, "\
A dance-like style with its roots in Brazilian slavery, capoeira is focused\n\
on fluid movement and sweeping kicks. Moving a tile will boost attack and\n\
dodge; attacking boosts dodge, and dodging boosts attack.",

"bluff", TEC_FEINT, 1,
"low kick", TEC_SWEEP, 3,
"spin and hit", TEC_COUNTER, 4,
"spin-kick", TEC_WIDE, 5
);

STYLE("style_krav_maga", "krav maga", 4, "\
Originating in Israel, Krav Maga is based on taking down an enemy quickly and\n\
effectively. It focuses on applicable attacks rather than showy or complex\n\
moves. Popular among police and armed forces everywhere.",

"quickly punch", TEC_RAPID, 2,
"block", TEC_BLOCK, 2,
"feint at", TEC_FEINT, 3,
"jab", TEC_PRECISE, 3,
"disarm", TEC_DISARM, 3,
"block", TEC_BLOCK_LEGS, 4,
"counter-attack", TEC_COUNTER, 4,
"disarm", TEC_DEF_DISARM, 4,
"", TEC_BREAK, 4,
"grab", TEC_GRAB, 5
);

STYLE("style_muay_thai", "muay thai", 4, "\
Also referred to as the \"Art of 8 Limbs,\" Muay Thai is a popular fighting\n\
technique from Thailand that uses powerful strikes. It does extra damage\n\
against large or strong opponents.",

"slap", TEC_RAPID, 2,
"block", TEC_BLOCK, 3,
"block", TEC_BLOCK_LEGS, 4,
"power-kick", TEC_BRUTAL, 4,
"counter-attack", TEC_COUNTER, 5
);

STYLE("style_ninjutsu", "ninjutsu", 1, "\
Ninjutsu is a martial art and set of tactics used by ninja in feudal Japan.\n\
It focuses on rapid, precise, silent strikes. Ninjutsu is entirely silent.\n\
It also provides small combat bonuses the turn after moving a tile.",

"quickly punch", TEC_RAPID, 3,
"jab", TEC_PRECISE, 4,
"block", TEC_BLOCK, 4
);

STYLE("style_taekwondo", "taekwondo", 2, "\
Taekwondo is the national sport of Korea, and was used by the South Korean\n\
army in the 20th century. Focused on kicks and punches, it also includes\n\
strength training; your blocks absorb extra damage the stronger you are.",

"block", TEC_BLOCK, 2,
"block", TEC_BLOCK_LEGS, 3,
"jab", TEC_PRECISE, 4,
"brutally kick", TEC_BRUTAL, 4,
"spin-kick", TEC_SWEEP, 5
);

STYLE("style_tiger", "tiger style", 4, "\
One of the five Shaolin animal styles. Tiger style focuses on relentless\n\
attacks above all else. Strength, not Dexterity, is used to determine hits;\n\
you also receive an accumulating bonus for several turns of sustained attack.",

"grab", TEC_GRAB, 4
);

STYLE("style_crane", "crane style", 0, "\
One of the five Shaolin animal styles. Crane style uses intricate hand\n\
techniques and jumping dodges. Dexterity, not Strength, is used to determine\n\
damage; you also receive a dodge bonus the turn after moving a tile.",

"feint at", TEC_FEINT, 2,
"block", TEC_BLOCK, 3,
"", TEC_BREAK, 3,
"hand-peck", TEC_PRECISE, 4
);

STYLE("style_leopard", "leopard style", 3, "\
One of the five Shaolin animal styles. Leopard style focuses on rapid,\n\
strategic strikes. Your Perception and Intelligence boost your accuracy, and\n\
moving a single tile provides an increased boost for one turn.",

"swiftly jab", TEC_RAPID, 2,
"counter-attack", TEC_COUNTER, 4,
"leopard fist", TEC_PRECISE, 5
);

STYLE("style_snake", "snake style", 1, "\
One of the five Shaolin animal styles. Snake style uses sinuous movement and\n\
precision strikes. Perception increases your chance to hit as well as the\n\
damage you deal.",

"swiftly jab", TEC_RAPID, 2,
"feint at", TEC_FEINT, 3,
"snakebite", TEC_PRECISE, 4,
"writhe free from", TEC_BREAK, 4
);

STYLE("style_dragon", "dragon style", 2, "\
One of the five Shaolin animal styles. Dragon style uses fluid movements and\n\
hard strikes. Intelligence increases your chance to hit as well as the\n\
damage you deal. Moving a tile will boost damage further for one turn.",

"", TEC_BLOCK, 2,
"grab", TEC_GRAB, 4,
"counter-attack", TEC_COUNTER, 4,
"spin-kick", TEC_SWEEP, 5,
"dragon strike", TEC_BRUTAL, 6
);

STYLE("style_centipede", "centipede style", 0, "\
One of the Five Deadly Venoms. Centipede style uses an onslaught of rapid\n\
strikes. Every strike you make reduces the movement cost of attacking by 4;\n\
this is cumulative, but is reset entirely if you are hit even once.",

"swiftly hit", TEC_RAPID, 2,
"block", TEC_BLOCK, 3
);

STYLE("style_venom_snake", "viper style", 2, "\
One of the Five Deadly Venoms. Viper Style has a unique three-hit combo; if\n\
you score a critical hit, it is initiated. The second hit uses a coned hand\n\
to deal piercing damage, and the 3rd uses both hands in a devastating strike.",

"", TEC_RAPID, 3,
"feint at", TEC_FEINT, 3,
"writhe free from", TEC_BREAK, 4
);

STYLE("style_scorpion", "scorpion style", 3, "\
One of the Five Deadly Venoms. Scorpion Style is a mysterious art that focuses\n\
on utilizing pincer-like fists and a stinger-like kick. Critical hits will do\n\
massive damage, knocking your target far back.",

"block", TEC_BLOCK, 3,
"pincer fist", TEC_PRECISE, 4
);

STYLE("style_lizard", "lizard style", 1, "\
One of the Five Deadly Venoms. Lizard Style focuses on using walls to one's\n\
advantage. Moving alongside a wall will make you run up along it, giving you\n\
a large to-hit bonus. Standing by a wall allows you to use it to boost dodge.",

"block", TEC_BLOCK, 2,
"counter-attack", TEC_COUNTER, 4
);

STYLE("style_toad", "toad style", 0, "\
One of the Five Deadly Venoms. Immensely powerful, and immune to nearly any\n\
weapon. You may meditate by pausing for a turn; this will give you temporary\n\
armor, proportional to your Intelligence and Perception.",

"block", TEC_BLOCK, 3,
"grab", TEC_GRAB, 4
);

STYLE("style_zui_quan", "zui quan", 1, "\
Also known as \"drunken boxing,\" Zui Quan imitates the movement of a drunk\n\
to confuse the enemy. The turn after you attack, you may dodge any number of\n\
attacks with no penalty.",

"stumble and leer at", TEC_FEINT, 3,
"counter-attack", TEC_COUNTER, 4
);

}

std::string ammo_name(ammotype t)
{
 switch (t) {
  case AT_NAIL:   return "nails";
  case AT_BB:	  return "BBs";
  case AT_BOLT:	  return "bolts";
  case AT_ARROW:  return "arrows";
  case AT_PEBBLE: return "pebbles";
  case AT_SHOT:	  return "shot";
  case AT_22:	  return ".22";
  case AT_9MM:	  return "9mm";
  case AT_762x25: return "7.62x25mm";
  case AT_38:	  return ".38";
  case AT_40:	  return ".40";
  case AT_44:	  return ".44";
  case AT_45:	  return ".45";
  case AT_57:	  return "5.7mm";
  case AT_46:	  return "4.6mm";
  case AT_762:	  return "7.62x39mm";
  case AT_223:	  return ".223";
  case AT_3006:   return ".30-06";
  case AT_308:	  return ".308";
  case AT_40MM:   return "40mm grenade";
  case AT_66MM:   return "High Explosive Anti Tank Warhead";
  case AT_GAS:	  return "gasoline";
  case AT_THREAD: return "thread";
  case AT_BATT:   return "batteries";
  case AT_PLUT:   return "plutonium";
  case AT_MUSCLE: return "Muscle";
  case AT_FUSION: return "fusion cell";
  case AT_12MM:   return "12mm slugs";
  case AT_PLASMA: return "hydrogen";
  case AT_WATER: return "clean water";
  default:	  return "XXX";
 }
}

itype_id default_ammo(ammotype guntype)
{
 switch (guntype) {
 case AT_NAIL:	return "nail";
 case AT_BB:	return "bb";
 case AT_BOLT:	return "bolt_wood";
 case AT_ARROW: return "arrow_wood";
 case AT_PEBBLE:return "pebble";
 case AT_SHOT:	return "shot_00";
 case AT_22:	return "22_lr";
 case AT_9MM:	return "9mm";
 case AT_762x25:return "762_25";
 case AT_38:	return "38_special";
 case AT_40:	return "10mm";
 case AT_44:	return "44magnum";
 case AT_45:	return "45_acp";
 case AT_57:	return "57mm";
 case AT_46:	return "46mm";
 case AT_762:	return "762_m43";
 case AT_223:	return "223";
 case AT_308:	return "308";
 case AT_3006:	return "270";
 case AT_40MM:  return "40mm_concussive";
 case AT_66MM:  return "66mm_HEAT";
 case AT_BATT:	return "battery";
 case AT_FUSION:return "laser_pack";
 case AT_12MM:  return "12mm";
 case AT_PLASMA:return "plasma";
 case AT_PLUT:	return "plut_cell";
 case AT_GAS:	return "gasoline";
 case AT_THREAD:return "thread";
 case AT_WATER:return "water_clean";
 }
 return "null";
}

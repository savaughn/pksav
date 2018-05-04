/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "common/text_common.h"

#include <pksav/gen4/text.h>

#include <assert.h>
#include <string.h>
#include <wchar.h>

#define PKSAV_GEN4_TERMINATOR 0xFFFF

/*
 * Character map for Generation IV
 *
 * Source: https://github.com/Chase-san/PokeLib/blob/master/lib/PokeText.cpp
 */
static const wchar_t PKSAV_GEN4_CHAR_MAP1[] = // 0x0000,
{
    0x0000,0x3000,0x3041,0x3042,0x3043,0x3044,0x3045,0x3046,0x3047,0x3048,0x3049,0x304A,0x304B,0x304C,0x304D,0x304E,
    0x304F,0x3050,0x3051,0x3052,0x3053,0x3054,0x3055,0x3056,0x3057,0x3058,0x3059,0x305A,0x305B,0x305C,0x305D,0x305E,
    0x305F,0x3060,0x3061,0x3062,0x3063,0x3064,0x3065,0x3066,0x3067,0x3068,0x3069,0x306A,0x306B,0x306C,0x306D,0x306E,
    0x306F,0x3070,0x3071,0x3072,0x3073,0x3074,0x3075,0x3076,0x3077,0x3078,0x3079,0x307A,0x307B,0x307C,0x307D,0x307E,
    0x307F,0x3080,0x3081,0x3082,0x3083,0x3084,0x3085,0x3086,0x3087,0x3088,0x3089,0x308A,0x308B,0x308C,0x308D,0x308F,
    0x3092,0x3093,0x30A1,0x30A2,0x30A3,0x30A4,0x30A5,0x30A6,0x30A7,0x30A8,0x30A9,0x30AA,0x30AB,0x30AC,0x30AD,0x30AE,
    0x30AF,0x30B0,0x30B1,0x30B2,0x30B3,0x30B4,0x30B5,0x30B6,0x30B7,0x30B8,0x30B9,0x30BA,0x30BB,0x30BC,0x30BD,0x30BE,
    0x30BF,0x30C0,0x30C1,0x30C2,0x30C3,0x30C4,0x30C5,0x30C6,0x30C7,0x30C8,0x30C9,0x30CA,0x30CB,0x30CC,0x30CD,0x30CE,
    0x30CF,0x30D0,0x30D1,0x30D2,0x30D3,0x30D4,0x30D5,0x30D6,0x30D7,0x30D8,0x30D9,0x30DA,0x30DB,0x30DC,0x30DD,0x30DE,
    0x30DF,0x30E0,0x30E1,0x30E2,0x30E3,0x30E4,0x30E5,0x30E6,0x30E7,0x30E8,0x30E9,0x30EA,0x30EB,0x30EC,0x30ED,0x30EF,
    0x30F2,0x30F3,0xFF10,0xFF11,0xFF12,0xFF13,0xFF14,0xFF15,0xFF16,0xFF17,0xFF18,0xFF19,0xFF21,0xFF22,0xFF23,0xFF24,
    0xFF25,0xFF26,0xFF27,0xFF28,0xFF29,0xFF2A,0xFF2B,0xFF2C,0xFF2D,0xFF2E,0xFF2F,0xFF30,0xFF31,0xFF32,0xFF33,0xFF34,
    0xFF35,0xFF36,0xFF37,0xFF38,0xFF39,0xFF3A,0xFF41,0xFF42,0xFF43,0xFF44,0xFF45,0xFF46,0xFF47,0xFF48,0xFF49,0xFF4A,
    0xFF4B,0xFF4C,0xFF4D,0xFF4E,0xFF4F,0xFF50,0xFF51,0xFF52,0xFF53,0xFF54,0xFF55,0xFF56,0xFF57,0xFF58,0xFF59,0xFF5A,
    0x0000, // Missing ###
    0xFF01,0xFF1F,0x3001,0x3002,0x22EF,0x30FB,0xFF0F,0x300C,0x300D,0x300E,0x300F,0xFF08,0xFF09,0x329A,0x329B,0xFF0B,
    0xFF0D,0x2297,0x2298,0xFF1D,0xFF5A,0xFF1A,0xFF1B,0xFF0E,0xFF0C,0x2664,0x2667,0x2661,0x2662,0x2606,0x25CE,0x25CB,
    0x25A1,0x25B3,0x25C7,0xFF20,0x266B,0xFF05,0x263C,0x2614,0x2630,0x2744,0x260B,0x2654,0x2655,0x260A,0x2934,0x2935,
    0x263E,0x00A5,0x2648,0x2649,0x264A,0x264B,0x264C,0x264D,0x264E,0x264F,0x2190,0x2191,0x2193,0x2192,0x2023,0xFF06,
    0x0030,0x0031,0x0032,0x0033,0x0034,0x0035,0x0036,0x0037,0x0038,0x0039,0x0041,0x0042,0x0043,0x0044,0x0045,0x0046,
    0x0047,0x0048,0x0049,0x004A,0x004B,0x004C,0x004D,0x004E,0x004F,0x0050,0x0051,0x0052,0x0053,0x0054,0x0055,0x0056,
    0x0057,0x0058,0x0059,0x005A,0x0061,0x0062,0x0063,0x0064,0x0065,0x0066,0x0067,0x0068,0x0069,0x006A,0x006B,0x006C,
    0x006D,0x006E,0x006F,0x0070,0x0071,0x0072,0x0073,0x0074,0x0075,0x0076,0x0077,0x0078,0x0079,0x007A,0x00C0,0x00C1,
    0x00C2,0x00C3,0x00C4,0x00C5,0x00C6,0x00C7,0x00C8,0x00C9,0x00CA,0x00CB,0x00CC,0x00CD,0x00CE,0x00CF,0x00D0,0x00D1,
    0x00D2,0x00D3,0x00D4,0x00D5,0x00D6,0x00D7,0x00D8,0x00D9,0x00DA,0x00DB,0x00DC,0x00DD,0x00DE,0x00DF,0x00E0,0x00E1,
    0x00E2,0x00E3,0x00E4,0x00E5,0x00E6,0x00E7,0x00E8,0x00E9,0x00EA,0x00EB,0x00EC,0x00ED,0x00EE,0x00EF,0x00F0,0x00F1,
    0x00F2,0x00F3,0x00F4,0x00F5,0x00F6,0x00F7,0x00F8,0x00F9,0x00FA,0x00FB,0x00FC,0x00FD,0x00FE,0x00FF,0x0152,0x0153,
    0x015E,0x015F,0x00AA,0x00BA,0x00B9,0x00B2,0x00B3,0x0024,0x00A1,0x00BF,0x0021,0x003F,0x002C,0x002E,0x2026,0x00B7,
    0x002F,0x2018,0x2019,0x201C,0x201D,0x201E,0x300A,0x300B,0x0028,0x0029,0x2642,0x2640,0x002B,0x002D,0x002A,0x0023,
    0x003D,0x0026,0x007E,0x003A,0x003B,0x2660,0x2663,0x2665,0x2666,0x2605,0x25C9,0x25CF,0x25A0,0x25B2,0x25C6,0x0040,
    0x266A,0x0025,0x2600,0x2601,0x2602,0x2603,0x263A,0x265A,0x265B,0x2639,0x2197,0x2198,0x263D,0x0020,0x2074,0x20A7,
    0x20A6,0x00B0,0x005F,0xFF3F,
};
static const size_t PKSAV_GEN4_CHAR_MAP1_SIZE = sizeof(PKSAV_GEN4_CHAR_MAP1)/sizeof(wchar_t);

static const wchar_t PKSAV_GEN4_CHAR_MAP2[] =
{
    0xAC00,0xAC01,0xAC04,0xAC07,0xAC08,0xAC09,0xAC0A,0xAC10,0xAC11,0xAC12,0xAC13,0xAC14,0xAC15,0xAC16,0xAC17,0xAC19,
    0xAC1A,0xAC1B,0xAC1C,0xAC1D,0xAC20,0xAC24,0xAC2C,0xAC2D,0xAC2F,0xAC30,0xAC31,0xAC38,0xAC39,0xAC3C,0xAC40,0xAC4B,
    0xAC4D,0xAC54,0xAC58,0xAC5C,0xAC70,0xAC71,0xAC74,0xAC77,0xAC78,0xAC7A,0xAC80,0xAC81,0xAC83,0xAC84,0xAC85,0xAC86,
    0xAC89,0xAC8A,0xAC8B,0xAC8C,0xAC90,0xAC94,0xAC9C,0xAC9D,0xAC9F,0xACA0,0xACA1,0xACA8,0xACA9,0xACAA,0xACAC,0xACAF,
    0xACB0,0xACB8,0xACB9,0xACBB,0xACBC,0xACBD,0xACC1,0xACC4,0xACC8,0xACCC,0xACD5,0xACD7,0xACE0,0xACE1,0xACE4,0xACE7,
    0xACE8,0xACEA,0xACEC,0xACEF,0xACF0,0xACF1,0xACF3,0xACF5,0xACF6,0xACFC,0xACFD,0xAD00,0xAD04,0xAD06,0xAD0C,0xAD0D,
    0xAD0F,0xAD11,0xAD18,0xAD1C,0xAD20,0xAD29,0xAD2C,0xAD2D,0xAD34,0xAD35,0xAD38,0xAD3C,0xAD44,0xAD45,0xAD47,0xAD49,
    0xAD50,0xAD54,0xAD58,0xAD61,0xAD63,0xAD6C,0xAD6D,0xAD70,0xAD73,0xAD74,0xAD75,0xAD76,0xAD7B,0xAD7C,0xAD7D,0xAD7F,
    0xAD81,0xAD82,0xAD88,0xAD89,0xAD8C,0xAD90,0xAD9C,0xAD9D,0xADA4,0xADB7,0xADC0,0xADC1,0xADC4,0xADC8,0xADD0,0xADD1,
    0xADD3,0xADDC,0xADE0,0xADE4,0xADF8,0xADF9,0xADFC,0xADFF,0xAE00,0xAE01,0xAE08,0xAE09,0xAE0B,0xAE0D,0xAE14,0xAE30,
    0xAE31,0xAE34,0xAE37,0xAE38,0xAE3A,0xAE40,0xAE41,0xAE43,0xAE45,0xAE46,0xAE4A,0xAE4C,0xAE4D,0xAE4E,0xAE50,0xAE54,
    0xAE56,0xAE5C,0xAE5D,0xAE5F,0xAE60,0xAE61,0xAE65,0xAE68,0xAE69,0xAE6C,0xAE70,0xAE78,0xAE79,0xAE7B,0xAE7C,0xAE7D,
    0xAE84,0xAE85,0xAE8C,0xAEBC,0xAEBD,0xAEBE,0xAEC0,0xAEC4,0xAECC,0xAECD,0xAECF,0xAED0,0xAED1,0xAED8,0xAED9,0xAEDC,
    0xAEE8,0xAEEB,0xAEED,0xAEF4,0xAEF8,0xAEFC,0xAF07,0xAF08,0xAF0D,0xAF10,0xAF2C,0xAF2D,0xAF30,0xAF32,0xAF34,0xAF3C,
    0xAF3D,0xAF3F,0xAF41,0xAF42,0xAF43,0xAF48,0xAF49,0xAF50,0xAF5C,0xAF5D,0xAF64,0xAF65,0xAF79,0xAF80,0xAF84,0xAF88,
    0xAF90,0xAF91,0xAF95,0xAF9C,0xAFB8,0xAFB9,0xAFBC,0xAFC0,0xAFC7,0xAFC8,0xAFC9,0xAFCB,0xAFCD,0xAFCE,0xAFD4,0xAFDC,
    0xAFE8,0xAFE9,0xAFF0,0xAFF1,0xAFF4,0xAFF8,0xB000,0xB001,0xB004,0xB00C,0xB010,0xB014,0xB01C,0xB01D,0xB028,0xB044,
    0xB045,0xB048,0xB04A,0xB04C,0xB04E,0xB053,0xB054,0xB055,0xB057,0xB059,0xB05D,0xB07C,0xB07D,0xB080,0xB084,0xB08C,
    0xB08D,0xB08F,0xB091,0xB098,0xB099,0xB09A,0xB09C,0xB09F,0xB0A0,0xB0A1,0xB0A2,0xB0A8,0xB0A9,0xB0AB,0xB0AC,0xB0AD,
    0xB0AE,0xB0AF,0xB0B1,0xB0B3,0xB0B4,0xB0B5,0xB0B8,0xB0BC,0xB0C4,0xB0C5,0xB0C7,0xB0C8,0xB0C9,0xB0D0,0xB0D1,0xB0D4,
    0xB0D8,0xB0E0,0xB0E5,0xB108,0xB109,0xB10B,0xB10C,0xB110,0xB112,0xB113,0xB118,0xB119,0xB11B,0xB11C,0xB11D,0xB123,
    0xB124,0xB125,0xB128,0xB12C,0xB134,0xB135,0xB137,0xB138,0xB139,0xB140,0xB141,0xB144,0xB148,0xB150,0xB151,0xB154,
    0xB155,0xB158,0xB15C,0xB160,0xB178,0xB179,0xB17C,0xB180,0xB182,0xB188,0xB189,0xB18B,0xB18D,0xB192,0xB193,0xB194,
    0xB198,0xB19C,0xB1A8,0xB1CC,0xB1D0,0xB1D4,0xB1DC,0xB1DD,0xB1DF,0xB1E8,0xB1E9,0xB1EC,0xB1F0,0xB1F9,0xB1FB,0xB1FD,
    0xB204,0xB205,0xB208,0xB20B,0xB20C,0xB214,0xB215,0xB217,0xB219,0xB220,0xB234,0xB23C,0xB258,0xB25C,0xB260,0xB268,
    0xB269,0xB274,0xB275,0xB27C,0xB284,0xB285,0xB289,0xB290,0xB291,0xB294,0xB298,0xB299,0xB29A,0xB2A0,0xB2A1,0xB2A3,
    0xB2A5,0xB2A6,0xB2AA,0xB2AC,0xB2B0,0xB2B4,0xB2C8,0xB2C9,0xB2CC,0xB2D0,0xB2D2,0xB2D8,0xB2D9,0xB2DB,0xB2DD,0xB2E2,
    0xB2E4,0xB2E5,0xB2E6,0xB2E8,0xB2EB,0xB2EC,0xB2ED,0xB2EE,0xB2EF,0xB2F3,0xB2F4,0xB2F5,0xB2F7,0xB2F8,0xB2F9,0xB2FA,
    0xB2FB,0xB2FF,0xB300,0xB301,0xB304,0xB308,0xB310,0xB311,0xB313,0xB314,0xB315,0xB31C,0xB354,0xB355,0xB356,0xB358,
    0xB35B,0xB35C,0xB35E,0xB35F,0xB364,0xB365,0xB367,0xB369,0xB36B,0xB36E,0xB370,0xB371,0xB374,0xB378,0xB380,0xB381,
    0xB383,0xB384,0xB385,0xB38C,0xB390,0xB394,0xB3A0,0xB3A1,0xB3A8,0xB3AC,0xB3C4,0xB3C5,0xB3C8,0xB3CB,0xB3CC,0xB3CE,
    0xB3D0,0xB3D4,0xB3D5,0xB3D7,0xB3D9,0xB3DB,0xB3DD,0xB3E0,0xB3E4,0xB3E8,0xB3FC,0xB410,0xB418,0xB41C,0xB420,0xB428,
    0xB429,0xB42B,0xB434,0xB450,0xB451,0xB454,0xB458,0xB460,0xB461,0xB463,0xB465,0xB46C,0xB480,0xB488,0xB49D,0xB4A4,
    0xB4A8,0xB4AC,0xB4B5,0xB4B7,0xB4B9,0xB4C0,0xB4C4,0xB4C8,0xB4D0,0xB4D5,0xB4DC,0xB4DD,0xB4E0,0xB4E3,0xB4E4,0xB4E6,
    0xB4EC,0xB4ED,0xB4EF,0xB4F1,0xB4F8,0xB514,0xB515,0xB518,0xB51B,0xB51C,0xB524,0xB525,0xB527,0xB528,0xB529,0xB52A,
    0xB530,0xB531,0xB534,0xB538,0xB540,0xB541,0xB543,0xB544,0xB545,0xB54B,0xB54C,0xB54D,0xB550,0xB554,0xB55C,0xB55D,
    0xB55F,0xB560,0xB561,0xB5A0,0xB5A1,0xB5A4,0xB5A8,0xB5AA,0xB5AB,0xB5B0,0xB5B1,0xB5B3,0xB5B4,0xB5B5,0xB5BB,0xB5BC,
    0xB5BD,0xB5C0,0xB5C4,0xB5CC,0xB5CD,0xB5CF,0xB5D0,0xB5D1,0xB5D8,0xB5EC,0xB610,0xB611,0xB614,0xB618,0xB625,0xB62C,
    0xB634,0xB648,0xB664,0xB668,0xB69C,0xB69D,0xB6A0,0xB6A4,0xB6AB,0xB6AC,0xB6B1,0xB6D4,0xB6F0,0xB6F4,0xB6F8,0xB700,
    0xB701,0xB705,0xB728,0xB729,0xB72C,0xB72F,0xB730,0xB738,0xB739,0xB73B,0xB744,0xB748,0xB74C,0xB754,0xB755,0xB760,
    0xB764,0xB768,0xB770,0xB771,0xB773,0xB775,0xB77C,0xB77D,0xB780,0xB784,0xB78C,0xB78D,0xB78F,0xB790,0xB791,0xB792,
    0xB796,0xB797,0xB798,0xB799,0xB79C,0xB7A0,0xB7A8,0xB7A9,0xB7AB,0xB7AC,0xB7AD,0xB7B4,0xB7B5,0xB7B8,0xB7C7,0xB7C9,
    0xB7EC,0xB7ED,0xB7F0,0xB7F4,0xB7FC,0xB7FD,0xB7FF,0xB800,0xB801,0xB807,0xB808,0xB809,0xB80C,0xB810,0xB818,0xB819,
    0xB81B,0xB81D,0xB824,0xB825,0xB828,0xB82C,0xB834,0xB835,0xB837,0xB838,0xB839,0xB840,0xB844,0xB851,0xB853,0xB85C,
    0xB85D,0xB860,0xB864,0xB86C,0xB86D,0xB86F,0xB871,0xB878,0xB87C,0xB88D,0xB8A8,0xB8B0,0xB8B4,0xB8B8,0xB8C0,0xB8C1,
    0xB8C3,0xB8C5,0xB8CC,0xB8D0,0xB8D4,0xB8DD,0xB8DF,0xB8E1,0xB8E8,0xB8E9,0xB8EC,0xB8F0,0xB8F8,0xB8F9,0xB8FB,0xB8FD,
    0xB904,0xB918,0xB920,0xB93C,0xB93D,0xB940,0xB944,0xB94C,0xB94F,0xB951,0xB958,0xB959,0xB95C,0xB960,0xB968,0xB969,
    0xB96B,0xB96D,0xB974,0xB975,0xB978,0xB97C,0xB984,0xB985,0xB987,0xB989,0xB98A,0xB98D,0xB98E,0xB9AC,0xB9AD,0xB9B0,
    0xB9B4,0xB9BC,0xB9BD,0xB9BF,0xB9C1,0xB9C8,0xB9C9,0xB9CC,0xB9CE,0xB9CF,0xB9D0,0xB9D1,0xB9D2,0xB9D8,0xB9D9,0xB9DB,
    0xB9DD,0xB9DE,0xB9E1,0xB9E3,0xB9E4,0xB9E5,0xB9E8,0xB9EC,0xB9F4,0xB9F5,0xB9F7,0xB9F8,0xB9F9,0xB9FA,0xBA00,0xBA01,
    0xBA08,0xBA15,0xBA38,0xBA39,0xBA3C,0xBA40,0xBA42,0xBA48,0xBA49,0xBA4B,0xBA4D,0xBA4E,0xBA53,0xBA54,0xBA55,0xBA58,
    0xBA5C,0xBA64,0xBA65,0xBA67,0xBA68,0xBA69,0xBA70,0xBA71,0xBA74,0xBA78,0xBA83,0xBA84,0xBA85,0xBA87,0xBA8C,0xBAA8,
    0xBAA9,0xBAAB,0xBAAC,0xBAB0,0xBAB2,0xBAB8,0xBAB9,0xBABB,0xBABD,0xBAC4,0xBAC8,0xBAD8,0xBAD9,0xBAFC,0xBB00,0xBB04,
    0xBB0D,0xBB0F,0xBB11,0xBB18,0xBB1C,0xBB20,0xBB29,0xBB2B,0xBB34,0xBB35,0xBB36,0xBB38,0xBB3B,0xBB3C,0xBB3D,0xBB3E,
    0xBB44,0xBB45,0xBB47,0xBB49,0xBB4D,0xBB4F,0xBB50,0xBB54,0xBB58,0xBB61,0xBB63,0xBB6C,0xBB88,0xBB8C,0xBB90,0xBBA4,
    0xBBA8,0xBBAC,0xBBB4,0xBBB7,0xBBC0,0xBBC4,0xBBC8,0xBBD0,0xBBD3,0xBBF8,0xBBF9,0xBBFC,0xBBFF,0xBC00,0xBC02,0xBC08,
    0xBC09,0xBC0B,0xBC0C,0xBC0D,0xBC0F,0xBC11,0xBC14,0xBC15,0xBC16,0xBC17,0xBC18,0xBC1B,0xBC1C,0xBC1D,0xBC1E,0xBC1F,
    0xBC24,0xBC25,0xBC27,0xBC29,0xBC2D,0xBC30,0xBC31,0xBC34,0xBC38,0xBC40,0xBC41,0xBC43,0xBC44,0xBC45,0xBC49,0xBC4C,
    0xBC4D,0xBC50,0xBC5D,0xBC84,0xBC85,0xBC88,0xBC8B,0xBC8C,0xBC8E,0xBC94,0xBC95,0xBC97,0xBC99,0xBC9A,0xBCA0,0xBCA1,
    0xBCA4,0xBCA7,0xBCA8,0xBCB0,0xBCB1,0xBCB3,0xBCB4,0xBCB5,0xBCBC,0xBCBD,0xBCC0,0xBCC4,0xBCCD,0xBCCF,0xBCD0,0xBCD1,
    0xBCD5,0xBCD8,0xBCDC,0xBCF4,0xBCF5,0xBCF6,0xBCF8,0xBCFC,0xBD04,0xBD05,0xBD07,0xBD09,0xBD10,0xBD14,0xBD24,0xBD2C,
    0xBD40,0xBD48,0xBD49,0xBD4C,0xBD50,0xBD58,0xBD59,0xBD64,0xBD68,0xBD80,0xBD81,0xBD84,0xBD87,0xBD88,0xBD89,0xBD8A,
    0xBD90,0xBD91,0xBD93,0xBD95,0xBD99,0xBD9A,0xBD9C,0xBDA4,0xBDB0,0xBDB8,0xBDD4,0xBDD5,0xBDD8,0xBDDC,0xBDE9,0xBDF0,
    0xBDF4,0xBDF8,0xBE00,0xBE03,0xBE05,0xBE0C,0xBE0D,0xBE10,0xBE14,0xBE1C,0xBE1D,0xBE1F,0xBE44,0xBE45,0xBE48,0xBE4C,
    0xBE4E,0xBE54,0xBE55,0xBE57,0xBE59,0xBE5A,0xBE5B,0xBE60,0xBE61,0xBE64,0xBE68,0xBE6A,0xBE70,0xBE71,0xBE73,0xBE74,
    0xBE75,0xBE7B,0xBE7C,0xBE7D,0xBE80,0xBE84,0xBE8C,0xBE8D,0xBE8F,0xBE90,0xBE91,0xBE98,0xBE99,0xBEA8,0xBED0,0xBED1,
    0xBED4,0xBED7,0xBED8,0xBEE0,0xBEE3,0xBEE4,0xBEE5,0xBEEC,0xBF01,0xBF08,0xBF09,0xBF18,0xBF19,0xBF1B,0xBF1C,0xBF1D,
    0xBF40,0xBF41,0xBF44,0xBF48,0xBF50,0xBF51,0xBF55,0xBF94,0xBFB0,0xBFC5,0xBFCC,0xBFCD,0xBFD0,0xBFD4,0xBFDC,0xBFDF,
    0xBFE1,0xC03C,0xC051,0xC058,0xC05C,0xC060,0xC068,0xC069,0xC090,0xC091,0xC094,0xC098,0xC0A0,0xC0A1,0xC0A3,0xC0A5,
    0xC0AC,0xC0AD,0xC0AF,0xC0B0,0xC0B3,0xC0B4,0xC0B5,0xC0B6,0xC0BC,0xC0BD,0xC0BF,0xC0C0,0xC0C1,0xC0C5,0xC0C8,0xC0C9,
    0xC0CC,0xC0D0,0xC0D8,0xC0D9,0xC0DB,0xC0DC,0xC0DD,0xC0E4,0xC0E5,0xC0E8,0xC0EC,0xC0F4,0xC0F5,0xC0F7,0xC0F9,0xC100,
    0xC104,0xC108,0xC110,0xC115,0xC11C,0xC11D,0xC11E,0xC11F,0xC120,0xC123,0xC124,0xC126,0xC127,0xC12C,0xC12D,0xC12F,
    0xC130,0xC131,0xC136,0xC138,0xC139,0xC13C,0xC140,0xC148,0xC149,0xC14B,0xC14C,0xC14D,0xC154,0xC155,0xC158,0xC15C,
    0xC164,0xC165,0xC167,0xC168,0xC169,0xC170,0xC174,0xC178,0xC185,0xC18C,0xC18D,0xC18E,0xC190,0xC194,0xC196,0xC19C,
    0xC19D,0xC19F,0xC1A1,0xC1A5,0xC1A8,0xC1A9,0xC1AC,0xC1B0,0xC1BD,0xC1C4,0xC1C8,0xC1CC,0xC1D4,0xC1D7,0xC1D8,0xC1E0,
    0xC1E4,0xC1E8,0xC1F0,0xC1F1,0xC1F3,0xC1FC,0xC1FD,0xC200,0xC204,0xC20C,0xC20D,0xC20F,0xC211,0xC218,0xC219,0xC21C,
    0xC21F,0xC220,0xC228,0xC229,0xC22B,0xC22D,0xC22F,0xC231,0xC232,0xC234,0xC248,0xC250,0xC251,0xC254,0xC258,0xC260,
    0xC265,0xC26C,0xC26D,0xC270,0xC274,0xC27C,0xC27D,0xC27F,0xC281,0xC288,0xC289,0xC290,0xC298,0xC29B,0xC29D,0xC2A4,
    0xC2A5,0xC2A8,0xC2AC,0xC2AD,0xC2B4,0xC2B5,0xC2B7,0xC2B9,0xC2DC,0xC2DD,0xC2E0,0xC2E3,0xC2E4,0xC2EB,0xC2EC,0xC2ED,
    0xC2EF,0xC2F1,0xC2F6,0xC2F8,0xC2F9,0xC2FB,0xC2FC,0xC300,0xC308,0xC309,0xC30C,0xC30D,0xC313,0xC314,0xC315,0xC318,
    0xC31C,0xC324,0xC325,0xC328,0xC329,0xC345,0xC368,0xC369,0xC36C,0xC370,0xC372,0xC378,0xC379,0xC37C,0xC37D,0xC384,
    0xC388,0xC38C,0xC3C0,0xC3D8,0xC3D9,0xC3DC,0xC3DF,0xC3E0,0xC3E2,0xC3E8,0xC3E9,0xC3ED,0xC3F4,0xC3F5,0xC3F8,0xC408,
    0xC410,0xC424,0xC42C,0xC430,0xC434,0xC43C,0xC43D,0xC448,0xC464,0xC465,0xC468,0xC46C,0xC474,0xC475,0xC479,0xC480,
    0xC494,0xC49C,0xC4B8,0xC4BC,0xC4E9,0xC4F0,0xC4F1,0xC4F4,0xC4F8,0xC4FA,0xC4FF,0xC500,0xC501,0xC50C,0xC510,0xC514,
    0xC51C,0xC528,0xC529,0xC52C,0xC530,0xC538,0xC539,0xC53B,0xC53D,0xC544,0xC545,0xC548,0xC549,0xC54A,0xC54C,0xC54D,
    0xC54E,0xC553,0xC554,0xC555,0xC557,0xC558,0xC559,0xC55D,0xC55E,0xC560,0xC561,0xC564,0xC568,0xC570,0xC571,0xC573,
    0xC574,0xC575,0xC57C,0xC57D,0xC580,0xC584,0xC587,0xC58C,0xC58D,0xC58F,0xC591,0xC595,0xC597,0xC598,0xC59C,0xC5A0,
    0xC5A9,0xC5B4,0xC5B5,0xC5B8,0xC5B9,0xC5BB,0xC5BC,0xC5BD,0xC5BE,0xC5C4,0xC5C5,0xC5C6,0xC5C7,0xC5C8,0xC5C9,0xC5CA,
    0xC5CC,0xC5CE,0xC5D0,0xC5D1,0xC5D4,0xC5D8,0xC5E0,0xC5E1,0xC5E3,0xC5E5,0xC5EC,0xC5ED,0xC5EE,0xC5F0,0xC5F4,0xC5F6,
    0xC5F7,0xC5FC,0xC5FD,0xC5FE,0xC5FF,0xC600,0xC601,0xC605,0xC606,0xC607,0xC608,0xC60C,0xC610,0xC618,0xC619,0xC61B,
    0xC61C,0xC624,0xC625,0xC628,0xC62C,0xC62D,0xC62E,0xC630,0xC633,0xC634,0xC635,0xC637,0xC639,0xC63B,0xC640,0xC641,
    0xC644,0xC648,0xC650,0xC651,0xC653,0xC654,0xC655,0xC65C,0xC65D,0xC660,0xC66C,0xC66F,0xC671,0xC678,0xC679,0xC67C,
    0xC680,0xC688,0xC689,0xC68B,0xC68D,0xC694,0xC695,0xC698,0xC69C,0xC6A4,0xC6A5,0xC6A7,0xC6A9,0xC6B0,0xC6B1,0xC6B4,
    0xC6B8,0xC6B9,0xC6BA,0xC6C0,0xC6C1,0xC6C3,0xC6C5,0xC6CC,0xC6CD,0xC6D0,0xC6D4,0xC6DC,0xC6DD,0xC6E0,0xC6E1,0xC6E8,
    0xC6E9,0xC6EC,0xC6F0,0xC6F8,0xC6F9,0xC6FD,0xC704,0xC705,0xC708,0xC70C,0xC714,0xC715,0xC717,0xC719,0xC720,0xC721,
    0xC724,0xC728,0xC730,0xC731,0xC733,0xC735,0xC737,0xC73C,0xC73D,0xC740,0xC744,0xC74A,0xC74C,0xC74D,0xC74F,0xC751,
    0xC752,0xC753,0xC754,0xC755,0xC756,0xC757,0xC758,0xC75C,0xC760,0xC768,0xC76B,0xC774,0xC775,0xC778,0xC77C,0xC77D,
    0xC77E,0xC783,0xC784,0xC785,0xC787,0xC788,0xC789,0xC78A,0xC78E,0xC790,0xC791,0xC794,0xC796,0xC797,0xC798,0xC79A,
    0xC7A0,0xC7A1,0xC7A3,0xC7A4,0xC7A5,0xC7A6,0xC7AC,0xC7AD,0xC7B0,0xC7B4,0xC7BC,0xC7BD,0xC7BF,0xC7C0,0xC7C1,0xC7C8,
    0xC7C9,0xC7CC,0xC7CE,0xC7D0,0xC7D8,0xC7DD,0xC7E4,0xC7E8,0xC7EC,0xC800,0xC801,0xC804,0xC808,0xC80A,0xC810,0xC811,
    0xC813,0xC815,0xC816,0xC81C,0xC81D,0xC820,0xC824,0xC82C,0xC82D,0xC82F,0xC831,0xC838,0xC83C,0xC840,0xC848,0xC849,
    0xC84C,0xC84D,0xC854,0xC870,0xC871,0xC874,0xC878,0xC87A,0xC880,0xC881,0xC883,0xC885,0xC886,0xC887,0xC88B,0xC88C,
    0xC88D,0xC894,0xC89D,0xC89F,0xC8A1,0xC8A8,0xC8BC,0xC8BD,0xC8C4,0xC8C8,0xC8CC,0xC8D4,0xC8D5,0xC8D7,0xC8D9,0xC8E0,
    0xC8E1,0xC8E4,0xC8F5,0xC8FC,0xC8FD,0xC900,0xC904,0xC905,0xC906,0xC90C,0xC90D,0xC90F,0xC911,0xC918,0xC92C,0xC934,
    0xC950,0xC951,0xC954,0xC958,0xC960,0xC961,0xC963,0xC96C,0xC970,0xC974,0xC97C,0xC988,0xC989,0xC98C,0xC990,0xC998,
    0xC999,0xC99B,0xC99D,0xC9C0,0xC9C1,0xC9C4,0xC9C7,0xC9C8,0xC9CA,0xC9D0,0xC9D1,0xC9D3,0xC9D5,0xC9D6,0xC9D9,0xC9DA,
    0xC9DC,0xC9DD,0xC9E0,0xC9E2,0xC9E4,0xC9E7,0xC9EC,0xC9ED,0xC9EF,0xC9F0,0xC9F1,0xC9F8,0xC9F9,0xC9FC,0xCA00,0xCA08,
    0xCA09,0xCA0B,0xCA0C,0xCA0D,0xCA14,0xCA18,0xCA29,0xCA4C,0xCA4D,0xCA50,0xCA54,0xCA5C,0xCA5D,0xCA5F,0xCA60,0xCA61,
    0xCA68,0xCA7D,0xCA84,0xCA98,0xCABC,0xCABD,0xCAC0,0xCAC4,0xCACC,0xCACD,0xCACF,0xCAD1,0xCAD3,0xCAD8,0xCAD9,0xCAE0,
    0xCAEC,0xCAF4,0xCB08,0xCB10,0xCB14,0xCB18,0xCB20,0xCB21,0xCB41,0xCB48,0xCB49,0xCB4C,0xCB50,0xCB58,0xCB59,0xCB5D,
    0xCB64,0xCB78,0xCB79,0xCB9C,0xCBB8,0xCBD4,0xCBE4,0xCBE7,0xCBE9,0xCC0C,0xCC0D,0xCC10,0xCC14,0xCC1C,0xCC1D,0xCC21,
    0xCC22,0xCC27,0xCC28,0xCC29,0xCC2C,0xCC2E,0xCC30,0xCC38,0xCC39,0xCC3B,0xCC3C,0xCC3D,0xCC3E,0xCC44,0xCC45,0xCC48,
    0xCC4C,0xCC54,0xCC55,0xCC57,0xCC58,0xCC59,0xCC60,0xCC64,0xCC66,0xCC68,0xCC70,0xCC75,0xCC98,0xCC99,0xCC9C,0xCCA0,
    0xCCA8,0xCCA9,0xCCAB,0xCCAC,0xCCAD,0xCCB4,0xCCB5,0xCCB8,0xCCBC,0xCCC4,0xCCC5,0xCCC7,0xCCC9,0xCCD0,0xCCD4,0xCCE4,
    0xCCEC,0xCCF0,0xCD01,0xCD08,0xCD09,0xCD0C,0xCD10,0xCD18,0xCD19,0xCD1B,0xCD1D,0xCD24,0xCD28,0xCD2C,0xCD39,0xCD5C,
    0xCD60,0xCD64,0xCD6C,0xCD6D,0xCD6F,0xCD71,0xCD78,0xCD88,0xCD94,0xCD95,0xCD98,0xCD9C,0xCDA4,0xCDA5,0xCDA7,0xCDA9,
    0xCDB0,0xCDC4,0xCDCC,0xCDD0,0xCDE8,0xCDEC,0xCDF0,0xCDF8,0xCDF9,0xCDFB,0xCDFD,0xCE04,0xCE08,0xCE0C,0xCE14,0xCE19,
    0xCE20,0xCE21,0xCE24,0xCE28,0xCE30,0xCE31,0xCE33,0xCE35,0xCE58,0xCE59,0xCE5C,0xCE5F,0xCE60,0xCE61,0xCE68,0xCE69,
    0xCE6B,0xCE6D,0xCE74,0xCE75,0xCE78,0xCE7C,0xCE84,0xCE85,0xCE87,0xCE89,0xCE90,0xCE91,0xCE94,0xCE98,0xCEA0,0xCEA1,
    0xCEA3,0xCEA4,0xCEA5,0xCEAC,0xCEAD,0xCEC1,0xCEE4,0xCEE5,0xCEE8,0xCEEB,0xCEEC,0xCEF4,0xCEF5,0xCEF7,0xCEF8,0xCEF9,
    0xCF00,0xCF01,0xCF04,0xCF08,0xCF10,0xCF11,0xCF13,0xCF15,0xCF1C,0xCF20,0xCF24,0xCF2C,0xCF2D,0xCF2F,0xCF30,0xCF31,
    0xCF38,0xCF54,0xCF55,0xCF58,0xCF5C,0xCF64,0xCF65,0xCF67,0xCF69,0xCF70,0xCF71,0xCF74,0xCF78,0xCF80,0xCF85,0xCF8C,
    0xCFA1,0xCFA8,0xCFB0,0xCFC4,0xCFE0,0xCFE1,0xCFE4,0xCFE8,0xCFF0,0xCFF1,0xCFF3,0xCFF5,0xCFFC,0xD000,0xD004,0xD011,
    0xD018,0xD02D,0xD034,0xD035,0xD038,0xD03C,0xD044,0xD045,0xD047,0xD049,0xD050,0xD054,0xD058,0xD060,0xD06C,0xD06D,
    0xD070,0xD074,0xD07C,0xD07D,0xD081,0xD0A4,0xD0A5,0xD0A8,0xD0AC,0xD0B4,0xD0B5,0xD0B7,0xD0B9,0xD0C0,0xD0C1,0xD0C4,
    0xD0C8,0xD0C9,0xD0D0,0xD0D1,0xD0D3,0xD0D4,0xD0D5,0xD0DC,0xD0DD,0xD0E0,0xD0E4,0xD0EC,0xD0ED,0xD0EF,0xD0F0,0xD0F1,
    0xD0F8,0xD10D,0xD130,0xD131,0xD134,0xD138,0xD13A,0xD140,0xD141,0xD143,0xD144,0xD145,0xD14C,0xD14D,0xD150,0xD154,
    0xD15C,0xD15D,0xD15F,0xD161,0xD168,0xD16C,0xD17C,0xD184,0xD188,0xD1A0,0xD1A1,0xD1A4,0xD1A8,0xD1B0,0xD1B1,0xD1B3,
    0xD1B5,0xD1BA,0xD1BC,0xD1C0,0xD1D8,0xD1F4,0xD1F8,0xD207,0xD209,0xD210,0xD22C,0xD22D,0xD230,0xD234,0xD23C,0xD23D,
    0xD23F,0xD241,0xD248,0xD25C,0xD264,0xD280,0xD281,0xD284,0xD288,0xD290,0xD291,0xD295,0xD29C,0xD2A0,0xD2A4,0xD2AC,
    0xD2B1,0xD2B8,0xD2B9,0xD2BC,0xD2BF,0xD2C0,0xD2C2,0xD2C8,0xD2C9,0xD2CB,0xD2D4,0xD2D8,0xD2DC,0xD2E4,0xD2E5,0xD2F0,
    0xD2F1,0xD2F4,0xD2F8,0xD300,0xD301,0xD303,0xD305,0xD30C,0xD30D,0xD30E,0xD310,0xD314,0xD316,0xD31C,0xD31D,0xD31F,
    0xD320,0xD321,0xD325,0xD328,0xD329,0xD32C,0xD330,0xD338,0xD339,0xD33B,0xD33C,0xD33D,0xD344,0xD345,0xD37C,0xD37D,
    0xD380,0xD384,0xD38C,0xD38D,0xD38F,0xD390,0xD391,0xD398,0xD399,0xD39C,0xD3A0,0xD3A8,0xD3A9,0xD3AB,0xD3AD,0xD3B4,
    0xD3B8,0xD3BC,0xD3C4,0xD3C5,0xD3C8,0xD3C9,0xD3D0,0xD3D8,0xD3E1,0xD3E3,0xD3EC,0xD3ED,0xD3F0,0xD3F4,0xD3FC,0xD3FD,
    0xD3FF,0xD401,0xD408,0xD41D,0xD440,0xD444,0xD45C,0xD460,0xD464,0xD46D,0xD46F,0xD478,0xD479,0xD47C,0xD47F,0xD480,
    0xD482,0xD488,0xD489,0xD48B,0xD48D,0xD494,0xD4A9,0xD4CC,0xD4D0,0xD4D4,0xD4DC,0xD4DF,0xD4E8,0xD4EC,0xD4F0,0xD4F8,
    0xD4FB,0xD4FD,0xD504,0xD508,0xD50C,0xD514,0xD515,0xD517,0xD53C,0xD53D,0xD540,0xD544,0xD54C,0xD54D,0xD54F,0xD551,
    0xD558,0xD559,0xD55C,0xD560,0xD565,0xD568,0xD569,0xD56B,0xD56D,0xD574,0xD575,0xD578,0xD57C,0xD584,0xD585,0xD587,
    0xD588,0xD589,0xD590,0xD5A5,0xD5C8,0xD5C9,0xD5CC,0xD5D0,0xD5D2,0xD5D8,0xD5D9,0xD5DB,0xD5DD,0xD5E4,0xD5E5,0xD5E8,
    0xD5EC,0xD5F4,0xD5F5,0xD5F7,0xD5F9,0xD600,0xD601,0xD604,0xD608,0xD610,0xD611,0xD613,0xD614,0xD615,0xD61C,0xD620,
    0xD624,0xD62D,0xD638,0xD639,0xD63C,0xD640,0xD645,0xD648,0xD649,0xD64B,0xD64D,0xD651,0xD654,0xD655,0xD658,0xD65C,
    0xD667,0xD669,0xD670,0xD671,0xD674,0xD683,0xD685,0xD68C,0xD68D,0xD690,0xD694,0xD69D,0xD69F,0xD6A1,0xD6A8,0xD6AC,
    0xD6B0,0xD6B9,0xD6BB,0xD6C4,0xD6C5,0xD6C8,0xD6CC,0xD6D1,0xD6D4,0xD6D7,0xD6D9,0xD6E0,0xD6E4,0xD6E8,0xD6F0,0xD6F5,
    0xD6FC,0xD6FD,0xD700,0xD704,0xD711,0xD718,0xD719,0xD71C,0xD720,0xD728,0xD729,0xD72B,0xD72D,0xD734,0xD735,0xD738,
    0xD73C,0xD744,0xD747,0xD749,0xD750,0xD751,0xD754,0xD756,0xD757,0xD758,0xD759,0xD760,0xD761,0xD763,0xD765,0xD769,
    0xD76C,0xD770,0xD774,0xD77C,0xD77D,0xD781,0xD788,0xD789,0xD78C,0xD790,0xD798,0xD799,0xD79B,0xD79D,0x1100,0x1101,
    0x1102,0x1103,0x1104,0x1105,0x1106,0x1107,0x1108,0x1109,0x110A,0x110B,0x110C,0x110D,0x110E,0x110F,0x1110,0x1111,
    0x1112,0x1161,0x1162,0x1163,0x1164,0x1165,0x1166,0x1167,0x1168,0x1169,0x116D,0x116E,0x1172,0x1173,0x1175,0xB894,
    0xC330,0xC3BC,0xC4D4,0xCB2C,
};
static const size_t PKSAV_GEN4_CHAR_MAP2_SIZE = sizeof(PKSAV_GEN4_CHAR_MAP2)/sizeof(wchar_t);

static enum pksav_error _pksav_gen4_import_widetext(
    const uint16_t* p_input_buffer,
    wchar_t* p_output_widetext,
    size_t num_chars
)
{
    assert(p_input_buffer != NULL);
    assert(p_output_widetext != NULL);

    memset(p_output_widetext, 0, sizeof(wchar_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        if(p_input_buffer[char_index] < 0x400)
        {
            p_output_widetext[char_index] = PKSAV_GEN4_CHAR_MAP1[p_input_buffer[char_index]];
        }
        else if(p_input_buffer[char_index] <= PKSAV_GEN4_CHAR_MAP2_SIZE)
        {
            p_output_widetext[char_index] = PKSAV_GEN4_CHAR_MAP2[p_input_buffer[char_index]];
        }
        else
        {
            break;
        }
    }

    return PKSAV_ERROR_NONE;
}

static enum pksav_error _pksav_gen4_export_widetext(
    const wchar_t* p_input_widetext,
    uint16_t* p_output_buffer,
    size_t num_chars
)
{
    assert(p_input_widetext != NULL);
    assert(p_output_buffer != NULL);

    memset(p_output_buffer, 0xFF, sizeof(uint16_t)*num_chars);

    for(size_t char_index = 0; char_index < num_chars; ++char_index)
    {
        ssize_t index = wchar_map_index(
                            PKSAV_GEN4_CHAR_MAP1,
                            PKSAV_GEN4_CHAR_MAP1_SIZE,
                            p_input_widetext[char_index]
                        );
        if(index == -1)
        {
            index = wchar_map_index(
                        PKSAV_GEN4_CHAR_MAP2,
                        PKSAV_GEN4_CHAR_MAP2_SIZE,
                        p_input_widetext[char_index]
                    );
            if(index == -1)
            {
                break;
            }
            else
            {
                p_output_buffer[char_index] = (uint16_t)index;
            }
        } else {
            p_output_buffer[char_index] = (uint16_t)index;
        }
    }

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen4_import_text(
    const uint16_t* p_input_buffer,
    char* p_output_text,
    size_t num_chars
)
{
    if(!p_input_buffer || !p_output_text)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = malloc(sizeof(wchar_t)*num_chars);
    _pksav_gen4_import_widetext(
        p_input_buffer, p_widetext, num_chars
    );

    memset(p_output_text, 0, num_chars);
    pksav_wcstombs(p_output_text, p_widetext, num_chars);
    free(p_widetext);

    return PKSAV_ERROR_NONE;
}

enum pksav_error pksav_gen4_export_text(
    const char* p_input_text,
    uint16_t* p_output_buffer,
    size_t num_chars
)
{
    if(!p_input_text || !p_output_buffer)
    {
        return PKSAV_ERROR_NULL_POINTER;
    }

    wchar_t* p_widetext = malloc(sizeof(wchar_t)*num_chars);
    pksav_mbstowcs(p_widetext, p_input_text, num_chars);

    _pksav_gen4_export_widetext(
        p_widetext, p_output_buffer, num_chars
    );

    free(p_widetext);

    return PKSAV_ERROR_NONE;
}

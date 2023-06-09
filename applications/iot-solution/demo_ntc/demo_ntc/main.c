#include <stdio.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <hosal_adc.h>
#include <bl_adc.h>
#include <blog.h>

#define ADC_NTC_PIN 11
#define ADC_REF_PIN 12

#define R1 10000
#define R2 10000
#define R3 10000

const int rt_table[] = {
    2772251,
    2635767,
    2500702,
    2368296,
    2239522,
    2115129,
    1995661,
    1881489,
    1772844,
    1669829,
    1572459,
    1480666,
    1394321,
    1313254,
    1237257,
    1166103,
    1099548,
    1037340,
    979228,
    924959,
    874288,
    827877,
    784386,
    743586,
    705269,
    669248,
    635354,
    603432,
    573341,
    544954,
    518154,
    492833,
    468895,
    446248,
    424810,
    404505,
    385264,
    367020,
    349714,
    333292,
    317700,
    302534,
    288159,
    274531,
    261608,
    249350,
    237720,
    226684,
    216209,
    206263,
    196819,
    187848,
    179326,
    171227,
    163530,
    156212,
    149254,
    142637,
    136342,
    130353,
    124654,
    119229,
    114064,
    109146,
    104462,
    100000,
    95749,
    91697,
    87836,
    84155,
    80644,
    77297,
    74103,
    71057,
    68149,
    65375,
    62726,
    60197,
    57782,
    55475,
    53271,
    51165,
    49153,
    47229,
    45390,
    43632,
    41950,
    40341,
    38802,
    37330,
    35920,
    34571,
    33279,
    32042,
    30857,
    29722,
    28634,
    27592,
    26593,
    25636,
    24718,
    23837,
    22993,
    22183,
    21405,
    20659,
    19943,
    19256,
    18596,
    17962,
    17353,
    16768,
    16206,
    15666,
    15147,
    14648,
    14168,
    13707,
    13263,
    12836,
    12425,
    12030,
    11649,
    11283,
    10930,
    10590,
    10267,
    9955,
    9654,
    9363,
    9083,
    8812,
    8550,
    8297,
    8052,
    7816,
    7587,
    7366,
    7152,
    6945,
    6744,
    6558,
    6376,
    6199,
    6026,
    5858,
    5694,
    5535,
    5380,
    5229,
    5083,
    4941,
    4803,
    4669,
    4539,
    4412,
    4290,
    4171,
    4055,
    3944,
    3835,
    3730,
    3628,
    3530,
    3434,
    3341,
    3253,
    3167,
    3083,
    3002,
    2924,
    2848,
    2774,
    2702,
    2633,
    2565,
    2500,
    2437,
    2375,
    2316,
    2258,
    2202,
    2148,
    2095,
    2044,
    1994,
    1946,
    1900,
    1855,
    1811,
    1769,
    1728,
    1688,
    1650,
    1612,
    1576,
    1541,
    1507,
    1474,
    1441,
    1410,
    1379,
    1350,
    1321,
    1293,
    1265,
    1239,
    1213,
    1187,
    1163,
    1139,
    1115,
    1092,
    1070,
    1048,
    1027,
    1006,
    986,
    966,
    947,
    928,
    909,
    891,
    873,
    856,
    839,
    822,
    806,
    790,
    774,
    759,
    743,
    729,
    714,
    700,
    686,
    672,
    658,
    645,
    631,
    619,
    611,
    603,
    595,
    587,
    578,
    570,
    562,
    553,
    545,
    536,
    528,
    520,
    511,
    503,
    495,
    487,
    479,
    470,
    463,
    455,
    447,
    439,
    432,
    425,
    417,
    410,
    403,
    396,
    390,
    383,
    377,
    370,
    364,
    358,
    352,
    346,
    340,
    335,
    329,
    324,
    319,
    314,
    309,
    304,
    299,
    295,
    290,
    286,
    281,
    277,
    273,
    269,
    265,
    261,
    257,
    254,
    250,
    247,
    243,
    240,
    237,
    233,
    230,
    227,
    224,
    221,
    219,
    216,
    213,
    210,
    208,
    205,
    202,
    200,
    198,
    195,
    193,
    190,
    188,
    186,
    184,
    181,
    179,
    135,
    133,
    131,
    129,
    127,
    125,
    124,
    122,
    120,
    118,
    107,
    115,
    113,
    112,
    110,
    109,
    107,
};

uint32_t binary_search(const int *array, uint32_t size, int value)
{
    uint32_t begin = 0, end = size - 1, mid = 0, i = 0;
    if (value >= array[begin]) {
        return begin;
    }
    else if (value <= array[end]) {
        return end;
    }
    while (begin < end) {
        mid = (begin + end) / 2;
        if (value == array[mid]) {
            break;
        }
        if (value < array[mid] && value > array[mid + 1]) {
            break;
        }
        if (value > array[mid]) {
            end = mid;
        }
        else {
            begin = mid;
        }
        if (i++ > size) {
            break;
        }
    }
    
    if (begin > end) {
        return 0;
    }
    
    return mid;
}

void main(void)
{
    static hosal_adc_dev_t adc0 = {
        .cb = NULL,
        .config = {
            .mode = HOSAL_ADC_ONE_SHOT,
            .pin = ADC_NTC_PIN,
            .sampling_freq = 340,
        },
        .dma_chan = 0,
        .p_arg = NULL,
        .port = 0,
    };

    hosal_adc_init(&adc0);

    hosal_adc_add_channel(&adc0, bl_adc_get_channel_by_gpio(ADC_NTC_PIN));
    hosal_adc_add_channel(&adc0, bl_adc_get_channel_by_gpio(ADC_REF_PIN));

    for (;;)
    {
        int v_ref = hosal_adc_value_get(&adc0, bl_adc_get_channel_by_gpio(ADC_REF_PIN), 100);
        int v_ntc = hosal_adc_value_get(&adc0, bl_adc_get_channel_by_gpio(ADC_NTC_PIN), 100);

        int a = 20000u * v_ref - (v_ntc * 10000u);
        int b = v_ntc;
        int r_ntc = a / b;

        int index = binary_search(rt_table, sizeof rt_table / sizeof rt_table[0], r_ntc * 10);
        int temperature = index - 40;

        blog_info("V_REF = %ld mV\tV_NTC = %ld mV\tR_NTC = %ld ohm\tTemperature = %ld C\r\n", v_ref, v_ntc, r_ntc, temperature);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

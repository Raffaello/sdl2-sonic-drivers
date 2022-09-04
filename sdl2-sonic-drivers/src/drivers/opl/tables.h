#pragma once

#include <cstdint>

namespace drivers
{
    namespace opl
    {
        static uint16_t freqtable[] = {                                 /* note # */
            345, 365, 387, 410, 435, 460, 488, 517, 547, 580, 615, 651, /*  0 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 12 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 24 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 36 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 48 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 60 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 72 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 84 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 96 */
            690, 731, 774, 820, 869, 921, 975, 517, 547, 580, 615, 651, /* 108 */
            690, 731, 774, 820, 869, 921, 975, 517 };                   /* 120 */

        static uint8_t octavetable[] = {        /* note # */
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, /*  0 */
            0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, /* 12 */
            1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, /* 24 */
            2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, /* 36 */
            3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, /* 48 */
            4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, /* 60 */
            5, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, /* 72 */
            6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, /* 84 */
            7, 7, 7, 7, 7, 7, 7, 8, 8, 8, 8, 8, /* 96 */
            8, 8, 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, /* 108 */
            9, 9, 9, 9, 9, 9, 9,10 };           /* 120 */

        static uint16_t pitchtable[] = {                               /* pitch wheel */
             29193U,29219U,29246U,29272U,29299U,29325U,29351U,29378U,  /* -128 */
             29405U,29431U,29458U,29484U,29511U,29538U,29564U,29591U,  /* -120 */
             29618U,29644U,29671U,29698U,29725U,29752U,29778U,29805U,  /* -112 */
             29832U,29859U,29886U,29913U,29940U,29967U,29994U,30021U,  /* -104 */
             30048U,30076U,30103U,30130U,30157U,30184U,30212U,30239U,  /*  -96 */
             30266U,30293U,30321U,30348U,30376U,30403U,30430U,30458U,  /*  -88 */
             30485U,30513U,30541U,30568U,30596U,30623U,30651U,30679U,  /*  -80 */
             30706U,30734U,30762U,30790U,30817U,30845U,30873U,30901U,  /*  -72 */
             30929U,30957U,30985U,31013U,31041U,31069U,31097U,31125U,  /*  -64 */
             31153U,31181U,31209U,31237U,31266U,31294U,31322U,31350U,  /*  -56 */
             31379U,31407U,31435U,31464U,31492U,31521U,31549U,31578U,  /*  -48 */
             31606U,31635U,31663U,31692U,31720U,31749U,31778U,31806U,  /*  -40 */
             31835U,31864U,31893U,31921U,31950U,31979U,32008U,32037U,  /*  -32 */
             32066U,32095U,32124U,32153U,32182U,32211U,32240U,32269U,  /*  -24 */
             32298U,32327U,32357U,32386U,32415U,32444U,32474U,32503U,  /*  -16 */
             32532U,32562U,32591U,32620U,32650U,32679U,32709U,32738U,  /*   -8 */
             32768U,32798U,32827U,32857U,32887U,32916U,32946U,32976U,  /*    0 */
             33005U,33035U,33065U,33095U,33125U,33155U,33185U,33215U,  /*    8 */
             33245U,33275U,33305U,33335U,33365U,33395U,33425U,33455U,  /*   16 */
             33486U,33516U,33546U,33576U,33607U,33637U,33667U,33698U,  /*   24 */
             33728U,33759U,33789U,33820U,33850U,33881U,33911U,33942U,  /*   32 */
             33973U,34003U,34034U,34065U,34095U,34126U,34157U,34188U,  /*   40 */
             34219U,34250U,34281U,34312U,34343U,34374U,34405U,34436U,  /*   48 */
             34467U,34498U,34529U,34560U,34591U,34623U,34654U,34685U,  /*   56 */
             34716U,34748U,34779U,34811U,34842U,34874U,34905U,34937U,  /*   64 */
             34968U,35000U,35031U,35063U,35095U,35126U,35158U,35190U,  /*   72 */
             35221U,35253U,35285U,35317U,35349U,35381U,35413U,35445U,  /*   80 */
             35477U,35509U,35541U,35573U,35605U,35637U,35669U,35702U,  /*   88 */
             35734U,35766U,35798U,35831U,35863U,35895U,35928U,35960U,  /*   96 */
             35993U,36025U,36058U,36090U,36123U,36155U,36188U,36221U,  /*  104 */
             36254U,36286U,36319U,36352U,36385U,36417U,36450U,36483U,  /*  112 */
             36516U,36549U,36582U,36615U,36648U,36681U,36715U,36748U }; /*  120 */

        /// <summary>
        /// Lookup table to convert MIDI volume ranges into OPL
        /// </summary>
        static uint8_t volumetable[128] = {
                  0,   1,   3,   5,   6,   8,  10,  11,
                 13,  14,  16,  17,  19,  20,  22,  23,
                 25,  26,  27,  29,  30,  32,  33,  34,
                 36,  37,  39,  41,  43,  45,  47,  49,
                 50,  52,  54,  55,  57,  59,  60,  61,
                 63,  64,  66,  67,  68,  69,  71,  72,
                 73,  74,  75,  76,  77,  79,  80,  81,
                 82,  83,  84,  84,  85,  86,  87,  88,
                 89,  90,  91,  92,  92,  93,  94,  95,
                 96,  96,  97,  98,  99,  99, 100, 101,
                101, 102, 103, 103, 104, 105, 105, 106,
                107, 107, 108, 109, 109, 110, 110, 111,
                112, 112, 113, 113, 114, 114, 115, 115,
                116, 117, 117, 118, 118, 119, 119, 120,
                120, 121, 121, 122, 122, 123, 123, 123,
                124, 124, 125, 125, 126, 126, 127, 127 };
    }
}

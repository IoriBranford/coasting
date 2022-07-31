#include "audio.h"

#include <stdio.h>
#include <types.h>
// CD library
#include <libcd.h>
// SPU library
#include <libspu.h>

SpuCommonAttr spuSettings;

CdlLOC loc[100];
int ntoc;

void audio_setup() {
    CdInit();
    SpuInit();

    spuSettings.mask = (SPU_COMMON_MVOLL | SPU_COMMON_MVOLR | SPU_COMMON_CDVOLL | SPU_COMMON_CDVOLR | SPU_COMMON_CDMIX);
    // Master volume should be in range 0x0000 - 0x3fff
    spuSettings.mvol.left  = 0x2000;
    spuSettings.mvol.right = 0x2000;
    // Cd volume should be in range 0x0000 - 0x7fff
    spuSettings.cd.volume.left = 0x4000;
    spuSettings.cd.volume.right = 0x4000;
    // Enable CD input ON
    spuSettings.cd.mix = SPU_ON;
    // Apply settings
    SpuSetCommonAttr(&spuSettings);
    // Set transfer mode 
    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);

    if ((ntoc = CdGetToc(loc)) == 0) { /* Read TOC */
        printf("No TOC found\n");
        return;
    }
    // Prevent out of bound pos
    for (int i = 1; i < ntoc; i++) {
        CdIntToPos(CdPosToInt(&loc[i]) - 74, &loc[i]);
    }
    // Those array will hold the return values of the CD commands
    u_char param[4];
    // Set CD parameters ; Report Mode ON, CD-DA ON. See LibeOver47.pdf, p.188
    param[0] = CdlModeRept | CdlModeDA;
    CdControlB(CdlSetmode, param, 0); /* set mode */
}

void play_cdda(int track) {
    if (!ntoc) return;
    CdControlB (CdlPlay, (u_char *)&loc[track], 0);
}
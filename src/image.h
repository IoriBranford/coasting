#ifndef __IMAGE_H
#define __IMAGE_H

typedef enum IMGID {
    NUM_IMGS
} IMGID;

#include <types.h>	// This provides typedefs needed by libgte.h and libgpu.h
#include <libgte.h>	// GTE header, not really used but libgpu.h depends on it
#include <libgpu.h>	// GPU library header

TIM_IMAGE* get_image(IMGID id);

#define is_indexed_image(image) (image->mode & 0x8)

#endif
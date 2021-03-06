/*
 * android-efi
 * Copyright (C) 2017 lambdadroid
 *
 * SPDX-License-Identifier: GPL-3.0+
 */

#ifndef ANDROID_EFI_GRAPHICS_H
#define ANDROID_EFI_GRAPHICS_H

#include <efi.h>

struct graphics_image {
    UINTN width;
    UINTN height;
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *blt;
};

EFI_STATUS graphics_display_image(struct graphics_image *image);

#endif //ANDROID_EFI_GRAPHICS_H

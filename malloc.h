/*
 * android-efi
 * Copyright (C) 2017 lambdadroid
 *
 * SPDX-License-Identifier: GPL-3.0+
 */

#ifndef ANDROID_EFI_MALLOC_H
#define ANDROID_EFI_MALLOC_H

#include <efi.h>

EFI_STATUS malloc_high(UINTN size, EFI_PHYSICAL_ADDRESS *addr);
EFI_STATUS malloc_low(UINTN size, UINTN align, EFI_PHYSICAL_ADDRESS *addr);

#endif //ANDROID_EFI_MALLOC_H

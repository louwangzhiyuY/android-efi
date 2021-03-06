/*
 * android-efi
 * Copyright (C) 2017 lambdadroid
 *
 * SPDX-License-Identifier: GPL-3.0+
 */

#ifndef ANDROID_EFI_ANDROID_H
#define ANDROID_EFI_ANDROID_H

#include <efi.h>
#include <efilib.h>
#include "bootimg.h"
#include "partition.h"

struct android_image {
    struct efi_partition partition;
    boot_img_hdr header;
};

static inline EFI_STATUS android_open_image(struct android_image *image) {
    EFI_STATUS err = partition_read(&image->partition, 0, &image->header, sizeof(image->header));
    if (err) {
        return err;
    }

    if (CompareMem(image->header.magic, (CHAR8*) BOOT_MAGIC, BOOT_MAGIC_SIZE)) {
        Print(L"Partition does not appear to be an Android boot partition\n");
        return EFI_VOLUME_CORRUPTED;
    }

    return EFI_SUCCESS;
}

static inline EFI_STATUS android_read_kernel(struct android_image *image, UINT64 offset, VOID *kernel, UINTN size) {
    return partition_read(&image->partition, image->header.page_size + offset, kernel, size);
}

static inline EFI_STATUS android_load_kernel(struct android_image *image, UINT64 offset, VOID *kernel) {
    return android_read_kernel(image, offset, kernel, image->header.kernel_size - offset);
}

static inline UINT64 android_ramdisk_offset(struct android_image *image) {
    UINTN mask = image->header.page_size - 1;
    return image->header.page_size + ((image->header.kernel_size + mask) & ~mask);
}

static inline UINT32 android_ramdisk_size(struct android_image *image) {
    return image->header.ramdisk_size;
}

static inline EFI_STATUS android_load_ramdisk(struct android_image *image, VOID *ramdisk) {
    return partition_read(&image->partition, android_ramdisk_offset(image), ramdisk, android_ramdisk_size(image));
}

static inline UINT32 android_cmdline_len(struct android_image *image) {
    UINT32 len = (UINT32) strlena(image->header.cmdline);
    if (len == (BOOT_ARGS_SIZE - 1)) {
        // Check extra command line
        len += strlena(image->header.extra_cmdline);
    }

    return len;
}

static inline VOID android_copy_cmdline(struct android_image *image, CHAR8* cmdline, UINT32 len) {
    if (len >= BOOT_ARGS_SIZE) {
        CopyMem(cmdline, image->header.cmdline, BOOT_ARGS_SIZE - 1);
        CopyMem(&cmdline[BOOT_ARGS_SIZE], image->header.extra_cmdline, len - BOOT_ARGS_SIZE);
    } else {
        CopyMem(cmdline, image->header.cmdline, len);
    }

    cmdline[len] = 0;
}

#endif //ANDROID_EFI_ANDROID_H

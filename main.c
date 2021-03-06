/*
 * android-efi
 * Copyright (C) 2017 lambdadroid
 *
 * SPDX-License-Identifier: GPL-3.0+
 */

#include <efi.h>
#include <efilib.h>

#include "guid.h"
#include "android.h"
#include "linux.h"
#include "graphics.h"

static EFI_STATUS load_kernel(EFI_HANDLE image, VOID **boot_params) {
    // Check command line arguments for partition GUID
    CHAR16 **argv;
    INTN argc = GetShellArgcArgv(image, &argv);
    if (argc < 2) {
        Print(L"Usage: android.efi <Boot Partition GUID>\n");
        return EFI_INVALID_PARAMETER;
    }

    // Parse command line partition GUID
    EFI_GUID partition_guid;
    if (!guid_parse(argv[1], &partition_guid)) {
        Print(L"Expected valid partition GUID, got '%s'\n", argv[1]);
        return EFI_INVALID_PARAMETER;
    }

    struct android_image android_image;

    // Open partition
    EFI_STATUS err = partition_open(&android_image.partition, image, &partition_guid);
    if (err) {
        return err;
    }

    // Read Android boot image header
    err = android_open_image(&android_image);
    if (err) {
        return err;
    }

    // Allocate boot params
    err = linux_allocate_boot_params(boot_params);
    if (err) {
        return err;
    }

    struct linux_setup_header *kernel_header = linux_kernel_header(*boot_params);

    // Load kernel header
    err = android_read_kernel(&android_image, LINUX_SETUP_HEADER_OFFSET, (VOID*) kernel_header, sizeof(*kernel_header));
    if (err) {
        goto boot_params_err;
    }

    // Prepare kernel header
    err = linux_check_kernel_header(kernel_header);
    if (err) {
        goto boot_params_err;
    }

    // Allocate kernel memory
    err = linux_allocate_kernel(kernel_header);
    if (err) {
        goto boot_params_err;
    }

    // Load kernel
    err = android_load_kernel(&android_image, linux_kernel_offset(kernel_header), linux_kernel_pointer(kernel_header));
    if (err) {
        goto kernel_err;
    }

    // Prepare command line
    UINT32 cmdline_len = android_cmdline_len(&android_image);

    // Allocate command line
    err = linux_allocate_cmdline(kernel_header, cmdline_len);
    if (err) {
        goto ramdisk_err;
    }

    android_copy_cmdline(&android_image, linux_cmdline_pointer(kernel_header), cmdline_len);

    // Allocate ramdisk memory
    err = linux_allocate_ramdisk(kernel_header, android_ramdisk_size(&android_image));
    if (err) {
        goto kernel_err;
    }

    // Load ramdisk
    err = android_load_ramdisk(&android_image, linux_ramdisk_pointer(kernel_header));
    if (err) {
        goto ramdisk_err;
    }

    // Close partition (not needed anymore)
    partition_close(&android_image.partition, image);

    return EFI_SUCCESS;

/*cmdline_err:
    linux_free_cmdline(kernel_header);*/

ramdisk_err:
    linux_free_ramdisk(kernel_header);

kernel_err:
    linux_free_kernel(kernel_header);

boot_params_err:
    linux_free_boot_params(*boot_params);

    return err;
}

extern struct graphics_image splash_image;

EFI_STATUS efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *system_table) {
    InitializeLib(image, system_table);

    // Display splash image
    graphics_display_image(&splash_image);

    VOID *boot_params;
    EFI_STATUS err = load_kernel(image, &boot_params);
    if (err) {
        Print(L"Failed to load kernel: %r\n", err);
        return err;
    }

    linux_efi_boot(image, boot_params);
    linux_free(boot_params);
    return EFI_SUCCESS;
}

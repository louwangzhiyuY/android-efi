#
# android-efi
# Copyright (C) 2017 lambdadroid
#
# SPDX-License-Identifier: GPL-3.0+
#

libpng = dependency('libpng', version: '>=1.6.0')

png2efi_exe = executable('png2efi', 'png2efi.c', dependencies: libpng)
png2efi = generator(png2efi_exe,
    output: '@BASENAME@.c',
    arguments: ['@BASENAME@', '@INPUT@', '@OUTPUT@'])

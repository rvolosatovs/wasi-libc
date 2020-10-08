// Copyright (c) 2015 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <dirent.h>

#include "dirent_impl.h"
#include "wasi/libc.h"

void seekdir(DIR *dirp, long loc) {
  // Update cookie.
#ifdef __wasm32__
  __wasi_dircookie_t cookie = dirp->offsets[(unsigned long)loc];
#else
  __wasi_dircookie_t cookie = (unsigned long)loc;
#endif
  __wasilibc_seekdir(dirp, cookie);
}

void __wasilibc_seekdir(DIR *dirp, __wasi_dircookie_t loc) {
  // Update cookie.
  dirp->cookie = loc;
  // Mark entire buffer as processed to force a read of new data.
  // TODO(ed): We could prevent a read if the offset is in the buffer.
  dirp->buffer_used = dirp->buffer_processed = dirp->buffer_size;
}

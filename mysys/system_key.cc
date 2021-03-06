/* Copyright (c) 2018 Percona LLC and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; version 2 of
   the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA */

#include "system_key.h"

#include <limits.h>
#include <string.h>

#include "my_dbug.h"
#include "my_macros.h"
#include "mysql/psi/psi_base.h"
#include "mysql/service_mysql_alloc.h"

/**
   System keys cannot have ':' in their name. We use ':' as a separator between
   system key's name and system key's version.
*/
const char *valid_percona_system_keys[] = {PERCONA_BINLOG_KEY_NAME};
const size_t valid_percona_system_keys_size =
    array_elements(valid_percona_system_keys);

uchar *parse_system_key(const uchar *key, const size_t key_length,
                        uint *key_version, uchar **key_data,
                        size_t *key_data_length) noexcept {
  if (key == nullptr || key_length == 0) return nullptr;

  size_t key_version_length = 0;
  for (; key[key_version_length] != ':' && key_version_length < key_length;
       ++key_version_length)
    ;
  if (key_version_length == 0 || key_version_length == key_length)
    return nullptr;  // no version found

  char *version =
      (char *)(my_malloc(PSI_NOT_INSTRUMENTED, key_version_length + 1, MYF(0)));
  if (version == nullptr) return nullptr;

  memcpy(version, key, key_version_length);
  version[key_version_length] = '\0';
  char *endptr = version;

  const ulong ulong_key_version = strtoul(version, &endptr, 10);
  if (ulong_key_version > UINT_MAX || *endptr != '\0') {
    my_free(version);
    return nullptr;  // conversion failed
  }

  DBUG_ASSERT(ulong_key_version <= UINT_MAX);  // sanity check
  *key_version = (uint)ulong_key_version;

  my_free(version);

  *key_data_length =
      key_length - (key_version_length + 1);  // skip ':' after key version
  if (*key_data_length == 0) return nullptr;
  DBUG_ASSERT(*key_data_length <= 512);

  *key_data = (uchar *)(my_malloc(PSI_NOT_INSTRUMENTED,
                                  sizeof(uchar) * (*key_data_length), MYF(0)));
  if (*key_data == nullptr) return nullptr;

  memcpy(*key_data, key + key_version_length + 1,
         *key_data_length);  // skip ':' after key version
  return *key_data;
}

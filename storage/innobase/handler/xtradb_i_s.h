/*****************************************************************************

Copyright (c) 2010-2012, Percona Inc. All Rights Reserved.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; version 2 of the License.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Suite 500, Boston, MA 02110-1335 USA

*****************************************************************************/

#ifndef XTRADB_I_S_H
#define XTRADB_I_S_H

extern struct st_mysql_plugin i_s_xtradb_read_view;
// Percona commented out until zip dictionary reimplementation in the new DD
#if 0
extern struct st_mysql_plugin	i_s_xtradb_zip_dict;
extern struct st_mysql_plugin	i_s_xtradb_zip_dict_cols;
#endif

#endif /* XTRADB_I_S_H */

/*
 * Copyright 2001-2004 Brandon Long
 * All Rights Reserved.
 *
 * ClearSilver Templating System
 *
 * This code is made available under the terms of the ClearSilver License.
 * http://www.clearsilver.net/license.hdf
 *
 */

#ifndef __CLEARSILVER_H_
#define __CLEARSILVER_H_ 1

#include "cs_config.h"

/* If you need these backward compatible definitions, define CS_COMPAT */
/* These changed after v0.9.1 */
#define CS_COMPAT 0

#if defined(CS_COMPAT) || !defined(HASH)
#define HASH NE_HASH
#define HASHNODE NE_HASHNODE
#define hash_init ne_hash_init
#define hash_destroy ne_hash_destroy
#define hash_lookup ne_hash_lookup
#define hash_has_key ne_hash_has_key
#define hash_remove ne_hash_remove
#define hash_next ne_hash_next
#define hash_str_comp ne_hash_str_comp
#define hash_str_hash ne_hash_str_hash
#endif /* CS_COMPAT */

#include <stdlib.h>
#include <sys/stat.h>

/* Base libraries */
#include "neo_misc.h"
#include "neo_err.h"
#include "neo_date.h"
#include "neo_files.h"
#include "neo_hash.h"
#include "neo_hdf.h"
#include "neo_rand.h"
#include "neo_net.h"
#include "neo_server.h"
#include "neo_str.h"
#include "ulist.h"
#include "wildmat.h"
#include "filter.h"

#ifdef HAVE_LOCKF
# include "ulocks.h"
# include "rcfs.h"

/* These are dependent on the pthread locking code in ulocks */
# ifdef HAVE_PTHREADS
#  include "skiplist.h"
#  include "dict.h"
# endif
#endif

/* This is dependent on Berkeley DB v2 */
#ifdef HAVE_DB2
# include "wdb.h"
#endif

/* The ClearSilver Template language */
#include "cs.h"

#endif /* __CLEARSILVER_H_ */

/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Dory
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 *
 * Dory is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Dory is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Suite 500, MA 02110-1335, USA.
 *
 * Author: Darin Adler <darin@bentspoon.com>
 */
   
/* dory-self-check-functions.c: Wrapper for all self check functions
 * in Dory proper.
 */

#include <config.h>

#if ! defined (DORY_OMIT_SELF_CHECK)

#include "dory-self-check-functions.h"

void dory_run_self_checks(void)
{
	DORY_FOR_EACH_SELF_CHECK_FUNCTION (DORY_CALL_SELF_CHECK_FUNCTION)
}

#endif /* ! DORY_OMIT_SELF_CHECK */

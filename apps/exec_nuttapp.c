/****************************************************************************
 * drivers/sbin/exec_nuttapp.c
 *
 *   Copyright (C) 2011 Uros Platise. All rights reserved.
 *   Author: Uros Platise <uros.platise@isotel.eu>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/nuttapp.h>
#include <sched.h>

#include <string.h>
#include <errno.h>


/****************************************************************************
 * Private Types
 ****************************************************************************/

/* Load builtin function prototypes */

#undef EXTERN
#if defined(__cplusplus)
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

#include "exec_nuttapp_proto.h"

static const struct nuttapp_s nuttapps[] = {
# include "exec_nuttapp_list.h"
  {.name=NULL}
};

#undef EXTERN
#if defined(__cplusplus)
}
#endif


/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/


/****************************************************************************
 * Private Data
 ****************************************************************************/


/****************************************************************************
 * Private Functions
 ****************************************************************************/


/****************************************************************************
 * Public Functions
 ****************************************************************************/

int exec_nuttapp(FAR const char *appname, FAR const char *argv[])
{
  int i, ret = ERROR;
  
  // Not sure what to do with exports and nexports ... as found in exec
  // FAR const struct symtab_s *exports, int nexports
  // so they are ommited in the args list.
  
  for (i=0; nuttapps[i].name; i++) 
  {
    if ( !strcmp(nuttapps[i].name, appname) ) 
    {
#ifndef CONFIG_CUSTOM_STACK
      ret = task_create(nuttapps[i].name, nuttapps[i].priority, 
                        nuttapps[i].stacksize, nuttapps[i].main, &argv[1]);
#else
      ret = task_create(nuttapps[i].name, nuttapps[i].priority, nuttapps[i].main, &argv[1]);
#endif
      return ret;
    }
  }
  errno = ENOENT;
  return ret;
}
/* woodstock.c -- readline() logging wrapper 
 * Copyright (c) 2009 moncelier@devlife.org
 * Version 1.0
 *
 * $Id: $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * 
 */

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <syslog.h>
#include "woodstock.h"
#include "string.h"

#define min(a,b) a<b?a:b

#if defined(RTLD_NEXT)
#  define REAL_LIBC RTLD_NEXT
#else
#  define REAL_LIBC ((void *) -1L)
#endif

#define FN(ptr,type,name,args)  ptr = (type (*)args)dlsym (REAL_LIBC, name)

static inline void logline(const char *line) {
	char login[256];
	char exepath[256], args[512];
	char* exename;
	int ret;
	unsigned int i;
	FILE* fd;

	#if ROOT_ONLY
	if (getuid() != 0) 
		return; 
	#endif

	#if IGNORE_NULL
	if (getlogin() == 0)
		return;
	#endif


	if( getlogin_r(login, 255) != 0 ) {
		strcpy(login,"unknown");
	}

	ret = readlink("/proc/self/exe", exepath, sizeof(exepath));
	exepath[ret] = '\0';
	exename = basename(exepath);
	
	openlog(exename, LOG_PID, LOG_USER);

	fd = fopen("/proc/self/cmdline", "r");
	if(fd){
		if( fread(args, 1, sizeof(args), fd) > 0){
			for(i = 0; i < sizeof(args) - 2; i++){
				if(args[i] == '\0' && args[i+1] != '\0'){
					args[i] = ' ';
				}
			}
			args[sizeof(args)-1] = '\0';
		}
		fclose(fd);
	}else{
		args[0] = '\0';
	}

	syslog(LOG_INFO, "[%s, uid:%d sid:%d, cmdline:%s]: %s", login, getuid(), getsid(0), args, line); 
}

/*@null@*/
char * readline (const char *prompt){
	char * ret;

	static char * (*func)(const char*);
	FN(func,char *,"readline",(const char *));

	ret = (*func)(prompt);

	if(ret != NULL)
		logline(ret);

	return ret;
}

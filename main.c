/*
 * Copyright (C) 2017 francis_hao <francis_hao@126.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 */

#include <sys/epoll.h>
#include <stdio.h>
#include <string.h>
int main(void)
{
	int epoll_fd;
	int fds_num;
	char str[50]={0};
	struct epoll_event ev_in,ev_out;
	fds_num = 0;

	epoll_fd = epoll_create(2); /*this 'size' argument is ignored*/
	if (-1 == epoll_fd){
		perror("epoll_create");
		return -1;
	}
	memset(&ev_in, 0, sizeof (ev_in));
	memset(&ev_out, 0, sizeof (ev_out));
	ev_in.events = EPOLLIN;
	if (-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &ev_in)){/*Register the target file descriptor 0,that is stdin*/
		perror("epoll_ctl");
		return -1;
	}
#define TIMEOUT 20	
	while(1){
		fds_num = epoll_wait(epoll_fd, &ev_out, 5, TIMEOUT*1000);
		//fds_num = epoll_wait(epoll_fd, &ev_out, 5, -1);
		if (-1 == fds_num){
			perror("epoll_wait");
			return -1;
		}else if (0 == fds_num){
			printf("none data within %u s\n",TIMEOUT);
			return 0;
		}
		if ( 0 == ev_out.data.fd){
			if ((ev_out.events & EPOLLIN) != 0){
				//printf("events is %x\n",ev_out.events);					
				if (-1 == read(0, str, 50)){
					perror("read");
					return -1;
				}
				printf("%s",str);
				memset(str, 0 ,sizeof (str));
			}
		}else{
			printf("%d\n",ev_out.data.fd);
		}
	}
	return 0;
}


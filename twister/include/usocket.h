#ifndef _USOCKET_H_
#define _USOCKET_H_

struct socket_info{
	uint32_t ip_addr;
	uint32_t port;
};
extern int maxfd;
extern struct socket_info sockets[1024];
extern int udp_socket(uint32_t,uint32_t);
extern int udp_send(int sockfd, void *,uint16_t, uint32_t, uint32_t);

#endif

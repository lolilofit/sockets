#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>

#define TIMEOUT 5

struct adresses {
	char adr[16];
	struct adresses* next;
};

void add_adr(struct adresses* list, char* buf) {
	struct adresses* new_adr = (struct adresses*)malloc(sizeof(struct adresses));
//	new_adr->adr = buf;
	sprintf(new_adr->adr, "%s", buf);
	new_adr->next = NULL;
	if(list == NULL) {
		list = new_adr;
		return;
	}
	struct adresses* cur = list;
	while(cur->next != NULL)
		cur  = cur->next;
	cur->next = new_adr;
	return;
}

int find_adr(struct adresses* list, char *adr) {
	struct adresses* cur = list;
	if(cur == NULL)
		return 1;
	while(cur) {
		if(strcmp(adr, cur->adr) != 0)
			return 1;
		cur = cur->next;
	}
	return 0;
}

void delete_adr(struct adresses* list) {
	if(list == NULL)
		return;
	struct adresses* cur = list;
	struct adresses* tmp;
	while(cur) {
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
	list = NULL;
}

struct adresses* list_adr;
struct adresses* tmp;

int main(int argc, char* argv[]) {
	int i, sc, n;
	struct sockaddr_in adr, other_adr, recv;
	char buf[3];
	long int time_now;
	socklen_t socklen;

	sc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sc == -1) {
		printf("can't create socket\n");
		return 1;
	}
	int broadcastEnable=1;
	int ret=setsockopt(sc, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
//	struct timeval read_timeout;
//	read_timeout.tv_sec = 0;
//	read_timeout.tv_usec = 10;
//	setsockopt(sc, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof(read_timeout));


	memset(&adr, '0', sizeof(adr));
	adr.sin_family = AF_INET;
//	adr.sin_addr.s_addr = inet_addr("192.168.1.16");
	adr.sin_addr.s_addr = inet_addr("255.255.255.255");
	adr.sin_port = htons(1900);

	memset(&other_adr, '0', sizeof(other_adr));
	other_adr.sin_family = AF_INET;
	other_adr.sin_addr.s_addr = inet_addr("192.168.1.7");
	other_adr.sin_port = htons(1900);
	memset(&recv, '0', sizeof(recv));

	int count = 0, res;
	bind(sc, (struct sockaddr*)&adr, sizeof(adr));
	printf("bibd\n");
	while(1) {
		char mes[] = "1";
		res = sendto(sc, mes, strlen(mes)+1, 0, (struct sockaddr*)&other_adr, sizeof(other_adr));
		if(res < 0)
			printf("erroe sending\n");
		printf("send\n");

		count = 0;
		time_now = time(NULL);
		while((time(NULL) - time_now) < TIMEOUT) {
			socklen = sizeof(recv);
			if(recvfrom(sc, buf, strlen(buf)+1, MSG_DONTWAIT, (struct sockaddr*)&recv, &socklen) >= 0)
			//	printf("err\n");
		//		return 2;
		//	}
		//	printf("rescv\n");

			printf("addr is: %s\n", inet_ntoa(recv.sin_addr));
		//	add_adr(tmp, inet_ntoa(recv.sin_addr));
			count++;

		}
		close(sc);
		return 0;

		struct adresses* cur = tmp;
		for(i = 0; i < count; i++) {
			if(find_adr(list_adr, cur->adr) == 1) {
				i = count + 1;
				list_adr = tmp;
			}
			cur = cur->next;
		}
		delete_adr(tmp);
		cur = list_adr;
		while(cur) {
			printf(cur->adr);
			cur = cur->next;
		}
		printf("------------");
	
	}
	return 0;
}

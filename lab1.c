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

struct adresses* add_adr(struct adresses* list, char* buf) {
	struct adresses* new_adr = (struct adresses*)malloc(sizeof(struct adresses));
	int i;
	for(i = 0; i < strlen(buf); i++) {
		(new_adr->adr)[i]=buf[i];
	}
	(new_adr->adr)[strlen(buf)+1]='\0';

	new_adr->next = NULL;
	if(list == NULL) {
		list = new_adr;
		return list;
	}
	struct adresses* cur = list;
	while(cur->next != NULL)
		cur  = cur->next;
	cur->next = new_adr;
	return list;
}

int find_adr(struct adresses* list, char *adr) {
	struct adresses* cur = list;
	if(cur == NULL) {
		return 1;
	}
	while(cur) {
		if(strcmp(adr, cur->adr) == 0) {
			return 0;
		}
		cur = cur->next;
	}
	return 1;
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

struct adresses* list_adr = NULL;
struct adresses* tmp= NULL;

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

	memset(&adr, '0', sizeof(adr));
	adr.sin_family = AF_INET;
	adr.sin_addr.s_addr = INADDR_ANY;
	adr.sin_port = htons(1900);

	memset(&other_adr, '0', sizeof(other_adr));
	other_adr.sin_family = AF_INET;
	other_adr.sin_addr.s_addr = inet_addr("192.168.1.255");
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
	
		count = 0;
		time_now = time(NULL);
		tmp = NULL;
		while((time(NULL) - time_now) < TIMEOUT) {
			socklen = sizeof(recv);
			if(recvfrom(sc, buf, strlen(buf)+1, MSG_DONTWAIT, (struct sockaddr*)&recv, &socklen) >= 0) {
			if(find_adr(tmp, inet_ntoa(recv.sin_addr)))
				tmp = add_adr(tmp, inet_ntoa(recv.sin_addr));

			if(find_adr(list_adr, inet_ntoa(recv.sin_addr))) {
				printf("add missing to list\n");
				list_adr = add_adr(list_adr, inet_ntoa(recv.sin_addr));
				struct adresses* tmp_cur = list_adr;
				while(tmp_cur) {
				printf("%s\n", tmp_cur->adr);
					tmp_cur=tmp_cur->next;
				}
			
			}
			printf("<---------------->\n");
			}

		}

		struct adresses* c;
		c=list_adr;
		while(c) {
			count++;
			c=c->next;
		}
		
		struct adresses* cur = list_adr;
		for(i = 0; i < count; i++) {
			if(find_adr(tmp, cur->adr) == 1) {
				i = count + 1;
				list_adr = tmp;
				struct adresses* tmp_cur = list_adr;
				while(tmp_cur) {
					printf("%s\n", tmp_cur->adr);
					tmp_cur=tmp_cur->next;
				}
			}
			cur = cur->next;
		}
		delete_adr(tmp);

		printf("------------\n");
	
	}
	return 0;
}

#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define TIMEOUT 10

struct adresses {
	char[16] adr;
	struct adresses* next;
};

void add_adr(struct adresses* list, char* buf) {
	struct adresses* new_adr = (struct adresses*)malloc(sizeof(struct adresses));
	new_adr->adr = buf;
	new_adr->next = NULL;
	if(list == NULL) {
		list = new_adr;
		return;
	}
	struct adresses* cur = list;
	while(cur->next != NULL)
		cur  = cur->next;
	cur->next = new_cur;
	return;
}

int find_adr(struct adresses* list, char *adr) {
	strcut adresses* cur = list;
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
	struct sockaddr_in adr, other_adr;
	char buf[3];
	long int time_now;
	socketlen_t socklen;

	sc = socket(AF_NET, sock_DGRAM, IPPROTO_UDP);
	if(sc == -1) {
		printf("can't create socket\n");
		return 1;
	}
	memset(&adr, '0', sizeof(struct socketaddr_in));
	adr.sun_family = AF_NET;
	adr.sin_addr.s_addr = inet_addr("");
	adr.sin_port = htons(8081);

	memset(&other_adr, '0', sizeof(struct socketaddr_in));
	other_adr.sun_family = AF_NET;
	other_adr.sun_addr.s_addr = inet_addr("255.255.255.255");
	other_adr.sin_port = htons(8081);
	
	int count = 0;
	bind(sc, (struct sockaddr*)&adr, sizeof(adr));
	while(1) {
		sendto(sc, "1", 1, (struct sockaddr*)&other_adr, sizeof(other_adr));
		count = 0;
		time_now = time(NULL);
		while((time(NULL) - time_now) < TIMEOUT) {
			if(recvfrom(sc, buf, 3, 0, (struct sockaddr*)&other_adr, &socklen) == -1){
				printf("err\n");
				return 2;
			}
			add_adr(tmp, inet_ntoa(other_adr.sun_addr.s_addr));
			count++;
		}
		struct adresses* cur = tmp;
		for(i = 0; i < count; i++) {
			if(find_adr(list_adr, cur->adr) == 1) {
				i = count + 1;
				list_adr = tmp;
			}
			cur = cur->next;
		}
		delete_adr(tmp);
	}
	return 0;
}

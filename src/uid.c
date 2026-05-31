/**********************
 * uid.c
 * Author: Tomer Harel
 * Reviewer: Shahar Lasri
 **********************/

#include <arpa/inet.h> /*inet_ntoa*/
#include <ifaddrs.h>   /*getifaddrs*/
#include <stdatomic.h>
#include <string.h> /*memcpy, memcmp*/
#include <unistd.h> /*getpid*/

#include "uid.h" /*ilrd_uid_t*/

#define IP_BUFFER_LENGTH (14)

static int GetPersonalIP(unsigned char* ip);
const ilrd_uid_t UIDBadUID = {0, 0, 0, {0}};

ilrd_uid_t UIDCreate(void)
{
	size_t status = 0;
	static atomic_size_t counter = 1;
	ilrd_uid_t uid = UIDBadUID;

	time(&uid.timestamp);
	if (-1 == uid.timestamp)
	{
		return UIDBadUID;
	}

	uid.counter = __sync_fetch_and_add(&counter,1);

	uid.pid = getpid();

	status = GetPersonalIP(uid.ip_address);
	if (0 != status)
	{
		return UIDBadUID;
	}

	return uid;
}

int UIDIsSame(ilrd_uid_t uid1, ilrd_uid_t uid2)
{
	return !((uid1.timestamp - uid2.timestamp) | (uid1.counter - uid2.counter) |
			 (uid1.pid - uid2.pid) |
			 memcmp(uid1.ip_address, uid2.ip_address, IP_BUFFER_LENGTH));
}

static int GetPersonalIP(unsigned char* ip)
{
	struct ifaddrs* ifap;
	struct ifaddrs* ifa;
	struct sockaddr_in* sa;
	char* buffer = NULL;

	if (-1 == getifaddrs(&ifap))
	{
		return 1;
	}
	for (ifa = ifap; NULL != ifa; ifa = ifa->ifa_next)
	{
		if (NULL != ifa->ifa_addr && AF_INET == ifa->ifa_addr->sa_family &&
			0 != strcmp(ifa->ifa_name, "lo"))
		{
			sa = (struct sockaddr_in*)ifa->ifa_addr;
			buffer = inet_ntoa(sa->sin_addr);
			strncpy((char*)ip, buffer, IP_BUFFER_LENGTH);
			freeifaddrs(ifap);
			return 0;
		}
	}

	freeifaddrs(ifap);
	return 1;
}

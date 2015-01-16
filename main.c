#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char* argv)
{
	FILE *ps;
	char date_str[32]={0};

	if((ps = popen("date +%Y%m%d-%H%M%S", "r")) == NULL )
	{
		printf("popen() error!\n");
		return 1;
	}

	fgets(date_str, sizeof(date_str),ps);
	printf("%s\n",date_str);

	pclose(ps);

	if((ps = popen("df -h | grep rootfs | awk '{print $5}'", "r")) == NULL )
	{
		printf("popen() error!\n");
		return 1;
	}

	memset(date_str,0,sizeof(date_str));
	fgets(date_str, sizeof(date_str),ps);
	printf("%s\n",date_str);

	pclose(ps);

	return 0;
}

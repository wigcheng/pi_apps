#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FILE_SEC (900*1000) //300 secs


static int get_time_now(char *str);
static int check_time(void);
static int auto_set_time(void);
static int check_capacity(void);
static int start_record(char *str);
static int delete_oldest_file(void);

static int get_time_now(char *str)
{
	FILE *ps;
	char buf[32]={0};

	if((ps = popen("date +%Y%m%d%H%M%S", "r")) == NULL )
		return 1;

	fgets(buf, sizeof(buf),ps);
	sprintf(str,"%s",buf);
	pclose(ps);

	return 0;
}

static int check_time(void)
{
	FILE *ps;
	char buf[8]={0};

	if((ps = popen("date +%Y", "r")) == NULL )
		return 1;

	fgets(buf, sizeof(buf),ps);
	if(atoi(buf)<2015)
	{
		printf("Your date not tune!\n");
		return 1;
	}

	pclose(ps);

	return 0;
}


static int check_capacity(void)
{
	FILE *ps;
	char buf[8]={0};
	int percent;

	if((ps = popen("df -h | grep rootfs | awk '{print $5}'", "r")) == NULL )
		return -1;

	fgets(buf, sizeof(buf),ps);
	pclose(ps);
	sscanf(buf,"%d",&percent);
	return percent;
}

static int start_record(char *str)
{
	char buf[128]={0};
	int sec;

	sec=FILE_SEC;
	sprintf(buf,"raspivid -t %d -o /home/pi/rec/%s",sec,str);

	if(system(buf))
		return 1;

	return 0;
}

static int delete_oldest_file(void)
{
	FILE *ps;
	char buf[32]={0};
	char del_cmd[64]={0};

	if((ps = popen("cd /home/pi/rec/ | find -type f -printf '%T+ %p\n' | sort | head -1 | awk '{print $2}'", "r")) == NULL )
		return 1;

	fgets(buf, sizeof(buf),ps);
	pclose(ps);

	sprintf(del_cmd,"rm %s",buf);

	return system(del_cmd);
}

int main(int argc,char* argv)
{
	FILE *ps;
	char date_str[32]={0};
	int value;

	for(;;)
	{
		memset(date_str,0,sizeof(date_str));
		value=check_capacity();
		if(value>0 && value<80)
		{
			get_time_now(date_str);
			printf("date=%s\n",date_str);
			start_record(date_str);
			usleep(1000000);
		}
		else
		{
			if(delete_oldest_file())
			{
				printf("delete file error!!\n");
			}
		}
	}

	return 0;
}

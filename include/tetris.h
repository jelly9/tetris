#include <keyboard.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <signal.h>

#define W 12
#define H 18

#define LOG(format, ...)                                                                 \
do{                                                                                      \
    time_t tt = time(0);                                                                  \
    struct tm ptm;                                                                       \
    localtime_r(&tt, &ptm);                                                               \
    fprintf(stdout, "[LOG][%4d-%02d-%02d %02d:%02d:%02d][%s:%s:%d]:\033[31m " format "\033[0m",   \
            ptm.tm_year + 1900, ptm.tm_mon + 1, ptm.tm_mday, ptm.tm_hour,                \
            ptm.tm_min, ptm.tm_sec, __FILE__, __FUNCTION__ , __LINE__, ##__VA_ARGS__) ;  \
    fflush(stdout);                                                                      \
}while(0)

struct shape{
    int s[5][5];
};

struct pos{
    int x,y;
};

int g_backgroud[H][W];
struct pos g_pos;
struct shape g_blank_shape = { 
    1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1 
};
int g_shape_idx;
int g_shape_next_idx;
int g_score;
int FC;
int BC;

struct shape shape_arr[7] = {
    { 0,0,0,0,0, 0,0,1,0,0, 0,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0 },
    { 0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,0,0,0 },
    { 0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,0,1,1,0, 0,0,0,0,0 },
    { 0,0,0,0,0, 0,0,1,0,0, 0,0,1,0,0, 0,1,1,0,0, 0,0,0,0,0 },
    { 0,0,0,0,0, 0,1,1,0,0, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0 },
    { 0,0,0,0,0, 0,1,1,0,0, 0,0,1,1,0, 0,0,0,0,0, 0,0,0,0,0 },
    { 0,0,0,0,0, 0,0,1,1,0, 0,1,1,0,0, 0,0,0,0,0, 0,0,0,0,0 } 
};

void start_game();
void help();
void exit_game();
void destroy_game();
void surprise();


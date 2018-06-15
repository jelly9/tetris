//#include <tetris.h>
#include <tetris.h>

//绘制块
static void draw_element(int x, int y, int c){
    x *= 2, x++, y++;
    printf("\033[%d;%dH\033[3%dm\033[4%dm[]\033[0m", y, x, c, c);
    fflush(stdout);
}

//绘制图形
static void draw_shape(struct shape *sh, struct pos *p, int c){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(sh->s[i][j] != 0){
                draw_element(p->x+j,p->y+i, c);
            }
        }
    }
}

static void menu(char key){
    for(int i = 0; i < 14; i++){
        draw_element(i, 0, 5);
    }
    printf("\n+--------------------------+\n");
    printf("|    welcome to teleis     |\n");
    printf("|                          |\n");
    switch(key){
    case 1:
        printf("|   -> 1.start.            |\n");
        printf("|      2.help.             |\n");
        printf("|      3.exit.             |\n");
        break;                              
    case 2:                                 
        printf("|      1.start.            |\n");
        printf("|   -> 2.help.             |\n");
        printf("|      3.exit.             |\n");
        break;                              
    case 3:                                 
        printf("|      1.start.            |\n");
        printf("|      2.help.             |\n");
        printf("|   -> 3.exit.             |\n");
        break;
    default:
        break;
    }
    printf("|                          |\n");
    printf("+--------------------------+\n");
    for(int i = 0; i < 14; i++){
        draw_element(i, 9, 5);
    }
}

//绘制背景
static void draw_back(){
    //打印游戏面板背景
    for(int i = 0; i < H; i++){
        for(int j = 0; j < W; j++){
            if(g_backgroud[i][j] == 1){
                draw_element(j, i, FC);
            }else{
                draw_element(j, i, BC);
            }
        }
    }
    
    //显示得分
    printf("\033[%d;%dH\033[35m [score] %d\033[0m", 2, 28, g_score);
    
    //显示下一个图形
    struct pos temp_pos = {14, 5};
    printf("\033[%d;%dH\033[35m [next shape] \033[0m", 4, 28);
    draw_shape(&g_blank_shape, &temp_pos, BC);
    draw_shape(&shape_arr[g_shape_next_idx], &temp_pos, FC);

    //显示时间
    time_t t = time(0);                                                                  
    struct tm ptm;                                                                       
    localtime_r(&t, &ptm);                                                               
    printf("\033[%d;%dH\033[33m [time %d:%d:%d] \033[0m", 12, 28, ptm.tm_hour, ptm.tm_min, ptm.tm_sec);
}

//设置背景，即修改g_backgroud
static void set_back(struct shape *sp, struct pos *p){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(sp->s[i][j] != 0){
                g_backgroud[p->y+i][p->x+j] = 1;
            }
        }
    }
}

            
//判断是否能移动，如果到达边界，或者发生碰撞
static int can_move(struct shape *sp, struct pos *p){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(sp->s[i][j] == 0) continue;
            if(p->x+j<0 || p->x+j>=W) return 0;
            if(p->y+i>=H) return 0;
            if(g_backgroud[p->y+i][p->x+j] != 0) return 0;
        }
    }
    return 1;
}

//shape旋转90度
static struct shape turn90(struct shape *sp){
    struct shape res;
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            res.s[i][j] = sp->s[j][5-i];
        }
    }
    return res;
}

static void clean_line(){
    int count = 0;
    for(int i = 0; i < H; i++){
        count = 0;
        for(int j = 0; j < W; j++){//统计改行块的个数
            if(g_backgroud[i][j] !=0 ) count++;
        }
        if(count == W){//如果改行块的个数等于宽度
            g_score += 10;           
            for(int k = i; k >= 1; k--){//消去该行，并且将该行以上的数据都向下移动一行
                memcpy(g_backgroud[k],g_backgroud[k-1], sizeof(g_backgroud[k]));
            }
            memset(g_backgroud[0], 0x00, sizeof(g_backgroud[0]));
        }
    }
}

static void is_over(){
    for(int i = 0; i < W; i++){
        if(g_backgroud[1][i] != 0){
            destroy_game();
            exit(0);
        }  
    }
}

static int tetris_timer(struct pos *p){
    struct pos temp = {p->x, p->y+1};
    if(can_move(&shape_arr[g_shape_idx], &temp)){//如果可以向下移动，则向下移动
        draw_shape(&shape_arr[g_shape_idx], p, BC);
        p->y++;
        draw_shape(&shape_arr[g_shape_idx], p, FC);
    }else{//否则说明到底了
        set_back(&shape_arr[g_shape_idx], p);
        clean_line();//判断是否需要消行
        is_over();
        //g_shape_idx = 1;
        //g_shape_next_idx = 1;
        g_shape_idx = g_shape_next_idx;
        g_shape_next_idx = rand()%7;
        draw_back();
        p->x = 4,p->y = 0;
        FC = rand()%6+1;
    }
}

static void tetris(struct pos *p){
    int key = get_key();
    struct pos temp_left = {p->x-1, p->y};
    struct pos temp_right= {p->x+1, p->y};
    struct pos temp_down = {p->x, p->y+1};
    if(is_left(key) && can_move(&shape_arr[g_shape_idx], &temp_left)){//向左移动
        draw_shape(&shape_arr[g_shape_idx], p, BC);
        p->x--;
        draw_shape(&shape_arr[g_shape_idx], p, FC);
    }else if(is_right(key) && can_move(&shape_arr[g_shape_idx], &temp_right)){//向右移动
        draw_shape(&shape_arr[g_shape_idx], p, BC);
        p->x++;
        draw_shape(&shape_arr[g_shape_idx], p, FC);
    }else if(is_up(key)){//旋转
        draw_shape(&shape_arr[g_shape_idx], p, BC);
        shape_arr[g_shape_idx] = turn90(&shape_arr[g_shape_idx]);
        if(!can_move(&shape_arr[g_shape_idx], p)){
            shape_arr[g_shape_idx] = turn90(&shape_arr[g_shape_idx]);
            shape_arr[g_shape_idx] = turn90(&shape_arr[g_shape_idx]);
            shape_arr[g_shape_idx] = turn90(&shape_arr[g_shape_idx]);
        }
        draw_shape(&shape_arr[g_shape_idx], p, FC);
    }else if(is_down(key) &&can_move(&shape_arr[g_shape_idx], &temp_down)){//加速向下
        draw_shape(&shape_arr[g_shape_idx], p, BC);
        p->y++;
        draw_shape(&shape_arr[g_shape_idx], p, FC);
    }
}

void handler(int arg){
    (void)arg;
    tetris_timer(&g_pos);
}

void handler_int(int arg){
    (void)arg;
    destroy_game();
    exit(0);
}

void start_game(){
    system("clear");
    
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, NULL);
    
    struct itimerval itv;
    itv.it_value.tv_sec = 0;
    itv.it_value.tv_usec = 1000*400;
    itv.it_interval.tv_sec = 0;
    itv.it_interval.tv_usec = 1000*400;
    setitimer(ITIMER_REAL, &itv, NULL);

    signal(SIGINT, handler_int);
    signal(SIGQUIT, SIG_IGN);

    draw_back();
    for(;;){
        tetris(&g_pos);//玩家控制
    }
}

void surprise(){
    int key, n;
    for(;;){
        n = rand()%8;
        printf("                          \033[3%dmGood\033[0m", n);
        n = rand()%8;
        printf("    \033[3%dmLuck\033[0m", n);
        n = rand()%8;
        printf("    \033[3%dmTo\033[0m", n);
        n = rand()%8;
        printf("    \033[3%dmYou\033[0m\n\n", n);
        key = get_key();
        if(is_esc(key)){
            return;
        }
    }
}

void help(){
    surprise();
}

void exit_game(){
    destroy_game();
    exit(0);
}

void init_game(){
    memset(g_backgroud, 0x00, W*H);
    g_pos.x = 3,g_pos.y = 0;
    g_shape_idx = 2;
    g_shape_next_idx = 5;
    g_score = 0;
    FC = 2;
    BC = 7;
    srand(time(NULL));
    init_keyboard();
    printf("\033[?25l");
}

void destroy_game(){
    printf("\033[?25h");
    recover_keyboard();
    system("clear");
}

typedef void(*pFun)(void);

int main()
{
    init_game();
    pFun fun[3] = {start_game, help, exit_game};
    int select = 1;
    int key;
    do{

        usleep(1000*150);
        system("clear");
        menu(select);
        key = get_key();
        if(is_enter(key)){
           fun[select-1]();
        }else if(is_down(key)){
            if(++select==4) select = 1;
        }else if(is_up(key)){
            if(--select==0) select = 3;
        }else{
            //nothing to do
        }
    }while(1);
    
    destroy_game();
    return 0;
}


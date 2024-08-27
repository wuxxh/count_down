#include "REG52.H"

#define TIMS (65536 - 9216) // 10ms定时

sbit lcd1 = P2 ^ 0;

sbit lsc = P2 ^ 4;
sbit lsb = P2 ^ 3;
sbit lsa = P2 ^ 2;

sbit key1 = P3 ^ 1;
sbit key2 = P3 ^ 0;
sbit key3 = P3 ^ 2;
sbit key4 = P3 ^ 3;

void set_138(int arr[3])
{
    lsc = arr[0];
    lsb = arr[1];
    lsa = arr[2];
}

char DSY_CODE[] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; // 共阴极数码管0~9

char hour = 0, min = 0, sec = 0;
char b = 0, c = 0, d = 0;

void reset_mode()
{
    hour = 0, min = 0, sec = 0, b = 0, c = 0, d = 0;
}

char mode_ = 0;

void delay(unsigned int n) // 延时函数（运行空项目以达到延迟时间的效果）
{
    unsigned int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 120; j++)
            ;
    }
}

void init_timer0()
{
    TMOD = 0x01;
    TL0 = TIMS;
    TH0 = TIMS >> 8;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void init_exit0()
{
    IT0 = 1;
    EX0 = 1;
    EA = 1;
}

int location[8][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}}; // 晶体管8个位置
// int locate = 0;

void record_display() // 计时器
{
    unsigned char t[] = {0, 0, 0, 0, 0, 0}; // 代表时针分针秒针的个位和十位
    t[0] = hour / 10;                       // 时针十位
    t[1] = hour % 10;
    t[2] = min / 10;
    t[3] = min % 10;
    t[4] = sec / 10;
    t[5] = sec % 10;
    set_138(location[0]); // 控制第一个数码管
    P0 = DSY_CODE[t[5]];
    delay(1);
    set_138(location[1]);
    P0 = DSY_CODE[t[4]];
    delay(1);
    set_138(location[2]);
    P0 = 0x40;
    delay(1);
    set_138(location[3]);
    P0 = DSY_CODE[t[3]];
    delay(1);
    set_138(location[4]);
    P0 = DSY_CODE[t[2]];
    delay(1);
    set_138(location[5]);
    P0 = 0x40;
    delay(1);
    set_138(location[6]);
    P0 = DSY_CODE[t[1]];
    delay(1);
    set_138(location[7]);
    P0 = DSY_CODE[t[0]];
    delay(1);
}

void main()
{
    init_timer0();
    init_exit0();
    reset_mode();
    // 设置模式,mode0时计时模式
    while (1)
    {
        switch (mode_)
        {
        case 0:
            record_display();
            break;
        case 1:
            lcd1 = ~lcd1;
            break;
        case 2:
            break;
        case 3:
            reset_mode();
            mode_ = 0; // 记得要重新回到计时
            break;
        }
        if (key1 == 0)
        {
            delay(1);
            if (key1 == 0)
                mode_ = 0;
        }
        while (!key1)
            ;
        if (key2 == 0)
        {
            delay(1);
            if (key2 == 0)
                mode_ = 1;
        }
        while (!key2)
            ;
        if (key4 == 0)
        {
            delay(1);
            if (key4 == 0)
                mode_ = 3;
        }
        while (!key4)
            ;
    }
}

int count = 0;

void timer0() interrupt 1 // 定时器用于规范
{
    TH0 = TIMS >> 8;
    TL0 = TIMS;
    count++;
    if (count >= 100)
    {
        count = 0;
        sec++;
    }
    if (sec == 60)
    {
        sec = 0;
        min++;
    }
    if (min == 60)
    {
        min = 0;
        hour++;
    }
    if (hour == 24)
    {
        hour = 0;
    }
}

void exit0() interrupt 0
{
    if (key1 == 0)
    {
        delay(1);
        if (key1 == 0)
            lcd1 = ~lcd1;
    }
    while (!key1)
        ;
}
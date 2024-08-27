#include "REG52.H"

#define TIMS (65536 - 9216) // 10ms��ʱ

sbit lcd1 = P2 ^ 0;

sbit lsc = P2 ^ 4;
sbit lsb = P2 ^ 3;
sbit lsa = P2 ^ 2;

sbit key0 = P3 ^ 0;
sbit key1 = P3 ^ 2;
sbit key2 = P3 ^ 3;

void set_138(int arr[3])
{
    lsc = arr[0];
    lsb = arr[1];
    lsa = arr[2];
}

char DSY_CODE[] =
    {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F}; // �����������0~9

char a = 0, hour = 0, min = 0, sec = 0;
char b = 0, c = 0, d = 0;

void delay(unsigned int n) // ��ʱ���������п���Ŀ�Դﵽ�ӳ�ʱ���Ч����
{
    unsigned int i, j;
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < 120; j++)
            ;
    }
}

void init()
{
    TMOD = 0x01;
    TL0 = TIMS;
    TH0 = TIMS >> 8;
    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

int location[8][3] = {{0, 0, 0}, {0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}}; // �����8��λ��
// int locate = 0;

void display()
{
    unsigned char t[] = {0, 0, 0, 0, 0, 0}; // ����ʱ���������ĸ�λ��ʮλ
    t[0] = hour / 10;                       // ʱ��ʮλ
    t[1] = hour % 10;
    t[2] = min / 10;
    t[3] = min % 10;
    t[4] = sec / 10;
    t[5] = sec % 10;
    set_138(location[0]); // ���Ƶ�һ�������
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
    init();
    while (1)
    {
        display();
    }
}

int count = 0;

void timer0() interrupt 1 // ��ʱ�����ڹ淶
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
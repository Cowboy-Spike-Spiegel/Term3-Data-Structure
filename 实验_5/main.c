#include "head.h"

int main()
{
    char mode;
    while (mode!='3')
    {
        mode=0;
        mode=selectMode();
        //���ģʽ��1�ͱ���
        if (mode=='1')
        {
            encode();
            printf("Encoded Successfully\n\n");
        }
        //���ģʽ��2�ͽ���
        else if (mode=='2')
        {
            decode();
            printf("Decoded Successfully\n\n");
        }
    }

    return 0;
}



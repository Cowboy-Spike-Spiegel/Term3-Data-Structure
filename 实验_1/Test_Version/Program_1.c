#include "PROGRAM_1.h"

typedef struct member
{
    struct member * next;
    struct member * prev;
    int data;
}*mem;

int main()
{
    mem head,last,cur,pre;
    int i,n,x,y,yLow,yTop;
    x=n=yLow=yTop=0;
    //��������
    i=1;
    while (n<2||yLow>yTop)
    {
        printf("Please input the n, the lower bound of y and the upper bound of y:\n");
        scanf("%d %d %d",&n,&yLow,&yTop);
        if (n<2||yLow>yTop)
            printf("Invalid value\n");
    }
    head=malloc(sizeof(struct member));
    head->data=0;
    cur=last=head;
    while (i<n)
    {
        cur->next=malloc(sizeof(struct member));
        cur->next->prev=cur;
        cur=cur->next;
        cur->data=i;
        last=cur;
        i=i+1;
    }
    last->next=head;
    head->prev=last;

    //��ʼģ��
    cur=head;
    pre=last;
    //����X��λ��
    while (x<n)
    {
        i=0;
        while (i<x)
        {
            cur=cur->next;
            pre=pre->next;
            i=i+1;
        }
        y=yLow;
        while (y<=yTop)
        {
            while (head!=last)
            {
                i=0;
                if (y>=yTop)
                {
                    break;
                }
                //��Y����0ʱ����ת
                if (y>=0)
                {
                    while (i<y)
                    {
                        cur=cur->next;
                        pre=pre->next;
                        i=i+1;
                    }
                    if (cur->next==cur&&cur->data==0)
                    {
                        break;
                    }
                    else if (cur->data==0)
                    {
                        break;
                    }
                    else
                    {
                        if (cur==last)
                            last=pre;
                        pre->next=cur->next;
                        cur=cur->next;
                        free(cur->prev);
                        cur->prev=pre;
                    }
                }
                //��YС��0ʱ����ת
                else if (y<0)
                {
                    pre=cur->next;
                    i=-1;
                    while (i>y)
                    {
                        pre=pre->prev;
                        cur=cur->prev;
                        i=i-1;
                    }
                    if (cur->next==cur&&cur->data==0)
                    {
                        break;
                    }
                    else if (cur->data==0)
                    {
                        break;
                    }
                    else
                    {
                        if (cur==last)
                            last=cur->prev;
                        pre->prev=cur->prev;
                        cur=cur->prev;
                        free(cur->next);
                        cur->next=pre;
                    }
                }
            }
            //��������ȷ���ӡ
            if (cur->next==cur&&cur->data==0)
            {
                if (y>=0)
                    printf("n:%d x:%d y:%d\n",n,x+1,y+1);
                else
                    printf("n:%d x:%d y:%d\n",n,x+1,y);
            }
            //���´�������
            i=1;
            cur=last=head;
            while (i<n)
            {
                cur->next=malloc(sizeof(struct member));
                cur->next->prev=cur;
                cur=cur->next;
                cur->data=i;
                last=cur;
                i=i+1;
            }
            last->next=head;
            head->prev=last;
            cur=head;
            pre=last;
            y=y+1;
            //ȥ��X��λ��
            i=0;
            while (i<x)
            {
                cur=cur->next;
                pre=pre->next;
                i=i+1;
            }
        }
        x=x+1;
        cur=head;
        pre=last;
    }
    return 0;
}

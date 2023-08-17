#ifndef HEAD_H_INCLUDED
#define HEAD_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���������Ľڵ�
typedef struct Huffman
{
    int word;
    struct Huffman* parent;
    struct Huffman* lk;//left kid
    struct Huffman* rk;//right kid
}Node;

//��������ά��������ڵ�
typedef struct countChar
{
    char word;
    int count;
    Node *cur;
    char coding[50];
}countsOfWords;


char selectMode(void);//ѡ��ģʽ
void encode(void);//����
void decode(void);//����
char* openFile(void);//���ļ�
countsOfWords* countFrequency(FILE* doc,int* numsOfWord);//ͳ�Ƴ���Ƶ��
Node* createTree(countsOfWords* words,int count);//������������
int cmpfunc (const void * a, const void * b);//���ڿ��ŵıȽϺ���
Node* createNode(void);//�����ڵ�
char** encoding(countsOfWords* words,Node* root);//��ÿ���ַ����б���
void dfs(char** words,Node* cur,int i,char code[]);//����
void outputFile(FILE* in,FILE* out,char** words, char* filePath);//����ɶ������ļ�
void getCode(FILE* doc,int words[]);//��ȡÿ���ַ��ı���
void outputDecodeFile(FILE* doc,int words[],char* fileName);//�����ѹ�ļ�

char selectMode(void)
{
    char mode,c;
    mode=0;
    //���������ַ�ʱ����ѭ��
    while ((mode!='1'&&mode!='2')&&mode!='3')
    {
        printf("Please enter number to select the mode:\n");
        printf("1:Encode\n");
        printf("2:Decode\n");
        printf("3:Quit\n");
        scanf("%c",&mode);
        //�����û���������ַ�
        while ((c=getchar()!='\n'&&c!=EOF));
    }
    return mode;
}

void encode(void)
{
    FILE *doc=NULL;
    FILE *output=NULL;
    countsOfWords* words;
    int numsOfWords=0;
    Node* root;
    char** code;
    char* inputFile;
    root=NULL;
    words=NULL;
    inputFile=openFile();
    doc=fopen(inputFile,"rb+");
    //���ļ�
    words=countFrequency(doc,&numsOfWords);
    //ͳ�Ƴ���Ƶ��
    root=createTree(words,numsOfWords);
    //����������
    Node* tmp;
    code=encoding(words,root);
    //���б���
    rewind(doc);
    //�ص��ļ�ͷ
    outputFile(doc,output,code,inputFile);
    //����������ļ�
}

char* openFile(void)
{
    int flag=0;
    int i=0;
    FILE* file;
    char* filePath;
    filePath=malloc(sizeof(char)*100);
    while (flag==0)
    {
        i=0;
        //�����û�����
        printf("Please enter the path of the file:\n");
        while (filePath[i-1]!='\n')
        {
            scanf("%c",&filePath[i]);
            i=i+1;
        }
        //����ַ����룬�����������
        filePath[i-1]=0;
        file=fopen(filePath,"rb+");
        if (file==NULL)
        {
            printf("Can not open that file, please check it.\n");
        }
        else
        {
            flag=1;
        }
    }
    return filePath;
}

countsOfWords* countFrequency(FILE* doc,int* numsOfWord)
{
    int words[256]={};
    char str[500]={};
    int i,j,count;
    countsOfWords* wordsCount;
    count=fread(str,sizeof(char),256,doc);
    //countΪ������ַ�����
    while (count!=0)
    {
        i=0;
        while (i<count)
        {
            if ( words[(int)str[i]+128]==0)
                //+128��Ϊ�˰Ѹ�����Ϊ����
                *numsOfWord=*numsOfWord+1;
            words[(int)str[i]+128]=words[(int)str[i]+128]+1;
            i=i+1;
        }
        count=fread(str,sizeof(char),256,doc);
    }
    wordsCount=malloc(sizeof(countsOfWords)*(*numsOfWord));
    //�������������ά��������
    i=j=0;
    while (i<256)
    {
        if (words[i]!=0)
        {
            wordsCount[j].count=words[i];
            wordsCount[j].word=i;
            wordsCount[j].cur=malloc(sizeof(Node));
            wordsCount[j].cur->word=i;
            wordsCount[j].cur->lk=NULL;
            wordsCount[j].cur->rk=NULL;
            wordsCount[j].cur->parent=NULL;
            j=j+1;
        }
        i=i+1;
    }
    //��ʼ������
    qsort(wordsCount,*numsOfWord,sizeof(countsOfWords),cmpfunc);
    return wordsCount;
}

Node* createTree(countsOfWords* words,int count)
{
    int i;
    Node *l,*r,*parent;
    l=NULL;
    r=NULL;
    parent=NULL;
    i=0;
    if (count==1)
    {
        parent=words[0].cur;
    }
    //����ֻ����һ���ַ������
    else
    {
        while (i<count-1)
        {
            l=words[i].cur;
            r=words[i+1].cur;
            parent=createNode();
            parent->lk=l;
            parent->rk=r;
            parent->word=-1;
            words[i+1].count=words[i].count+words[i+1].count;
            words[i+1].cur=parent;
            words[i].count=0;
            //Ϊ��ǰ������Сֵ�������ڵ㣬��������
            i=i+1;
            qsort(words,count,sizeof(countsOfWords),cmpfunc);
            //��������ά������
        }
    }
    return parent;
}

int cmpfunc (const void * a, const void * b)
{
   countsOfWords *c,*d;
   c=(countsOfWords *)a;
   d=(countsOfWords *)b;
   if (c->count>=d->count)
   {
       return 1;
   }
   else
   {
       return -1;
   }
   //��С��������
}

Node* createNode(void)
{
    Node* cur;
    cur=malloc(sizeof(Node));
    cur->lk=NULL;
    cur->parent=NULL;
    cur->rk=NULL;
    return cur;
}

char** encoding(countsOfWords* words,Node* root)
{
    char** coding;
    char code[100]={};
    int i,j;
    code[0]='1';
    coding=malloc(sizeof(char*)*256);
    i=0;
    //Ϊÿ���ַ�����һ�����飬���ڴ����
    while (i<256)
    {
        coding[i]=malloc(sizeof(char)*20);
        j=0;
        while (j<20)
        {
            coding[i][j]=0;
            j=j+1;
        }
        i=i+1;
    }
    dfs(coding,root,0,code);
    return coding;
}

void dfs(char** words,Node* cur,int i,char code[])
{
    if (cur->lk!=NULL)
    {
        code[i+1]='0';
        //��������0
        dfs(words,cur->lk,i+1,code);
    }
    if (cur->rk!=NULL)
    {
        code[i+1]='1';
        //��������1
        dfs(words,cur->rk,i+1,code);
    }
    if (cur->lk==NULL&&cur->rk==NULL)
    {
        code[i+1]=0;
        strcpy(words[cur->word],code);
    }
}

void outputFile(FILE* in,FILE* out,char** words, char* filePath)
{
    int count,i,tmp,j;
    char str[256];
    char buffer[200]={};
    strcat(filePath,".hfm");
    //�����ʽΪxxx.hfm
    out=fopen(filePath,"wb+");
    i=0;
    //�Ȱ������ַ��ı���������������������
    while (i<256)
    {
        j=0;
        tmp=0;
        while (j<strlen(words[i]))
        {
            tmp=tmp<<1;
            if (words[i][j]=='1')
            {
                tmp=tmp+1;
            }
            j=j+1;
        }
        fprintf(out,"%d\n",tmp);
        i=i+1;
    }
    //��ʼ���ļ�
    count=fread(str,sizeof(char),256,in);
    while (count!=0)
    {
        i=0;
        //���ַ���Ӧ�ı�������������
        while (i<count)
        {
            strcat(buffer,words[(int)str[i]+128]);
            //����������еı�����ڵ���8�Ļ���ת��Ϊascii�����
            while (strlen(buffer)>=8)
            {
                tmp=128*(buffer[0]-'0')+64*(buffer[1]-'0')+32*(buffer[2]-'0')+16*(buffer[3]-'0')+8*(buffer[4]-'0')+4*(buffer[5]-'0')+2*(buffer[6]-'0')+1*(buffer[7]-'0');
                j=8;
                int len;
                if (strlen(buffer)>16)
                {
                    len=strlen(buffer);
                }
                else
                {
                    len=16;
                }
                while (j<len)
                {
                    buffer[j-8]=buffer[j];
                    buffer[j]=0;
                    j=j+1;
                }
                fprintf(out,"%c",tmp-128);
            }
            i=i+1;
        }
        count=fread(str,sizeof(char),256,in);
    }
    //��ʣ��Ĳ������
    if (strlen(buffer)>0)
    {
        //printf("%s\n",buffer);
        i=tmp=0;
        while (i<8)
        {
            tmp=tmp<<1;
            if (buffer[i]=='1')
                tmp=tmp+1;
            i=i+1;
        }
        fprintf(out,"%c",tmp-128);
    }
    fclose(out);
    fclose(in);
}

void decode()
{
    FILE *doc=NULL;
    FILE *output=NULL;
    int code[256];
    char* inputFile;
    inputFile=openFile();
    //���ļ�
    doc=fopen(inputFile,"rb+");
    //��ȡ�������
    getCode(doc,code);
    //�����ѹ�ļ�
    outputDecodeFile(doc,code,inputFile);
}

void getCode(FILE* doc,int words[])
{
    int i=0;
    //��ȡ�������
    while (i<256)
    {
        fscanf(doc,"%d",&words[i]);
        i=i+1;
    }
}

void outputDecodeFile(FILE* doc,int words[],char* fileName)
{
    int tmp,i,code,j,count,k,flag,size,test;
    tmp=test=0;
    char outPutFileName[100]={};//����ļ���
    char buffer1[200]={};//������1�����ڴ�ż�������ı���
    char buffer2[100]={};//������2�����ڴ�Ŷ����ַ��ı���
    char code1;//�����ַ�
    FILE *outPut;
    i=j=0;
    //��������ļ���
    while (fileName[i]!='.')
    {
        outPutFileName[j]=fileName[i];
        i=i+1;
        j=j+1;
    }
    strcat(outPutFileName,"(decode).");
    j=j+9;
    i=i+1;
    while (i<strlen(fileName)-4)
    {
        outPutFileName[j]=fileName[i];
        i=i+1;
        j=j+1;
    }
    outPut=fopen(outPutFileName,"wb+");
    //�������з�
    fscanf(doc,"%c",&code1);
    //��ʼ��ʽ��ѹ
    count=fscanf(doc,"%c",&code1);
    //���ļ�û����򻺴���1�ڻ����ַ�ʱһֱ����
    while (count>=0||strlen(buffer1)!=0)
    {
        code=code1+128;
        //+128�����⸺��
        if (code/128==1)
        {
            buffer2[0]='1';
            code=code%128;
        }
        else
        {
            buffer2[0]='0';
        }
        if (code/64==1)
        {
            buffer2[1]='1';
            code=code%64;
        }
        else
        {
            buffer2[1]='0';
        }
        if (code/32==1)
        {
            buffer2[2]='1';
            code=code%32;
        }
        else
        {
            buffer2[2]='0';
        }
        if (code/16==1)
        {
            buffer2[3]='1';
            code=code%16;
        }
        else
        {
            buffer2[3]='0';
        }
        if (code/8==1)
        {
            buffer2[4]='1';
            code=code%8;
        }
        else
        {
            buffer2[4]='0';
        }
        if (code/4==1)
        {
            buffer2[5]='1';
            code=code%4;
        }
        else
        {
            buffer2[5]='0';
        }
        if (code/2==1)
        {
            buffer2[6]='1';
            code=code%2;
        }
        else
        {
            buffer2[6]='0';
        }
        if (code/1==1)
        {
            buffer2[7]='1';
            code=0;
        }
        else
        {
            buffer2[7]='0';
        }
        strcat(buffer1,buffer2);
        i=tmp=0;
        //����������1�����Ƿ���Խ���
        while (i<strlen(buffer1))
        {
            if (buffer1[i]=='1')
            {
                tmp=tmp+1;
            }
            j=0;
            while (j<256)
            {
                if (tmp==words[j]&&tmp!=0)
                {
                    test=test+1;
                    fprintf(outPut,"%c",j-128);
                    k=0;
                    size=strlen(buffer1);
                    while (k<size)
                    {
                        buffer1[k]=buffer1[i+1];
                        buffer1[i+1]=0;
                        k=k+1;
                        i=i+1;
                    }
                    i=-1;
                    tmp=0;
                    break;
                }
                j=j+1;
            }
            tmp=tmp<<1;
            i=i+1;
        }
        if (tmp==0)
        {
            buffer1[0]=0;
        }
        count=fscanf(doc,"%c",&code1);
    }
    fclose(doc);
    fclose(outPut);
}

#endif // HEAD_H_INCLUDED

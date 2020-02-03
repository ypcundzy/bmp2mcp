/*
* 将bmp格式的图片文件转化为mcp格式的图片文件
* 
* argc:
*       2或者2以上的值，在传入大于2个参数的时候只取前两个
* argv:
*       0   :   程序名
*               bmp2mcp
*       1   :   bmp文件名
*               ×××.bmp
*
* BUG:
*       1、读取文件时的逻辑存在问题
* log:
*       1、
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "params.h"
#include "bmp2mcp.h"

#define GO_TO_OUT(__ret,__label) \
do{\
    ret = __ret;\
    goto __label;\
}while(0);


static void print_bmp_head(struct bmp *bmpP);
static void print_data_by_hex(void *data,int size);
static int get_real_file_name(char *file_name);
static int read_bmp_head(int fd,struct bmp *bmpP);
static int read_transh_data(int fd,int size);
static void read_and_print(int fd);
static void *read_line(int fd,struct bmp *bmpP);
static struct mcp *read_data_save_as_items(int fd,struct bmp *bmpP);
static struct mcp_item *save_data_as_items(struct mcp_item *end,int line,void *data,int width);
int main(int argc, char const *argv[])
{
    int ret = 0;
    if(argc<2){
        printf("usage:bmp2mcp file_name\n");
        GO_TO_OUT(-1,out);
    }
    int bmp_fd = open(argv[1],O_RDONLY);
    if(bmp_fd<0){
        printf("open file %s failed",argv[1]);
        GO_TO_OUT(-1,out);
    }
    struct bmp *bmpP = malloc(sizeof(*bmpP));
    if(!bmpP){
        printf("malloc failed \n");
        GO_TO_OUT(-1,out);
    }

    ret = read_bmp_head(bmp_fd,bmpP);
    if(ret < 0){
        printf("read bmp head failed \n");
        GO_TO_OUT(-1,out1);
    }
    // print_bmp_head(bmpP);
    read_transh_data(bmp_fd,bmpP->bitmap_offset-sizeof(*bmpP));
    read_data_save_as_items(bmp_fd,bmpP);

out1:
    free(bmpP);
out:
    return ret;
}

static void print_bmp_head(struct bmp *bmpP)
{
    printf("file_type     = %x\n",bmpP->file_type);
    printf("file_size     = %x\n",bmpP->file_size);
    printf("reserve1      = %x\n",bmpP->reserve1);
    printf("bitmap_offset = %x\n",bmpP->bitmap_offset);
    printf("reserve2      = %x\n",bmpP->reserve2);
    printf("width         = %x\n",bmpP->width);
    printf("height        = %x\n",bmpP->height);
}

static void print_data_by_hex(void *data,int size)
{
    int i;
    for(i = 0;i < size;i++){
        printf("%02x  ",TO_8BIT_P(data)[i]);
        if((i+1)%10==0)
            printf("\n");
    }
    printf("\n");
}

static int get_real_file_name(char *file_name)
{
    /* TO DO */
    return 0;
}

static int read_transh_data(int fd,int size)
{
    int ret = 0;
    int tmp = size;
    void *buf = malloc(size);
    if(!buf){
        printf("malloc failed\n");
        GO_TO_OUT(-1,out);
    }
    do{
        ret = read(fd,buf+ret,size);
        if(ret<=0){
            printf("%s:read failed\n",__func__);
            GO_TO_OUT(-1,out1);
        }
        size -= ret;
    }while(size);
out1:
    free(buf);
out:
    return ret;
}


static int read_bmp_head(int fd,struct bmp *bmpP)
{
    int ret = 0;
    int size = sizeof(*bmpP);
    do{
        ret = read(fd,bmpP+ret,size);
        if(ret<=0){
            printf("%s:read failed\n",__func__);
            GO_TO_OUT(-1,out);
        }
        size -= ret;
    }while(size);

out:
    return ret;    
}
/*
* 从文件中读取一行数据
* fd            :           读取的文件
* bmpP          :           bmpP结构体
* return        :           返回读取后的文件
*/
static void *read_line(int fd,struct bmp *bmpP)
{
    int ret = 0;
    int size = align_up(bmpP->width,32)*4;
    // printf("line size = %d\n",size);
    void *data = malloc(size);
    if(!data){
        printf("malloc failed\n");
        GO_TO_OUT(-1,out);
    }   
    do{
        ret = read(fd,data+ret,size);
        if(ret<=0){
            printf("%s:read failed\n",__func__);
            GO_TO_OUT(-1,out);
        }
        size -= ret;
    }while(size);
    //print_data_by_hex(data,align_up(bmpP->width,8));
    goto out;
out1:
    free(data);
    data = NULL;
out:
    return data;
}

/*
* 从文件中读取数据并组合成mcpP文件结构
* fd            :               bmp文件的文件描述符
* bmpP          :               bmpP结构体
* return        :               返回合成的mcp的结构体指针，失败返回NULL
*/
static struct mcp *read_data_save_as_items(int fd,struct bmp *bmpP)
{
    void *buf = NULL;
    int i;
    struct mcp *mcpP = malloc(sizeof(*mcpP));
    if(!mcpP){
        printf("malloc failed\n");
        goto out;
    }
    struct mcp_item *end = NULL;
    for(i=0;i<bmpP->height;i++){
        // printf("______________%d____________\n",i);
        buf = read_line(fd,bmpP);
        // printf("%d\n",align_up(bmpP->width,32)*4);
        end = save_data_as_items(end,i,buf,bmpP->width);
        free(buf);
    }
    struct mcp_item *p = end->next;
    i = 0;
    printf("#include \"ss_types.h\"\n");
    printf("#include \"ss_mcp.h\"\n\n");
    printf("const struct mcp_item far_car[100] = \n");
    printf("{\n");
    while(1){
        i++;
        printf("    {%-3d,%-3d,%-3d},\n",p->line,p->start,p->end);
        p = p->next;
        if(p == end->next)
            break;
    }
    printf("    {%-3d,%-3d,%-3d},\n",255,255,255);
    printf("};\n");
    printf("%d\n",i);
out:
    return mcpP;    
}


static struct mcp_item *get_mcp_item(int line,int start,int end)
{
    struct mcp_item *itemP = malloc(sizeof(*itemP));
    if(!itemP){
        printf("malloc failed\n");
        return NULL;
    }
    itemP->line  = line;
    itemP->start = start;
    itemP->end   = end;
    itemP->next  = NULL;
    return itemP;
}

/* 
* 将传入的数据解析成mcp_item结构的链表 
* end       :       将生成的item放在end之后，如果为NULL，就不用放
* data      :       需要转换的数据的指针
* size      :       data的数据长度
* return    :       新的end指针
*/
static struct mcp_item *save_data_as_items(struct mcp_item *end,int line,void *data,int width)
{
    int i;/* 控制扫描的字节 */
    int j;/* 控制字节中扫描的位 */
    int item_start = -1;/* 记录item起始 */
    int item_end   = -1;/* 记录item结束 */
    int size  = align_up(width,32)*4;
    struct mcp_item *tmp = NULL;
    for(i=0;i<size;i++){
        for(j=0;j<8 && i*8+j<width;j++){
            if(0 == BIT_8(TO_8BIT_P(data)[i],7-j)){
                if(-1 == item_start){
                    item_start = i*8+j;
                }
            }else{
                if(-1 != item_start && -1 == item_end){
                    item_end = i*8+j;
                    if(NULL != end){
                        tmp         = end->next;
                        end->next   = get_mcp_item(line,item_start,item_end);
                        end         = end->next;
                        end->next   = tmp;
                    }else{
                        end         = get_mcp_item(line,item_start,item_end);
                        end->next   = end;
                    }
                    // printf("line = %d,start = %d,end = %d\n",line,item_start,item_end);
                    item_start  = -1;
                    item_end    = -1;           
                }
            }
        }
        if(i*8+j>=width && -1 != item_start && 0 == BIT_8(TO_8BIT_P(data)[i],7-j-1)){
            item_end = i*8+j;
            if(NULL != end){
                tmp         = end->next;
                end->next   = get_mcp_item(line,item_start,item_end);
                end         = end->next;
                end->next   = tmp;
            }else{
                end         = get_mcp_item(line,item_start,item_end);
                end->next   = end;
            }
            // printf("line = %d,start = %d,end = %d\n",line,item_start,item_end);            
        }
    }
    return end;
}

#ifndef __BMP2MCP_INCLUDE__
#define __BMP2MCP_INCLUDE__

#include "params.h"

struct bmp{
    u16 file_type;/* 文件类型 bmp文件为0x4d42 */
    u32 file_size;/* 文件大小 */
    u32 reserve1;/* 保留，必须为0 */
    u32 bitmap_offset;/* 位图的偏移 */
    u32 reserve2;/* 保留 */
    u32 width;/* 宽度(像素) */
    u32 height;/* 高度(像素) */
}__attribute__((packed));

struct mcp_item{
    u32 line;/* 行号 */
    u32 start;/* 起点 */
    u32 end;/* 终点 */
    struct mcp_item *next;
};

struct mcp{
    u16 file_type;/* 文件类型 mcp文件为0x5e53 */
    u32 file_size;/* 文件大小 */
    u32 width;/* 左右宽度 */
    u32 height;/* 上下高度 */
    struct mcp_item *items;/* 消息 */
};



#endif
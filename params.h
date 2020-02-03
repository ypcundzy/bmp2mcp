#ifndef __PARAMS_INCLUDE__
#define __PARAMS_INCLUDE__

typedef unsigned char u8;
typedef signed char i8;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned int u32;
typedef signed int i32;
#define FALSE 0
#define TRUE  1
#define TO_8BIT(data)	((u8)(data))
#define TO_8BIT_P(p)	((u8*)(p))
#define TO_16BIT(data)	((u16)(data))
#define TO_16BIT_P(p)	((u16*)(p))
#define TO_32BIT(data)	((u32)(data))
#define TO_32BIT_P(p)	((u32*)(p))
#define KEY_DISABLE				(0)
#define KEY_ALLOW				(1)
#define BIT_8(data,num) 		( ( TO_8BIT(data)  & (TO_8BIT(1)<<(num)) ) >> (num))
#define BIT_16(data,num) 		( ( TO_16BIT(data) & (TO_16BIT(1)<<(num)) ) >> (num))
#define BIT_32(data,num) 		( ( TO_32BIT(data) & (TO_32BIT(1)<<(num)) ) >> (num))
#define align_up(dividend,divisor) \
	(dividend%divisor?dividend/divisor+1:dividend/divisor)



#define moMax(a,b)	(a>b?a:b)
#define moMin(a,b)	(a<b?a:b)	


#endif

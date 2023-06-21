#include "debug.h"
#include "uart.h"

typedef char * va_list;  

#ifdef CONFIG_HOST_DARWIN
#define _INTSIZEOF(n)  ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap,v) (ap = (va_list)&v - (7 * _INTSIZEOF(v)))
#define va_arg(ap,t)   (*(t *)((ap += (2 * _INTSIZEOF(t))) - (2 * _INTSIZEOF(t))))
#define va_end(ap)     ( ap = (va_list)0 )
#else // CONFIG_HOST_LINUX
#define _INTSIZEOF(n)  ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap,v) (ap = (va_list)&v - (7 * _INTSIZEOF(v)))
#define va_arg(ap,t)   (*(t *)((ap += (2 * _INTSIZEOF(t))) - (2 * _INTSIZEOF(t))))
#define va_end(ap)     ( ap = (va_list)0 )
#endif

char num_tab[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9','a','b','c','d','e','f'};


void putnum(long num, int base, int flag)
{
    unsigned long m;
    char buf[30];

    char *s = buf + sizeof(buf);
    *--s = '\0';

    if(num < 0 && flag == 1) m = -num;
    else m = (unsigned long)num;

    do{
       *--s = num_tab[m % base];
       m /= base;
    }while(m != 0);

    if(num < 0 && flag == 1) *--s = '-';
    
    uart_puts(s);
}


static int vprintf(const char *fmt, va_list ap)
{
    //move the value of x1 to the addr of 0x00
    asm volatile (
        "mov x10, #0x0"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x1,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    //move the value of x2 to the addr of 0x10
    asm volatile (
        "mov x10, #0x10"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x2,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    //move the value of x3 to the addr of 0x20
    asm volatile (
        "mov x10, #0x20"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x3,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    //move the value of x4 to the addr of 0x30
    asm volatile (
        "mov x10, #0x30"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x4,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    //move the value of x5 to the addr of 0x40
    asm volatile (
        "mov x10, #0x40"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x5,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    //move the value of x6 to the addr of 0x50
    asm volatile (
        "mov x10, #0x50"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x6,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    //move the value of x7 to the addr of 0x60
    asm volatile (
        "mov x10, #0x60"   // 使用mov指令将立即数0x0存储到X10寄存器
    );
    asm volatile (
        "str x7,[x10]"   // 使用str指令将x0寄存器的值存储到X10寄存器的地址中
    );

    int i = 0;

    int addr = 0;


    for(; *fmt != '\0'; fmt++)
    {
        if(*fmt != '%'){
            if (*fmt == '\n')
                uart_putc('\r');
            uart_putc(*fmt);
            continue;      /*终止本次for循环，即本次循环执行到这里不再往下执行，开始下一次for循环*/
        }else{
            i+=1;
        }

        switch (i)
        {
            case 1:
                addr = 0x00;
                break;
            case 2:
                addr = 0x10;
                break;
            case 3:
                addr = 0x20;
                break;
            case 4:
                addr = 0x30;
                break;
            case 5:
                addr = 0x40;
                break;
            case 6:
                addr = 0x50;
                break;
            case 7:
                addr = 0x60;
                break;

            default:
                break;
        }
        fmt++;
        switch(*fmt){
        case 'd': putnum(va_arg(addr, int), 10, 1);break;
        case 'o': putnum(va_arg(addr, unsigned int), 8, 0); break;
        case 'u': putnum(va_arg(addr, unsigned int), 10, 0); break;
        case 'x': putnum(va_arg(addr, unsigned int), 16, 0); break;
        case 'c': uart_putc(va_arg(addr, int)); break;
        case 's': uart_puts((char *)va_arg(addr,int)); break;
        default:
            uart_putc(*fmt);
            break;
        }
    }
    return 0;
}

void printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}
#ifndef MACROS_H
#define MACROS_H
#define SET_BIT(reg, bit) ((reg) |= (1 << (bit)))
#define CLR_BIT(reg, bit) ((reg) &= ~(1 << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^= (1 << (bit)))
#define READ_BIT(reg, bit) (((reg) >> (bit)) & 1)

#define set_bit(reg, bit) SET_BIT(reg, bit)
#define clear_bit(reg, bit) CLR_BIT(reg, bit)
#define toggle_bit(reg, bit) TOGGLE_BIT(reg, bit)
#define read_bit(reg, bit) READ_BIT(reg, bit)
#endif
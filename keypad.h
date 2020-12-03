#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_UP     1
#define KEYPAD_DOWN  (1 << 1)
#define KEYPAD_LEFT  (1 << 2)
#define KEYPAD_RIGHT (1 << 3)

void mcp23x17_write(i2c_t *i2c, uint8_t reg, uint8_t data);
uint8_t mcp23x17_read(i2c_t *i2c, uint8_t reg);
void keypad_init(i2c_t *i2c, rcc_t *rcc);
uint16_t keypad_read(i2c_t *i2c);

#endif

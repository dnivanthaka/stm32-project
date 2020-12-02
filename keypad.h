#ifndef KEYPAD_H
#define KEYPAD_H

void mcp23x17_write(i2c_t *i2c, uint8_t reg, uint8_t data);
uint8_t mcp23x17_read(i2c_t *i2c, uint8_t reg);
void keypad_init(i2c_t *i2c, rcc_t *rcc);
uint16_t keypad_read(i2c_t *i2c);

#endif

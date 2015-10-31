#ifndef __SPI_H__
#define __SPI_H__

void SPI_Init(void);
void SPI_WriteByte(uint8_t b);
void SPI_WriteBuf(void *bufr, size_t n);
uint8_t SPI_ReadByte(void);
void SPI_ReadBuf(void *bufr, size_t n);

#endif

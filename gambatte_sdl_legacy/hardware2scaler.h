//
// Created by littlehui on 2020/11/22.
//

#ifndef GAMBATTE_HARDWARE2SCALER_H
#define GAMBATTE_HARDWARE2SCALER_H

#endif //GAMBATTE_HARDWARE2SCALER_H
#include <stdint.h>
#include <SDL/SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

void gbc_convert_2x(uint16_t* out_buf, uint16_t* in_buf,
                                  uint32_t in_pitch, uint32_t out_pitch);
void gbc_convert_2x_scanline_grid(uint16_t* out_buf, uint16_t* in_buf,
                                  uint32_t in_pitch, uint32_t out_pitch);
#ifdef __cplusplus
}
#endif

//
// Created by littlehui on 2020/11/22.
//
#include "hardware2scaler.h"
#include <SDL/SDL.h>

#define GBC_SCREEN_HEIGHT 144
#define GBC_SCREEN_WIDTH 160
#define bgr555_to_native_16(px) (px)

/* Equivalent to gba_convert(), but performs
 * additional 2x nearest neighbour upscaling
 * of the source image */
void gbc_convert_2x(uint16_t* out_buf, uint16_t* in_buf,
                                  uint32_t in_pitch, uint32_t out_pitch)
{
    uint16_t *src      = in_buf;
    uint16_t *dst      = out_buf;
    /* Buffers are 16bit -> divide by 2 */
    uint32_t src_width = in_pitch  >> 1;
    uint32_t dst_width = out_pitch >> 1;
    size_t x, y;

    for (y = 0; y < GBC_SCREEN_HEIGHT; y++)
    {
        uint16_t *dst_ptr = dst;

        for (x = 0; x < GBC_SCREEN_WIDTH; x++)
        {
            /* Get current (converted) pixel colour */
            uint32_t color = (uint32_t)bgr555_to_native_16(*(src + x));

            /* Double it up to 32bit, so we can set
             * two destination pixels at once
             * > Note: This is not strictly correct;
             *   should use a temporary 2 entry 16bit colour
             *   array and memcpy() it to dst_ptr instead.
             *   But these kinds of pointer tricks are used
             *   everywhere, so might as well do the same
             *   here... */
            color = (color << 16) | color;

            /* Assign colours for current row */
            *(uint32_t*)dst_ptr = color;

            /* Assign colours for next row */
            *(uint32_t*)(dst_ptr + dst_width) = color;

            dst_ptr += 2;
        }

        src += src_width;
        dst += dst_width << 1;
    }
}


/* Equivalent to gba_convert_2x(), but adds
 * a simple grid effect */
void gbc_convert_2x_scanline_grid(uint16_t* out_buf, uint16_t* in_buf,
                                                uint32_t in_pitch, uint32_t out_pitch)
{
    uint16_t *src      = in_buf;
    uint16_t *dst      = out_buf;
    /* Buffers are 16bit -> divide by 2 */
    uint32_t src_width = in_pitch  >> 1;
    uint32_t dst_width = out_pitch >> 1;
    size_t x, y;
    //160 X 144
    for (y = 0; y < GBC_SCREEN_HEIGHT; y++)
    {
        uint16_t *dst_ptr = dst;

        for (x = 0; x < GBC_SCREEN_WIDTH; x++)
        {

            uint32_t color = (uint32_t)bgr555_to_native_16(*(src + x));
            uint32_t scanline_color = (color + (color & 0x7000)) >> 1;

            scanline_color = (color + scanline_color + ((color ^ scanline_color) & 0x0421)) >> 1;
            // result = (X + Y - ((X ^ Y) & 0x0421)) >> 1;
            //scanline_color = (color + scanline_color - ((color ^ scanline_color) & 0x0421)) >> 1;

            /* c.f "Mixing Packed RGB Pixels Efficiently"
             * http://blargg.8bitalley.com/info/rgb_mixing.html */

            /* Convert colours */
            color          = bgr555_to_native_16(color);
            scanline_color = bgr555_to_native_16(scanline_color);

            /* Assign colours for current row
             * - <colour>|<scanline_color> */
            *(uint32_t*)dst_ptr = (color << 16) | scanline_color;

            /* Assign colours for next row
             * - <scanline_color>|<scanline_color> */
            *(uint32_t*)(dst_ptr + dst_width) = (scanline_color << 16) | scanline_color;
            dst_ptr += 2;
        }
        src += src_width;
        dst += dst_width << 1;
    }
}

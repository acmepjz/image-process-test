/*====================================================================*
 -  Copyright (C) 2001 Leptonica.  All rights reserved.
 -
 -  Redistribution and use in source and binary forms, with or without
 -  modification, are permitted provided that the following conditions
 -  are met:
 -  1. Redistributions of source code must retain the above copyright
 -     notice, this list of conditions and the following disclaimer.
 -  2. Redistributions in binary form must reproduce the above
 -     copyright notice, this list of conditions and the following
 -     disclaimer in the documentation and/or other materials
 -     provided with the distribution.
 -
 -  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 -  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 -  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 -  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL ANY
 -  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 -  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 -  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 -  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 -  OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 -  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 -  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *====================================================================*/

/*!
 *     Low-level fast binary morphology with auto-generated sels
 *
 *      Dispatcher:
 *             l_int32    fmorphopgen_low_102()
 *
 *      Static Low-level:
 *             void       fdilate_102_*()
 *             void       ferode_102_*()
 */

#include "allheaders.h"

static void  fdilate_102_0(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_0(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_1(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_1(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_2(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_2(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_3(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_3(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_4(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_4(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_5(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_5(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_6(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_6(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_7(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_7(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_8(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_8(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_9(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_9(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  fdilate_102_10(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);
static void  ferode_102_10(l_uint32 *, l_int32, l_int32, l_int32, l_uint32 *, l_int32);


/*---------------------------------------------------------------------*
 *                          Fast morph dispatcher                      *
 *---------------------------------------------------------------------*/
/*!
 *  fmorphopgen_low_102()
 *
 *       a dispatcher to appropriate low-level code
 */
l_int32
fmorphopgen_low_102(l_uint32  *datad,
                  l_int32    w,
                  l_int32    h,
                  l_int32    wpld,
                  l_uint32  *datas,
                  l_int32    wpls,
                  l_int32    index)
{

    switch (index)
    {
    case 0:
        fdilate_102_0(datad, w, h, wpld, datas, wpls);
        break;
    case 1:
        ferode_102_0(datad, w, h, wpld, datas, wpls);
        break;
    case 2:
        fdilate_102_1(datad, w, h, wpld, datas, wpls);
        break;
    case 3:
        ferode_102_1(datad, w, h, wpld, datas, wpls);
        break;
    case 4:
        fdilate_102_2(datad, w, h, wpld, datas, wpls);
        break;
    case 5:
        ferode_102_2(datad, w, h, wpld, datas, wpls);
        break;
    case 6:
        fdilate_102_3(datad, w, h, wpld, datas, wpls);
        break;
    case 7:
        ferode_102_3(datad, w, h, wpld, datas, wpls);
        break;
    case 8:
        fdilate_102_4(datad, w, h, wpld, datas, wpls);
        break;
    case 9:
        ferode_102_4(datad, w, h, wpld, datas, wpls);
        break;
    case 10:
        fdilate_102_5(datad, w, h, wpld, datas, wpls);
        break;
    case 11:
        ferode_102_5(datad, w, h, wpld, datas, wpls);
        break;
    case 12:
        fdilate_102_6(datad, w, h, wpld, datas, wpls);
        break;
    case 13:
        ferode_102_6(datad, w, h, wpld, datas, wpls);
        break;
    case 14:
        fdilate_102_7(datad, w, h, wpld, datas, wpls);
        break;
    case 15:
        ferode_102_7(datad, w, h, wpld, datas, wpls);
        break;
    case 16:
        fdilate_102_8(datad, w, h, wpld, datas, wpls);
        break;
    case 17:
        ferode_102_8(datad, w, h, wpld, datas, wpls);
        break;
    case 18:
        fdilate_102_9(datad, w, h, wpld, datas, wpls);
        break;
    case 19:
        ferode_102_9(datad, w, h, wpld, datas, wpls);
        break;
    case 20:
        fdilate_102_10(datad, w, h, wpld, datas, wpls);
        break;
    case 21:
        ferode_102_10(datad, w, h, wpld, datas, wpls);
        break;
    }

    return 0;
}


/*--------------------------------------------------------------------------*
 *                 Low-level auto-generated static routines                 *
 *--------------------------------------------------------------------------*/
/*
 *  N.B.  In all the low-level routines, the part of the image
 *        that is accessed has been clipped by 32 pixels on
 *        all four sides.  This is done in the higher level
 *        code by redefining w and h smaller and by moving the
 *        start-of-image pointers up to the beginning of this
 *        interior rectangle.
 */
static void
fdilate_102_0(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) |
                    ((*(sptr) >> 1) | (*(sptr - 1) << 31));
        }
    }
}

static void
ferode_102_0(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) &
                    ((*(sptr) << 1) | (*(sptr + 1) >> 31));
        }
    }
}

static void
fdilate_102_1(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) |
                    (*(sptr - wpls));
        }
    }
}

static void
ferode_102_1(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) &
                    (*(sptr + wpls));
        }
    }
}

static void
fdilate_102_2(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) |
                    ((*(sptr - wpls) >> 1) | (*(sptr - wpls - 1) << 31));
        }
    }
}

static void
ferode_102_2(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) &
                    ((*(sptr + wpls) << 1) | (*(sptr + wpls + 1) >> 31));
        }
    }
}

static void
fdilate_102_3(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) |
                    ((*(sptr - wpls) << 1) | (*(sptr - wpls + 1) >> 31));
        }
    }
}

static void
ferode_102_3(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) &
                    ((*(sptr + wpls) >> 1) | (*(sptr + wpls - 1) << 31));
        }
    }
}

static void
fdilate_102_4(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = ((*(sptr) << 1) | (*(sptr + 1) >> 31)) |
                    (*sptr) |
                    ((*(sptr) >> 1) | (*(sptr - 1) << 31));
        }
    }
}

static void
ferode_102_4(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = ((*(sptr) >> 1) | (*(sptr - 1) << 31)) &
                    (*sptr) &
                    ((*(sptr) << 1) | (*(sptr + 1) >> 31));
        }
    }
}

static void
fdilate_102_5(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*(sptr + wpls)) |
                    (*sptr) |
                    (*(sptr - wpls));
        }
    }
}

static void
ferode_102_5(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*(sptr - wpls)) &
                    (*sptr) &
                    (*(sptr + wpls));
        }
    }
}

static void
fdilate_102_6(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) |
                    ((*(sptr) >> 1) | (*(sptr - 1) << 31)) |
                    (*(sptr - wpls));
        }
    }
}

static void
ferode_102_6(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) &
                    ((*(sptr) << 1) | (*(sptr + 1) >> 31)) &
                    (*(sptr + wpls));
        }
    }
}

static void
fdilate_102_7(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = ((*(sptr) << 1) | (*(sptr + 1) >> 31)) |
                    (*sptr) |
                    (*(sptr - wpls));
        }
    }
}

static void
ferode_102_7(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = ((*(sptr) >> 1) | (*(sptr - 1) << 31)) &
                    (*sptr) &
                    (*(sptr + wpls));
        }
    }
}

static void
fdilate_102_8(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*(sptr + wpls)) |
                    (*sptr) |
                    ((*(sptr) >> 1) | (*(sptr - 1) << 31));
        }
    }
}

static void
ferode_102_8(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*(sptr - wpls)) &
                    (*sptr) &
                    ((*(sptr) << 1) | (*(sptr + 1) >> 31));
        }
    }
}

static void
fdilate_102_9(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*(sptr + wpls)) |
                    ((*(sptr) << 1) | (*(sptr + 1) >> 31)) |
                    (*sptr);
        }
    }
}

static void
ferode_102_9(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*(sptr - wpls)) &
                    ((*(sptr) >> 1) | (*(sptr - 1) << 31)) &
                    (*sptr);
        }
    }
}

static void
fdilate_102_10(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) |
                    ((*(sptr) >> 1) | (*(sptr - 1) << 31)) |
                    (*(sptr - wpls)) |
                    ((*(sptr - wpls) >> 1) | (*(sptr - wpls - 1) << 31));
        }
    }
}

static void
ferode_102_10(l_uint32  *datad,
            l_int32    w,
            l_int32    h,
            l_int32    wpld,
            l_uint32  *datas,
            l_int32    wpls)
{
l_int32             i;
register l_int32    j, pwpls;
register l_uint32  *sptr, *dptr;

    pwpls = (l_uint32)(w + 31) / 32;  /* proper wpl of src */

    for (i = 0; i < h; i++) {
        sptr = datas + i * wpls;
        dptr = datad + i * wpld;
        for (j = 0; j < pwpls; j++, sptr++, dptr++) {
            *dptr = (*sptr) &
                    ((*(sptr) << 1) | (*(sptr + 1) >> 31)) &
                    (*(sptr + wpls)) &
                    ((*(sptr + wpls) << 1) | (*(sptr + wpls + 1) >> 31));
        }
    }
}


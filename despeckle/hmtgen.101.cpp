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
 *      Top-level fast hit-miss transform with auto-generated sels
 *
 *             PIX     *pixHMTDwa_101()
 *             PIX     *pixFHMTGen_101()
 */

#include <string.h>
#include "allheaders.h"

PIX *pixHMTDwa_101(PIX *pixd, PIX *pixs, const char *selname);
PIX *pixFHMTGen_101(PIX *pixd, PIX *pixs, const char *selname);
l_int32 fhmtgen_low_101(l_uint32 *datad, l_int32 w,
                      l_int32 h, l_int32 wpld,
                      l_uint32 *datas, l_int32 wpls,
                      l_int32 index);

static l_int32   NUM_SELS_GENERATED = 29;
static char  SEL_NAMES[][80] = {
                             "3hm",
                             "5hm",
                             "3hm2_h",
                             "3hm2_v",
                             "3hm2_d1",
                             "3hm2_d2",
                             "3hm3_h",
                             "3hm3_v",
                             "3hm3_c1",
                             "3hm3_c2",
                             "3hm3_c3",
                             "3hm3_c4",
                             "3hm4_c",
                             "st_av1",
                             "st_ah1",
                             "st_av2",
                             "st_ah2",
                             "st_bv1",
                             "st_bh1",
                             "st_bv2",
                             "st_bh2",
                             "st_cv1",
                             "st_ch1",
                             "st_cv2",
                             "st_ch2",
                             "st_dv1",
                             "st_dh1",
                             "st_dv2",
                             "st_dh2"};

/*!
 *  pixHMTDwa_101()
 *
 *      Input:  pixd (usual 3 choices: null, == pixs, != pixs)
 *              pixs (1 bpp)
 *              sel name
 *      Return: pixd
 *
 *  Notes:
 *      (1) This simply adds a 32 pixel border, calls the appropriate
 *          pixFHMTGen_*(), and removes the border.
 *          See notes below for that function.
 */
PIX *
pixHMTDwa_101(PIX         *pixd,
            PIX         *pixs,
            const char  *selname)
{
PIX  *pixt1, *pixt2, *pixt3;

    PROCNAME("pixHMTDwa_101");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, pixd);
    if (pixGetDepth(pixs) != 1)
        return (PIX *)ERROR_PTR("pixs must be 1 bpp", procName, pixd);

    pixt1 = pixAddBorder(pixs, 32, 0);
    pixt2 = pixFHMTGen_101(NULL, pixt1, selname);
    pixt3 = pixRemoveBorder(pixt2, 32);
    pixDestroy(&pixt1);
    pixDestroy(&pixt2);

    if (!pixd)
        return pixt3;

    pixCopy(pixd, pixt3);
    pixDestroy(&pixt3);
    return pixd;
}


/*!
 *  pixFHMTGen_101()
 *
 *      Input:  pixd (usual 3 choices: null, == pixs, != pixs)
 *              pixs (1 bpp)
 *              sel name
 *      Return: pixd
 *
 *  Notes:
 *      (1) This is a dwa implementation of the hit-miss transform
 *          on pixs by the sel.
 *      (2) The sel must be limited in size to not more than 31 pixels
 *          about the origin.  It must have at least one hit, and it
 *          can have any number of misses.
 *      (3) This handles all required setting of the border pixels
 *          before erosion and dilation.
 */
PIX *
pixFHMTGen_101(PIX         *pixd,
             PIX         *pixs,
             const char  *selname)
{
l_int32    i, index, found, w, h, wpls, wpld;
l_uint32  *datad, *datas, *datat;
PIX       *pixt;

    PROCNAME("pixFHMTGen_101");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, pixd);
    if (pixGetDepth(pixs) != 1)
        return (PIX *)ERROR_PTR("pixs must be 1 bpp", procName, pixd);

    found = FALSE;
    for (i = 0; i < NUM_SELS_GENERATED; i++) {
        if (strcmp(selname, SEL_NAMES[i]) == 0) {
            found = TRUE;
            index = i;
            break;
        }
    }
    if (found == FALSE)
        return (PIX *)ERROR_PTR("sel index not found", procName, pixd);

    if (!pixd) {
        if ((pixd = pixCreateTemplate(pixs)) == NULL)
            return (PIX *)ERROR_PTR("pixd not made", procName, NULL);
    }
    else  /* for in-place or pre-allocated */
        pixResizeImageData(pixd, pixs);
    wpls = pixGetWpl(pixs);
    wpld = pixGetWpl(pixd);

        /*  The images must be surrounded with 32 additional border
         *  pixels, that we'll read from.  We fabricate a "proper"
         *  image as the subimage within the border, having the
         *  following parameters:  */
    w = pixGetWidth(pixs) - 64;
    h = pixGetHeight(pixs) - 64;
    datas = pixGetData(pixs) + 32 * wpls + 1;
    datad = pixGetData(pixd) + 32 * wpld + 1;

    if (pixd == pixs) {  /* need temp image if in-place */
        if ((pixt = pixCopy(NULL, pixs)) == NULL)
            return (PIX *)ERROR_PTR("pixt not made", procName, pixd);
        datat = pixGetData(pixt) + 32 * wpls + 1;
        fhmtgen_low_101(datad, w, h, wpld, datat, wpls, index);
        pixDestroy(&pixt);
    }
    else {  /* not in-place */
        fhmtgen_low_101(datad, w, h, wpld, datas, wpls, index);
    }

    return pixd;
}


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
 *      Top-level fast binary morphology with auto-generated sels
 *
 *             PIX     *pixMorphDwa_102()
 *             PIX     *pixFMorphopGen_102()
 */

#include <string.h>
#include "allheaders.h"

PIX *pixMorphDwa_102(PIX *pixd, PIX *pixs, l_int32 operation, const char *selname);
PIX *pixFMorphopGen_102(PIX *pixd, PIX *pixs, l_int32 operation, const char *selname);
l_int32 fmorphopgen_low_102(l_uint32 *datad, l_int32 w,
                          l_int32 h, l_int32 wpld,
                          l_uint32 *datas, l_int32 wpls,
                          l_int32 index);

static l_int32   NUM_SELS_GENERATED = 11;
static char  SEL_NAMES[][80] = {
                             "morph2_h",
                             "morph2_v",
                             "morph2_d1",
                             "morph2_d2",
                             "morph3_h",
                             "morph3_v",
                             "morph3_c1",
                             "morph3_c2",
                             "morph3_c3",
                             "morph3_c4",
                             "morph4_c"};

/*!
 *  pixMorphDwa_102()
 *
 *      Input:  pixd (usual 3 choices: null, == pixs, != pixs)
 *              pixs (1 bpp)
 *              operation  (L_MORPH_DILATE, L_MORPH_ERODE,
 *                          L_MORPH_OPEN, L_MORPH_CLOSE)
 *              sel name
 *      Return: pixd
 *
 *  Notes:
 *      (1) This simply adds a border, calls the appropriate
 *          pixFMorphopGen_*(), and removes the border.
 *          See the notes for that function.
 *      (2) The size of the border depends on the operation
 *          and the boundary conditions.
 */
PIX *
pixMorphDwa_102(PIX     *pixd,
              PIX     *pixs,
              l_int32  operation,
              const char    *selname)
{
l_int32  bordercolor, bordersize;
PIX     *pixt1, *pixt2, *pixt3;

    PROCNAME("pixMorphDwa_102");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, pixd);
    if (pixGetDepth(pixs) != 1)
        return (PIX *)ERROR_PTR("pixs must be 1 bpp", procName, pixd);

        /* Set the border size */
    bordercolor = getMorphBorderPixelColor(L_MORPH_ERODE, 1);
    bordersize = 32;
    if (bordercolor == 0 && operation == L_MORPH_CLOSE)
        bordersize += 32;

    pixt1 = pixAddBorder(pixs, bordersize, 0);
    pixt2 = pixFMorphopGen_102(NULL, pixt1, operation, selname);
    pixt3 = pixRemoveBorder(pixt2, bordersize);
    pixDestroy(&pixt1);
    pixDestroy(&pixt2);

    if (!pixd)
        return pixt3;

    pixCopy(pixd, pixt3);
    pixDestroy(&pixt3);
    return pixd;
}


/*!
 *  pixFMorphopGen_102()
 *
 *      Input:  pixd (usual 3 choices: null, == pixs, != pixs)
 *              pixs (1 bpp)
 *              operation  (L_MORPH_DILATE, L_MORPH_ERODE,
 *                          L_MORPH_OPEN, L_MORPH_CLOSE)
 *              sel name
 *      Return: pixd
 *
 *  Notes:
 *      (1) This is a dwa operation, and the Sels must be limited in
 *          size to not more than 31 pixels about the origin.
 *      (2) A border of appropriate size (32 pixels, or 64 pixels
 *          for safe closing with asymmetric b.c.) must be added before
 *          this function is called.
 *      (3) This handles all required setting of the border pixels
 *          before erosion and dilation.
 *      (4) The closing operation is safe; no pixels can be removed
 *          near the boundary.
 */
PIX *
pixFMorphopGen_102(PIX     *pixd,
                 PIX     *pixs,
                 l_int32  operation,
                 const char    *selname)
{
l_int32    i, index, found, w, h, wpls, wpld, bordercolor, erodeop, borderop;
l_uint32  *datad, *datas, *datat;
PIX       *pixt;

    PROCNAME("pixFMorphopGen_102");

    if (!pixs)
        return (PIX *)ERROR_PTR("pixs not defined", procName, pixd);
    if (pixGetDepth(pixs) != 1)
        return (PIX *)ERROR_PTR("pixs must be 1 bpp", procName, pixd);

        /* Get boundary colors to use */
    bordercolor = getMorphBorderPixelColor(L_MORPH_ERODE, 1);
    if (bordercolor == 1)
        erodeop = PIX_SET;
    else
        erodeop = PIX_CLR;

    found = FALSE;
    for (i = 0; i < NUM_SELS_GENERATED; i++) {
        if (strcmp(selname, SEL_NAMES[i]) == 0) {
            found = TRUE;
            index = 2 * i;
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

        /* The images must be surrounded, in advance, with a border of
         * size 32 pixels (or 64, for closing), that we'll read from.
         * Fabricate a "proper" image as the subimage within the 32
         * pixel border, having the following parameters:  */
    w = pixGetWidth(pixs) - 64;
    h = pixGetHeight(pixs) - 64;
    datas = pixGetData(pixs) + 32 * wpls + 1;
    datad = pixGetData(pixd) + 32 * wpld + 1;

    if (operation == L_MORPH_DILATE || operation == L_MORPH_ERODE) {
        borderop = PIX_CLR;
        if (operation == L_MORPH_ERODE) {
            borderop = erodeop;
            index++;
        }
        if (pixd == pixs) {  /* in-place; generate a temp image */
            if ((pixt = pixCopy(NULL, pixs)) == NULL)
                return (PIX *)ERROR_PTR("pixt not made", procName, pixd);
            datat = pixGetData(pixt) + 32 * wpls + 1;
            pixSetOrClearBorder(pixt, 32, 32, 32, 32, borderop);
            fmorphopgen_low_102(datad, w, h, wpld, datat, wpls, index);
            pixDestroy(&pixt);
        }
        else { /* not in-place */
            pixSetOrClearBorder(pixs, 32, 32, 32, 32, borderop);
            fmorphopgen_low_102(datad, w, h, wpld, datas, wpls, index);
        }
    }
    else {  /* opening or closing; generate a temp image */
        if ((pixt = pixCreateTemplate(pixs)) == NULL)
            return (PIX *)ERROR_PTR("pixt not made", procName, pixd);
        datat = pixGetData(pixt) + 32 * wpls + 1;
        if (operation == L_MORPH_OPEN) {
            pixSetOrClearBorder(pixs, 32, 32, 32, 32, erodeop);
            fmorphopgen_low_102(datat, w, h, wpls, datas, wpls, index+1);
            pixSetOrClearBorder(pixt, 32, 32, 32, 32, PIX_CLR);
            fmorphopgen_low_102(datad, w, h, wpld, datat, wpls, index);
        }
        else {  /* closing */
            pixSetOrClearBorder(pixs, 32, 32, 32, 32, PIX_CLR);
            fmorphopgen_low_102(datat, w, h, wpls, datas, wpls, index);
            pixSetOrClearBorder(pixt, 32, 32, 32, 32, erodeop);
            fmorphopgen_low_102(datad, w, h, wpld, datat, wpls, index+1);
        }
        pixDestroy(&pixt);
    }

    return pixd;
}


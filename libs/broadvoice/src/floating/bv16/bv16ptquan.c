/*
 * broadvoice - a library for the BroadVoice 16 and 32 codecs
 *
 * ptquan.c - Pitch quantizer
 *
 * Adapted by Steve Underwood <steveu@coppice.org> from code which is
 * Copyright 2000-2009 Broadcom Corporation
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * $Id: bv16ptquan.c,v 1.1.1.1 2009/11/19 12:10:48 steveu Exp $
 */

/*! \file */

#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include "typedef.h"
#include "bv16cnst.h"
#include "bv16externs.h"

int pitchtapquan(
    Float *x,
    int	pp,
    Float *b,
    Float *re)
{
    Float p[9], t, s0, s1, s2, cormax, cor;
    Float t0, t1, t2;
    Float *xt;
    Float *fp0;
    Float *fp1;
    const Float *fp2;
    int	ppm2, qidx=0, i, j;

    ppm2 = pp - 2;
    xt = x + XOFF;

    for (i = 0;  i < 3;  i++)
    {
        fp0 = xt;
        fp1 = x + XOFF - ppm2 - i - 1;
        t = 0;
        for (j = 0;  j < FRSZ;  j++)
            t += (*fp0++) * (*fp1++);
        p[i] = t;
    }

    fp0 = x+XOFF-ppm2-3;
    s0 = *fp0++;
    s1 = *fp0++;
    s2 = *fp0--;
    t0 = p[8] = s0*s0;
    t1 = p[4] = s0*s1;
    p[5] = s0*s2;
    s0 = *fp0++;
    s1 = *fp0++;
    s2 = *fp0--;
    t2 = s0*s0;
    p[8] += t2;
    p[4] += s0*s1;
    p[5] += s0*s2;
    for (i = 0;  i < (FRSZ - 2);  i++)
    {
        s0 = *fp0++;
        s1 = *fp0++;
        s2 = *fp0--;
        p[8] += s0*s0;
        p[4] += s0*s1;
        p[5] += s0*s2;
    }
    s0 = *fp0++;
    s1 = *fp0++;
    s2 = *fp0--;
    p[7] = p[8] + (s0*s0) - t0;
    p[3] = p[4] + (s0*s1) - t1;
    p[6] = p[7] + (s1*s1) - t2;

    cormax = -1.0e30;
    fp2 = bv16_pp9cb;
    for (i = 0;  i < PPCBSZ;  i++)
    {
        cor = 0.0;
        fp1 = p;
        for (j = 0;  j < 9;  j++)
            cor += (*fp2++)*(*fp1++);
        if (cor > cormax)
        {
            cormax = cor;
            qidx = i;
        }
    }

    fp2 = bv16_pp9cb + qidx*9;
    for (i = 0;  i < 3;  i++)
        b[i] = fp2[i]*0.5;

    fp0 = x + XOFF;
    fp1 = x + XOFF - ppm2 - 1;
    t = 0;
    for (i = 0;  i < FRSZ;  i++)
    {
        t0 = *fp0++ - b[0]*fp1[0] - b[1]*fp1[-1] - b[2]*fp1[-2];
        fp1++;
        t += t0*t0;
    }
    *re = t;
    return qidx;
}


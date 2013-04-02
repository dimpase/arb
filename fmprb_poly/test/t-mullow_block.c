/*=============================================================================

    This file is part of ARB.

    ARB is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ARB is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ARB; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2012, 2013 Fredrik Johansson

******************************************************************************/

#include "fmprb_poly.h"

int main()
{
    long iter;
    flint_rand_t state;

    printf("mullow_block....");
    fflush(stdout);

    flint_randinit(state);

    /* compare with fmpq_poly */
    for (iter = 0; iter < 10000; iter++)
    {
        long qbits1, qbits2, rbits1, rbits2, rbits3, trunc;
        fmpq_poly_t A, B, C;
        fmprb_poly_t a, b, c, d;

        qbits1 = 2 + n_randint(state, 1000);
        qbits2 = 2 + n_randint(state, 1000);
        rbits1 = 2 + n_randint(state, 1000);
        rbits2 = 2 + n_randint(state, 1000);
        rbits3 = 2 + n_randint(state, 1000);
        trunc = n_randint(state, 100);

        fmpq_poly_init(A);
        fmpq_poly_init(B);
        fmpq_poly_init(C);

        fmprb_poly_init(a);
        fmprb_poly_init(b);
        fmprb_poly_init(c);
        fmprb_poly_init(d);

        fmpq_poly_randtest(A, state, 1 + n_randint(state, 100), qbits1);
        fmpq_poly_randtest(B, state, 1 + n_randint(state, 100), qbits2);
        fmpq_poly_mullow(C, A, B, trunc);

        fmprb_poly_set_fmpq_poly(a, A, rbits1);
        fmprb_poly_set_fmpq_poly(b, B, rbits2);
        fmprb_poly_mullow_block(c, a, b, trunc, rbits3);

        if (!fmprb_poly_contains_fmpq_poly(c, C))
        {
            printf("FAIL\n\n");
            printf("bits3 = %ld\n", rbits3);
            printf("trunc = %ld\n", trunc);

            printf("A = "); fmpq_poly_print(A); printf("\n\n");
            printf("B = "); fmpq_poly_print(B); printf("\n\n");
            printf("C = "); fmpq_poly_print(C); printf("\n\n");

            printf("a = "); fmprb_poly_printd(a, 15); printf("\n\n");
            printf("b = "); fmprb_poly_printd(b, 15); printf("\n\n");
            printf("c = "); fmprb_poly_printd(c, 15); printf("\n\n");

            abort();
        }

        fmprb_poly_set(d, a);
        fmprb_poly_mullow_block(d, d, b, trunc, rbits3);
        if (!fmprb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 1)\n\n");
            abort();
        }

        fmprb_poly_set(d, b);
        fmprb_poly_mullow_block(d, a, d, trunc, rbits3);
        if (!fmprb_poly_equal(d, c))
        {
            printf("FAIL (aliasing 2)\n\n");
            abort();
        }

        fmpq_poly_clear(A);
        fmpq_poly_clear(B);
        fmpq_poly_clear(C);

        fmprb_poly_clear(a);
        fmprb_poly_clear(b);
        fmprb_poly_clear(c);
        fmprb_poly_clear(d);
    }

    flint_randclear(state);
    _fmpz_cleanup();
    printf("PASS\n");
    return EXIT_SUCCESS;
}


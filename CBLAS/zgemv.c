
/*  -- translated by f2c (version 19940927).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/
#include <string.h>
#include "f2c.h"

/* Subroutine */ void zgemv_(char *trans, integer *m, integer *n, 
	doublecomplex *alpha, doublecomplex *a, integer *lda, doublecomplex *
	x, integer *incx, doublecomplex *beta, doublecomplex *y, integer *
	incy)
{


    /* System generated locals */

    doublecomplex z__1, z__2, z__3;

    /* Builtin functions */
    void d_cnjg(doublecomplex *, doublecomplex *);

    /* Local variables */
    integer info;
    doublecomplex temp;
    integer lenx, leny, i, j;
    integer ix, iy, jx, jy, kx, ky;
    logical noconj;

    extern int input_error(char *, int *);

/*  Purpose   
    =======   

    ZGEMV  performs one of the matrix-vector operations   

       y := alpha*A*x + beta*y,   or   y := alpha*A'*x + beta*y,   or   

       y := alpha*conjg( A' )*x + beta*y,   

    where alpha and beta are scalars, x and y are vectors and A is an   
    m by n matrix.   

    Parameters   
    ==========   

    TRANS  - CHARACTER*1.   
             On entry, TRANS specifies the operation to be performed as   
             follows:   

                TRANS = 'N' or 'n'   y := alpha*A*x + beta*y.   

                TRANS = 'T' or 't'   y := alpha*A'*x + beta*y.   

                TRANS = 'C' or 'c'   y := alpha*conjg( A' )*x + beta*y.   

             Unchanged on exit.   

    M      - INTEGER.   
             On entry, M specifies the number of rows of the matrix A.   
             M must be at least zero.   
             Unchanged on exit.   

    N      - INTEGER.   
             On entry, N specifies the number of columns of the matrix A. 
  
             N must be at least zero.   
             Unchanged on exit.   

    ALPHA  - COMPLEX*16      .   
             On entry, ALPHA specifies the scalar alpha.   
             Unchanged on exit.   

    A      - COMPLEX*16       array of DIMENSION ( LDA, n ).   
             Before entry, the leading m by n part of the array A must   
             contain the matrix of coefficients.   
             Unchanged on exit.   

    LDA    - INTEGER.   
             On entry, LDA specifies the first dimension of A as declared 
  
             in the calling (sub) program. LDA must be at least   
             max( 1, m ).   
             Unchanged on exit.   

    X      - COMPLEX*16       array of DIMENSION at least   
             ( 1 + ( n - 1 )*abs( INCX ) ) when TRANS = 'N' or 'n'   
             and at least   
             ( 1 + ( m - 1 )*abs( INCX ) ) otherwise.   
             Before entry, the incremented array X must contain the   
             vector x.   
             Unchanged on exit.   

    INCX   - INTEGER.   
             On entry, INCX specifies the increment for the elements of   
             X. INCX must not be zero.   
             Unchanged on exit.   

    BETA   - COMPLEX*16      .   
             On entry, BETA specifies the scalar beta. When BETA is   
             supplied as zero then Y need not be set on input.   
             Unchanged on exit.   

    Y      - COMPLEX*16       array of DIMENSION at least   
             ( 1 + ( m - 1 )*abs( INCY ) ) when TRANS = 'N' or 'n'   
             and at least   
             ( 1 + ( n - 1 )*abs( INCY ) ) otherwise.   
             Before entry with BETA non-zero, the incremented array Y   
             must contain the vector y. On exit, Y is overwritten by the 
  
             updated vector y.   

    INCY   - INTEGER.   
             On entry, INCY specifies the increment for the elements of   
             Y. INCY must not be zero.   
             Unchanged on exit.   


    Level 2 Blas routine.   

    -- Written on 22-October-1986.   
       Jack Dongarra, Argonne National Lab.   
       Jeremy Du Croz, Nag Central Office.   
       Sven Hammarling, Nag Central Office.   
       Richard Hanson, Sandia National Labs.   

       Test the input parameters.   

    
   Parameter adjustments   
       Function Body */
#define X(I) x[(I)-1]
#define Y(I) y[(I)-1]

#define A(I,J) a[(I)-1 + ((J)-1)* ( *lda)]

    info = 0;
    if ( strncmp(trans, "N", 1)!=0 && strncmp(trans, "T", 1)!=0 &&
	 strncmp(trans, "C", 1)!=0 ) {
	info = 1;
    } else if (*m < 0) {
	info = 2;
    } else if (*n < 0) {
	info = 3;
    } else if (*lda < max(1,*m)) {
	info = 6;
    } else if (*incx == 0) {
	info = 8;
    } else if (*incy == 0) {
	info = 11;
    }
    if (info != 0) {
	input_error("ZGEMV ", &info);
	return;
    }

/*     Quick return if possible. */

    if (*m == 0 || *n == 0 || (alpha->r == 0. && alpha->i == 0.
			       && beta->r == 1. && beta->i == 0.)) {
	return;
    }

    noconj = ( strncmp(trans, "T", 1)==0 );

/*     Set  LENX  and  LENY, the lengths of the vectors x and y, and set 
       up the start points in  X  and  Y. */

    if (strncmp(trans, "N", 1)==0) {
	lenx = *n;
	leny = *m;
    } else {
	lenx = *m;
	leny = *n;
    }
    if (*incx > 0) {
	kx = 1;
    } else {
	kx = 1 - (lenx - 1) * *incx;
    }
    if (*incy > 0) {
	ky = 1;
    } else {
	ky = 1 - (leny - 1) * *incy;
    }

/*     Start the operations. In this version the elements of A are   
       accessed sequentially with one pass through A.   

       First form  y := beta*y. */

    if (beta->r != 1. || beta->i != 0.) {
	if (*incy == 1) {
	    if (beta->r == 0. && beta->i == 0.) {
		for (i = 1; i <= leny; ++i) {
		    Y(i).r = 0., Y(i).i = 0.;
/* L10: */
		}
	    } else {
		for (i = 1; i <= leny; ++i) {
		    z__1.r = beta->r * Y(i).r - beta->i * Y(i).i, 
			    z__1.i = beta->r * Y(i).i + beta->i * Y(i)
			    .r;
		    Y(i).r = z__1.r, Y(i).i = z__1.i;
/* L20: */
		}
	    }
	} else {
	    iy = ky;
	    if (beta->r == 0. && beta->i == 0.) {
		for (i = 1; i <= leny; ++i) {
		    Y(iy).r = 0., Y(iy).i = 0.;
		    iy += *incy;
/* L30: */
		}
	    } else {
		for (i = 1; i <= leny; ++i) {
		    z__1.r = beta->r * Y(iy).r - beta->i * Y(iy).i, 
			    z__1.i = beta->r * Y(iy).i + beta->i * Y(iy)
			    .r;
		    Y(iy).r = z__1.r, Y(iy).i = z__1.i;
		    iy += *incy;
/* L40: */
		}
	    }
	}
    }
    if (alpha->r == 0. && alpha->i == 0.) {
	return;
    }
    if (strncmp(trans, "N", 1)==0) {

/*        Form  y := alpha*A*x + y. */

	jx = kx;
	if (*incy == 1) {
	    for (j = 1; j <= *n; ++j) {
		if (X(jx).r != 0. || X(jx).i != 0.) {
		    z__1.r = alpha->r * X(jx).r - alpha->i * X(jx).i, 
			    z__1.i = alpha->r * X(jx).i + alpha->i * X(jx)
			    .r;
		    temp.r = z__1.r, temp.i = z__1.i;
		    for (i = 1; i <= *m; ++i) {
			z__2.r = temp.r * A(i,j).r - temp.i * A(i,j).i, 
				z__2.i = temp.r * A(i,j).i + temp.i * A(i,j)
				.r;
			z__1.r = Y(i).r + z__2.r, z__1.i = Y(i).i + 
				z__2.i;
			Y(i).r = z__1.r, Y(i).i = z__1.i;
/* L50: */
		    }
		}
		jx += *incx;
/* L60: */
	    }
	} else {
	    for (j = 1; j <= *n; ++j) {
		if (X(jx).r != 0. || X(jx).i != 0.) {
		    z__1.r = alpha->r * X(jx).r - alpha->i * X(jx).i, 
			    z__1.i = alpha->r * X(jx).i + alpha->i * X(jx)
			    .r;
		    temp.r = z__1.r, temp.i = z__1.i;
		    iy = ky;
		    for (i = 1; i <= *m; ++i) {
			z__2.r = temp.r * A(i,j).r - temp.i * A(i,j).i, 
				z__2.i = temp.r * A(i,j).i + temp.i * A(i,j)
				.r;
			z__1.r = Y(iy).r + z__2.r, z__1.i = Y(iy).i + 
				z__2.i;
			Y(iy).r = z__1.r, Y(iy).i = z__1.i;
			iy += *incy;
/* L70: */
		    }
		}
		jx += *incx;
/* L80: */
	    }
	}
    } else {

/*        Form  y := alpha*A'*x + y  or  y := alpha*conjg( A' )*x + y.
 */

	jy = ky;
	if (*incx == 1) {
	    for (j = 1; j <= *n; ++j) {
		temp.r = 0., temp.i = 0.;
		if (noconj) {
		    for (i = 1; i <= *m; ++i) {
			z__2.r = A(i,j).r * X(i).r - A(i,j).i * X(i)
				.i, z__2.i = A(i,j).r * X(i).i + A(i,j)
				.i * X(i).r;
			z__1.r = temp.r + z__2.r, z__1.i = temp.i + z__2.i;
			temp.r = z__1.r, temp.i = z__1.i;
/* L90: */
		    }
		} else {
		    for (i = 1; i <= *m; ++i) {
			d_cnjg(&z__3, &A(i,j));
			z__2.r = z__3.r * X(i).r - z__3.i * X(i).i, 
				z__2.i = z__3.r * X(i).i + z__3.i * X(i)
				.r;
			z__1.r = temp.r + z__2.r, z__1.i = temp.i + z__2.i;
			temp.r = z__1.r, temp.i = z__1.i;
/* L100: */
		    }
		}
		z__2.r = alpha->r * temp.r - alpha->i * temp.i, z__2.i = 
			alpha->r * temp.i + alpha->i * temp.r;
		z__1.r = Y(jy).r + z__2.r, z__1.i = Y(jy).i + z__2.i;
		Y(jy).r = z__1.r, Y(jy).i = z__1.i;
		jy += *incy;
/* L110: */
	    }
	} else {
	    for (j = 1; j <= *n; ++j) {
		temp.r = 0., temp.i = 0.;
		ix = kx;
		if (noconj) {
		    for (i = 1; i <= *m; ++i) {
			z__2.r = A(i,j).r * X(ix).r - A(i,j).i * X(ix)
				.i, z__2.i = A(i,j).r * X(ix).i + A(i,j)
				.i * X(ix).r;
			z__1.r = temp.r + z__2.r, z__1.i = temp.i + z__2.i;
			temp.r = z__1.r, temp.i = z__1.i;
			ix += *incx;
/* L120: */
		    }
		} else {
		    for (i = 1; i <= *m; ++i) {
			d_cnjg(&z__3, &A(i,j));
			z__2.r = z__3.r * X(ix).r - z__3.i * X(ix).i, 
				z__2.i = z__3.r * X(ix).i + z__3.i * X(ix)
				.r;
			z__1.r = temp.r + z__2.r, z__1.i = temp.i + z__2.i;
			temp.r = z__1.r, temp.i = z__1.i;
			ix += *incx;
/* L130: */
		    }
		}
		z__2.r = alpha->r * temp.r - alpha->i * temp.i, z__2.i = 
			alpha->r * temp.i + alpha->i * temp.r;
		z__1.r = Y(jy).r + z__2.r, z__1.i = Y(jy).i + z__2.i;
		Y(jy).r = z__1.r, Y(jy).i = z__1.i;
		jy += *incy;
/* L140: */
	    }
	}
    }

    return;

/*     End of ZGEMV . */

} /* zgemv_ */


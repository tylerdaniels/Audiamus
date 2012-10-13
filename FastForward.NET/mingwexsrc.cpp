// mingwexsource.cpp
// ---------------------
// This file is to define and fill in libmingwex.a functions
// This is not a complete list by any means, and was coded due to incompatabilities of libmingwex.a
// With MSVC.  This file was created by Justin Ahn, and any questions can be directed to justin@soonahn.ca
//
#define WIN32_LEAN_AND_MEAN
#define _CRTIMP

#include <windows.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <float.h>
#include <math.h>
#include <io.h>
#include <sys/timeb.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int __cdecl _fpclass (double);
/*
#ifndef _ieeemisc_.obj
int __cdecl _fpclassf (float x) {
   return _fpclass(double(x));
}
#else
extern int __cdecl _fpclassf (float);
#endif
*/
int __cdecl _fpclassf (float x) {
   return _fpclass(double(x));
}
int __cdecl _fpclassl (long double x) {
   return _fpclass(double(x));
}
int __cdecl __fpclassify (float x) {
   return _fpclass(double(x));
}

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
   #define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
   #define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
#define fpclass(x) (sizeof (x) == sizeof (float) ? _fpclassf (x)     \
  : sizeof (x) == sizeof (double) ? _fpclass (x) \
  : _fpclassl (x))
#ifndef _WINSOCK2API_
struct timeval {
    long tv_sec;
    long tv_usec;
};
#endif
#define isfinite(x) ((fpclass(x) & FP_NAN) == 0)
#define FP_NAN      0x0100
#define FP_NORMAL   0x0400
#define FP_ZERO      0x4000
#define APICHAR char
#define PFORMAT_NOLIMIT     0x4000
extern int __cdecl _fstat64 (int,struct _stat64*);
extern void __cdecl _ftime (struct __timeb64*);
extern __int64 __cdecl _strtoi64 (const char * _String, char ** _EndPtr, int _Radix);
extern float __cdecl log10f (float);
extern float __cdecl sqrtf (float);
extern double __cdecl round (double);
extern float __cdecl cosf (float);
extern float __cdecl sinf (float);
extern float __cdecl log2f (float);
extern float __cdecl atan2f (float, float);
extern float __cdecl atanf (float);
extern double  __cdecl log(double);
extern __int64 __cdecl _filelengthi64 (int);
extern char* __cdecl gai_strerrorA (int);
typedef long _off64_t;
typedef long off64_t;

struct timezone {
    int  tz_minuteswest; /* minutes W of Greenwich */
    int  tz_dsttime;     /* type of dst correction */
};

float __cdecl truncf(float n) {
    return n > 0.0 ? floorf(n) : ceilf(n);
}

int __cdecl __imp___fstat64(__in int _FileDes, __out struct _stat64 * _Stat) {
   return _fstat64(_FileDes, _Stat);
}

void __cdecl __imp___ftime64(__out struct __timeb64 *_Time) {
   _ftime(_Time);
}
/*
int __cdecl __imp_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
   return getaddrinfo(node, service, hints, res);
}
*/
double log2(double n) { 
    return log(n)/log(2.0); 
}
float log2f(float n) { 
    return logf(n)/logf(2.0); 
}

float exp2f(float n) {
   return powf(2.0, n);
}

float roundf(float x)
{
	return floor(x + 0.5f);
}

int __cdecl snprintf(char* buffer, size_t n, const char *format, ...)
{
  int retval;
  va_list argptr;

  va_start(argptr, format);
  retval = _vsnprintf (buffer, n, format, argptr);
  va_end(argptr);
  return retval;
}

long long __cdecl strtoll (const char* __restrict nptr, char ** __restrict endptr, int base){
   return _strtoi64(nptr, endptr, base);
}

static const float CBRT2 = 1.25992104989487316477f;
static const float CBRT4 = 1.58740105196819947475f;
float cbrtf (float x) {
        int e, rem, sign;
        float z;
      if (!isfinite (x) || x == 0.0F)
                return x;
        if (x > 0)
                sign = 1;
        else
        {
                sign = -1;
                x = -x;
        }

        z = x;
        // extract power of 2, leaving                                                                                             
        // mantissa between 0.5 and 1                                                                                               
        //
        x = frexpf(x, &e);

        // Approximate cube root of number between .5 and 1,                                                                       
        // peak relative error = 9.2e-6                                                                                             
        //
        x = (((-0.13466110473359520655053f  * x
              + 0.54664601366395524503440f ) * x
              - 0.95438224771509446525043f ) * x
              + 1.1399983354717293273738f  ) * x
              + 0.40238979564544752126924f;

        // exponent divided by 3
        if (e >= 0)
        {
                rem = e;
                e /= 3;
                rem -= 3*e;
                if (rem == 1)
                        x *= CBRT2;
                else if (rem == 2)
                        x *= CBRT4;
        }
// argument less than 1
        else
        {
                e = -e;
                rem = e;
                e /= 3;
                rem -= 3*e;
                if (rem == 1)
                        x /= CBRT2;
                else if (rem == 2)
                        x /= CBRT4;
                e = -e;
        }

        // multiply by power of 2
        x = ldexpf(x, e);

        // Newton iteration
        x -= ( x - (z/(x*x)) ) * 0.333333333333f;

        if (sign < 0)
                x = -x;
        return (x);
}

double round (double x) {
  double res;
  if (x >= 0.0)
    {
      res = ceil (x);
      if (res - x > 0.5)
        res -= 1.0;
    }
  else
    {
      res = ceil (-x);
      if (res + x > 0.5)
        res -= 1.0;
      res = -res;
    }
  return res;
}

int fseeko64 (FILE* stream, _off64_t offset, int whence) {
  fpos_t pos;
  if (whence == SEEK_CUR)
    {
      // If stream is invalid, fgetpos sets errno.
      if (fgetpos (stream, &pos))
        return (-1);
      pos += (fpos_t) offset;
    }
  else if (whence == SEEK_END)
    {
      // If writing, we need to flush before getting file length.
      fflush (stream);
      pos = (fpos_t) (_filelengthi64 (_fileno (stream)) + offset);
    }
  else if (whence == SEEK_SET)
    pos = (fpos_t) offset;
  else
    {
      errno = EINVAL;
      return (-1);
    }
  return fsetpos (stream, &pos);
}

_off64_t ftello64 (FILE * stream) {
  fpos_t pos;
  if (fgetpos(stream, &pos))
    return  -1LL;
  else
   return ((off64_t) pos);
}


#ifdef __cplusplus
}
#endif
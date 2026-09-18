#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#define M_TWO_PI 6.28318530718
#if defined(_MSC_VER)
#define popen _popen
#endif
#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <cmath>
class Texture{
public:
/** from 0 to 1 **/
   double opacity, reflection, ambient;
   Texture(double am, double op, double ref);
   virtual void getColor(unsigned char* toFill, double* am, double *opacity, double *reflection,double x, double y) = 0;
   Texture* clone();
};

inline double interpolate(double a,double b,double x)
{
   double f=(1.0-cos(x * M_PI))* 0.5;
   return a*(1.0-f)+b*f;
}
 
inline const char* findExtension(const char* ola){
   const char* end = ola;
   while(*end!='\0') end++;
   const char* start = end;
   while(*start!='.' && start>ola) start--;
   if(*start=='.') start++;
   return start;
}

inline double ground(double e){
   return (e>1.)?1.:e;
}

inline double fix(double a){
   a = fmod(a, 1.);
   if(a<0) a+=1.;
   return a;
}

inline char lowerCase(char c){
   if ((c >= 'A') && (c <= 'Z')) 
      return c-'a'+'A'; 
   else 
      return c;
}

inline int fpeek(FILE *stream)
{
   int c;
   c = fgetc(stream);
   ungetc(c, stream);
   return c;
}

inline bool extensionEquals(const char* a, const char* knownExt){
   while(*a!='\0'){
      if(a[0] != knownExt[0]) 
         return false;
      a++;
      knownExt++;
   }
   return knownExt[0]=='\0';
}


#endif

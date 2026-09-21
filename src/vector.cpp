#include<string.h>
#include<stdio.h>
#include<limits>
#include<math.h>
#include<stdlib.h>
//#include <printf.h>
#include <stddef.h>
#include "vector.h"
#include <immintrin.h>
static double out[8] = {0};
static double out1[8] = {0};
Vector::Vector(double a, double b, double c) : x(a), y(b), z(c) {
}
void Vector::operator -= (const Vector rhs) {
   x-=rhs.x; y-=rhs.y; z-=rhs.z;
}
void Vector::operator += (const Vector rhs) {
   x+=rhs.x; y+=rhs.y; z+=rhs.z;
}
void Vector::operator *= (const double rhs) {
   x*=rhs; y*=rhs; z*=rhs;
}
void Vector::operator *= (const float rhs) {
   x*=rhs; y*=rhs; z*=rhs;
}
void Vector::operator *= (const int rhs) {
   x*=rhs; y*=rhs; z*=rhs;
}
void Vector::operator /= (const double rhs) {
   x/=rhs; y/=rhs; z/=rhs;
}
void Vector::operator /= (const float rhs) {
   x/=rhs; y/=rhs; z/=rhs;
}
void Vector::operator /= (const int rhs) {
   x/=rhs; y/=rhs; z/=rhs;
}


Vector Vector::operator - (const Vector rhs) {
   return Vector(x-rhs.x, y-rhs.y, z-rhs.z);
}
Vector Vector::operator + (const Vector rhs) {
   return Vector(x+rhs.x, y+rhs.y, z+rhs.z);
}
/*
Vector Vector::operator * (const Vector a) {
   return Vector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
}*/
Vector Vector::operator * (const double rhs) {
   return Vector(x*rhs, y*rhs, z*rhs);
}
Vector Vector::operator * (const float rhs) {
   return Vector(x*rhs, y*rhs, z*rhs);
}
Vector Vector::operator * (const int rhs) {
   return Vector(x*rhs, y*rhs, z*rhs);
}
Vector Vector::operator / (const double rhs) {
   return Vector(x/rhs, y/rhs, z/rhs);
}
Vector Vector::operator / (const float rhs) {
   return Vector(x/rhs, y/rhs, z/rhs);
}
Vector Vector::operator / (const int rhs) {
   return Vector(x/rhs, y/rhs, z/rhs);
}
Vector Vector::cross(const Vector a) {
   return Vector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
}

  
Vector solveScalers(Vector v1, Vector v2, Vector v3, Vector C){
   double init[8] = {v1.z, v1.y, v1.z, v1.x, v1.y, v1.x, 0, 0};
   double init1[8] = {v2.y, v2.z, v2.x, v2.z, v2.x, v2.y, 0, 0};
   _mm512_store_pd(out, _mm512_mul_pd(_mm512_load_pd(init),
                                          _mm512_load_pd(init1)));
   double init2[8] = {v3.x, v3.x, v3.y, v3.y, v3.z, v3.z};
   double init3[8] = {out[0], out[1], out[3], out[4], out[5]};
   _mm512_store_pd(out1, _mm512_mul_pd(_mm512_load_pd(init2),
                                          _mm512_load_pd(init3)));

   double denom = out1[0] - out1[1] - out1[2] + out1[3] + out1[4] - out1[5];
   double a = C.z*v2.y*v3.x-C.y*v2.z*v3.x-C.z*v2.x*v3.y+C.x*v2.z*v3.y+C.y*v2.x*v3.z-C.x*v2.y*v3.z;
   double b = -C.z*v1.y*v3.x+C.y*v1.z*v3.x+C.z*v1.x*v3.y-C.x*v1.z*v3.y-C.y*v1.x*v3.z+C.x*v1.y*v3.z;
   double c = C.z*out[4] - C.y*out[2] - C.z*out[5]+ C.x*out[0] + C.y*out[3] - C.x*out[1];
   return Vector(a/denom, b/denom, c/denom);
}

Ray::Ray(const Vector& po, const Vector& ve): point(po), vector(ve){}

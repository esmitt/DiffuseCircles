#ifndef __VEC2_H
#define __VEC2_H
#include <math.h>

typedef struct {
	float x, y ; 
} vec2_t ;

float mag( vec2_t v ) ;
float getAngle( vec2_t v );
vec2_t add( vec2_t v, vec2_t w );
vec2_t sub( vec2_t v, vec2_t w );
vec2_t mul( float k, vec2_t v ) ;
vec2_t  unit( vec2_t v ) ;
float  dot( vec2_t v, vec2_t w ) ;
vec2_t  direction( float angle );
float magSqr( vec2_t v ) ;

#endif
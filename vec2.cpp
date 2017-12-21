
#include "vec2.h"

// Magnitude
float mag( vec2_t v ) {
   return sqrt( v.x * v.x + v.y * v.y ) ; 
}

float magSqr( vec2_t v ) {
  return v.x * v.x + v.y * v.y ;
}

// Angle
float getAngle( vec2_t v ) {
	return atan2( v.y, v.x ) ;
}

// Add
vec2_t add( vec2_t v, vec2_t w ) {
	vec2_t tmp ;
	tmp.x = v.x + w.x ;
	tmp.y = v.y + w.y ;
	return tmp;
}

// Sub
vec2_t sub( vec2_t v, vec2_t w ) {
	vec2_t tmp ;
	tmp.x = v.x - w.x ;
	tmp.y = v.y - w.y ;
	return tmp;
}

// Multiply with a scalar value
vec2_t mul( float k, vec2_t v ) {
	vec2_t tmp ;
	tmp.x = k * v.x ;
	tmp.y = k * v.y ;
	return tmp ;
}

// unit vector, normalization
vec2_t  unit( vec2_t v ) {
	return mul( 1.0f / mag(v), v ) ;
}

// dot product
float  dot( vec2_t v, vec2_t w ) {
	return v.x * w.x + v.y * w.y ; 
}

// unit vector in the direction of given angle
vec2_t  direction( float angle ) {
	vec2_t dir ;
	dir.x = cos( angle ) ;
	dir.y = sin( angle ) ;
	return dir ;
}
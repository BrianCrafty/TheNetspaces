#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
//#include <windows.h>
#define WIDTH 150
#define HEIGHT 37
#define MAXDEPTH 100000
#define INVERSEZBUFFER
#define CUBEWIDTH 25
#define WALLSSCALE 25
#define DISTANCEFROMCAMERA 50
//#define DISTANCEFROMCAMERA 200
#define MAXBUFFERSIZE 512
#define CHARSCALE 2
#define CAMERA 50
#define CHARMINGCIRCLE 1
#define CHARMINGCUBE 2
#define CHARMINGMAZEMAP 8
#define CHARMINGMAZE 9
#define PRINTWALLS 10 
#define CHARSDENSITY 1
//#define CHARSDENSITY 0.1
//#define CHARSDENSITY 0.5
#define MOUSEMOVE 0.1
#define KEYBOARDMOVE -25
//#define CAMERANEAR 0.1
#define CAMERANEAR 0.01
#define CAMERAFAR 10000
//#define CAMERAFAR 1000
//#define NUMBER float
#define NUMBER double
//#define NUMBER long double
//float epsilon=10e-5;
//float epsilon=10e-15;
#define SCENESHIFTX 0
#define SCENESHIFTY 0
double epsilon = 1e-15;
char buffer[WIDTH * HEIGHT + 1];
NUMBER zBuffer[WIDTH * HEIGHT];
int I, J, K;
NUMBER ALPHA;
bool blnPause = false;
const NUMBER pi = 3.141592653589793;
//const NUMBER degree = pi / 180;
NUMBER degree=3.141592653589793/180.;
NUMBER toRadian(NUMBER a) { return a * degree; }
int ij(int i, int j, int theCols, int theRows) { return j * theCols + i; }
int ijk(int i, int j, int k, int Xcount, int Ycount, int Zcount) { return Xcount * Ycount * k + Xcount * j + i; }
/*mov bx,ax*/
NUMBER* vec2_create_() { NUMBER* a = (NUMBER*)malloc(2 * sizeof(NUMBER)); memset(a, 0, 2 * sizeof(NUMBER)); return a; }
NUMBER* vec2_create(NUMBER x, NUMBER y) { NUMBER* a = vec2_create_(); a[0] = x; a[1] = y; return a; }
void vec2_dispose(NUMBER* a) { free(a); }
void vec2_printf(NUMBER* a) { for (int ii = 0; ii < 2; ii++)printf("%lf, ", a[ii]); }
NUMBER* vec2_set(NUMBER* b, NUMBER* a) { b[0] = a[0]; b[1] = a[1]; return b; }
NUMBER* vec2_add(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; return c; }
NUMBER* vec2_subtract(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; return c; }
NUMBER* vec2_scale(NUMBER* c, NUMBER* a, NUMBER b) { c[0] = a[0] * b; c[1] = a[1] * b; return c; }
NUMBER vec2_multiply(NUMBER* c, NUMBER* a, NUMBER* b) { return a[0] * b[0] + a[1] * b[1]; }
NUMBER* vec2_mutliplyTransposed(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] * b[0]; c[1] = a[1] * b[1]; return c; }
NUMBER vec2_dot(NUMBER* c, NUMBER* a, NUMBER* b) { return a[0] * b[0] + a[1] * b[1]; }
NUMBER* vec2_normalize(NUMBER* b, NUMBER* a) { NUMBER c = a[0], d = a[1], e = sqrt(c * c + d * d); if (0 == e) { b[0] = 0; b[1] = 0; return b; }b[0] = c / e; b[1] = d / e; return b; }
int vec2_length(NUMBER* a) { return 2; }
NUMBER vec2_module(NUMBER* a) { return sqrt(a[0] * a[0] + a[1] * a[1]); }
NUMBER* vec2_linearCombination(NUMBER* y, NUMBER a, NUMBER* x, NUMBER b) { y[0] = a * x[0] + b; y[1] = a * x[1] + b; return y; }
NUMBER* vec3_create_() { NUMBER* a = (NUMBER*)malloc(3 * sizeof(NUMBER)); memset(a, 0, 3 * sizeof(NUMBER)); return a; }
NUMBER* vec3_create(NUMBER x, NUMBER y, NUMBER z) { NUMBER* a = vec3_create_(); a[0] = x; a[1] = y; a[2] = z; return a; }
void vec3_dispose(NUMBER* a) { free(a); }
void vec3_printf(NUMBER* a) { for (int ii = 0; ii < 3; ii++)printf("%lf, ", a[ii]); }
NUMBER* vec3_set(NUMBER* b, NUMBER* a) { b[0] = a[0]; b[1] = a[1]; b[2] = a[2]; return b; }
NUMBER* vec3_add(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; return c; }
NUMBER* vec3_subtract(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; return c; }
NUMBER* vec3_scale(NUMBER* c, NUMBER* a, NUMBER b) { c[0] = a[0] * b; c[1] = a[1] * b; c[2] = a[2] * b; return c; }
NUMBER vec3_multiply(NUMBER* c, NUMBER* a, NUMBER* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
NUMBER* vec3_mutliplyTransposed(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] * b[0]; c[1] = a[1] * b[1]; c[2] = a[2] * b[2]; return c; }
NUMBER vec3_dot(NUMBER* c, NUMBER* a, NUMBER* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2]; }
NUMBER* vec3_normalize(NUMBER* b, NUMBER* a) { NUMBER c = a[0], d = a[1], e = a[2], f = sqrt(c * c + d * d + e * e); if (0 == f) { b[0] = 0; b[1] = 0; b[2] = 0; return b; }b[0] = c / f; b[1] = d / f; b[2] = e / f; return b; }
NUMBER* vec3_cross(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = a[0], e = a[1], f = a[2], g = b[0], h = b[1], i = b[2]; c[0] = e * i - f * h; c[1] = f * g - d * i; c[2] = d * h - e * g; return c; }
int vec3_length(NUMBER* a) { return 3; };
NUMBER vec3_module(NUMBER* a) { return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]); }
NUMBER* vec3_linearCombination(NUMBER* y, NUMBER a, NUMBER* x, NUMBER b) { y[0] = a * x[0] + b; y[1] = a * x[1] + b; y[2] = a * x[2] + b; return y; }
NUMBER* vec4_create_() { NUMBER* a = (NUMBER*)malloc(4 * sizeof(NUMBER)); memset(a, 0, 4 * sizeof(NUMBER)); return a; }
NUMBER* vec4_create(NUMBER x, NUMBER y, NUMBER z, NUMBER w) { NUMBER* a = vec4_create_(); a[0] = x; a[1] = y; a[2] = z; a[3] = w; return a; }
void vec4_dispose(NUMBER* a) { free(a); }
void vec4_printf(NUMBER* a) { for (int ii = 0; ii < 4; ii++)printf("%lf, ", a[ii]); }
NUMBER* vec4_set(NUMBER* b, NUMBER* a) { b[0] = a[0]; b[1] = a[1]; b[2] = a[2]; b[3] = a[3]; return b; }
NUMBER* vec4_add(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; c[3] = a[3] + b[3]; return c; }
NUMBER* vec4_subtract(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; c[3] = a[3] - b[3]; return c; }
NUMBER* vec4_scale(NUMBER* c, NUMBER* a, NUMBER b) { c[0] = a[0] * b; c[1] = a[1] * b; c[2] = a[2] * b; c[3] = a[3] * b; return c; }
NUMBER vec4_multiply(NUMBER* c, NUMBER* a, NUMBER* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]; }NUMBER* vec4_mutliplyTransposed(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] * b[0]; c[1] = a[1] * b[1]; c[2] = a[2] * b[2]; c[3] = a[3] * b[3]; return c; }NUMBER vec4_dot(NUMBER* c, NUMBER* a, NUMBER* b) { return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]; }
NUMBER* vec4_normalize(NUMBER* b, NUMBER* a) { NUMBER c = a[0], d = a[1], e = a[2], f = a[3], g = sqrt(c * c + d * d + e * e + f * f); if (0 == g) { b[0] = 0; b[1] = 0; b[2] = 0; b[3] = 0; return b; }b[0] = c / g; b[1] = d / g; b[2] = e / g; b[3] = f / g; return b; }
int vec4_length(NUMBER* a) { return 4; };
NUMBER vec4_module(NUMBER* a) { return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]); }
NUMBER* vec4_linearCombination(NUMBER* y, NUMBER a, NUMBER* x, NUMBER b) { y[0] = a * x[0] + b; y[1] = a * x[1] + b; y[2] = a * x[2] + b; y[3] = a * x[3] + b; return y; }
NUMBER* mat3_create_() { NUMBER* a = (NUMBER*)malloc(9 * sizeof(NUMBER)); memset(a, 0, 9 * sizeof(NUMBER)); return a; }
//in algebra ji, where meaning of the first and second variable exactly the same
NUMBER* mat3_create(NUMBER a00, NUMBER a10, NUMBER a20, NUMBER a01, NUMBER a11, NUMBER a21, NUMBER a02, NUMBER a12, NUMBER a22) { NUMBER* a = vec3_create_(); a[0] = a00; a[1] = a10; a[2] = a20; a[3] = a01; a[4] = a11; a[5] = a21; a[6] = a02; a[7] = a12; a[8] = a22; return a; }
void mat3_dispose(NUMBER* a) { free(a); }
void mat3_printf(NUMBER* a) { for (int i = 0; i < 3; i++)for (int j = 0; j < 3; j++)printf("%lf, ", a[ij(i, j, 3, 3)]); }
NUMBER* mat3_identity(NUMBER* a) { a[0] = 1; a[1] = 0; a[2] = 0; a[3] = 0; a[4] = 1; a[5] = 0; a[6] = 0; a[7] = 0; a[8] = 1; return a; }
NUMBER* mat3_identity_() { NUMBER* a = mat3_create_(); return mat3_identity(a); }
NUMBER* mat3_transpose(NUMBER* b, NUMBER* a) { NUMBER* c = (NUMBER*)malloc(9 * sizeof(NUMBER)); c[0] = a[0]; c[1] = a[3]; c[2] = a[6]; c[3] = a[1]; c[4] = a[4]; c[5] = a[7]; c[6] = a[2]; c[7] = a[5]; c[8] = a[8]; b[0] = c[0]; b[1] = c[1]; b[2] = c[2]; b[3] = c[3]; b[4] = c[4]; b[5] = c[5]; b[6] = c[6]; b[7] = c[7]; b[8] = c[8]; free(c); return b; }
NUMBER* mat3_set(NUMBER* b, NUMBER* a) { b[0] = a[0]; b[1] = a[1]; b[2] = a[2]; b[3] = a[3]; b[4] = a[4]; b[5] = a[5]; b[6] = a[6]; b[7] = a[7]; b[8] = a[8]; return b; }
NUMBER* mat3_add(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; c[3] = a[3] + b[3]; c[4] = a[4] + b[4]; c[5] = a[5] + b[5]; c[6] = a[6] + b[6]; c[7] = a[7] + b[7]; c[8] = a[8] + b[8]; return c; }
NUMBER* mat3_subtract(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; c[3] = a[3] - b[3]; c[4] = a[4] - b[4]; c[5] = a[5] - b[5]; c[6] = a[6] - b[6]; c[7] = a[7] - b[7]; c[8] = a[8] - b[8]; return c; }
NUMBER* mat3_multiply(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = a[0], e = a[1], f = a[2], g = a[3], h = a[4], i = a[5], j = a[6], k = a[7], l = a[8], m = b[0], n = b[1], o = b[2], p = b[3], q = b[4], r = b[5], s = b[6], t = b[7], u = b[8]; c[0] = d * m + e * p + f * s; c[1] = d * n + e * q + f * t; c[2] = d * o + e * r + f * u; c[3] = g * m + h * p + i * s; c[4] = g * n + h * q + i * t; c[5] = g * o + h * r + i * u; c[6] = j * m + k * p + l * s; c[7] = j * n + k * q + l * t; c[8] = j * o + k * r + l * u; return c; }
NUMBER* mat3_multiplyVec3(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = a[0], e = a[1], f = a[2], g = a[3], h = a[4], i = a[5], j = a[6], k = a[7], l = a[8], m = b[0], n = b[1], o = b[2]; c[0] = d * m + e * n + f * o; c[1] = g * m + h * n + o * i; c[2] = j * m + k * n + l * o; return c; }
NUMBER* mat4_create_() { NUMBER* a = (NUMBER*)malloc(16 * sizeof(NUMBER)); memset(a, 0, 16 * sizeof(NUMBER)); return a; }
//in algebra ji, where meaning of the first and second variable exactly the same
NUMBER* mat4_create(NUMBER a00, NUMBER a10, NUMBER a20, NUMBER a30, NUMBER a01, NUMBER a11, NUMBER a21, NUMBER a31, NUMBER a02, NUMBER a12, NUMBER a22, NUMBER a32, NUMBER a03, NUMBER a13, NUMBER a23, NUMBER a33) { NUMBER* a = vec4_create_(); a[0] = a00; a[1] = a10; a[2] = a20; a[3] = a30; a[4] = a01; a[5] = a11; a[6] = a21; a[7] = a31; a[8] = a02; a[9] = a12; a[10] = a22; a[11] = a32; a[12] = a03; a[13] = a13; a[14] = a23; a[15] = a33; return a; }
void mat4_dispose(NUMBER* a) { free(a); }
void mat4_printf(NUMBER* a) { for (int j = 0; j < 4; j++) { for (int i = 0; i < 4; i++)printf("%lf, ", a[ij(i, j, 4, 4)]); printf("\n"); } }
NUMBER* mat4_identity(NUMBER* a) { a[0] = 1; a[1] = 0; a[2] = 0; a[3] = 0; a[4] = 0; a[5] = 1; a[6] = 0; a[7] = 0; a[8] = 0; a[9] = 0; a[10] = 1; a[11] = 0; a[12] = 0; a[13] = 0; a[14] = 0; a[15] = 1; return a; }
NUMBER* mat4_identity_() { NUMBER* a = mat4_create_(); return mat4_identity(a); }
NUMBER* mat4_transpose(NUMBER* b, NUMBER* a) { NUMBER* c = (NUMBER*)malloc(16 * sizeof(NUMBER)); c[0] = a[0]; c[1] = a[4]; c[2] = a[8]; c[3] = a[12]; c[4] = a[1]; c[5] = a[5]; c[6] = a[9]; c[7] = a[13]; c[8] = a[2]; c[9] = a[6]; c[10] = a[10]; c[11] = a[14]; c[12] = a[3]; c[13] = a[7]; c[14] = a[11]; c[15] = a[15]; b[0] = c[0]; b[1] = c[1]; b[2] = c[2]; b[3] = c[3]; b[4] = c[4]; b[5] = c[5]; b[6] = c[6]; b[7] = c[7]; b[8] = c[8]; b[9] = c[9]; b[10] = c[10]; b[11] = c[11]; b[12] = c[12]; b[13] = c[13]; b[14] = c[14]; b[15] = c[15]; free(c); return b; }
NUMBER* mat4_multiply(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = a[0], e = a[1], f = a[2], g = a[3], h = a[4], i = a[5], j = a[6], k = a[7], l = a[8], m = a[9], n = a[10], o = a[11], p = a[12], q = a[13], r = a[14], s = a[15], t = b[0], u = b[1], v = b[2], w = b[3], x = b[4], y = b[5], z = b[6], A = b[7], B = b[8], C = b[9], D = b[10], E = b[11], F = b[12], G = b[13], H = b[14], I = b[15]; c[0] = t * d + u * h + v * l + w * p; c[1] = t * e + u * i + v * m + w * q; c[2] = t * f + u * j + v * n + w * r; c[3] = t * g + u * k + v * o + w * s; c[4] = x * d + y * h + z * l + A * p; c[5] = x * e + y * i + z * m + A * q; c[6] = x * f + y * j + z * n + A * r; c[7] = x * g + y * k + z * o + A * s; c[8] = B * d + C * h + D * l + E * p; c[9] = B * e + C * i + D * m + E * q; c[10] = B * f + C * j + D * n + E * r; c[11] = B * g + C * k + D * o + E * s; c[12] = F * d + G * h + H * l + I * p; c[13] = F * e + G * i + H * m + I * q; c[14] = F * f + G * j + H * n + I * r; c[15] = F * g + G * k + H * o + I * s; return c; }

NUMBER* mat4_multiplyVec4(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = b[0], e = b[1], f = b[2], g = b[3]; c[0] = a[0] * d + a[4] * e + a[8] * f + a[12] * g; c[1] = a[1] * d + a[5] * e + a[9] * f + a[13] * g; c[2] = a[2] * d + a[6] * e + a[10] * f + a[14] * g; c[3] = a[3] * d + a[7] * e + a[11] * f + a[15] * g; return c; }
NUMBER* mat4_translate(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = a[0], e = a[1], f = a[2], g = a[3], h = a[4], i = a[5], j = a[6], k = a[7], l = a[8], m = a[9], n = a[10], o = a[11], p = b[0], q = b[1], r = b[2]; c[0] = d; c[1] = e; c[2] = f; c[3] = g; c[4] = h; c[5] = i; c[6] = j; c[7] = k; c[8] = l; c[9] = m; c[10] = n; c[11] = o; c[12] = d * p + h * q + l * r + a[12]; c[13] = e * p + i * q + m * r + a[13]; c[14] = f * p + j * q + n * r + a[14]; c[15] = g * p + k * q + o * r + a[15]; return c; }
NUMBER* mat4_set(NUMBER* b, NUMBER* a) { b[0] = a[0]; b[1] = a[1]; b[2] = a[2]; b[3] = a[3]; b[4] = a[4]; b[5] = a[5]; b[6] = a[6]; b[7] = a[7]; b[8] = a[8]; b[9] = a[9]; b[10] = a[10]; b[11] = a[11]; b[12] = a[12]; b[13] = a[13]; b[14] = a[14]; b[15] = a[15]; return b; }
NUMBER* mat4_add(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] + b[0]; c[1] = a[1] + b[1]; c[2] = a[2] + b[2]; c[3] = a[3] + b[3]; c[4] = a[4] + b[4]; c[5] = a[5] + b[5]; c[6] = a[6] + b[6]; c[7] = a[7] + b[7]; c[8] = a[8] + b[8]; c[9] = a[9] + b[9]; c[10] = a[10] + b[10]; c[11] = a[11] + b[11]; c[12] = a[12] + b[12]; c[13] = a[13] + b[13]; c[14] = a[14] + b[14]; c[15] = a[15] + b[15]; return c; }
NUMBER* mat4_subtract(NUMBER* c, NUMBER* a, NUMBER* b) { c[0] = a[0] - b[0]; c[1] = a[1] - b[1]; c[2] = a[2] - b[2]; c[3] = a[3] - b[3]; c[4] = a[4] - b[4]; c[5] = a[5] - b[5]; c[6] = a[6] - b[6]; c[7] = a[7] - b[7]; c[8] = a[8] - b[8]; c[9] = a[9] - b[9]; c[10] = a[10] - b[10]; c[11] = a[11] - b[11]; c[12] = a[12] - b[12]; c[13] = a[13] - b[13]; c[14] = a[14] - b[14]; c[15] = a[15] - b[15]; return c; }
NUMBER* mat4_scale(NUMBER* c, NUMBER* a, NUMBER* b) { NUMBER d = b[0], e = b[1], f = b[2]; c[0] = a[0] * d; c[1] = a[1] * d; c[2] = a[2] * d; c[3] = a[3] * d; c[4] = a[4] * e; c[5] = a[5] * e; c[6] = a[6] * e; c[7] = a[7] * e; c[8] = a[8] * f; c[9] = a[9] * f; c[10] = a[10] * f; c[11] = a[11] * f; c[12] = a[12]; c[13] = a[13]; c[14] = a[14]; c[15] = a[15]; return c; }
NUMBER* mat4_rotateX(NUMBER* c, NUMBER* a, NUMBER b) { NUMBER d = sin(b), e = cos(b), f = a[0], g = a[1], h = a[2], i = a[3], j = a[4], k = a[5], l = a[6], m = a[7], n = a[8], o = a[9], p = a[10], q = a[11], r = a[12], s = a[13], t = a[14], u = a[15]; c[0] = f; c[1] = g; c[2] = h; c[3] = i; c[4] = e * j + d * n; c[5] = e * k + d * o; c[6] = e * l + d * p; c[7] = e * m + d * q; c[8] = -d * j + e * n; c[9] = -d * k + e * o; c[10] = -d * l + e * p; c[11] = -d * m + e * q; c[12] = r; c[13] = s; c[14] = t; c[15] = u; return c; }
NUMBER* mat4_rotateY(NUMBER* c, NUMBER* a, NUMBER b) { NUMBER d = sin(b), e = cos(b), f = a[0], g = a[1], h = a[2], i = a[3], j = a[4], k = a[5], l = a[6], m = a[7], n = a[8], o = a[9], p = a[10], q = a[11], r = a[12], s = a[13], t = a[14], u = a[15]; c[0] = e * f - d * n; c[1] = e * g - d * o; c[2] = e * h - d * p; c[3] = e * i - d * q; c[4] = j; c[5] = k; c[6] = l; c[7] = m; c[8] = d * f + e * n; c[9] = d * g + e * o; c[10] = d * h + e * p; c[11] = d * i + e * q; c[12] = r; c[13] = s; c[14] = t; c[15] = u; return c; }
NUMBER* mat4_rotateZ(NUMBER* c, NUMBER* a, NUMBER b) { NUMBER d = sin(b), e = cos(b), f = a[0], g = a[1], h = a[2], i = a[3], j = a[4], k = a[5], l = a[6], m = a[7], n = a[8], o = a[9], p = a[10], q = a[11], r = a[12], s = a[13], t = a[14], u = a[15]; c[0] = e * f + d * j; c[1] = e * g + d * k; c[2] = e * h + d * l; c[3] = e * i + d * m; c[4] = -d * f + e * j; c[5] = -d * g + e * k; c[6] = -d * h + e * l; c[7] = -d * i + e * m; c[8] = n; c[9] = o; c[10] = p; c[11] = q; c[12] = r; c[13] = s; c[14] = t; c[15] = u; return c; }
NUMBER* mat4_lookAt(NUMBER* d, NUMBER* a, NUMBER* b, NUMBER* c) {NUMBER e = a[0], f = a[1], g = a[2], h = b[0], i = b[1], j = b[2], k = c[0], l = c[1], m = c[2];if (fabs(e - h) < epsilon && fabs(f - i) < epsilon && fabs(g - j) < epsilon) {d[0] = 1; d[1] = 0; d[2] = 0; d[3] = 0; d[4] = 0; d[5] = 1; d[6] = 0; d[7] = 0; d[8] = 0; d[9] = 0; d[10] = 1; d[11] = 0; d[12] = 0; d[13] = 0; d[14] = 0; d[15] = 1;return d;}NUMBER n = e - h, o = f - i, p = g - j, q = sqrt(n * n + o * o + p * p); n /= q; o /= q; p /= q;NUMBER r = l * p - m * o, s = m * n - k * p, t = k * o - l * n; q = sqrt(r * r + s * s + t * t);if (fabs(q) < epsilon) {r = 0; s = 0; t = 0;}else {r /= q; s /= q; t /= q;}NUMBER u = o * t - p * s, v = p * r - n * t, w = n * s - o * r; q = sqrt(u * u + v * v + w * w);if (fabs(q) < epsilon) {u = 0; v = 0; w = 0;}else {u /= q; v /= q; w /= q;}d[0] = r; d[1] = u; d[2] = n; d[3] = 0; d[4] = s; d[5] = v; d[6] = o; d[7] = 0; d[8] = t; d[9] = w; d[10] = p; d[11] = 0; d[12] = -(r * e + s * f + t * g); d[13] = -(u * e + v * f + w * g); d[14] = -(n * e + o * f + p * g); d[15] = 1;return d;}
NUMBER* mat4_getTranslation(NUMBER* b, NUMBER* a) { b[0] = a[12]; b[1] = a[13]; b[2] = a[14]; return b; }
/*
NUMBER rand(int min,int max){return min+(fabs(max-min)*random());}
int floorRand(int min,int max){return min+floor(fabs(max-min)*random());}
*/
//input section
/*
HANDLE hStdin;
DWORD fdwSaveOldMode;

VOID ErrorExit(LPCSTR);
VOID KeyEventProc(KEY_EVENT_RECORD);
VOID MouseEventProc(MOUSE_EVENT_RECORD);
VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD);
DWORD cNumRead, fdwMode, i;
INPUT_RECORD irInBuf[128];
VOID initMouse() {

	// Get the standard input handle.

	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if (hStdin == INVALID_HANDLE_VALUE)
		ErrorExit("GetStdHandle");

	// Save the current input mode, to be restored on exit.

	if (!GetConsoleMode(hStdin, &fdwSaveOldMode))
		ErrorExit("GetConsoleMode");

	// Enable the window and mouse input events.
	// Disable quick edit mode because it interfers with receiving mouse inputs.

	fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
	//fdwMode = (ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS) & ~ENABLE_QUICK_EDIT_MODE;
	//fdwMode = (ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	//fdwMode = (ENABLE_MOUSE_INPUT);
	if (!SetConsoleMode(hStdin, fdwMode))
		ErrorExit("SetConsoleMode");
}
VOID mouseScan() {
	if (!ReadConsoleInput(
		hStdin,      // input buffer handle
		irInBuf,     // buffer to read into
		128,         // size of read buffer
		&cNumRead)) // number of records read
		ErrorExit("ReadConsoleInput");

	// Dispatch the events to the appropriate handler.

	for (i = 0; i < cNumRead; i++)
	{
		switch (irInBuf[i].EventType)
		{
		case KEY_EVENT: // keyboard input
			//KeyEventProc(irInBuf[i].Event.KeyEvent);
			break;

		case MOUSE_EVENT: // mouse input
			MouseEventProc(irInBuf[i].Event.MouseEvent);
			break;

		case WINDOW_BUFFER_SIZE_EVENT: // scrn buf. resizing
			//ResizeEventProc( irInBuf[i].Event.WindowBufferSizeEvent );
			break;

		case FOCUS_EVENT:  // disregard focus events

		case MENU_EVENT:   // disregard menu events
			break;

		default:
			ErrorExit("Unknown event type");
			break;
		}
	}

}
VOID ErrorExit(LPCSTR lpszMessage)
{
	fprintf(stderr, "%s\n", lpszMessage);

	// Restore input mode on exit.

	SetConsoleMode(hStdin, fdwSaveOldMode);

	ExitProcess(0);
}

VOID KeyEventProc(KEY_EVENT_RECORD ker)
{
	printf("Key event: ");

	if (ker.bKeyDown)
		printf("key pressed: %c %d %d\n", ker.uChar.AsciiChar, ker.wVirtualKeyCode, ker.wVirtualScanCode);
	else printf("key released: %c %d %d\n", ker.uChar.AsciiChar, ker.wVirtualKeyCode, ker.wVirtualScanCode);
	switch (ker.wVirtualKeyCode) {
	case 37:printf("ArrowLeft \n"); break;
	case 38:printf("ArrowUp \n"); break;
	case 39:printf("ArrowRight \n"); break;
	case 40:printf("ArrowDown\n"); break;

	}
}

VOID MouseEventProc(MOUSE_EVENT_RECORD mer)
{
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
	printf("Mouse event: ");

	switch (mer.dwEventFlags)
	{
	case 0:

		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			printf("left button press \n");
		}
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			printf("right button press \n");
		}
		else
		{
			printf("button press\n");
		}
		break;
	case DOUBLE_CLICK:
		printf("double click\n");
		break;
	case MOUSE_HWHEELED:
		printf("horizontal mouse wheel\n");
		break;
	case MOUSE_MOVED:
		printf("mouse moved\n");
		break;
	case MOUSE_WHEELED:
		printf("vertical mouse wheel\n");
		break;
	default:
		printf("unknown\n");
		break;
	}
}

VOID ResizeEventProc(WINDOW_BUFFER_SIZE_RECORD wbsr)
{
	printf("Resize event\n");
	printf("Console screen buffer is %d columns by %d rows.\n", wbsr.dwSize.X, wbsr.dwSize.Y);
}
*/
/*
int readkeybfile() {
	size_t nread;
	unsigned char buf[MAXBUFFERSIZE];
	FILE* theInput;
	memset(buf, 0, MAXBUFFERSIZE * sizeof(unsigned char));
	theInput = fopen("./keybfile.txt", "r");
	nread = fread(buf, sizeof(unsigned char), MAXBUFFERSIZE, theInput);
	fclose(theInput);
	unlink("./keybfile.txt");
	theInput = fopen("./keybfile.txt", "w");
	fclose(theInput);
	return nread;
}
*/
typedef struct Camera {
	NUMBER* vec3position;
	NUMBER* vec3lookat;
	NUMBER* vec3up;
	int intWidth;
	int intHeight;
	int prevMouseX;
	int prevMouseY;
	bool blnMouseBlocked;
	NUMBER* vec3forward;
	NUMBER* vec3right;
	NUMBER* angle;
	NUMBER* vec2mousePosition;
	NUMBER* mat4view;
	NUMBER* mat4proj;
	NUMBER* mat4transformation;
}Camera;
void keybfileAction(Camera* camera, unsigned char* buf, int intSize){
	unsigned char keyPressed=buf[0];
	NUMBER* tmpVec3=vec3_create(0,0,0);
	NUMBER* tmpVec3test=vec3_create(0,0,0);
	NUMBER* vec3position=vec3_create(0,0,0);
	switch(keyPressed){
		case 0x57:
	//if (GetAsyncKeyState(0x57) & 0x8000) {
		//printf("W key detected in background!\n");
		//camera->vec3position[2]-=KEYBOARDMOVE;
		vec3_scale(tmpVec3,camera->vec3forward,KEYBOARDMOVE);
		vec3_add(camera->vec3position,camera->vec3position,tmpVec3);
		//this.intKeyboardClick&=~1;
		//vec3_scale(tmpVec3test,camera->vec3forward,KEYBOARDMOVE);
		//vec3_add(vec3position,camera->vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	//}
	break;
		case 0x41:
	//if (GetAsyncKeyState(0x41) & 0x8000) {
		//printf("A key detected in background!\n");
		//camera->vec3position[0]-=KEYBOARDMOVE;
		vec3_scale(tmpVec3,camera->vec3right,KEYBOARDMOVE);
		vec3_add(camera->vec3position,camera->vec3position,tmpVec3);
		//this.intKeyboardClick&=~2;
		//vec3_scale(tmpVec3test,camera->vec3right,KEYBOARDMOVE);
		//vec3_add(vec3position,camera->vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	//}
		break;
	case 0x53:
	//if (GetAsyncKeyState(0x53) & 0x8000) {
		//camera->vec3position[2]+=KEYBOARDMOVE;
		//printf("S key detected in background!\n");
		vec3_scale(tmpVec3,camera->vec3forward,-KEYBOARDMOVE);
		vec3_add(camera->vec3position,camera->vec3position,tmpVec3);
		//this.intKeyboardClick&=~4;
		//vec3_scale(tmpVec3test,camera->vec3forward,-KEYBOARDMOVE);
		//vec3_add(vec3positionTest,this.camera.vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	//}
	break;
	case 0x44:
	//if (GetAsyncKeyState(0x44) & 0x8000) {
		//camera->vec3position[0]+=KEYBOARDMOVE;
		//printf("D key detected in background!\n");
		vec3_scale(tmpVec3,camera->vec3right,-KEYBOARDMOVE);
		vec3_add(camera->vec3position,camera->vec3position,tmpVec3);
		//this.intKeyboardClick&=~8;
		//vec3.scale(tmpVec3test,this.camera.vec3right,-this.slowTestSpeed*dt);
		//vec3.add(vec3positionTest,this.camera.vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	//}
	break;
	case 0x52:
	//if (GetAsyncKeyState(0x52) & 0x8000) {
		//printf("R key detected in background!\n");
		//camera->vec3position[1]+=KEYBOARDMOVE;
		camera->vec3position[1]+=KEYBOARDMOVE;
		//this.intKeyboardClick&=~16;
		//vec3positionTest[1]+=this.slowTestSpeed*dt;
		//this.decideMovement(vec3position,vec3positionTest);
	//}
	break;
	case 0x46:
	//if (GetAsyncKeyState(0x46) & 0x8000) {
		//printf("F key detected in background!\n");
		//camera->vec3position[1]-=KEYBOARDMOVE;
		camera->vec3position[1]-=KEYBOARDMOVE;
		//this.intKeyboardClick&=~32;
		//vec3positionTest[1]-=this.slowTestSpeed*dt;
		//this.decideMovement(vec3position,vec3positionTest);
	//}
	break;
	case 0x49:
	//if (GetAsyncKeyState(0x49) & 0x8000) {
		//printf("I key detected in background!\n");
		camera->vec2mousePosition[1]+=MOUSEMOVE;
	//}
	break;
	case 0x4C:
	//if (GetAsyncKeyState(0x4C) & 0x8000) {
		//printf("L key detected in background!\n");
		camera->vec2mousePosition[0]+=MOUSEMOVE;
	//}
	break;
	case 0x4B:
	//if (GetAsyncKeyState(0x4B) & 0x8000) {
		//printf("K key detected in background!\n");
		camera->vec2mousePosition[1]-=MOUSEMOVE;
	//}
	break;
	case 0x4A:
	//if (GetAsyncKeyState(0x4A) & 0x8000) {
		//printf("J key detected in background!\n");
		camera->vec2mousePosition[0]-=MOUSEMOVE;
	//}
	break;
	}
	vec3_dispose(tmpVec3);
	vec3_dispose(tmpVec3test);
	vec3_dispose(vec3position);

}
int readkeybfile(Camera* camera){
	FILE* theInput;
	theInput=fopen("./keybfile.txt","r");
	if(NULL==theInput)return -1;
	size_t nread;
	unsigned char buf[MAXBUFFERSIZE];
	memset(buf,0,MAXBUFFERSIZE*sizeof(unsigned char));
	nread=fread(buf,sizeof(unsigned char),MAXBUFFERSIZE,theInput);
	fclose(theInput);
	unlink("./keybfile.txt");
	theInput=fopen("./keybfile.txt","w");
	fclose(theInput);
	keybfileAction(camera,buf,MAXBUFFERSIZE);
	return nread;
}
/*
VOID keybScan(Camera* camera) {
	NUMBER* tmpVec3 = vec3_create(0, 0, 0);
	NUMBER* tmpVec3test = vec3_create(0, 0, 0);
	NUMBER* vec3position = vec3_create(0, 0, 0);
	if (GetAsyncKeyState(0x57) & 0x8000) {
		//printf("W key detected in background!\n");
//camera->vec3position[2]-=KEYBOARDMOVE;
		vec3_scale(tmpVec3, camera->vec3forward, KEYBOARDMOVE);
		vec3_add(camera->vec3position, camera->vec3position, tmpVec3);
		//this.intKeyboardClick&=~1;
		//vec3_scale(tmpVec3test,camera->vec3forward,KEYBOARDMOVE);
		//vec3_add(vec3position,camera->vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	}
	if (GetAsyncKeyState(0x41) & 0x8000) {
		//printf("A key detected in background!\n");
//camera->vec3position[0]-=KEYBOARDMOVE;
		vec3_scale(tmpVec3, camera->vec3right, KEYBOARDMOVE);
		vec3_add(camera->vec3position, camera->vec3position, tmpVec3);
		//this.intKeyboardClick&=~2;
		//vec3_scale(tmpVec3test,camera->vec3right,KEYBOARDMOVE);
		//vec3_add(vec3position,camera->vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	}
	if (GetAsyncKeyState(0x53) & 0x8000) {
		//camera->vec3position[2]+=KEYBOARDMOVE;
				//printf("S key detected in background!\n");
		vec3_scale(tmpVec3, camera->vec3forward, -KEYBOARDMOVE);
		vec3_add(camera->vec3position, camera->vec3position, tmpVec3);
		//this.intKeyboardClick&=~4;
		//vec3_scale(tmpVec3test,camera->vec3forward,-KEYBOARDMOVE);
		//vec3_add(vec3positionTest,this.camera.vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	}
	if (GetAsyncKeyState(0x44) & 0x8000) {
		//camera->vec3position[0]+=KEYBOARDMOVE;
				//printf("D key detected in background!\n");
		vec3_scale(tmpVec3, camera->vec3right, -KEYBOARDMOVE);
		vec3_add(camera->vec3position, camera->vec3position, tmpVec3);
		//this.intKeyboardClick&=~8;
		//vec3.scale(tmpVec3test,this.camera.vec3right,-this.slowTestSpeed*dt);
		//vec3.add(vec3positionTest,this.camera.vec3position,tmpVec3test);
		//this.decideMovement(vec3position,vec3positionTest);
	}
	if (GetAsyncKeyState(0x52) & 0x8000) {
		//printf("R key detected in background!\n");
//camera->vec3position[1]+=KEYBOARDMOVE;
		camera->vec3position[1] += KEYBOARDMOVE;
		//this.intKeyboardClick&=~16;
		//vec3positionTest[1]+=this.slowTestSpeed*dt;
		//this.decideMovement(vec3position,vec3positionTest);
	}
	if (GetAsyncKeyState(0x46) & 0x8000) {
		//printf("F key detected in background!\n");
//camera->vec3position[1]-=KEYBOARDMOVE;
		camera->vec3position[1] -= KEYBOARDMOVE;
		//this.intKeyboardClick&=~32;
		//vec3positionTest[1]-=this.slowTestSpeed*dt;
		//this.decideMovement(vec3position,vec3positionTest);
	}
	if (GetAsyncKeyState(0x49) & 0x8000) {
		//printf("I key detected in background!\n");
		camera->vec2mousePosition[1] += MOUSEMOVE;
	}
	if (GetAsyncKeyState(0x4C) & 0x8000) {
		//printf("L key detected in background!\n");
		camera->vec2mousePosition[0] += MOUSEMOVE;
	}
	if (GetAsyncKeyState(0x4B) & 0x8000) {
		//printf("K key detected in background!\n");
		camera->vec2mousePosition[1] -= MOUSEMOVE;
	}
	if (GetAsyncKeyState(0x4A) & 0x8000) {
		//printf("J key detected in background!\n");
		camera->vec2mousePosition[0] -= MOUSEMOVE;
	}
	vec3_dispose(tmpVec3);
	vec3_dispose(tmpVec3test);
	vec3_dispose(vec3position);
}
*/
NUMBER* Camera_getViewMatrix(Camera* camera, NUMBER* mat4view) {
	NUMBER* tmpVec3 = vec3_create(0, 0, 0);
	vec3_add(tmpVec3, camera->vec3position, camera->vec3lookat);
	mat4_lookAt(camera->mat4view, camera->vec3position, tmpVec3, camera->vec3up);
	//mat4_printf(camera->mat4view);
	vec3_dispose(tmpVec3);
	return mat4view;
}
Camera* CameraConstructor(NUMBER* vec3position, NUMBER* vec3lookat, NUMBER* vec3up, int intWidth, int intHeight) {
	Camera* camera = (Camera*)malloc(sizeof(Camera));
	camera->vec3position = (NUMBER*)malloc(3 * sizeof(NUMBER));
	memcpy(camera->vec3position, vec3position, 3 * sizeof(NUMBER));
	camera->vec3lookat = (NUMBER*)malloc(3 * sizeof(NUMBER));
	memcpy(camera->vec3lookat, vec3lookat, 3 * sizeof(NUMBER));
	camera->vec3up = (NUMBER*)malloc(3 * sizeof(NUMBER));
	memcpy(camera->vec3up, vec3up, 3 * sizeof(NUMBER));
	camera->intWidth = intWidth;
	camera->intHeight = intHeight;
	camera->prevMouseX = -1;
	camera->prevMouseY = -1;
	camera->blnMouseBlocked = false;
	camera->vec3forward = vec3_create(0, 0, 0);
	camera->vec3right = vec3_create(0, 0, 0);
	camera->angle = vec3_create(0, 0, 0);
	camera->vec2mousePosition = vec2_create(0, 0);
	vec3_subtract(camera->vec3forward, camera->vec3lookat, camera->vec3position);
	vec3_cross(camera->vec3up, camera->vec3right, camera->vec3forward);
	vec3_cross(camera->vec3right, camera->vec3forward, camera->vec3up);
	vec3_normalize(camera->vec3forward, camera->vec3forward);
	vec3_normalize(camera->vec3right, camera->vec3right);
	vec3_normalize(camera->vec3up, camera->vec3up);
	camera->mat4view = mat4_identity_();
	Camera_getViewMatrix(camera, camera->mat4view);
	camera->mat4proj = mat4_create_();
	//exit(0);

	return camera;
}
void Camera_updateVectors(Camera* camera) {
	vec3_subtract(camera->vec3forward, camera->vec3lookat, camera->vec3position);
	vec3_cross(camera->vec3up, camera->vec3right, camera->vec3forward);
	vec3_cross(camera->vec3right, camera->vec3forward, camera->vec3up);
	vec3_normalize(camera->vec3forward, camera->vec3forward);
	vec3_normalize(camera->vec3right, camera->vec3right);
	vec3_normalize(camera->vec3up, camera->vec3up);
}
void Camera_resizeScreen(Camera* camera, int intWidth, int intHeight) {
	camera->intWidth = intWidth;
	camera->intHeight = intHeight;
}
void Camera_updateMouse(Camera* camera) {
	camera->vec2mousePosition[0] = fmodf(camera->vec2mousePosition[0], 6.28);
	camera->vec2mousePosition[1] = fmodf(camera->vec2mousePosition[1], 6.28);
	camera->vec3forward[0] = sin(camera->vec2mousePosition[0]);
	camera->vec3forward[1] = 0;
	camera->vec3forward[2] = cos(camera->vec2mousePosition[0]);
	camera->vec3right[0] = -1. * cos(camera->vec2mousePosition[0]);
	camera->vec3right[1] = 0;
	camera->vec3right[2] = sin(camera->vec2mousePosition[0]);
	camera->vec3lookat[0] = (sin(camera->vec2mousePosition[0])) * (cos(camera->vec2mousePosition[1]));
	camera->vec3lookat[1] = sin(camera->vec2mousePosition[1]);
	camera->vec3lookat[2] = (cos(camera->vec2mousePosition[0])) * (cos(camera->vec2mousePosition[1]));
	vec3_cross(camera->vec3up, camera->vec3right, camera->vec3lookat);
}
void Camera_initMouse(Camera* camera) {
	camera->vec2mousePosition[0] = 0;
	camera->vec2mousePosition[1] = 0;
	Camera_updateMouse(camera);
}
void updateScene(Camera* camera, NUMBER dt, int* intSceneUpdate) {
	//camera->vec2mousePosition[0]+=0.1;//in radians
	//camera->vec3position[2]+=0.1;
	//camera->vec3position[3]+=1;
	/*
	if(0==*intSceneUpdate%3)
		camera->vec3position[0]+=1;
	if(1==*intSceneUpdate%3)
		camera->vec3position[1]+=1;
	if(2==*intSceneUpdate%3)
		camera->vec3position[2]+=1;
	*/
	(*intSceneUpdate)++;
	if (*intSceneUpdate > 100)*intSceneUpdate = 0;
	//camera->vec3position[2]+=1;
	//printf("vec2mousePosition: %lf, %lf \n",camera->vec2mousePosition[0],camera->vec2mousePosition[1]);
	//printf("vec3position: ");vec3_printf(camera->vec3position);printf("\n");
	//printf("vec3forward: ");vec3_printf(camera->vec3forward);printf("\n");
	if (camera->vec2mousePosition[0] > 100)camera->vec2mousePosition[0] = 0;
	Camera_updateMouse(camera);
	//Camera_updateVectors(camera);
	Camera_getViewMatrix(camera, camera->mat4view);
	//mat4_multiply(camera->mat4transformation,camera->mat4view,camera->mat4proj);
}
Camera* initCamera() {
	int intWidth = WIDTH;
	int intHeight = HEIGHT;
	//NUMBER* vec3position=vec3_create(0,0,0);
	//NUMBER* vec3position=vec3_create(-CUBEWIDTH,0,-1000);
	//NUMBER* vec3position=vec3_create(CUBEWIDTH/2,0,-CUBEWIDTH);
	//NUMBER* vec3position=vec3_create(WIDTH/2,HEIGHT/2,CAMERA);
	//NUMBER* vec3position=vec3_create(0,0,CAMERA);
	NUMBER* vec3position = vec3_create(0, 0, 0);
	//NUMBER* vec3position=vec3_create(-10000000000,0,-10*CUBEWIDTH);
	//NUMBER* vec3lookat=vec3_create(0,0,0);
	//NUMBER* vec3lookat=vec3_create(0,0,0);
	NUMBER* vec3lookat = vec3_create(0, 0, 0);
	//NUMBER* vec3lookat=vec3_create(0,0,.99999);
	NUMBER* vec3up = vec3_create(0, 0, 1);
	///Camera* camera=CameraConstructor(vec3position,vec3lookat,vec3up,intWidth/2,intHeight);
	Camera* camera = CameraConstructor(vec3position, vec3lookat, vec3up, intWidth, intHeight);
	mat4_printf(camera->mat4view);
	Camera_initMouse(camera);
	camera->vec2mousePosition[0] = toRadian(45);
	vec3_dispose(vec3position);
	vec3_dispose(vec3lookat);
	vec3_dispose(vec3up);
	return camera;
}
void transform(int* I, int* J, int* K, char theChar, Camera* camera) {
	NUMBER* mat4transform = mat4_identity_();
	NUMBER* vec4position = vec4_create((NUMBER)(*I), (NUMBER)(*J), (NUMBER)(*K), 1.);
	mat4_multiplyVec4(vec4position, camera->mat4view, vec4position);
	*I = (int)vec4position[0]; *J = (int)vec4position[1]; *K = (int)vec4position[2];
	(*K) += DISTANCEFROMCAMERA;

	NUMBER x, y;
	if (fabs(*K) > 0) {
		x = CHARSCALE * (*I) * DISTANCEFROMCAMERA / (*K) + WIDTH / 2;
		y = (*J) * DISTANCEFROMCAMERA/ (*K) + HEIGHT / 2;
	}

	*I = x; *J = y;
	NUMBER invAbsK = 1. / (fabs(*K));
	if (*I >= 0 && *J >= 0 && *I < WIDTH && *J < HEIGHT) {
		if (invAbsK > zBuffer[*I + (*J) * WIDTH]) {
			zBuffer[*I + (*J) * WIDTH] = invAbsK;
			buffer[*I + (*J) * WIDTH] = theChar;
		}
	}

	mat4_dispose(mat4transform);
	vec4_dispose(vec4position);
}
void transformFlt(NUMBER* I, NUMBER* J, NUMBER* K, char theChar, Camera* camera) {
	NUMBER* mat4transform = mat4_identity_();
	NUMBER* vec4position = vec4_create((*I), (*J), (*K), 1.);
	mat4_multiplyVec4(vec4position, camera->mat4view, vec4position);///
	*I = vec4position[0]; *J = vec4position[1]; *K = vec4position[2];
	(*K) += DISTANCEFROMCAMERA;
	NUMBER zrange = (CAMERAFAR + CAMERANEAR) / (CAMERAFAR - CAMERANEAR) + (1 / (*K)) * ((-2 * CAMERAFAR * CAMERANEAR) / (CAMERAFAR - CAMERANEAR));

	NUMBER invK = 1 / (*K);
	NUMBER x, y;
	if (zrange >= 1 || zrange <= -1)return;
	x = (*I) * DISTANCEFROMCAMERA / (*K) + WIDTH / 2;
	y = (*J) * DISTANCEFROMCAMERA / (*K) + HEIGHT / 2;

	*I = x; *J = y;

#ifdef INVERSEZBUFFER
	int intI = (int)(round(*I));
	int intJ = (int)(round(*J));
	int intK = (int)(round(*K));
	if (fabs(*K) > 0) {
		NUMBER invAbsK=1./(fabs(*K));
		if (intI >= 0 && intJ >= 0 && intI < WIDTH && intJ < HEIGHT) {
			if (invAbsK > zBuffer[intI + (intJ)*WIDTH]) {
				zBuffer[intI + (intJ)*WIDTH] = invAbsK;
				buffer[intI + (intJ)*WIDTH] = theChar;
			}
		}
	}
#else
	int intI = (int)(round(*I));
	int intJ = (int)(round(*J));
	int intK = (int)(round(*K));
	if (fabs(*K) > 0) {
		if (intI >= 0 && intJ >= 0 && intI < WIDTH && intJ < HEIGHT) {
			if (*K < zBuffer[intI + (intJ)*WIDTH]) {
				zBuffer[intI + (intJ)*WIDTH] = *K;
				buffer[intI + (intJ)*WIDTH] = theChar;
			}
		}
	}
#endif

	mat4_dispose(mat4transform);
	vec4_dispose(vec4position);
}
void transformAngular(int* I, int* J, int* K, NUMBER alpha, NUMBER beta, NUMBER gamma) {
	NUMBER sinAlpha = sin(alpha);
	NUMBER sinBeta = sin(beta);
	NUMBER sinGamma = sin(gamma);
	NUMBER cosAlpha = cos(alpha);
	NUMBER cosBeta = cos(beta);
	NUMBER cosGamma = cos(gamma);
	int i = round((*I) * cosBeta * cosGamma + (*J) * cosAlpha * sinGamma + (*J) * sinAlpha * sinBeta * cosGamma + (*K) * sinAlpha * sinGamma - (*K) * cosAlpha * sinBeta * cosGamma);
	int j = round(-(*I) * sinGamma * cosBeta + (*J) * cosAlpha * cosGamma - (*J) * sinAlpha * sinBeta * sinGamma + (*K) * sinAlpha * cosGamma + (*K) * cosAlpha * sinBeta * sinGamma);
	int k = round((*I) * sinBeta - (*J) * sinAlpha * cosBeta + (*K) * cosAlpha * cosBeta);
	*I = i; *J = j; *K = k;
}
void transformAngularFlt(NUMBER* I, NUMBER* J, NUMBER* K, NUMBER alpha, NUMBER beta, NUMBER gamma) {
	NUMBER sinAlpha = sin(alpha);
	NUMBER sinBeta = sin(beta);
	NUMBER sinGamma = sin(gamma);
	NUMBER cosAlpha = cos(alpha);
	NUMBER cosBeta = cos(beta);
	NUMBER cosGamma = cos(gamma);
	NUMBER i = ((*I) * cosBeta * cosGamma + (*J) * cosAlpha * sinGamma + (*J) * sinAlpha * sinBeta * cosGamma + (*K) * sinAlpha * sinGamma - (*K) * cosAlpha * sinBeta * cosGamma);
	NUMBER j = (-(*I) * sinGamma * cosBeta + (*J) * cosAlpha * cosGamma - (*J) * sinAlpha * sinBeta * sinGamma + (*K) * sinAlpha * cosGamma + (*K) * cosAlpha * sinBeta * sinGamma);
	NUMBER k = ((*I) * sinBeta - (*J) * sinAlpha * cosBeta + (*K) * cosAlpha * cosBeta);
	*I = i;
	*J = j;
	*K = k;
}
void cube(char* buffer, NUMBER ALPHA, Camera* camera) {
	printf("[%lf, %lf, %lf]   [%lf, %lf] \n", camera->vec3position[0], camera->vec3position[1], camera->vec3position[2], camera->vec2mousePosition[0], camera->vec2mousePosition[1]);
	int CUBEWIDTHHALF = CUBEWIDTH / 2;
	for (int i = -CUBEWIDTHHALF; i < CUBEWIDTHHALF; i++)
		for (int j = -CUBEWIDTHHALF; j < CUBEWIDTHHALF; j++) {
			I = i; J = j;
			K = -CUBEWIDTHHALF;
			transformAngular(&I, &J, &K, ALPHA, ALPHA, ALPHA);
			transform(&I, &J, &K, '@', camera);
		}

	for (int i = -CUBEWIDTHHALF; i < CUBEWIDTHHALF; i++)
		for (int j = -CUBEWIDTHHALF; j < CUBEWIDTHHALF; j++) {
			I = CUBEWIDTHHALF; J = j;
			K = i;
			transformAngular(&I, &J, &K, ALPHA, ALPHA, ALPHA);
			transform(&I, &J, &K, '~', camera);
		}

	for (int i = -CUBEWIDTHHALF; i < CUBEWIDTHHALF; i++)
		for (int j = -CUBEWIDTHHALF; j < CUBEWIDTHHALF; j++) {
			I = i; J = j;
			K = CUBEWIDTHHALF;
			transformAngular(&I, &J, &K, ALPHA, ALPHA, ALPHA);
			transform(&I, &J, &K, '+', camera);
		}

	for (int i = -CUBEWIDTHHALF; i < CUBEWIDTHHALF; i++)
		for (int j = -CUBEWIDTHHALF; j < CUBEWIDTHHALF; j++) {
			I = -CUBEWIDTHHALF; J = j;
			K = -i;
			transformAngular(&I, &J, &K, ALPHA, ALPHA, ALPHA);
			transform(&I, &J, &K, '#', camera);
		}

	for (int i = -CUBEWIDTHHALF; i < CUBEWIDTHHALF; i++)
		for (int j = -CUBEWIDTHHALF; j < CUBEWIDTHHALF; j++) {
			I = i; J = -CUBEWIDTHHALF;
			K = -j;
			transformAngular(&I, &J, &K, ALPHA, ALPHA, ALPHA);
			transform(&I, &J, &K, ';', camera);
		}
	for (int i = -CUBEWIDTHHALF; i < CUBEWIDTHHALF; i++)
		for (int j = -CUBEWIDTHHALF; j < CUBEWIDTHHALF; j++) {
			I = i; J = CUBEWIDTHHALF;
			K = j;
			transformAngular(&I, &J, &K, ALPHA, ALPHA, ALPHA);
			transform(&I, &J, &K, '.', camera);
		}
}
void circle(char* buffer, NUMBER ALPHA) {
	int R = 10;
	for (NUMBER alpha = 0; alpha < 6.28; alpha += 0.01) {
		I = (int)round(R * cos(alpha + ALPHA)) + WIDTH / 2; J = ((int)round(R * sin(alpha + ALPHA))) + HEIGHT / 2;
		if (I >= 0 && J >= 0 && I < WIDTH && J < HEIGHT)
			if (alpha < 1.57 && alpha >= 0.785)
				buffer[I + J * WIDTH] = '@';
			else
				buffer[I + J * WIDTH] = '+';
	}
}
typedef struct AmazingMazeLabyrinth {
	int intVisited; //=0
	int intMapWidth; //=10;
	int intMapHeight; //=10;
	int vec2Position[2]; //[2]={0,0};
	int vec2AmazingMazeLabyrinthEntryPoint[2];//[2]={0,0};
	int vec2AmazingMazeLabyrinthExitPoint[2];//[2]={this->intMapWidth/2,0};
	int* lstVec4Path;
	long lstVec4PathLength;
	unsigned char* arrMazeMap;
	int intMapPreviewCellSize;//=3
	int intMapPreviewWallSize;//=1
	char chrEmptySpace;
	char chrHorizonalWall;
	char chrVerticalWall;
	char chrCrawler;
	char chrEntryPoint;
	char chrExitPoint;
	char chrNullSpace;
	char fltMapPreviewScallingFactor;
	bool blnCameraCrawling;
	char chrWallTextures[14];//='#';
	char chrFloorTexture;//='~';
	char chrCeilTexture;//='.';
	int MAXWALLS;
	NUMBER* lstFlt4Walls;
	long lstFlt4WallsII;
	int intCellScallingFactor;//=2;
}AmazingMazeLabyrinth;
AmazingMazeLabyrinth* AmazingMazeLabyrinth_constructor() {
	AmazingMazeLabyrinth* maze = (AmazingMazeLabyrinth*)malloc(sizeof(AmazingMazeLabyrinth));
	maze->intVisited = 0;
	maze->intMapWidth = 10;
	maze->intMapHeight = 10;
	maze->vec2Position[0] = 0; maze->vec2Position[1] = 0;
	maze->vec2AmazingMazeLabyrinthExitPoint[0] = 0; maze->vec2AmazingMazeLabyrinthExitPoint[1] = 0;
	maze->vec2AmazingMazeLabyrinthExitPoint[0] = maze->intMapWidth / 2; maze->vec2AmazingMazeLabyrinthExitPoint[1] = 0;
	maze->arrMazeMap = (unsigned char*)malloc(maze->intMapWidth * maze->intMapHeight * sizeof(unsigned char));
	memset(maze->arrMazeMap, 0, maze->intMapWidth * maze->intMapHeight * sizeof(unsigned char));
	//unsigned char exampleMap[]={17,24,17,24,17,16,24,17,18,17,24,20,17,20,24,20,17,18,20,18,20,17,24,20,24,24,24,24,24,17,20,18,18,18,17,18,18,20,17,18,17,18,18,20,18,17,20,17,18,20,24,17,24,17,24,24,20,17,24,20,17,18,20,17,20,18,18,24,17,20,17,20,20,17,18,24,20,20,17,20,24,17,20,17,20,20,18,20,24,17,20,24,20,24,24,24,20,18,18,18};
	//unsigned char exampleMap[]={24,17,18,17,24,16,24,24,24,17,17,18,20,18,20,17,20,24,17,17,17,24,24,24,20,24,20,18,18,17,17,20,18,18,24,17,18,17,18,18,17,18,24,20,17,18,20,18,24,20,24,17,20,18,18,24,17,18,20,18,20,24,24,24,17,18,18,24,24,20,20,17,24,17,17,24,18,18,20,24,20,17,24,20,24,17,24,20,20,18,18,18,18,18,18,24,20,18};
	unsigned char exampleMap[] = { 17,18,18,18,18,16,18,18,18,17,17,24,20,24,24,17,18,17,20,18,17,20,18,20,17,17,20,18,24,20,17,24,24,20,17,17,24,20,20,18,24,20,24,17,18,24,17,20,24,20,20,17,18,17,20,18,17,20,20,18,20,18,20,18,17,20,24,24,24,20,17,18,18,18,18,20,24,24,17,18,24,17,24,24,17,20,20,18,17,20,20,24,20,18,24,20,18,18,18,20 };
	memcpy(maze->arrMazeMap, exampleMap, maze->intMapWidth * maze->intMapHeight * sizeof(unsigned char));
	maze->intMapPreviewCellSize = 3;
	maze->intMapPreviewWallSize = 1;
	maze->chrEmptySpace = ' ';
	maze->chrHorizonalWall = '-';
	maze->chrVerticalWall = '|';
	maze->chrCrawler = '@';
	maze->chrEntryPoint = '#';
	maze->chrExitPoint = '=';
	maze->chrNullSpace = ' ';
	//maze->fltMapPreviewScallingFactor=5;
	maze->fltMapPreviewScallingFactor = 1;
	maze->blnCameraCrawling = true;
	int intExamplePath[] = { 5,0,6,0,7,0,8,0,8,1,9,1,9,2,8,2,8,3,9,3,9,4,8,4,8,5,9,5,9,6,8,6,7,6,6,6,6,5,6,4,5,4,5,3,5,2,5,1,4,1,3,1,3,2,3,3,2,3,1,3,1,4,0,4,0,5,0,6,1,6,1,5,2,5,2,6,3,6,3,5,3,4,4,4,4,5,5,5,5,6,5,7,5,8,5,9,6,9,7,9,8,9,8,8,8,7,7,7,6,7,6,8,7,8 };
	maze->lstVec4PathLength = 114;
	maze->lstVec4Path = (int*)malloc(maze->lstVec4PathLength * sizeof(int));
	memcpy(maze->lstVec4Path, intExamplePath, maze->lstVec4PathLength * sizeof(int));
	//memcpy(maze->chrWallTextures,".,-~+:;=!*$#@",14);
	memcpy(maze->chrWallTextures, ".@,#-$~*+!:=;", 14);
	maze->chrFloorTexture = '~';
	maze->chrCeilTexture = '.';
	maze->MAXWALLS = (maze->intMapWidth + 1) * maze->intMapWidth + (maze->intMapHeight + 1) * maze->intMapHeight + 2 * maze->intMapWidth + 2 * maze->intMapWidth;
	maze->lstFlt4Walls = (NUMBER*)malloc(12 * maze->MAXWALLS * sizeof(NUMBER));
	maze->intCellScallingFactor = 2;
	maze->lstFlt4WallsII = 0;
	return maze;
}
unsigned char AmazingMazeLabyrinth_getMapIJ(AmazingMazeLabyrinth* maze, int i, int j) {
	return maze->arrMazeMap[ij(i, j, maze->intMapWidth, maze->intMapHeight)];
};
void AmazingMazeLabyrinth_setMapIJ(AmazingMazeLabyrinth* maze, int i, int j, unsigned char byteValue) {
	maze->arrMazeMap[ij(i, j, maze->intMapWidth, maze->intMapHeight)] = byteValue;
};
void AmazingMazeLabyrinth_createWall(AmazingMazeLabyrinth* maze, NUMBER fltX, NUMBER fltY, NUMBER fltZ, bool blnRotation) {
	//fltX*=WALLSSCALE;
	//fltY*=WALLSSCALE;
	//fltZ*=WALLSSCALE;
	if (false == blnRotation) {
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		//arrWallVertices=[fltX+0,fltY+0,fltZ+0,fltX+intCellScallingFactor,fltY+0,fltZ+0,fltX+intCellScallingFactor,fltY+intCellScallingFactor,fltZ+0,fltX+0,fltY+intCellScallingFactor,fltZ+0];
	}
	else {
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltX + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltY + maze->intCellScallingFactor);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		maze->lstFlt4Walls[maze->lstFlt4WallsII++] = WALLSSCALE * (fltZ + 0);
		printf("maze->lstFlt4Walls[ii]=%lf \n", maze->lstFlt4Walls[maze->lstFlt4WallsII - 1]);
		//arrWallVertices=[fltX+0,fltY+0,fltZ+0,fltX+0,fltY+0,fltZ+intCellScallingFactor,fltX+0,fltY+intCellScallingFactor,fltZ+intCellScallingFactor,fltX+0,fltY+intCellScallingFactor,fltZ+0];
	}
	printf("length=%d, limit=%d \n", maze->lstFlt4WallsII, 12 * maze->MAXWALLS);
};
void AmazingMazeLabyrinth_generateWallsTest(AmazingMazeLabyrinth* maze) {
	//void AmazingMazeLabyrinth_generateWalls(AmazingMazeLabyrinth* maze){
	AmazingMazeLabyrinth_createWall(maze, 0, 0, 0, true);
	AmazingMazeLabyrinth_createWall(maze, 0, 0, 0, false);
}
void AmazingMazeLabyrinth_generateWalls(AmazingMazeLabyrinth* maze) {

	NUMBER fltX, fltY, fltZ;
	fltY = 0;
	for (int I = 0; I < maze->intMapWidth; I++) {
		for (int J = 0; J < maze->intMapHeight; J++) {
			fltX = SCENESHIFTX + I * maze->intCellScallingFactor;
			fltZ = SCENESHIFTY + J * maze->intCellScallingFactor;
			if (maze->arrMazeMap[ij(I, J, maze->intMapWidth, maze->intMapHeight)] & 2 || maze->arrMazeMap[ij(I - 1, J, maze->intMapWidth, maze->intMapHeight)] & 8) {
			}
			else {
				//adding vertical wall,blue, rgb(0.,0.,1.)
				if (I - 1 > 0)
					AmazingMazeLabyrinth_createWall(maze, fltX, fltY, fltZ, true);
			}
			if (maze->arrMazeMap[ij(I, J - 1, maze->intMapWidth, maze->intMapHeight)] & 1 || maze->arrMazeMap[ij(I, J, maze->intMapWidth, maze->intMapHeight)] & 4) {
			}
			else {
				//adding horizontal wall, green, rgb(0.,1.,0.)
				if (J - 1 > 0)
					AmazingMazeLabyrinth_createWall(maze, fltX, fltY, fltZ, false);
			}
		}
	}
	/*
	for(int I=0;I<maze->intMapWidth;I++){
		J=maze->intMapHeight;
		fltX=I*maze->intCellScallingFactor;
		fltZ=(J)*maze->intCellScallingFactor;
		//adding horizontal wall, green, rgb(0.,1.,0.)
		AmazingMazeLabyrinth_createWall(maze,fltX,fltY,fltZ,false);
	}
	for(int J=0;J<maze->intMapHeight;J++){
		I=maze->intMapWidth;
		fltX=I*maze->intCellScallingFactor;
		fltZ=J*maze->intCellScallingFactor;
		//adding vertical wall,blue, rgb(0.,0.,1.)
		AmazingMazeLabyrinth_createWall(maze,fltX,fltY,fltZ,true);
	}
	*/
	//maze->lstFlt4WallsII--;
};
void AmazingMazeLabyrinth_inversePath(AmazingMazeLabyrinth* maze) {
	int* lstVec4PathInversed = (int*)malloc(maze->lstVec4PathLength * sizeof(int));
	int ii;
	for (ii = (maze->lstVec4PathLength) / 2 - 1; ii >= 0; ii--) {
		lstVec4PathInversed[2 * ii + 0] = maze->lstVec4Path[2 * (maze->lstVec4PathLength / 2 - ii - 1) + 0];
		lstVec4PathInversed[2 * ii + 1] = maze->lstVec4Path[2 * (maze->lstVec4PathLength / 2 - ii - 1) + 1];
		printf("%d -> %d, %d -> %d = %d, %d \n", 2 * ii + 0, 2 * (maze->lstVec4PathLength / 2 - ii - 1) + 0, 2 * ii + 1, 2 * (maze->lstVec4PathLength / 2 - ii - 1) + 1, maze->lstVec4Path[2 * (maze->lstVec4PathLength / 2 - ii - 1) + 0], maze->lstVec4Path[2 * (maze->lstVec4PathLength / 2 - ii - 1) + 1]);
	}
	for (ii = 0; ii < maze->lstVec4PathLength; ii++)
		maze->lstVec4Path[ii] = lstVec4PathInversed[ii];
	maze->vec2Position[0] = maze->lstVec4Path[0];
	maze->vec2Position[1] = maze->lstVec4Path[1];
	printf("%d, %d \n", maze->vec2Position[0], maze->vec2Position[1]);
	free(lstVec4PathInversed);
}
void AmazingMazeLabyrinth_canvasMapPreview_drawGrid(AmazingMazeLabyrinth* maze, int intPreviewWidth, int intPreviewHeight) {
	unsigned char arrMazeMapCopy[100];
	int intMaxIndex = 0;
	int maxI = 0, maxJ = 0;
	memset(arrMazeMapCopy, 0, 100);
	int I, J, index;
	int intDisplay_i, intDisplay_j;
	int intWallToDraw = (maze->intMapPreviewCellSize + maze->intMapPreviewWallSize) * maze->fltMapPreviewScallingFactor;
	int i, j;
	for (i = 0; i < intPreviewWidth; i++) {
		for (j = 0; j < intPreviewHeight; j++) {
			I = floor(i / intWallToDraw);
			J = floor(j / intWallToDraw);
			if (I > maxI)maxI = I;
			if (J > maxJ)maxJ = J;
			intDisplay_i = i;
			intDisplay_j = j;
			index = (intDisplay_j * WIDTH + intDisplay_i);
			buffer[index] = '#';
			if (index > intMaxIndex)intMaxIndex = index;
			arrMazeMapCopy[ij(I, J, maze->intMapWidth, maze->intMapHeight)] = maze->arrMazeMap[ij(I, J, maze->intMapWidth, maze->intMapHeight)];
			if ((i % intWallToDraw >= 0 && i % intWallToDraw < intWallToDraw / 4) || (j % intWallToDraw >= 0 && j % intWallToDraw < intWallToDraw / 4)) {
				if (i % intWallToDraw >= 0 && i % intWallToDraw < intWallToDraw / 4) {
					/*vertical*/
					if (maze->arrMazeMap[ij(I, J, maze->intMapWidth, maze->intMapHeight)] & 2 || maze->arrMazeMap[ij(I - 1, J, maze->intMapWidth, maze->intMapHeight)] & 8) {
						if (I - 1 >= 0) {
							buffer[index] = maze->chrEmptySpace;
						}
						else buffer[index] = maze->chrVerticalWall;
					}
					else {
						buffer[index] = maze->chrVerticalWall;
					}
				}
				if (j % intWallToDraw >= 0 && j % intWallToDraw < intWallToDraw / 4) {
					/*horizontal*/
					if (maze->arrMazeMap[ij(I, J - 1, maze->intMapWidth, maze->intMapHeight)] & 1 || maze->arrMazeMap[ij(I, J, maze->intMapWidth, maze->intMapHeight)] & 4) {
						if (J - 1 >= 0) {
							buffer[index] = maze->chrEmptySpace;
						}
						else buffer[index] = maze->chrHorizonalWall;
					}
					else {
						buffer[index] = maze->chrHorizonalWall;
					}
				}
			}
			else {
				if (16 == (maze->arrMazeMap[ij(I, J, maze->intMapWidth, maze->intMapHeight)] & 16)) {
					if (I == maze->vec2Position[0] && J == maze->vec2Position[1]) {
						buffer[index] = maze->chrCrawler;
					}
					else if (I == maze->vec2AmazingMazeLabyrinthExitPoint[0] && J == maze->vec2AmazingMazeLabyrinthExitPoint[1]) {
						buffer[index] = maze->chrExitPoint;
					}
					else if (I == maze->vec2AmazingMazeLabyrinthEntryPoint[0] && J == maze->vec2AmazingMazeLabyrinthEntryPoint[1]) {
						buffer[index] = maze->chrEntryPoint;
					}
					else {
						buffer[index] = maze->chrEmptySpace;
					}
				}
				else {
					buffer[index] = maze->chrNullSpace;
				}
			}
		}
	}
	for (i = 0; i < intPreviewWidth; i++) {
		index = (maze->intMapWidth * intWallToDraw * WIDTH + i);
		buffer[index] = maze->chrHorizonalWall;
	}
	for (j = 0; j < intPreviewHeight; j++) {
		index = (j * WIDTH + maze->intMapWidth * intWallToDraw);
		buffer[index] = maze->chrVerticalWall;
	}
};
AmazingMazeLabyrinth* AmazingMazeLabyrinth_generateMaze() {
	AmazingMazeLabyrinth* maze = AmazingMazeLabyrinth_constructor();
	AmazingMazeLabyrinth_inversePath(maze);
	AmazingMazeLabyrinth_generateWalls(maze);
	printf("intMapWidth=%d, intMapHeight=%d \n", maze->intMapWidth, maze->intMapHeight);
	return maze;

}
void AmazingMazeLabyrinth_nextStep(AmazingMazeLabyrinth* maze) {
	maze->intVisited++;
	if (maze->intVisited >= maze->lstVec4PathLength / 2)maze->intVisited = 0;
	maze->vec2Position[0] = maze->lstVec4Path[2 * maze->intVisited + 0];
	maze->vec2Position[1] = maze->lstVec4Path[2 * maze->intVisited + 1];
}
void AmazingMazeLabyrinth_printWalls(AmazingMazeLabyrinth* maze, NUMBER ALPHA, Camera* camera) {
	NUMBER fltX, fltY, fltZ;
	NUMBER fltX0 = 0, fltY0 = 0, fltZ0 = 0;
	NUMBER fltX1 = 0, fltY1 = 0, fltZ1 = 0;
	NUMBER fltX2 = 0, fltY2 = 0, fltZ2 = 0;
	NUMBER fltX3 = 0, fltY3 = 0, fltZ3 = 0;
	int jj = 0;
	while (jj < maze->lstFlt4WallsII) {
		fltX0 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY0 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ0 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltX1 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY1 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ1 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltX2 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY2 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ2 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltX3 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY3 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ3 = maze->lstFlt4Walls[jj++];
		printf("%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf \n", fltX0, fltY0, fltZ0, fltX1, fltY1, fltZ1, fltX2, fltY2, fltZ2, fltX3, fltY3, fltZ3);
	}
	printf("=================================================================\n");
}
void AmazingMazeLabyrinth_renderWalls(AmazingMazeLabyrinth* maze, NUMBER ALPHA, Camera* camera) {
	//printf("AmazingMazeLabyrinth_renderWalls, %lf \n",camera->vec3position[2]);
	NUMBER fltX, fltY, fltZ;
	NUMBER fltX0 = 0, fltY0 = 0, fltZ0 = 0;
	NUMBER fltX1 = 0, fltY1 = 0, fltZ1 = 0;
	NUMBER fltX2 = 0, fltY2 = 0, fltZ2 = 0;
	NUMBER fltX3 = 0, fltY3 = 0, fltZ3 = 0;
	/*
	NUMBER* fltX=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltY=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltZ=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltX0=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltY0=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltZ0=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltX1=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltY1=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltZ1=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltX2=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltY2=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltZ2=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltX3=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltY3=(NUMBER*)malloc(sizeof(NUMBER));
	NUMBER* fltZ3=(NUMBER*)malloc(sizeof(NUMBER));
	*/
	int jj = 0;
	//int intWallTexture=0;
	int intWallTexture = 1;
	//printf("maze->lstFlt4WallsII=%d \n",maze->lstFlt4WallsII);
	printf("[%lf, %lf, %lf]   [%lf, %lf] \n", camera->vec3position[0], camera->vec3position[1], camera->vec3position[2], camera->vec2mousePosition[0], camera->vec2mousePosition[1]);
	while (jj < maze->lstFlt4WallsII) {
		fltX0 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY0 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ0 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltX1 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY1 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ1 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltX2 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY2 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ2 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltX3 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltY3 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		fltZ3 = maze->lstFlt4Walls[jj++];
		//printf("flt=%lf \n",maze->lstFlt4Walls[jj-1]);
		//printf("=========================================\n");
		//printf("%lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf, %lf \n",fltX0,fltY0,fltZ0,fltX1,fltY1,fltZ1,fltX2,fltY2,fltZ2,fltX3,fltY3,fltZ3);
		//if(fltX0==fltX1){
		if (fabs(fltX0 - fltX1) < epsilon) {
			//printf("fltX0==fltX1 \n");
			fltX = fltX0;
			for (NUMBER k = fltZ0; k < fltZ1; k += CHARSDENSITY)
				for (NUMBER j = fltY0; j < fltY3; j += CHARSDENSITY) {
					fltX = fltX0;
					fltY = j;
					fltZ = k;
					transformAngularFlt(&fltX, &fltY, &fltZ, ALPHA, ALPHA, ALPHA);
					transformFlt(&fltX, &fltY, &fltZ, maze->chrWallTextures[intWallTexture], camera);
				}
		}
		if (fabs(fltZ0 - fltZ1) < epsilon) {
			fltZ = fltZ0;
			for (NUMBER i = fltX0; i < fltX1; i += CHARSDENSITY)
				for (NUMBER j = fltY0; j < fltY3; j += CHARSDENSITY) {
					fltX = i; fltY = j;
					fltZ = fltZ0;
					transformAngularFlt(&fltX, &fltY, &fltZ, ALPHA, ALPHA, ALPHA);
					transformFlt(&fltX, &fltY, &fltZ, maze->chrWallTextures[intWallTexture], camera);
				}

		}
		intWallTexture++; if (intWallTexture > 13)intWallTexture = 0;
	}
	/*
	free(fltX);
	free(fltY);
	free(fltZ);
	free(fltX0);
	free(fltY0);
	free(fltZ0);
	free(fltX1);
	free(fltY1);
	free(fltZ1);
	free(fltX2);
	free(fltY2);
	free(fltZ2);
	free(fltX3);
	free(fltY3);
	free(fltZ3);
	*/
	//printf("renderWalls done \n");
}
int main(int argc, char** argv) {
	//initMouse();

	Camera* camera = initCamera();
	//exit(0);
	AmazingMazeLabyrinth* maze = NULL;
	int WIDTHHEIGHT = WIDTH * HEIGHT;
	int WIDTHHEIGHTSIZEOFCHAR = WIDTH * HEIGHT * sizeof(char);
	int WIDTHHEIGHTSIZEOFNUMBER = WIDTH * HEIGHT * sizeof(NUMBER);
	//struct timespec remaining, request = { 0,999999999 };
	struct timespec remaining, request = { 0,50000000 };
	int intChoice = 2;
	if (2 == argc)intChoice = atoi(argv[1]);
	if (intChoice >= 8) {
		int index;
		maze = AmazingMazeLabyrinth_generateMaze();
		for (int i = 0; i < maze->intMapWidth; i++) {
			for (int j = 0; j < maze->intMapHeight; j++) {
				printf("%03d ", (unsigned int)maze->arrMazeMap[ij(i, j, maze->intMapWidth, maze->intMapHeight)]);
			}
			printf("\n");
		}
		//return 0;
	}else camera->vec2mousePosition[1]=1.5;	
	ALPHA = 0;
	printf("\x1b[?25l");
	printf("\x1b[2J");
	printf("\x1b[H");
	int keybread = 0;
	NUMBER dt = 0;
	int intSceneUpdate = 0;
	camera->vec2mousePosition[0] = toRadian(45);
	camera->vec3position[1] = 25;
	while (1) {
		updateScene(camera, dt, &intSceneUpdate);
#ifndef SUPERFAST
		/*
		keybread = readkeybfile();
		if (keybread) {
			blnPause ^= 1;
		}
		if (true == blnPause)continue;
		*/
#endif
		memset(buffer, ' ', WIDTHHEIGHTSIZEOFCHAR);
#ifdef INVERSEZBUFFER
		memset(zBuffer, 0., WIDTHHEIGHTSIZEOFNUMBER);
#else
		//memset(zBuffer,MAXDEPTH,WIDTHHEIGHTSIZEOFNUMBER);
		//memset(zBuffer, -MAXDEPTH, WIDTHHEIGHTSIZEOFNUMBER);
#endif
		K = 0;

		switch (intChoice) {
		case CHARMINGCIRCLE:circle(buffer, ALPHA); break;
		case CHARMINGCUBE:cube(buffer, ALPHA, camera); break;
		case CHARMINGMAZEMAP:AmazingMazeLabyrinth_canvasMapPreview_drawGrid(maze, (maze->intMapPreviewCellSize + maze->intMapPreviewWallSize) * maze->intMapWidth, (maze->intMapPreviewCellSize + maze->intMapPreviewWallSize) * maze->intMapHeight); AmazingMazeLabyrinth_nextStep(maze); break;
		case CHARMINGMAZE:AmazingMazeLabyrinth_renderWalls(maze, ALPHA, camera); AmazingMazeLabyrinth_nextStep(maze); break;
		case PRINTWALLS:AmazingMazeLabyrinth_printWalls(maze, ALPHA, camera); AmazingMazeLabyrinth_nextStep(maze); break;
		}
		/*
		for(int j=0;j<HEIGHT;j++)
			for(int i=0;i<WIDTH;i++){
				putchar(buffer[i+j*WIDTH]);
				//printf("i=%d, j=%d \n",i,j);
			}
			putchar('\n');
		*/
		
		
		for (int jj = 0; jj < WIDTH * HEIGHT; jj++)
			if ('\0' == buffer[jj])buffer[jj] = ' ';
				
		buffer[WIDTHHEIGHT] = '\0';
		printf("%s", buffer); //much faster than putchar char by char
		
		printf("\x1b[H");
		if(intChoice<8)
			ALPHA+=0.1;
		if (ALPHA > 6.28)ALPHA -= 6.28;
		//usleep(1000);

		//keybScan(camera);
		readkeybfile(camera);
		//mouseScan();
		nanosleep(&request, &remaining);
	}
	//SetConsoleMode(hStdin, fdwSaveOldMode);
	return 0;

}

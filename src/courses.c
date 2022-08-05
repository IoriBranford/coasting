#include "courses.h"
static Track TRACKS_intro[] = {
{
.dx = 64, .dy = 0,
.x0 = 0, .y0 = 0,
.f_start = 0, .f_len = 262144,
.f_angle = 0, .f_dirx = 4096, .f_diry = 0,
},
{
.dx = 16, .dy = 8,
.x0 = 64, .y0 = 0,
.f_start = 262144, .f_len = 73271,
.f_angle = 302, .f_dirx = 3663, .f_diry = 1831,
},
{
.dx = 16, .dy = 16,
.x0 = 80, .y0 = 8,
.f_start = 335415, .f_len = 92681,
.f_angle = 512, .f_dirx = 2896, .f_diry = 2896,
},
{
.dx = 64, .dy = 208,
.x0 = 96, .y0 = 24,
.f_start = 428096, .f_len = 891385,
.f_angle = 829, .f_dirx = 1204, .f_diry = 3914,
},
{
.dx = 16, .dy = 16,
.x0 = 160, .y0 = 232,
.f_start = 1319481, .f_len = 92681,
.f_angle = 512, .f_dirx = 2896, .f_diry = 2896,
},
{
.dx = 16, .dy = 8,
.x0 = 176, .y0 = 248,
.f_start = 1412162, .f_len = 73271,
.f_angle = 302, .f_dirx = 3663, .f_diry = 1831,
},
{
.dx = 64, .dy = 0,
.x0 = 192, .y0 = 256,
.f_start = 1485433, .f_len = 262144,
.f_angle = 0, .f_dirx = 4096, .f_diry = 0,
},
};
static Track TRACKS_uphill[] = {
{
.dx = 64, .dy = 0,
.x0 = 0, .y0 = 0,
.f_start = 0, .f_len = 262144,
.f_angle = 0, .f_dirx = 4096, .f_diry = 0,
},
{
.dx = 16, .dy = -8,
.x0 = 64, .y0 = 0,
.f_start = 262144, .f_len = 73271,
.f_angle = -303, .f_dirx = 3663, .f_diry = -1832,
},
{
.dx = 16, .dy = -16,
.x0 = 80, .y0 = -8,
.f_start = 335415, .f_len = 92681,
.f_angle = -512, .f_dirx = 2896, .f_diry = -2897,
},
{
.dx = 16, .dy = -24,
.x0 = 96, .y0 = -24,
.f_start = 428096, .f_len = 118146,
.f_angle = -641, .f_dirx = 2272, .f_diry = -3409,
},
{
.dx = 16, .dy = -16,
.x0 = 112, .y0 = -48,
.f_start = 546242, .f_len = 92681,
.f_angle = -512, .f_dirx = 2896, .f_diry = -2897,
},
{
.dx = 16, .dy = -8,
.x0 = 128, .y0 = -64,
.f_start = 638923, .f_len = 73271,
.f_angle = -303, .f_dirx = 3663, .f_diry = -1832,
},
{
.dx = 112, .dy = 0,
.x0 = 144, .y0 = -72,
.f_start = 712194, .f_len = 458752,
.f_angle = 0, .f_dirx = 4096, .f_diry = 0,
},
};
Course COURSES[] = {
{
.name = "intro", .f_len = 1747577,
.num_tracks = 7,
.tracks = TRACKS_intro
},
{
.name = "uphill", .f_len = 1170946,
.num_tracks = 7,
.tracks = TRACKS_uphill
},
};

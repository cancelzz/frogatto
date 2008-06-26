#ifndef DRAW_TILE_HPP_INCLUDED
#define DRAW_TILE_HPP_INCLUDED

#include "level_object.hpp"
#include "texture.hpp"

void draw_tile(const level_tile& t);
void draw_from_tilesheet(const graphics::texture& t, int tile_num, int x, int y, bool reverse, int rotate=0);

#endif

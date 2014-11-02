/** @file utility.hpp
@author jim
@date 19/09/2014

The MIT License (MIT)

Copyright © 2014 jim

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <cmath>
#include <cassert>

#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <ostream>
#include <fstream>

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <Thor/Animations/FrameAnimation.hpp>

#include <SFML/Config.hpp>

#define SFTD_UNUSED(x) (void)(x)

std::string getFileContents(const std::string& filename);

inline size_t xyToTileIndex(sf::Vector2u t_point, unsigned int t_mapWidth,
                            unsigned int t_tileWidth)
{
    return t_point.y * t_mapWidth / t_tileWidth + t_point.x;
}

inline size_t xyToTileIndex(unsigned int t_x, unsigned int t_y,
                            unsigned int t_mapWidth, unsigned int t_tileWidth)
{
    return t_y * t_mapWidth / t_tileWidth + t_x;
}

inline sf::Vector2u tileIndexToXy(unsigned t_index,
                                  unsigned int t_tileWidth, unsigned int t_tileHeight)
{
    return sf::Vector2u(t_index * t_tileWidth, t_index * t_tileHeight);
}

template <typename T>
inline T strToT(const std::string& t_str)
{
    std::istringstream ss(t_str);
    T value;
    ss >> value;
    return value;
}

inline std::vector<std::string> split(const std::string &t_str, char t_delim)
{
    std::vector<std::string> elems;
    std::istringstream ss(t_str);
    std::string item;
    while (std::getline(ss, item, t_delim))
    {
        elems.push_back(item);
    }
    return elems;
}

template <typename T>
inline T map(T t_x, T t_in_min, T t_in_max, T t_out_min, T t_out_max)
{
  return (t_x - t_in_min) * (t_out_max - t_out_min) / (t_in_max - t_in_min) + t_out_min;
}

template <typename T>
inline sf::Vector2<T> stringToSfVec(const std::string& t_str, char t_delim)
{
    std::vector<std::string> strElems = split(t_str, t_delim);
    if (strElems.size() != 2)
    {
        throw std::invalid_argument("stringToSfVec : Input String must "
                                    "contain only x and y coordinates !");
    }
    return sf::Vector2<T>(strToT<T>(strElems[0]), strToT<T>(strElems[1]));
}

template <typename T>
inline bool isInRange(T t_val, T t_min, T t_max)
{
    return (t_val >= t_min) && (t_val <= t_max);
}

template <typename T>
std::ostream& operator<<(std::ostream& t_os, const sf::Vector2<T> & t_n)
{
    return t_os << "sfVector2(" << t_n.x << ", " << t_n.y << ")";
}

template <typename T>
std::ostream& operator<<(std::ostream& t_os, const sf::Rect<T> & t_n)
{
    return t_os << "sfRect({" << t_n.left << ", " << t_n.top << "}, "
                << "{"        << t_n.width << ", " << t_n.height << "})";
}

inline void addFramesY(thor::FrameAnimation& animation, int x, int yFirst,
                int yLast, int xMultiple, int yMultiple, float duration = 1.f)
{
    const int step = (yFirst < yLast) ? +1 : -1;
    yLast += step; // so yLast is excluded in the range

    for (int y {yFirst}; y != yLast; y += step)
    {
        animation.addFrame(duration, sf::IntRect(xMultiple*x, yMultiple*y,
                                                 xMultiple, yMultiple));
    }
}

inline void addFramesX(thor::FrameAnimation& animation, int y, int xFirst,
                int xLast, int xMultiple, int yMultiple, float duration = 1.f)
{
    const int step = (xFirst < xLast) ? +1 : -1;
    xLast += step; // so yLast is excluded in the range

    for (int x {xFirst}; x != xLast; x += step)
    {
        animation.addFrame(duration, sf::IntRect(xMultiple*x, yMultiple*y,
                                                 xMultiple, yMultiple));
    }
}

#endif // UTILITY_HPP

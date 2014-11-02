/** @file resourceIdentifiers.hpp
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

#ifndef RESOURCEIDENTIFIERS_H
#define RESOURCEIDENTIFIERS_H

#include <string>

// Forward declaration of SFML classes
namespace sf
{
class Texture;
class Font;
class SoundBuffer;
}

namespace Fonts
{

enum ID
{
    Main
};

}

namespace SoundEffect
{
enum ID
{
    AlliedGunfire,
    EnemyGunfire,
    Explosion1,
    Explosion2,
    LaunchMissile,
    CollectPickup,
    Button
};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, std::string> TextureHolder;

typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

typedef ResourceHolder<sf::SoundBuffer, SoundEffect::ID> SoundBufferHolder;

#endif // RESOURCEIDENTIFIERS_H

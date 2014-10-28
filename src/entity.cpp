/** @file entity.cpp
Entity
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

#include "entity.hpp"

void Entity::setVelocity(float t_dx, float t_dy)
{
    m_movement.x = t_dx;
    m_movement.y = t_dy;
}

void Entity::setVelocity(sf::Vector2f t_inVelocity)
{
    m_movement = t_inVelocity;
}

void Entity::setPosition(const sf::Vector2f &t_inPos)
{
    m_sprite.setPosition(t_inPos);
}

void Entity::setPosition(float t_x, float t_y)
{
    m_sprite.setPosition(t_x, t_y);
}

void Entity::setScale(const sf::Vector2f &t_factor)
{
    m_sprite.setScale(t_factor);
}

void Entity::setScale(float t_x, float t_y)
{
    m_sprite.setScale(t_x, t_y);
}

void Entity::setOrigin(const sf::Vector2f &t_inOrigin)
{
    m_sprite.setOrigin(t_inOrigin);
}

void Entity::setOrigin(float t_x, float t_y)
{
    m_sprite.setOrigin(t_x, t_y);
}

void Entity::setRotation(float t_angle)
{
    m_sprite.setRotation(t_angle);
}

void Entity::rotate(float t_angle)
{
    m_sprite.rotate(t_angle);
}

void Entity::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    if (visible())
    {
        t_states.transform *= getTransform();
        t_target.draw(m_sprite, t_states);
    }
}

/** @file enemy.cpp
Enemy
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

#include "enemy.hpp"

Enemy::Enemy(const std::string& t_inName, const TextureHolder& t_textures,
             const sf::Vector2f& t_position, const sf::Vector2f& t_scale,
             const unsigned int t_life, const Type t_type, const DmgType t_resist)
    : Enemy(t_inName, t_position, t_scale, t_life, t_type, t_resist)
{
    setTextureHolder(t_textures);
}

Enemy::Enemy(const std::string& t_inName, const sf::Vector2f& t_position,
             const sf::Vector2f& t_scale, const unsigned int t_life,
             const Type t_type, const DmgType t_resist)
    : m_curLife(t_life), m_maxLife(t_life), m_type(t_type), m_resistances(t_resist)
{
    m_name = t_inName;
    m_sprite.setScale(t_scale);
    m_sprite.setPosition(t_position);

    init();
}


void Enemy::init()
{
    m_lifeBar.setFillColor(sf::Color::Red);
    m_lifeBar.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - 10);
    m_lifeBar.setSize(sf::Vector2f(30, 4));

    m_explosion.setPosition(m_sprite.getPosition());
    m_explosion.setScale(m_sprite.getScale() / 2.f);

    if (m_resistances == DmgType::Ice)
    {
        m_sprite.setColor(sf::Color(138, 241, 255)); // Light blue
    }
    else if (m_resistances == DmgType::Fire)
    {
        m_sprite.setColor(sf::Color(255, 166, 128)); // Light orange
    }
    else if (m_resistances == DmgType::Shock)
    {
        m_sprite.setColor(sf::Color(255, 255, 128)); // Light yellow
    }
    else if (m_resistances == DmgType::Poison)
    {
        m_sprite.setColor(sf::Color(186, 255, 163)); // Light green
    }

    initAnimations();
}

void Enemy::initAnimations()
{
    auto addFramesY = [](thor::FrameAnimation& animation, int x, int yFirst,
                        int yLast, int xMultiple, int yMultiple, float duration)
    {
        const int step = (yFirst < yLast) ? +1 : -1;
        yLast += step; // so yLast is excluded in the range

        for (int y {yFirst}; y != yLast; y += step)
        {
            animation.addFrame(duration, sf::IntRect(xMultiple*x, yMultiple*y,
                                                     xMultiple, yMultiple));
        }
    };
    auto addFramesX = [](thor::FrameAnimation& animation, int y, int xFirst,
                        int xLast, int xMultiple, int yMultiple, float duration)
    {
        const int step = (xFirst < xLast) ? +1 : -1;
        xLast += step; // so yLast is excluded in the range

        for (int x {xFirst}; x != xLast; x += step)
        {
            animation.addFrame(duration, sf::IntRect(xMultiple*x, yMultiple*y,
                                                     xMultiple, yMultiple));
        }
    };
    thor::FrameAnimation walk;

    walk.addFrame(1.f, sf::IntRect(28*1, 0, 28, 27));
    addFramesX(walk, 0, 0, 2, 28, 27, 1.f);

    thor::FrameAnimation explode;
    for (int i {}; i < 5; ++i)
    {
        addFramesX(explode, i, 0, 4, 59, 48, 1.f);
    }

    m_spriteAnimator.addAnimation("walk", walk, sf::seconds(1.f));
    m_explosionAnimator.addAnimation("explode", explode, sf::seconds(0.5f));
}

void Enemy::setTextureHolder(const TextureHolder &t_textures)
{
    m_sprite.setTexture(t_textures.get(Textures::Beetle));
    m_explosion.setTexture(t_textures.get(Textures::Explosion));
}

void Enemy::setFontHolder(const FontHolder &t_fonts)
{
    SFTD_UNUSED(t_fonts);
}

void Enemy::takeDamage(unsigned int t_damageAmount, DmgType t_dmgtype)
{

    unsigned int resultingAmount = t_damageAmount;

    if (t_dmgtype == DmgType::Ice)
    {
        if (m_resistances == t_dmgtype)
        {
            resultingAmount /= 5;
        }
        else if (m_resistances == DmgType::Fire)
        {
            resultingAmount *= 3;
        }
    }
    else if (t_dmgtype == DmgType::Fire)
    {
        if (m_resistances == t_dmgtype)
        {
            resultingAmount /= 5;
        }
        else if (m_resistances == DmgType::Ice)
        {
            resultingAmount *= 3;
        }
    }
    else if (t_dmgtype == DmgType::Poison && m_resistances == t_dmgtype)
    {
        resultingAmount /= 5;
    }
    else if (t_dmgtype == DmgType::Shock && m_resistances == t_dmgtype)
    {
        resultingAmount /= 5;
    }

    if (resultingAmount >= m_curLife)
    {
        m_curLife = 0;
        m_visible = false;
    }
    else
    {
        m_curLife -= resultingAmount;
    }
    m_lifeBar.setSize(sf::Vector2f(map(m_curLife, 0u, m_maxLife, 0u, 30u), 5.f));
    m_explosionAnimator.playAnimation("explode");
}

void Enemy::heal(unsigned int t_lifeAmount)
{
    if (m_curLife > 0)
    {
        m_curLife += t_lifeAmount;
        if (m_curLife > m_maxLife)
        {
            m_curLife = m_maxLife;
        }
    }
    m_lifeBar.setSize(sf::Vector2f(map(m_curLife, 0u, m_maxLife, 0u, 30u), 5.f));
}

void Enemy::setVelocity(float t_dx, float t_dy)
{
    Entity::setVelocity(t_dx * speed(), t_dy * speed());
}

void Enemy::setVelocity(sf::Vector2f t_inVelocity)
{
    Entity::setVelocity(t_inVelocity * speed());
}

void Enemy::update(const sf::Time& t_elapsedTime)
{
    m_sprite.move(m_movement);
    if (!m_spriteAnimator.isPlayingAnimation())
    {
        m_spriteAnimator.playAnimation("walk");
    }
    m_spriteAnimator.update(t_elapsedTime);
    m_explosionAnimator.update(t_elapsedTime);
    m_spriteAnimator.animate(m_sprite);
    m_explosionAnimator.animate(m_explosion);
    m_lifeBar.setPosition(m_sprite.getPosition().x, m_sprite.getPosition().y - 10);
    m_explosion.setPosition(m_sprite.getPosition());
}

void Enemy::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    if (visible())
    {
        Entity::draw(t_target, t_states);
        if (m_explosionAnimator.isPlayingAnimation())
        {
            t_target.draw(m_explosion, t_states);
        }
        if (m_showLife)
        {
            t_target.draw(m_lifeBar, t_states);
        }
    }
}

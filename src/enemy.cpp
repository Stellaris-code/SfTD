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

void Enemy::setMoveAnimation(const thor::FrameAnimation &t_animation, sf::Time t_duration)
{
    m_spriteAnimator.addAnimation(Animations::Move, t_animation, t_duration);
    m_moveAnimAvailable = true;
}

void Enemy::setDieAnimation(const thor::FrameAnimation &t_animation, sf::Time t_duration)
{
    m_spriteAnimator.addAnimation(Animations::Die, t_animation, t_duration);
    m_dieAnimAvailable = true;
}

void Enemy::init()
{
    m_lifeBar.setFillColor(sf::Color::Red);
    m_lifeBar.setPosition(pos().x, pos().y - 20);
    m_lifeBar.setSize(sf::Vector2f(30, 4));
    m_lifeBar.setOrigin(m_lifeBar.getLocalBounds().left + m_lifeBar.getSize().x / 2,
                        m_lifeBar.getLocalBounds().top + m_lifeBar.getSize().y / 2);

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
}

void Enemy::setTexture(const sf::Texture &t_texture, bool t_changeColorForResistances)
{
    m_sprite.setTexture(t_texture, t_changeColorForResistances);
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
        m_alive = false;
        if (m_dieAnimAvailable)
        {
            m_spriteAnimator.playAnimation(Animations::Die);
        }
    }
    else
    {
        m_curLife -= resultingAmount;
    }
    m_lifeBar.setSize(sf::Vector2f(map(m_curLife, 0u, m_maxLife, 0u, 30u), 5.f));
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
    if ((m_moveAnimAvailable && !m_spriteAnimator.isPlayingAnimation())
    || (m_spriteAnimator.isPlayingAnimation()
        && m_spriteAnimator.getPlayingAnimation() != Animations::Move))
    {
        m_spriteAnimator.playAnimation(Animations::Move, true);
    }
    if (!m_spriteAnimator.isPlayingAnimation() && !m_alive)
    {
        hide();
    }
    m_spriteAnimator.update(t_elapsedTime);
    m_spriteAnimator.animate(m_sprite);

    setOrigin(localBounds().left + localBounds().width / 2,
              localBounds().top + localBounds().height / 2);

    m_lifeBar.setPosition(pos().x, pos().y - localBounds().height / 2 - 5);
}

void Enemy::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    if (visible())
    {
        Entity::draw(t_target, t_states);
        if (m_showLife)
        {
            t_target.draw(m_lifeBar, t_states);
        }
    }
}

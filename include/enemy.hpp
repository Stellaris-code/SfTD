/** @file enemy.hpp
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

#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <iostream>
#include <string>

#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include <Thor/Animations.hpp>

#include "entity.hpp"
#include "constants.hpp"
#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

class Enemy : public Entity
{
    public:
        enum class Type
        {
            Ground,
            Flying
        };

        Enemy(const std::string& t_inName, const sf::Vector2f& t_position,
              const sf::Vector2f& t_scale, const unsigned int t_life,
              const Type t_type, const DmgType t_resist = DmgType::Physical);

        ~Enemy() = default;

        void setLifebarVisibility(bool t_state) { m_showLife = t_state; }
        void hideLifeBar() { setLifebarVisibility(false); }
        void showLifeBar() { setLifebarVisibility(true); }
        bool lifebarVisible() const { return m_showLife; }

        void takeDamage(unsigned int t_damageAmount, DmgType t_dmgtype = DmgType::Physical);
        void heal(unsigned int t_lifeAmount);

        virtual void update(const sf::Time& t_elapsedTime);

        unsigned int incCurrPoint() { return ++m_actualPoint; }

        unsigned int currPoint() const { return m_actualPoint; }

        void setMoveAnimation(const thor::FrameAnimation& t_animation, sf::Time t_duration);

        void setDieAnimation(const thor::FrameAnimation& t_animation, sf::Time t_duration);

        void setSpeed(float t_speed)
        {
            if (t_speed <= 0)
            {
                throw game_error("Speed must be set >= 0 !"
                                 "(A value around 1 and 3 is usually ok.)");
            }
            m_speed = t_speed;
        }

        float speed() const { return m_speed; }

        void setTexture(const sf::Texture& t_texture, bool t_changeColorForResistances = true);

        void setVelocity(sf::Vector2f t_inVelocity);
        void setVelocity(float t_dx, float t_dy);

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const;

    private:
        void init();

    private:
        enum class Animations
        {
            Move,
            Die
        };


    private:
        unsigned int m_curLife {};
        const unsigned int m_maxLife {};
        float m_speed { 1 };
        const Type m_type;
        const DmgType m_resistances;
        sf::RectangleShape m_lifeBar;
        bool m_showLife { false };
        bool m_alive { true };
        bool m_dieAnimAvailable { false };
        bool m_moveAnimAvailable { false };
        unsigned int m_actualPoint {};
        thor::Animator<sf::Sprite, Animations> m_spriteAnimator;
};

#endif // ENEMY_HPP

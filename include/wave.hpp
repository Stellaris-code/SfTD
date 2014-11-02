/** @file wave.hpp
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

#ifndef WAVE_HPP
#define WAVE_HPP

#include <cmath>

#include <vector>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/Drawable.hpp>

#include <Thor/Time/Timer.hpp>

#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include "enemy.hpp"

class Wave : public sf::Drawable
{
    public:
        Wave(const std::string& t_description = "", const sf::Time& t_delay = sf::seconds(1));
        ~Wave() = default;

        void addEnemy(const Enemy& t_enemy, unsigned int amount = 1);

        void launchWave();

        void update(const sf::Time& t_elapsedTime, const std::vector<sf::Vector2f>& t_path);

        bool done() const;

        unsigned int lostLives() const
        { return m_lostLives; }

        // Provided for lua
        void setDescription(const std::string& t_desc)
        { description = t_desc; }

    public:
        std::string description {};

    protected:
        virtual void draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const;

    private:
        std::vector<Enemy> m_wave;
        size_t m_remainingEnemies {};
        unsigned int m_lostLives {};
        const sf::Time m_delay {};
        thor::Timer m_timer;

};

#endif // WAVE_HPP

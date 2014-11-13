/** @file wave.cpp
Wave
@author jim
@date 27/10/2014

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

#include "wave.hpp"

Wave::Wave(const sf::Time &t_delay)
    : m_delay(t_delay)
{}

void Wave::addEnemy(const Enemy &t_enemy, unsigned int amount)
{
    while (amount-- > 0)
    {
        ++m_remainingEnemies;
        m_wave.push_back(t_enemy);
        m_wave.back().hide();
        m_wave.back().setProperty("waiting");
    }
}

void Wave::launchWave()
{
    m_elapsedTime = m_delay; // Launch first enemy; don't wait !
}

void Wave::update(const sf::Time &t_elapsedTime, const std::vector<sf::Vector2f>& t_path)
{
    m_elapsedTime += t_elapsedTime;
    if (m_elapsedTime >= m_delay && m_remainingEnemies > 0)
    {
        m_elapsedTime = sf::Time::Zero;
        for (auto& enemy : m_wave)
        {
            if (enemy.hidden() && enemy.hasProperty("waiting"))
            {
                --m_remainingEnemies;
                enemy.show();
                enemy.removeProperty("waiting");
                break;
            }
        }
    }

    for (auto& enemy : m_wave)
    {
        if (enemy.visible())
        {
            if (std::abs(enemy.pos().x - t_path[enemy.currPoint()].x) <= 2.5f &&
                std::abs(enemy.pos().y - t_path[enemy.currPoint()].y) <= 2.5f)
            {
                if (enemy.incCurrPoint() == t_path.size())
                {
                    m_lostLives++;
                    enemy.hide();
                }
                else
                {
                    sf::Vector2f unitVector = thor::unitVector(
                                                  t_path[enemy.currPoint()] - enemy.pos());

                    enemy.setVelocity(unitVector);
                    //enemy.setRotation(-thor::toDegree(std::atan2(unitVector.x, unitVector.y)));
                }
            }
            enemy.update(t_elapsedTime);
        }
    }
}

bool Wave::done() const
{
    return m_remainingEnemies <= 0 &&
    std::all_of(m_wave.begin(), m_wave.end(),
    [](const Enemy& t_enemy)
    {
        return t_enemy.hidden();
    });
}

void Wave::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    for (const auto& enemy : m_wave)
    {
        t_target.draw(enemy, t_states);
    }
}

/** @file game.hpp
Game
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

#include "game.hpp"

const sf::Time Game::m_timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    :
      m_window(sf::VideoMode(480, 480 + 40), "SFML Application",
               sf::Style::Titlebar | sf::Style::Close),
      m_statisticsNumFrames(0)
{

    m_statisticsText.setFont(FontHolder::instance().get(Fonts::Main));
    m_statisticsText.setPosition(5.f, 5.f);
    m_statisticsText.setCharacterSize(10);

    m_window.setKeyRepeatEnabled(false);

}

void Game::changeState(State& t_state)
{
    if (!m_states.empty())
    {
        m_states.pop();
    }

    m_states.push(t_state);

    m_window.clear(sf::Color::Black);

}

void Game::pushState(State& t_state)
{
    if (!m_states.empty())
    {
        m_states.top().get().pause();
    }
    m_states.push(t_state);

    m_window.clear(sf::Color::Black);
}

void Game::popState()
{
    if (!m_states.empty())
    {
        m_states.pop();
    }

    if (!m_states.empty())
    {
        m_states.top().get().resume();
    }

    m_window.clear(sf::Color::Black);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update(elapsedTime);
        }
        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        m_states.top().get().handleEvent(event);
        switch (event.type)
        {
            break;
        case sf::Event::Closed:
            m_window.close();
            break;
        case sf::Event::Resized:
            break;
        case sf::Event::LostFocus:
            break;
        case sf::Event::GainedFocus:
            break;
        case sf::Event::TextEntered:
            break;
        case sf::Event::MouseWheelMoved:
            break;
        case sf::Event::MouseButtonReleased:
            break;
        case sf::Event::MouseMoved:
            break;
        case sf::Event::MouseEntered:
            break;
        case sf::Event::MouseLeft:
            break;
        case sf::Event::JoystickButtonPressed:
            break;
        case sf::Event::JoystickButtonReleased:
            break;
        case sf::Event::JoystickMoved:
            break;
        case sf::Event::JoystickConnected:
             break;
        case sf::Event::JoystickDisconnected:
            break;

        default:
            break;
        }
    }
}

void Game::update(const sf::Time& t_elapsedTime)
{
    m_states.top().get().update(t_elapsedTime);
}
void Game::render()
{
    m_window.clear();

    m_states.top().get().display(m_window);

    m_window.draw(m_statisticsText);
    
    m_window.display();
}
void Game::updateStatistics(const sf::Time& t_elapsedTime)
{
    m_statisticsUpdateTime += t_elapsedTime;
    
    m_statisticsNumFrames += 1;
    if (m_statisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_statisticsText.setString(
                    "Frames / Second = " + std::to_string(m_statisticsNumFrames)
                    + "\nTime / Update = " + std::to_string(
                        static_cast<std::size_t>
                        (m_statisticsUpdateTime.asMicroseconds()) / m_statisticsNumFrames)
                    + "us");
        m_statisticsUpdateTime -= sf::seconds(1.0f);
        m_statisticsNumFrames = 0;
    }
}

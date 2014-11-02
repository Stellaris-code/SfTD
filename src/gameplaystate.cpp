/** @file gameplaystate.cpp
GameplayState
@author jim
@date 29/10/2014

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

#include "gameplaystate.hpp"

GameplayState::GameplayState(Game& t_engine)
    : State(t_engine), m_map()
{
}

void GameplayState::init(const TextureHolder &t_th, const FontHolder &t_fh)
{
    m_map.loadShaderFromFile("water", "shaders/water.frag", sf::Shader::Fragment);

    m_map.load("Maps/level1.tmx");
}

void GameplayState::exit()
{

}

void GameplayState::pause()
{

}

void GameplayState::resume()
{

}

void GameplayState::handleEvent(const sf::Event &t_event)
{

}

void GameplayState::update(const sf::Time& t_elapsedTime)
{
    m_map.update(t_elapsedTime);
}

void GameplayState::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_map, states);
}

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

#ifndef GAME_HPP
#define GAME_HPP

#include <cmath>

#include <string>
#include <functional>
#include <fstream>
#include <memory>
#include <stack>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <Thor/Vectors.hpp>

#include <luawrapper/LuaContext.hpp>

#include "state.hpp"
#include "map.hpp"
#include "topbar.hpp"
#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"
#include "utility.hpp"

class State;

class Game : private sf::NonCopyable
{
        friend class GameplayState;

    public:
        Game();

        void run();

        void changeState(State& t_state);
        void pushState(State& t_state);
        void popState();

    private:
        void processEvents();
        void update(const sf::Time& t_elapsedTime);
        void render();
        void updateStatistics(const sf::Time& t_elapsedTime);

    private:
        std::stack<std::reference_wrapper<State>> m_states;
        sf::RenderWindow m_window {};
        sf::Text m_statisticsText {};
        sf::Time m_statisticsUpdateTime {};
        std::size_t m_statisticsNumFrames {};
        static const sf::Time m_timePerFrame;
};

#endif // GAME_HPP

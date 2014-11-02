/** @file state.hpp
State
@author jim
@date 28/10/2014

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

#ifndef STATE_HPP
#define STATE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "game.hpp"

class State : public sf::Drawable
{
    public:
        State(Game& t_engine)
            : m_engine(t_engine)
        {}

    public:
        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void init() = 0;
        virtual void exit() = 0;

        virtual void handleEvent(const sf::Event& t_event) = 0;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const = 0;
        virtual void update(const sf::Time& t_elapsedTime) = 0;

    protected:
        Game& m_engine;
};

#endif // STATE_HPP

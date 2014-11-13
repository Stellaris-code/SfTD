/** @file menustate.hpp
MenuState
@author jim
@date 09/11/2014

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

#ifndef MENUSTATE_HPP
#define MENUSTATE_HPP

#include <SFGUI/SFGUI.hpp>

#include "state.hpp"
#include "gameplaystate.hpp"

class MenuState : public State
{

    public:
        MenuState(Game& t_engine);

    public:
        virtual void pause();
        virtual void resume();

        virtual void handleEvent(const sf::Event& t_event);

        virtual void update(const sf::Time& t_elapsedTime);

        virtual void display(sf::RenderWindow& t_window);

    private:
        sfg::Desktop m_desktop {};
        sfg::Window::Ptr m_window {};
        sfg::Box::Ptr m_layout {};

};
#endif // MENUSTATE_HPP

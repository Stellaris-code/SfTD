/** @file menustate.cpp
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

#include "menustate.hpp"


MenuState::MenuState(Game& t_engine)
    : State(t_engine)
{
    m_window = sfg::Window::Create(sfg::Window::NO_STYLE);
    m_layout = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 0.f);

    auto playButton = sfg::Button::Create("Play");
    auto quitButton = sfg::Button::Create("Quit");

    playButton->GetSignal(sfg::Button::OnLeftClick).Connect({[this]
                                                             {
                                                                 static GameplayState state(m_engine);
                                                                 m_engine.pushState(state);
                                                             }});

    m_layout->SetRequisition(m_window->GetRequisition());

    m_layout->Pack(playButton, false, false);

    m_window->Add(m_layout);
    m_window->SetPosition(sf::Vector2f(m_engine.m_window.getSize() / 2u));

    m_desktop.LoadThemeFromFile("themes/main.theme");
    m_desktop.Add(m_window);
}

void MenuState::pause()
{
    m_window->Show(false);
}

void MenuState::resume()
{
    m_window->Show(true);
}

void MenuState::handleEvent(const sf::Event &t_event)
{
    m_desktop.HandleEvent(t_event);
    switch (t_event.type)
    {
    default:
        break;
    }

}

void MenuState::update(const sf::Time& t_elapsedTime)
{
    m_desktop.Update(t_elapsedTime.asSeconds());
}

void MenuState::display(sf::RenderWindow& t_window)
{
    Game::GUI::instance().Display(t_window);
}

/** @file topbar.cpp
TopBar
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

#include "topbar.hpp"

TopBarView::TopBarView(TopBarModel &t_model)
    : m_model(t_model)
{
    m_background.setSize(sf::Vector2f(200, 200));
    m_background.setFillColor(sf::Color(104, 161, 227));
    m_background.setOutlineThickness(5);
    m_background.setOutlineColor(sf::Color(100, 100, 100));

    m_font = FontHolder::instance().get(Fonts::Main);
    m_lifeLabel.setFont(m_font);
    m_lifeLabel.setColor(sf::Color::White);
    m_lifeLabel.setCharacterSize(24);

    m_description->setReadOnly();
    m_description->setBackgroundColor(sf::Color::Transparent);
    m_description->setTextSize(16);
    m_description->setTextColor(sf::Color::White);
    m_description->setSize(m_background.getSize().x - 100, m_background.getSize().y - 10);
    m_description->setPosition(getPosition().x + 100, getPosition().y);
}

void TopBarView::setGui(tgui::Gui& t_gui)
{
    t_gui.add(m_description, "Description");
}

void TopBarView::update()
{
    m_lifeLabel.setString(std::to_string(m_model.lives) + "/" +
                          std::to_string(m_model.maxLives));

    m_description->setSize(m_background.getSize().x - 100, m_background.getSize().y - 10);
    m_description->setPosition(getPosition().x + 100, getPosition().y);
    m_description->setText(m_model.description);
}

void TopBarView::setSize(const sf::Vector2f &t_size)
{
    m_background.setSize(t_size);
}

void TopBarView::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    t_target.draw(m_background, t_states);
    t_target.draw(m_lifeLabel, t_states);
}

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
    m_curLife = sfg::Label::Create();
    m_funds = sfg::Label::Create();
    m_launchWaveButton = sfg::Button::Create("Launch next wave");
    m_layout = sfg::Table::Create();

    m_background.setSize(sf::Vector2f(200, 200));
    m_background.setFillColor(sf::Color(104, 161, 227));

    m_layout->Attach(m_curLife, {0, 0, 1, 1});
    m_layout->Attach(m_funds, {1, 0, 1, 1});
    m_layout->Attach(m_launchWaveButton, {2, 0, 1, 1});

    std::shared_ptr<sf::Font> font =
            std::make_shared<sf::Font>(FontHolder::instance().get(Fonts::Main));
    sfg::Context::Get().GetEngine().GetResourceManager().SetDefaultFont(font);
    sfg::Context::Get().GetEngine().SetProperty("*",
                                                "Color", sf::Color::White);

    m_layout->SetPosition(getPosition());

    sfg::Context::Get().GetEngine().SetProperty(".decorative_frame",
                                                "BorderWidth", 1.f);
    sfg::Context::Get().GetEngine().SetProperty(".decorative_frame",
                                                "Padding", 1.f);
    sfg::Context::Get().GetEngine().SetProperty("#topbar_container",
                                                "Padding", 3.f);

}

void TopBarView::setDesktop(sfg::Desktop& t_desk)
{
    t_desk.Add(m_layout);
}

void TopBarView::assignCallback(std::function<void ()> t_function, Callbacks t_type)
{
    switch (t_type)
    {
    case Callbacks::LaunchWaveButton:
        m_launchWaveButton->GetSignal(sfg::Button::OnLeftClick).Connect(t_function);
        break;
    default:
        break;
    }
}

void TopBarView::update()
{
    m_curLife->SetText(std::to_string(m_model.lives) + "/" +
                          std::to_string(m_model.maxLives));

    m_funds->SetText(std::to_string(m_model.funds));

    float separation = m_background.getSize().y - m_layout->GetRequisition().y;

    m_layout->SetPosition({getPosition().x + 20.f, getPosition().y - separation / 2});
    m_layout->SetRequisition({m_background.getSize().x - 40.f, separation / 2});

}

void TopBarView::setSize(const sf::Vector2f &t_size)
{
    m_background.setSize(t_size);
    update();
}

void TopBarView::draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const
{
    t_states.transform *= getTransform();
    t_target.draw(m_background, t_states);
}

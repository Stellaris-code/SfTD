/** @file topbar.hpp
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

#ifndef TOPBAR_HPP
#define TOPBAR_HPP

#include <string>
#include <functional>
#include <map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Text.hpp>

#include <SFML/System/Vector2.hpp>

#include <SFGUI/SFGUI.hpp>

#include "resourceHolder.hpp"
#include "resourceIdentifiers.hpp"

#include "constants.hpp"

#include "utility.hpp"

struct TopBarModel
{
        unsigned int lives;
        unsigned int maxLives;
        unsigned int funds;
};

class TopBarView : public sf::Drawable, public sf::Transformable
{

    public:
        TopBarView(TopBarModel& t_model);

        void update();

        sf::Vector2f size() const
        {
            return sf::Vector2f(m_background.getLocalBounds().left,
                                m_background.getLocalBounds().top);
        }

        void setSize(const sf::Vector2f& t_size);

        void setDesktop(sfg::Desktop& t_desk);

        void assignCallback(std::function<void()> t_function, Callbacks t_type);

        void setVisible(bool t_visible)
        {
            m_layout->Show(t_visible);
        }

        ~TopBarView()
        {
            setVisible(false);
        }


    protected:
        void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const;


    private:
        TopBarModel& m_model;
        sf::RectangleShape m_background;
        sfg::Label::Ptr m_curLife;
        sfg::Label::Ptr m_funds;
        sfg::Button::Ptr m_launchWaveButton;
        sfg::Table::Ptr m_layout;
};

#endif // TOPBAR_HPP

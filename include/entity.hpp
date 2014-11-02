/** @file entity.hpp
Entity
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

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <unordered_map>

#include <SFML/Graphics/Rect.hpp>

#include <SFML/Graphics/Sprite.hpp>

#include <SFML/System/Vector2.hpp>

#include <SFML/Graphics/RenderWindow.hpp>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include <boost/any.hpp>

#include "utility.hpp"

#include "resourceIdentifiers.hpp"

class Entity : public sf::Transformable, public sf::Drawable
{
    public:
        virtual std::string name() const { return m_name; }

        Entity() = default;

        virtual ~Entity() = default;

        virtual void update(const sf::Time& t_elapsedTime) = 0;


        void setVelocity(float t_dx, float t_dy);
        void setVelocity(sf::Vector2f t_inVelocity);

        void setPosition(const sf::Vector2f& t_inPos);
        void setPosition(float t_x, float t_y);

        void setScale(const sf::Vector2f& t_factor);
        void setScale(float t_x, float t_y);

        void setOrigin(const sf::Vector2f& t_inOrigin);
        void setOrigin(float t_x, float t_y);

        void setRotation(float t_angle);
        void rotate(float t_angle);

        sf::Vector2f pos() const { return m_sprite.getPosition(); }

        sf::Vector2f scale() const { return m_sprite.getScale(); }

        sf::Vector2f velocity() const { return m_movement; }

        sf::FloatRect localBounds() const { return m_sprite.getLocalBounds(); }

        sf::FloatRect globalBounds() const { return m_sprite.getGlobalBounds(); }

        sf::Vector2f origin() const { return m_sprite.getOrigin(); }

        sf::IntRect textureRect() const { return m_sprite.getTextureRect(); }

        void setTextureRect(const sf::IntRect& t_rect)
        { m_sprite.setTextureRect(t_rect); }

        float rotation() const { return m_sprite.getRotation(); }

        sf::Vector2f centre() const
        {
            return sf::Vector2f(globalBounds().width/2, globalBounds().height/2);
        }

        bool contains(const sf::Vector2f& t_point) const
        {
            return m_sprite.getGlobalBounds().contains(t_point);
        }

        bool contains(float t_x, float t_y) const
        { return m_sprite.getGlobalBounds().contains(t_x, t_y); }

        void setVisibility(bool t_val)
        {
            m_visible = t_val;
        }

        void hide()
        {
            setVisibility(false);
        }

        void show()
        {
            setVisibility(true);
        }

        bool visible() const { return m_visible; }

        bool hidden() const { return !visible(); }

        void setProperty(const std::string& t_id, const boost::any t_data = boost::any())
        { m_properties[t_id] = t_data; }

        void removeProperty(const std::string& t_id)
        { m_properties.erase(t_id); }

        boost::any propertyAt(const std::string& t_id) const
        { return m_properties.at(t_id); }

        bool hasProperty(const std::string& t_id) const
        { return m_properties.find(t_id) != m_properties.end(); }

    protected:
        virtual void draw(sf::RenderTarget &t_target, sf::RenderStates t_states) const;

    protected:
        std::string m_name {};
        sf::Sprite m_sprite {};
        sf::Vector2f m_movement {};
        std::unordered_map<std::string, boost::any> m_properties {};
        bool m_visible { false };
};

#endif // ENTITY_HPP

/** @file map.hpp
Map
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

#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <map>
#include <utility>
#include <functional>

#include <SFML/Config.hpp>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/System/InputStream.hpp>

#include <SFML/Window/Event.hpp>

#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Drawable.hpp>


#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include <Thor/Animations/FadeAnimation.hpp>
#include <Thor/Animations/Animator.hpp>

#include <tmx/MapLoader.h>

#include "tower.hpp"
#include "wave.hpp"
#include "utility.hpp"
#include "game_error.hpp"

class Map : public sf::Drawable
{
    public:
        Map(unsigned int t_life = 50, const std::string& t_basepath = ".");

        void addTower(const Tower& tower) { m_towers.push_back(tower); }

        void load(const std::string& t_filename);

        sf::Vector2u mapSize() const { return m_loader.GetMapSize(); }

        void addWave(const Wave& wave)
        {
            m_inactiveWaves.push(wave);
            m_currentWaveDesc = m_inactiveWaves.front().description;
        }

        size_t launchWave();

        size_t remainingWaves() const { return m_inactiveWaves.size(); }

        sf::Vector2f startPoint() const { return m_startPoint; }

        sf::Vector2f finishPoint() const { return m_finishPoint; }

        std::string mapName() const { return m_mapName; }

        void loadShaderFromFile(const std::string& t_key,
                                const std::string& t_filename, sf::Shader::Type t_type);

        void loadShaderFromString(const std::string& t_key,
                                  const std::string& t_data, sf::Shader::Type t_type);

        void removeShader(const std::string& t_key);

        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, float t_x);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, float t_x, float t_y);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, float t_x, float t_y, float t_z);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name,
                                float t_x, float t_y, float t_z, float t_w);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, const sf::Vector2f& t_vec);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, const sf::Vector3f& t_vec);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, const sf::Color& t_color);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, const sf::Transform& t_transform);
        void setShaderParameter(const std::string& t_key,
                                const std::string &t_name, const sf::Texture& t_tex);

        void update(const sf::Time& t_elapsedTime);

        std::vector<tmx::MapLayer> mapLayers() const { return m_loader.GetLayers(); }

        void handleMouseClick(sf::Mouse::Button t_button, int t_x, int t_y);

        void handleMouseClick(sf::Mouse::Button t_button, const sf::Vector2i& t_vec)
        {
            handleMouseClick(t_button, t_vec.x, t_vec.y);
        }

        sf::Vector2u tileSize() const { return m_loader.tileSize(); }

        unsigned int remainingLives() const { return m_lives; }
        unsigned int maxLives() const { return m_maxLives; }

        const std::string& waveDescription()
        { return m_currentWaveDesc; }

    protected:
        virtual void draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const;

    private:
        void initMapPoints();
        tmx::MapTiles reorderedTiles(const tmx::MapTiles &t_tiles,
                                     const sf::Vector2f &t_startTile) const;
        tmx::MapTiles adjacentTiles(const tmx::MapTile& t_tile) const;
        bool isTileOnMap(const tmx::MapTile& t_tile) const;
        bool isTileVisible(const tmx::MapTile& t_tile) const;

    private:
        tmx::MapLoader m_loader;
        std::vector<Tower> m_towers;
        std::queue<Wave> m_inactiveWaves;
        std::vector<Wave> m_activeWaves;
        sf::Vector2f m_startPoint;
        sf::Vector2f m_finishPoint;
        std::string m_mapName;
        std::vector<tmx::MapObject> m_turns;
        tmx::MapLayer m_pathLay;
        std::vector<size_t> m_pathTilesIndex;
        std::vector<sf::Vector2f> m_path;
        std::map<std::string, sf::Shader> m_shaderTable;
        sf::Sprite m_selectedTile;
        unsigned int m_lives { 50 };
        const unsigned int m_maxLives { 50 };
        thor::Animator<sf::Shape, std::string> m_shapeAnimator;
        thor::Animator<sf::Sprite, std::string> m_spriteAnimator;
        std::string m_currentWaveDesc;
};

#endif // MAP_HPP

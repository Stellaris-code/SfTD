/** @file map.cpp
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

#include "map.hpp"

Map::Map(unsigned int t_life, const std::string& t_basepath)
    : m_loader(t_basepath), m_pathLay(tmx::Layer), m_lives(t_life), m_maxLives(m_lives)
{
    thor::FadeAnimation fadeOut(0.3f, 0.7f);
    thor::FadeAnimation fadeIn(0.3f, 0.f);

    m_spriteAnimator.addAnimation("selectedTile_fadeOut", fadeOut, sf::seconds(0.3f));
    m_spriteAnimator.addAnimation("selectedTile_fadeIn", fadeIn, sf::seconds(0.3f));

    m_selectedTile.setTexture(TextureHolder::instance().get("selectedTile"));
    m_selectedTile.setColor(sf::Color(0, 0, 0, 0));
}

void Map::handleMouseClick(sf::Mouse::Button t_button, int t_x, int t_y)
{
    if (t_button == sf::Mouse::Left)
    {
        for (const auto& layer : m_loader.GetLayers())
        {
            for (const auto& tile : layer.tiles)
            {
                if (tile.rect().contains(t_x, t_y))
                {
                    m_selectedTile.setPosition(tile.gridCoord.x, tile.gridCoord.y);
                    if (layer.properties.find("no_tower") != layer.properties.end())
                    {
                        m_selectedTile.setColor(sf::Color(255, 0, 0, 200));
                        m_spriteAnimator.playAnimation("selectedTile_fadeOut");
                        return;
                    }
                    else
                    {
                        m_selectedTile.setColor(sf::Color(0, 255, 0, 200));
                        m_spriteAnimator.playAnimation("selectedTile_fadeIn");
                        return;
                    }
                }
            }
        }
    }
}

void Map::load(const std::string &t_filename)
{
    // Clear inactive waves by swapping wave queue to empty queue
    decltype(m_inactiveWaves)().swap(m_inactiveWaves);
    m_activeWaves.clear();
    m_towers.clear();
    if (!m_loader.Load(t_filename))
    {
        throw game_error("Error : Cannot open " + t_filename + " !");
    }
    m_mapName = t_filename;
    initMapPoints();
    tmx::MapTiles path = reorderedTiles(m_pathLay.tiles, m_startPoint * 32.f);
    for (const auto& tile : path)
    {
        m_path.push_back(tile.gridCoord);
    }
    for (auto& point : m_path)
    {
        point += sf::Vector2f(m_loader.tileSize()) / 2.f;
    }
    m_selectedTile.setScale(m_loader.tileSize().x / m_selectedTile.getTextureRect().width,
                            m_loader.tileSize().y / m_selectedTile.getTextureRect().height);
}

void Map::draw(sf::RenderTarget& t_target, sf::RenderStates t_states) const
{
    t_target.draw(m_loader, t_states);
    for (const auto& wave : m_activeWaves)
    {
        t_target.draw(wave, t_states);
    }
    t_target.draw(m_selectedTile, t_states);
}

size_t Map::launchWave()
{
    if (m_inactiveWaves.size() <= 0)
    {
        std::cerr << "Cannot launch wave : no pending waves !\n" << std::flush;
        return 0;
    }
    m_activeWaves.push_back(m_inactiveWaves.front());
    m_inactiveWaves.pop();
    m_activeWaves.back().launchWave();

    return m_inactiveWaves.size();
}

void Map::update(const sf::Time& t_elapsedTime)
{
    for (auto& wave : m_activeWaves)
    {
        wave.update(t_elapsedTime, m_path);
    }

    m_shapeAnimator.update(t_elapsedTime);
    m_spriteAnimator.update(t_elapsedTime);
    m_spriteAnimator.animate(m_selectedTile);

    unsigned int lostLives {};

    for (const auto& wave : m_activeWaves)
    {
        lostLives += wave.lostLives();
    }

    if (m_maxLives <= lostLives)
    {
        m_lives = 0;
    }
    else
    {
        m_lives = m_maxLives - lostLives;
    }
}

void Map::initMapPoints()
{

    std::vector<tmx::MapLayer>& layers = m_loader.GetLayers();

    for (auto& layer : layers)
    {
        std::map<std::string, std::string> properties = layer.properties;
        if (properties.find("shader") != properties.end())
        {
            if (!properties["shader"].empty())
            {
                if (m_shaderTable.find(properties["shader"]) == m_shaderTable.end())
                {
                    throw game_error("Error : shader key \"" + properties["shader"]
                            + "\" not assigned !");
                }
                else
                {
                    layer.SetShader(m_shaderTable.at(properties["shader"]));
                }
            }
        }
    }

    auto path = std::find_if(layers.begin(), layers.end(),
                             [](const tmx::MapLayer& layer) -> bool
    {
        return layer.name == "Path" && layer.type == tmx::Layer;
    });
    if (path != layers.end())
    {
        m_pathLay = *path;
    }
    else
        throw game_error(
                R"(ERROR : Cannot find "Path" as tile   layer in map )"
                + mapName() + " !");

    std::map<std::string, std::string> properties = m_pathLay.properties;
    auto mapStart = std::find_if(properties.begin(), properties.end(),
                                 [](const std::pair<std::string, std::string>& property) -> bool
    {
        return property.first == "start_tile";
    });
    if (mapStart != properties.end())
    {
        m_startPoint = stringToSfVec<float>(mapStart->second, ',');
    }
    else
        throw game_error(
                R"(ERROR : Cannot find "start_tile" property in layer "Path", in map )"
                + mapName() + " !");

    auto mapFinish = std::find_if(properties.begin(), properties.end(),
                                  [](const std::pair<std::string, std::string>& property) -> bool
    {
        return property.first == "finish_tile";
    });
    if (mapFinish != properties.end())
    {
        m_finishPoint = stringToSfVec<float>(mapFinish->second, ',');
    }
    else
        throw game_error(
                R"(ERROR : Cannot find "finish_tile" property in layer "Path", in map )"
                + mapName() + " !");
}

bool Map::isTileVisible(const tmx::MapTile &t_tile) const
{
    return t_tile.gid != 0 && isTileOnMap(t_tile);
}

bool Map::isTileOnMap(const tmx::MapTile &t_tile) const
{
    return isInRange<unsigned int>(t_tile.gridCoord.x, 0, mapSize().x) &&
            isInRange<unsigned int>(t_tile.gridCoord.y, 0, mapSize().y);
}

tmx::MapTiles Map::reorderedTiles(const tmx::MapTiles& t_tiles,
                                  const sf::Vector2f& t_startTile) const
{
    tmx::MapTiles finalTileList;
    finalTileList.emplace_back(t_startTile, m_loader.tileSize());
    for (size_t i {}; i < finalTileList.size(); ++i)
    {
        for (const auto& adjTile : adjacentTiles(finalTileList[i]))
        {
            if (isTileOnMap(adjTile))
            {
                for (const auto& t : t_tiles)
                {
                    if ((finalTileList.size() <= 1 || adjTile.gridCoord !=
                         finalTileList[finalTileList.size() - 2].gridCoord)
                            && t.gridCoord == adjTile.gridCoord)
                    {
                        finalTileList.push_back(adjTile);
                        goto exit_adj_tile_loop;
                    }
                }
            }
        }
        return finalTileList;
exit_adj_tile_loop:
        continue;
    }
    return finalTileList;
}

void Map::loadShaderFromFile(const std::string &t_key,
                             const std::string &t_filename, sf::Shader::Type t_type)
{
    m_shaderTable[t_key].loadFromFile(t_filename, t_type);
}

void Map::loadShaderFromString(const std::string &key,
                               const std::string &data, sf::Shader::Type t_type)
{
    m_shaderTable[key].loadFromMemory(data, t_type);
}

void Map::removeShader(const std::string &t_key)
{
    m_shaderTable.erase(t_key);
}

tmx::MapTiles Map::adjacentTiles(const tmx::MapTile &t_tile) const
{
    // search tiles in clockwise direction
    return tmx::MapTiles
    {
        tmx::MapTile(t_tile.gridCoord.x,
                     t_tile.gridCoord.y - m_loader.tileSize().y,
                     m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(t_tile.gridCoord.x + m_loader.tileSize().x,
                             t_tile.gridCoord.y - m_loader.tileSize().y,
                             m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(t_tile.gridCoord.x + m_loader.tileSize().x,
                             t_tile.gridCoord.y,
                             m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(t_tile.gridCoord.x + m_loader.tileSize().x,
                             t_tile.gridCoord.y + m_loader.tileSize().y,
                             m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(t_tile.gridCoord.x,
                             t_tile.gridCoord.y + m_loader.tileSize().y,
                             m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(t_tile.gridCoord.x - m_loader.tileSize().x,
                             t_tile.gridCoord.y + m_loader.tileSize().y,
                             m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(static_cast<unsigned int>(t_tile.gridCoord.x) - m_loader.tileSize().x,
                             t_tile.gridCoord.y,
                             m_loader.tileSize().x, m_loader.tileSize().y),
                tmx::MapTile(t_tile.gridCoord.x - m_loader.tileSize().x,
                             static_cast<unsigned int>(t_tile.gridCoord.y) - m_loader.tileSize().y,
                             m_loader.tileSize().x, m_loader.tileSize().y)

    };
}

void Map::setShaderParameter(const std::string& t_key,
                             const std::string &t_name, float t_x)
{
    m_shaderTable.at(t_key).setParameter(t_name, t_x);
}

void Map::setShaderParameter(const std::string& t_key,
                             const std::string &t_name, float t_x, float t_y)
{
    m_shaderTable.at(t_key).setParameter(t_name, t_x, t_y);
}

void Map::setShaderParameter(const std::string& t_key,
                             const std::string &t_name, float t_x, float t_y, float t_z)
{
    m_shaderTable.at(t_key).setParameter(t_name, t_x, t_y, t_z);
}

void Map::setShaderParameter(const std::string& t_key,
                             const std::string &t_name, float t_x, float t_y, float t_z, float t_w)
{
    m_shaderTable.at(t_key).setParameter(t_name, t_x, t_y, t_z, t_w);
}

void Map::setShaderParameter(const std::string& key,
                             const std::string &t_name, const sf::Vector2f& t_vec)
{
    m_shaderTable.at(key).setParameter(t_name, t_vec);
}

void Map::setShaderParameter(const std::string& key,
                             const std::string &t_name, const sf::Vector3f& t_vec)
{
    m_shaderTable.at(key).setParameter(t_name, t_vec);
}

void Map::setShaderParameter(const std::string& t_key,
                             const std::string &t_name, const sf::Color& t_color)
{
    m_shaderTable.at(t_key).setParameter(t_name, t_color);
}

void Map::setShaderParameter(const std::string& key,
                             const std::string &name, const sf::Transform& t_transform)
{
    m_shaderTable.at(key).setParameter(name, t_transform);
}

void Map::setShaderParameter(const std::string& key,
                             const std::string &t_name, const sf::Texture& t_tex)
{
    m_shaderTable.at(key).setParameter(t_name, t_tex);
}

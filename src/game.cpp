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

#include "game.hpp"

const sf::Time Game::m_timePerFrame = sf::seconds(1.f / 60.f);

Game::Game()
    :
      m_window(sf::VideoMode(480, 480), "SFML Application", sf::Style::Default,
               sf::ContextSettings(0, 0, 4)),
      m_statisticsNumFrames(0), m_map("Maps")
{
    m_map.loadShaderFromFile("water", "shaders/water.frag", sf::Shader::Fragment);
    
    m_map.load("Maps/level1.tmx");
    
    m_textures.load(Textures::Beetle, "beetles.png");
    m_textures.load(Textures::Explosion, "explosion.png");
    m_textures.load(Textures::SelectedTile, "selected.png");
    
    m_fonts.load(Fonts::Main, "fonts/Ubuntu-M.ttf");
    
    m_map.setTextureHolder(m_textures);

    m_statisticsText.setFont(
                m_fonts.get(Fonts::Main));
    m_statisticsText.setPosition(5.f, 5.f);
    m_statisticsText.setCharacterSize(10);

    Enemy enemy2("Gérard", sf::Vector2f(10, 10), sf::Vector2f(1.f, 1.f), 100,
                 Enemy::Type::Ground, DmgType::Fire);

    enemy2.setPosition( m_map.startPoint() * 32.f);
    enemy2.setTextureHolder(m_textures);
    enemy2.setFontHolder(m_fonts);

    LuaContext lua;
    lua.writeVariable("dmg_fire", DmgType::Fire);
    lua.writeVariable("dmg_ice", DmgType::Ice);
    lua.writeVariable("dmg_physical", DmgType::Physical);
    lua.writeVariable("dmg_poison", DmgType::Poison);
    lua.writeVariable("dmg_shock", DmgType::Shock);

    std::function<void(const std::string&, DmgType, unsigned int, float, bool)>
            add_enemy_to_wave = [this](const std::string& name, DmgType dmgtype,
                                unsigned int amount, float speed, bool lifebar_visible)
    {
        Enemy enemy(name, sf::Vector2f(0, 0), sf::Vector2f(1.f, 1.f), 100,
                    Enemy::Type::Ground, dmgtype);

        enemy.setPosition( m_map.startPoint() * 32.f);
        enemy.setTextureHolder(m_textures);
        enemy.setFontHolder(m_fonts);
        enemy.setLifebarVisibility(lifebar_visible);
        enemy.setSpeed(speed);
        wave.addEnemy(enemy, amount);
    };
    lua.writeVariable("add_enemy_to_wave", add_enemy_to_wave);
    std::ifstream luaFile("test.lua");
    try
    {
        lua.executeCode(luaFile);
    }
    catch (const LuaContext::ExecutionErrorException& e)
    {
        std::cerr << e.what() << std::endl;           // prints an error message

        try
        {
            std::rethrow_if_nested(e);
        }
        catch (const std::runtime_error& e)
        {
            // e is the exception that was thrown from inside the lambda
            std::cerr << e.what() << std::endl;       // prints "Problem"
        }
    }
    
    m_map.addWave(wave);
    
    m_map.launchWave();
    
    m_window.setView(sf::View(sf::FloatRect(sf::Vector2f(0, 0),
                                            sf::Vector2f(m_map.mapSize()))));
    m_window.setKeyRepeatEnabled(false);

}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (
           m_window.isOpen())
    {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;
        while (timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update(m_timePerFrame);
        }
        updateStatistics(elapsedTime);
        render();
    }
}

void Game::processEvents()
{
    sf::Event event;
    while (
           m_window.pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
            break;
        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
            break;
        case sf::Event::MouseButtonPressed:
            m_map.handleMouseClick(event.mouseButton.button,
                                   sf::Mouse::getPosition(m_window));
            break;
        case sf::Event::Closed:
            m_window.close();
            break;

        default:
            break;
        }
    }
}

void Game::update(const sf::Time& t_elapsedTime)
{
    m_map.update(t_elapsedTime);
}
void Game::render()
{
    m_window.clear();
    
    m_map.setShaderParameter("water", "time",
                             m_shaderClock.getElapsedTime().asSeconds());
    m_window.draw(m_map);
    
    m_window.draw(m_statisticsText);
    
    m_window.display();
}
void Game::updateStatistics(const sf::Time& t_elapsedTime)
{
    m_statisticsUpdateTime += t_elapsedTime;
    
    m_statisticsNumFrames += 1;
    if (m_statisticsUpdateTime >= sf::seconds(1.0f))
    {
        m_statisticsText.setString(
                    "Frames / Second = " + std::to_string(m_statisticsNumFrames)
                    + "\nTime / Update = " + std::to_string(
                        static_cast<std::size_t>
                        (m_statisticsUpdateTime.asMicroseconds()) / m_statisticsNumFrames)
                    + "us");
        m_statisticsUpdateTime -= sf::seconds(1.0f);
        m_statisticsNumFrames = 0;
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key t_key, bool t_isPressed)
{
//    if (t_key == sf::Keyboard::W && t_isPressed)
//    {
//        enemy.takeDamage(10, DmgType::Poison);
//        std::cerr << "Poison !\n";
//    }
//    if (t_key == sf::Keyboard::X && t_isPressed)
//    {
//        enemy.takeDamage(10, DmgType::Ice);
//        std::cerr << "Ice !\n";
//    }
//    if (t_key == sf::Keyboard::C && t_isPressed)
//    {
//        enemy.takeDamage(10, DmgType::Fire);
//        std::cerr << "Fire !\n";
//    }
//    if (t_key == sf::Keyboard::V && t_isPressed)
//    {
//        enemy.takeDamage(10, DmgType::Shock);
//        std::cerr << "Shock !\n";
//    }
//    if (t_key == sf::Keyboard::B && t_isPressed)
//    {
//        enemy.takeDamage(10, DmgType::Physical);
//        std::cerr << "Physical !\n";
//    }

    if (t_key == sf::Keyboard::N && t_isPressed)
    {
        m_map.launchWave();
    }
}

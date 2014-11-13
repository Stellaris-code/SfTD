/** @file gameplaystate.cpp
GameplayState
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

#include "gameplaystate.hpp"

GameplayState::GameplayState(Game& t_engine)
    : State(t_engine), m_map(20, "Maps")
{
    m_topbarView.setDesktop(m_desktop);

    m_map.loadShaderFromFile("water", "shaders/water.frag", sf::Shader::Fragment);

    m_map.load("Maps/level1.tmx");

    m_topbarView.setSize(sf::Vector2f(480, 40));

    m_topbarView.setPosition(0, 480);

    LuaContext lua;
    lua.writeVariable("dmg_fire", DmgType::Fire);
    lua.writeVariable("dmg_ice", DmgType::Ice);
    lua.writeVariable("dmg_physical", DmgType::Physical);
    lua.writeVariable("dmg_poison", DmgType::Poison);
    lua.writeVariable("dmg_shock", DmgType::Shock);

    std::function<Enemy(const std::string&, const std::string&, DmgType, float, bool)>
            createEnemy = [this](const std::string& name, const std::string& t_textureID,
                          DmgType dmgtype, float speed, bool lifebar_visible)
    {
        Enemy enemy(name, sf::Vector2f(0, 0), sf::Vector2f(1.f, 1.f), 100,
                    Enemy::Type::Ground, dmgtype);
        enemy.setTexture(TextureHolder::instance().get(t_textureID));

        enemy.setPosition(m_map.startPoint() * 32.f + sf::Vector2f(m_map.tileSize()) / 2.f);
        enemy.setLifebarVisibility(lifebar_visible);
        enemy.setSpeed(speed);
        return enemy;
    };
    lua.writeFunction("registerTexture", [](const std::string& t_id,
                                            const std::string& t_filepath)
    {
        TextureHolder::instance().load(t_id, t_filepath);
    });
    lua.writeFunction("newFrameAnimation", []{ return thor::FrameAnimation(); });
    lua.writeFunction("intrect", [](int t_left, int t_top, int t_width, int t_height)
    {
        return sf::IntRect(t_left, t_top, t_width, t_height);
    });
    lua.writeFunction("floatrect", [](float t_left, float t_top, float t_width, float t_height)
    {
        return sf::FloatRect(t_left, t_top, t_width, t_height);
    });
    lua.writeFunction("seconds", [](float t_time)
    {
        return sf::seconds(t_time);
    });
    lua.writeFunction("milliseconds", [](sf::Int32 t_time)
    {
        return sf::milliseconds(t_time);
    });
    lua.writeFunction("microseconds", [](sf::Int64 t_time)
    {
        return sf::microseconds(t_time);
    });
    lua.registerFunction("addFrame", &thor::FrameAnimation::addFrame);
    lua.writeFunction("addFramesX", addFramesX);
    lua.writeFunction("addFramesY", addFramesY);
    lua.registerFunction("setMoveAnimation", &Enemy::setMoveAnimation);
    lua.registerFunction("setDieAnimation", &Enemy::setDieAnimation);
    lua.writeFunction("createEnemy", createEnemy);
    lua.registerFunction("addEnemy", &Wave::addEnemy);
    lua.writeFunction("newWave", [](const sf::Time& t_interval)
    { return Wave(t_interval); });
    lua.writeFunction("addWave", [this](const Wave& t_wave){ m_map.addWave(t_wave);});
    std::ifstream luaFile("main.lua");
    if (luaFile.is_open())
    {
    try
    {
        lua.executeCode(luaFile);
    }
    catch (const LuaContext::ExecutionErrorException& eee)
    {
        std::cerr << eee.what() << "\n";           // prints an error message

        try
        {
            std::rethrow_if_nested(eee);
        }
        catch (const std::exception& e)
        {
            // re is the exception that was thrown from inside the lambda
            std::cerr << e.what() << "\n";
            if (std::string(e.what()) == "basic_string::_S_construct null not valid")
            {
                std::cerr << "This usually mean syntax error or a mispell in function call, "
                             "or using an undeclared variable/function. Exiting.\n";
                throw;
            }
        }
    }
    }
    else
    {
        std::cerr << "Cannot open main.lua !\n";
    }

    m_topbarView.assignCallback(std::function<void()>([this]{ m_map.launchWave(); }),
                                Callbacks::LaunchWaveButton);

    m_desktop.LoadThemeFromFile("themes/main.theme");

    if ((sfg::Renderer::Get().GetName() == "Vertex Array Renderer" )
            && sfg::VertexBufferRenderer::IsAvailable())
    {
        std::shared_ptr<sfg::VertexBufferRenderer> renderer(
                    std::make_shared<sfg::VertexBufferRenderer>());
        sfg::Renderer::Set(renderer);
        renderer->TuneUseFBO(true);
        renderer->TuneAlphaThreshold(0.2f);
        renderer->TuneCull(true);
    }
    else
    {
        std::shared_ptr<sfg::VertexArrayRenderer> renderer(
                    std::make_shared<sfg::VertexArrayRenderer>());
        sfg::Renderer::Set(renderer);
        renderer->TuneAlphaThreshold(0.2f);
        renderer->TuneCull(true);
    }
    m_desktop.Refresh();
    update(sf::Time::Zero);
}

void GameplayState::pause()
{
    m_topbarView.setVisible(false);
}

void GameplayState::resume()
{
    m_topbarView.setVisible(true);
}

void GameplayState::handleEvent(const sf::Event &t_event)
{
    m_desktop.HandleEvent(t_event);
    switch (t_event.type)
    {

    case sf::Event::KeyPressed:

        if (t_event.key.code == sf::Keyboard::N)
        {
            m_map.launchWave();
        }

        if (t_event.key.code == sf::Keyboard::Q)
        {
            m_engine.popState();
        }

        break;

    case sf::Event::MouseButtonPressed:
        m_map.handleMouseClick(t_event.mouseButton.button, t_event.mouseButton.x,
                               t_event.mouseButton.y);
    default:
        break;
    }

}

void GameplayState::update(const sf::Time& t_elapsedTime)
{
    m_elapsedTime += t_elapsedTime;
    m_desktop.Update(t_elapsedTime.asSeconds());
    m_map.update(t_elapsedTime);
    m_topbarModel.lives = m_map.remainingLives();
    m_topbarModel.maxLives = m_map.maxLives();
    m_map.setShaderParameter("water", "time", m_elapsedTime.asSeconds());
    m_topbarView.update();
}

void GameplayState::display(sf::RenderWindow& t_window)
{
    t_window.draw(m_map);
    t_window.draw(m_topbarView);
    Game::GUI::instance().Display(t_window);
}

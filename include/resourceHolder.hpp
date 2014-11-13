/** @file resourceHolder.hpp
ResourceHolder
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

#ifndef RESSOURCEHOLDER_HPP
#define RESSOURCEHOLDER_HPP

#include <cassert>

#include <map>
#include <string>
#include <memory>

#include <SFML/System/NonCopyable.hpp>

#include "game_error.hpp"

template <typename Resource, typename Identifier>
class ResourceHolder : sf::NonCopyable
{
    public:
        void load(Identifier t_id, const std::string& t_filename);
        template <typename Parameter>
        void load(Identifier t_id, const std::string& t_filename, const Parameter& t_secondParam);
        Resource& get(Identifier t_id);
        const Resource& get(Identifier t_id) const;

        static ResourceHolder<Resource, Identifier>& instance()
        {
            static ResourceHolder<Resource, Identifier> instance;
            return instance;
        }

    private:
        ResourceHolder() = default;

    private:
        void insertResource(Identifier t_id, std::unique_ptr<Resource> t_resource);
    private:
        std::map<Identifier, std::unique_ptr<Resource>> m_resourceMap {};
};

#include "resourceHolder.tpp"

#endif // RESSOURCEHOLDER_HPP

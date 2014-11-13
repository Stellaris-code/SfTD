/** @file resourceHolder.tpp
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

template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::load(Identifier t_id,
                                                const std::string& t_filename)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(t_filename))
    {
        throw game_error("ResourceHolder::load - Failed to load " + t_filename);
    }

    // If loading successful, insert resource to map
    insertResource(t_id, std::move(resource));

}
template <typename Resource, typename Identifier>
template <typename Parameter>
void ResourceHolder<Resource, Identifier>::load(Identifier t_id,
                                                const std::string& t_filename,
                                                const Parameter& t_secondParam)
{
    // Create and load resource
    std::unique_ptr<Resource> resource(new Resource());
    if (!resource->loadFromFile(t_filename, t_secondParam))
    {
        throw game_error("ResourceHolder::load - Failed to load " + t_filename);
    }
    // If loading successful, insert resource to map
    insertResource(t_id, std::move(resource));
}
template <typename Resource, typename Identifier>
Resource& ResourceHolder<Resource, Identifier>::get(Identifier t_id)
{
    auto found = m_resourceMap.find(t_id);
    assert(found != m_resourceMap.end());
    return *found->second;
}
template <typename Resource, typename Identifier>
const Resource& ResourceHolder<Resource, Identifier>::get(Identifier t_id) const
{
    auto found = m_resourceMap.find(t_id);
    assert(found != m_resourceMap.end());
    return *found->second;
}
template <typename Resource, typename Identifier>
void ResourceHolder<Resource, Identifier>::insertResource(Identifier t_id,
                                                          std::unique_ptr<Resource> t_resource)
{
    // Insert and check success
    auto inserted = m_resourceMap.insert(std::make_pair(t_id, std::move(t_resource)));
    assert(inserted.second);
}

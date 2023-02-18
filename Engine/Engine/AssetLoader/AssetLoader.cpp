//
// Created by Flo on 15/12/2022.
//

#include "AssetLoader.hpp"

namespace Engine
{
    template<>
    sf::Texture* AssetLoader<sf::Texture>::StaticLoadAsset(const std::string& path)
    {
        sf::Texture texture;
        if(!texture.loadFromFile(path))
            return nullptr;

        s_paths[path] = texture;

        return &s_paths[path];
    }
} // Engine
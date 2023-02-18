//
// Created by Flo on 15/12/2022.
//

#include "AssetLoader.hpp"

namespace Engine
{
    template<>
    sf::Font* AssetLoader<sf::Font>::StaticLoadAsset(const std::string& path)
    {
        sf::Font texture;
        texture.loadFromFile(path);

        s_paths[path] = texture;

        return &s_paths[path];
    }
} // Engine
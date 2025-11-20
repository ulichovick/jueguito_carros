#ifndef GAME_GRAPHICS_HPP
#define GAME_GRAPHICS_HPP
#include <SFML/Graphics.hpp>
#include <filesystem>


//sf::Sprite spritesJuego(const std::filesystem::path& ruta, sf::Vector2i posIni, sf::Vector2i tamanio);

class Entidad: public sf::Sprite
{
    public:
        Entidad(const sf::Texture& textura)
        : sf::Sprite(textura)
        {
            this->setTexture(textura);
        }
};

#endif

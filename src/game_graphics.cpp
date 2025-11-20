#include "game_graphics.hpp"
#include <SFML/Graphics.hpp>
#include <filesystem>

sf::Sprite spritesJuego(const std::filesystem::path& ruta, sf::Vector2i posIni, sf::Vector2i tamanio)
{
    sf::Texture textura(ruta, false, sf::IntRect(posIni, tamanio));
    sf::Sprite spriteEntidad(textura);
    return spriteEntidad;
}

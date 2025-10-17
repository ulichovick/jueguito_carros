#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <random>

int main()
{
    auto windowW {1024u};
    auto windowH {700u};
    auto window = sf::RenderWindow(sf::VideoMode({windowW, windowH}), "Carreras");
    window.setFramerateLimit(60);

    //arreglar el acceso a filesystem... eta verga ta fea
    auto assetsPath = std::filesystem::current_path().parent_path().parent_path().parent_path() / "Assets";
    auto pistaPath = assetsPath / "Track.png";
    auto jugadorPath = assetsPath / "Player.png";
    auto enemigoPath = assetsPath / "Enemy.png";
    

    //dibujar el fondo (pista) y agregar su velocidad de desplazamiento (also, ponerla en bucle)
    float scrollSpeed {100.0};
    sf::Texture pistaTextura(pistaPath, false, sf::IntRect({0, 287}, {1024, 852}));
    pistaTextura.setRepeated(true);
    sf::Sprite pista(pistaTextura);
    pista.setTextureRect(sf::IntRect({0, 0}, {1024, 700}));
    pista.setPosition({0.f, 0.f});


    //dibujar el jugador y almacenarlo en un sprite
    sf::Texture jugadorTextura(jugadorPath, false, sf::IntRect({0, 45}, {1026, 1280}));
    sf::Sprite jugador(jugadorTextura);
    jugador.setPosition({466.f, 570.f});
    jugador.scale({0.1f, 0.1f});

    //dibujar el enemigo y almacenarlo en un sprite
    sf::Texture enemigoTextura(enemigoPath, false, sf::IntRect({0, 45}, {1026, 1280}));
    

    //fuente del score
    auto fontPath = std::filesystem::current_path().parent_path().parent_path();
    fontPath = fontPath.parent_path() / "Assets" / "Fonts" / "kenney_mini_square.ttf";

    sf::Font scoreFont;

    if (!scoreFont.openFromFile(fontPath))
    {
        std::cerr << "Fuente no encontrada!";
        exit(-1);
    }
    
    sf::String scoreString = "Score: ";
    sf::Text textoScore(scoreFont);
    textoScore.setString(scoreString);
    //textoScore.setPosition({250, 700});

    int puntuacion{};

    std::random_device rd{};
    std::seed_seq ss{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
    std::mt19937 mt{ss};
    float spawnEnemigos[3] {375.f, 466.f, 557.f};

    std::vector<std::shared_ptr<sf::Sprite>> enemigos;
    std::uniform_int_distribution distr{0, 5};
    
    sf::Clock clock;
    sf::Clock clockEnemigos;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            //controles del jugador (sobreescribir la posicion/teletransportar)
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
            {
                sf::Vector2f posActual = jugador.getPosition();
                if (posActual.x > spawnEnemigos[0])
                {
                    jugador.setPosition({posActual.x - 91.f , 570.f});
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
            {
                sf::Vector2f posActual = jugador.getPosition();
                if (posActual.x < spawnEnemigos[2])
                {
                    jugador.setPosition({posActual.x + 91.f , 570.f}); 
                }
                
                
            }
        }
        /* controles de jugador que agregan offset a la posicion
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
        {
            jugador.move({-15.f , 0}); 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
        {
            jugador.move({15.f , 0}); 
        } */

        // calcular el delta (tiempo transcurrido entre frames)
        float deltaTime = clock.restart().asSeconds();

        sf::IntRect pistaRect = pista.getTextureRect();
        pistaRect.position.y -= static_cast<int>(scrollSpeed * deltaTime);
        pista.setTextureRect(pistaRect);

        auto enemigo = std::make_shared<sf::Sprite>(enemigoTextura);
        if (clockEnemigos.getElapsedTime().asSeconds() >= 2.f)
        {
            enemigo->scale({0.1f, 0.1f});
            enemigo->setPosition({spawnEnemigos[distr(mt)/2], 0.f});
            enemigos.push_back(enemigo);
            clockEnemigos.restart();
        }
        /* bloque experimental en caso de querer hacer enemigos que se muevan mas rapido
        for (auto it = enemigos.begin(); it != enemigos.end(); )
        {
            auto& enemigo = *it;
            enemigo->move({0.f, scrollSpeed * deltaTime});

            // Check if enemy is below the window
            if (enemigo->getPosition().y > window.getSize().y)
            {
                it = enemigos.erase(it); // remove and advance safely
                std::cout << "Enemy removed, total: " << enemigos.size() << "\n";
            }
            else
            {
                window.draw(*enemigo);
                ++it; // advance only if not erased
            }
        }
        */

        
        for (auto& enemigoPtr : enemigos)
        {
            enemigoPtr->move({0.f, scrollSpeed * deltaTime});
        }

        if (!enemigos.empty() && enemigos.front()->getPosition().y > windowH)
        {
            enemigos.erase(enemigos.begin()); // removes the last element safely
            puntuacion++;
            textoScore.setString(scoreString + std::to_string(puntuacion));
        }

        window.clear();
        window.draw(pista);
        window.draw(textoScore);
        window.draw(jugador);
        
        for (auto& enemigoPtr : enemigos)
        {
            window.draw(*enemigoPtr);

        }

        window.display();
    }
}

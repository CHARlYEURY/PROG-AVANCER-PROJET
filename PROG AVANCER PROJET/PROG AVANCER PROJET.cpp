#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>
#include <time.h>
#include <vector>

using namespace std;

const int WINDOW_WIDTH = 1900;
const int WINDOW_HEIGHT = 1080;
const int WINDOW_MID = 540;

const int ROADLINE_SIZEW = 300;
const int ROADLINE_SIZEH = 10;



class twinguo {
private:
    sf::Texture textureEnnemi;

public:
    sf::Sprite twinguos;

    // CONSTRUCTEUR DE VOITURE TWINGUO
    twinguo(const sf::Texture texturePath) {
        /*if (!textureEnnemi.loadFromFile(texturePath)) {
            throw std::runtime_error("Erreur : Impossible de charger la texture");
        }*/
        textureEnnemi = texturePath;
        twinguos.setTexture(textureEnnemi);
        twinguos.setScale(0.5f, 0.5f); // Échelle de la voiture
        resetPosition();
    }

    // REINITIALISE LES TWINGUO 
    void resetPosition() {
        float x = WINDOW_WIDTH + rand() % 500;  // HORS DE L'ECRAN
        float y = rand() % (WINDOW_HEIGHT - 150);  // POSITION Y ALEATOIRE
        twinguos.setPosition(x, y);
    }

    // MET À JOUR LES DÉPLACEMENTS ET REPOSITIONNE SI NÉCESSAIRE
    void update(float speed) {
        twinguos.move(-speed, 0.f);
        if (twinguos.getPosition().x + twinguos.getGlobalBounds().width < 0) {
            resetPosition(); // Réinitialiser si hors écran
        }
    }

    // COLLISION 
    bool checkCollision(const sf::Sprite& other) const {
        return twinguos.getGlobalBounds().intersects(other.getGlobalBounds());
    }
    

    // RECUPERE LA FORME POUR L'AFFICHER
    const sf::Sprite& getShape() const {
        return twinguos;
    }    
    void setShape(){
        twinguos.setTexture(textureEnnemi);
    }
};


// Fonction pour réinitialiser les roadLines
void resetRoadLine(sf::RectangleShape& roadLine, const sf::RectangleShape& NOVISIBLEborder) {
    if (roadLine.getGlobalBounds().intersects(NOVISIBLEborder.getGlobalBounds())) {
        roadLine.setPosition(sf::Vector2f(1850.f, WINDOW_MID));
    }
}

int main() {

    bool isMenuActive = true;
    bool isGameOver = false;


    sf::Texture textureEnnemi;
    if (!textureEnnemi.loadFromFile("RESSOURCE/voiture6.png")) {
            throw std::runtime_error("Erreur : Impossible de charger la texture");
    }
    // CREATION FENETRE
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Fenêtre SFML");
    window.setFramerateLimit(60);
   

    // MUSIQUE
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("RESSOURCE/120_Synth Arp 1_A Minor.wav")) {
        std::cout << "Erreur : Impossible de charger le fichier audio." << std::endl;
        return -1;
    }

    // TEXTURES
    sf::Texture texture;
    if (!texture.loadFromFile("RESSOURCE/texture.jpg")) {
        return -1; // Erreur si le fichier est introuvable
    }

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("RESSOURCE/voiture5.png")) {
        return -1; // Erreur si le fichier est introuvable
    }

    sf::Texture gameoverTexture;
    if (!gameoverTexture.loadFromFile("RESSOURCE/gameover.jpg")) {
        return -1; // Erreur si le fichier est introuvable
    }

    sf::Texture button;
    if (!button.loadFromFile("RESSOURCE/button.png")) {
        return -1; // Erreur si le fichier est introuvable
    }
    sf::Sprite buttonsprite;
    buttonsprite.setTexture(button);
    buttonsprite.setScale(0.5f, 0.5f); // Réduire la taille si nécessaire
    buttonsprite.setPosition(WINDOW_WIDTH / 2.55, WINDOW_HEIGHT / 2.95); // Position initiale

    

    sf::Sprite gameover;
    gameover.setTexture(gameoverTexture);
    gameover.setScale(0.5f, 0.5f); // Réduire la taille si nécessaire
    gameover.setPosition(WINDOW_WIDTH / 4.f, WINDOW_HEIGHT / 4); // Position initiale

    sf::Sprite playerSprite;
    playerSprite.setTexture(playerTexture);
    playerSprite.setScale(0.5f, 0.5f); // Réduire la taille si nécessaire
    playerSprite.setPosition(WINDOW_WIDTH / 150.f, WINDOW_HEIGHT - 634.f); // Position initiale

    // Horloge pour la difficulté croissante
    sf::Clock difficultyClock;


    // SPRITES DE GRILLE
    std::vector<sf::Sprite> sprites;
    float spriteWidth = texture.getSize().x * 0.1f;
    float spriteHeight = texture.getSize().y * 0.1f;

    int cols = static_cast<int>(WINDOW_WIDTH / spriteWidth) + 2;
    int rows = static_cast<int>(WINDOW_HEIGHT / spriteHeight) + 1;

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sprite.setScale(0.2f, 0.1f);
            sprite.setPosition(col * spriteWidth, row * spriteHeight);
            sprites.push_back(sprite);
        }
    }

    srand(static_cast<unsigned>(time(0)));

    // ROADLINES
    sf::RectangleShape roadLines[] = {
        sf::RectangleShape(sf::Vector2f(ROADLINE_SIZEW, ROADLINE_SIZEH)),
        sf::RectangleShape(sf::Vector2f(ROADLINE_SIZEW, ROADLINE_SIZEH)),
        sf::RectangleShape(sf::Vector2f(ROADLINE_SIZEW, ROADLINE_SIZEH)),
        sf::RectangleShape(sf::Vector2f(ROADLINE_SIZEW, ROADLINE_SIZEH)),
        sf::RectangleShape(sf::Vector2f(ROADLINE_SIZEW, ROADLINE_SIZEH))
    };

    for (int i = 0; i < 5; ++i) {
        roadLines[i].setFillColor(sf::Color(255, 255, 255));
        roadLines[i].setPosition(sf::Vector2f(1800.f - (450.f * i), WINDOW_MID));
    }

    sf::RectangleShape NOVISIBLEborder(sf::Vector2f(1, WINDOW_HEIGHT));
    NOVISIBLEborder.setFillColor(sf::Color::Red);
    NOVISIBLEborder.setPosition(sf::Vector2f(-400, 0.f));

    // HORLOGES
    sf::Clock clockT, clockJ;
    sf::Clock spawnClock;
    float spawnDelay = 3.f;


    std::vector<twinguo> obstacles;


    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.setLoop(true);
    sound.play();

    

    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Gestion du clic sur le bouton Play
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);

            if (buttonsprite.getGlobalBounds().contains(mousePos)) {
                std::cout << "Bouton Play cliqué !" << std::endl;
                isMenuActive = false; // Quitte le menu
            }
        }
    

    // Affichage du menu
    if (isMenuActive) {
        window.clear();
        window.draw(buttonsprite);
        window.display();
        continue; // Retourne au début de la boucle pour ne pas exécuter le jeu
    }







        if (isGameOver) {
            // Afficher l'écran "Game Over"
            window.clear();
            window.draw(gameover); // Dessiner l'image de "Game Over"
            window.display();

            // Ajout d'un délai pour que le joueur voie l'écran (optionnel)
            sf::sleep(sf::seconds(5));
            window.close(); // Ferme la fenêtre après avoir affiché l'écran "Game Over"
            break;
        }



        // Calcul du temps écoulé depuis le début du jeu
        float elapsedTime = difficultyClock.getElapsedTime().asSeconds();
        // Augmenter la vitesse des éléments au fil du temps (augmentation linéaire)
        float speedMultiplier = 0.5 + elapsedTime / 20.f;  
        float speedMultiplierPlayer = 0.3 + elapsedTime / 40.f;  



        // DEPLACEMENT DU JOUEUR
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) playerSprite.move(0.f, -13);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) playerSprite.move(0.f, 13);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) playerSprite.move(13, 0.f);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) playerSprite.move(-13, 0.f);




        // DEPLACEMENT DES SPRITES DE LA ROUTE
        for (auto& sprite : sprites) {
            sprite.move(-12 * speedMultiplier, 0.f); // Déplace vers la gauche
             

           // Si le sprite dépasse l'écran, le réinitialiser en tenant compte de la largeur
            if (sprite.getPosition().x + sprite.getGlobalBounds().width < 0) {
                float offsetX = sprite.getPosition().x + sprite.getGlobalBounds().width; // Distance hors écran
                sprite.setPosition(WINDOW_WIDTH + offsetX, sprite.getPosition().y);
            }
        }

        // DEPLACEMENT DES ROADLINES
        for (auto& roadLine : roadLines) {
            roadLine.move(-12 * speedMultiplier, 0.f);   
            resetRoadLine(roadLine, NOVISIBLEborder);
        }

        // OBSTACLES
        if (spawnClock.getElapsedTime().asSeconds() > spawnDelay) {
            if (obstacles.size() < 5) {  
                obstacles.emplace_back(textureEnnemi);
            }
            spawnClock.restart();
        }

        // MISE À JOUR DES OBSTACLES
        for (auto& obstacle : obstacles) {
            obstacle.update(11 * speedMultiplier);
            if (obstacle.checkCollision(playerSprite)) {

                isGameOver = true;  // Change l'état du jeu à "Game Over"
                break; // Stoppe la vérification des autres obstacles

            }
        }




        // DESSIN
        window.clear();
        for (const auto& sprite : sprites) window.draw(sprite);
        for (const auto& roadLine : roadLines) window.draw(roadLine);
        for (auto& obstacle : obstacles) { obstacle.setShape(); window.draw(obstacle.getShape()); }
        window.draw(playerSprite);
        window.display();

    }

    

    return 0;
}

#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

using namespace sf;
using namespace std;

// ��������� ��� ������� �������� ���� � �������� �������
const int width = 20;
const int height = 20;
const int scale = 40;

// ������� ������ � ������, ����� ������
int x, y;
int fruitX, fruitY;
int nTail;
const int maxTailLength = 100;
int tailX[maxTailLength], tailY[maxTailLength];

// �������� ���� � ����
int speed;
int score;
bool gameOver;

// ����������� �������� ������
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;

// ������������� ����
void Setup(int difficulty) {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    nTail = 0;

    // ��������� �������� � ����������� �� ���������� ������ ���������
    switch (difficulty) {
    case 1:
        speed = 150;
        break;
    case 2:
        speed = 100;
        break;
    case 3:
        speed = 50;
        break;
    }
}

// ��������� ���� ��������� ����
void Draw(RenderWindow& window, Sprite& background, Sprite& snakeHead, Sprite& snakeTail, Sprite& fruit, Text& scoreText, Text& gameOverText, Text& againText, Text& exitText) {
    window.clear();
    window.draw(background);

    // ��������� ������ ������
    for (int i = 0; i < nTail; i++) {
        snakeTail.setPosition(tailX[i] * scale, tailY[i] * scale);
        window.draw(snakeTail);
    }

    // ��������� ������ ������
    snakeHead.setPosition(x * scale + scale / 2, y * scale + scale / 2);
    window.draw(snakeHead);

    // ��������� ������
    fruit.setPosition(fruitX * scale, fruitY * scale);
    window.draw(fruit);

    // ����������� �����
    scoreText.setString("Score: " + to_string(score));
    window.draw(scoreText);

    // ����������� ��������� ����� ���������� ����
    if (gameOver) {
        window.draw(gameOverText);
        window.draw(againText);
        window.draw(exitText);
    }

    window.display();
}

// ��������� ����� � ����������
void Input() {
    if (Keyboard::isKeyPressed(Keyboard::Left) && dir != RIGHT)
        dir = LEFT;
    else if (Keyboard::isKeyPressed(Keyboard::Right) && dir != LEFT)
        dir = RIGHT;
    else if (Keyboard::isKeyPressed(Keyboard::Up) && dir != DOWN)
        dir = UP;
    else if (Keyboard::isKeyPressed(Keyboard::Down) && dir != UP)
        dir = DOWN;
    else if (Keyboard::isKeyPressed(Keyboard::Escape))
        gameOver = true;
}

// �������� ��������� ������
bool IsCellOccupied() {
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == fruitX && tailY[i] == fruitY)
            return true;
    }
    if (fruitX == x && fruitY == y)
        return true;
    return false;
}

// ��������� ������ ������
void GenerateNewFruit() {
    do {
        fruitX = rand() % width;
        fruitY = rand() % height;
    } while (IsCellOccupied());
}

// ������ ����
void Logic() {
    // ���������� ������ ������
    int prevX = tailX[nTail - 1];
    int prevY = tailY[nTail - 1];
    for (int i = nTail - 1; i > 0; i--) {
        tailX[i] = tailX[i - 1];
        tailY[i] = tailY[i - 1];
    }
    tailX[0] = x;
    tailY[0] = y;

    // �������� �������� ������
    if (x == fruitX && y == fruitY) {
        score += 1;
        if (nTail < maxTailLength) {
            nTail++;
            tailX[nTail - 1] = prevX;
            tailY[nTail - 1] = prevY;
            GenerateNewFruit();
            if (speed > 10)
                speed -= 5;
        }
    }

    // ���������� ������� ������ ������ � ����������� �� �����������
    switch (dir) {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    }

    // �������� ������ �� ������� �������� ����
    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;
    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    // �������� ������������ � �������
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y) {
            gameOver = true;
            return;
        }
    }
}

// ��������� ���� ����
int StartMenu(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Font loading error" << endl;
        return -1;
    }

    Text title("Snake Game", font, 60);
    title.setFillColor(Color::Green);
    title.setPosition(250, 100);

    Text start("Start Game", font, 40);
    start.setFillColor(Color::White);
    start.setPosition(100, 250);

    Text exit("Exit", font, 40);
    exit.setFillColor(Color::White);
    exit.setPosition(100, 350);

    Text* options[] = { &start, &exit };
    int selected = 0;

    // ��������� ����� � ����
    while (true) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return -1;
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    selected = (selected - 1 + 2) % 2;
                if (event.key.code == Keyboard::Down)
                    selected = (selected + 1) % 2;
                if (event.key.code == Keyboard::Enter) {
                    if (selected == 0)
                        return 1;
                    if (selected == 1) {
                        window.close();
                        return -1;
                    }
                }
            }
        }

        window.draw(title);
        for (int i = 0; i < 2; i++) {
            if (i == selected)
                options[i]->setFillColor(Color::Red);
            else
                options[i]->setFillColor(Color::White);
            window.draw(*options[i]);
        }
        window.display();
    }
}

// ���� ������ ������ ���������
int LevelSelectionMenu(RenderWindow& window) {
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Font loading error" << endl;
        return -1;
    }

    Text title("Choose the difficulty level", font, 50);
    title.setFillColor(Color::Green);
    title.setPosition(100, 150);

    Text easy("1. Easy", font, 40);
    easy.setFillColor(Color::White);
    easy.setPosition(100, 250);

    Text medium("2. Medium", font, 40);
    medium.setFillColor(Color::White);
    medium.setPosition(100, 350);

    Text hard("3. Hard", font, 40);
    hard.setFillColor(Color::White);
    hard.setPosition(100, 450);

    Text exit("4. Exit", font, 40);
    exit.setFillColor(Color::White);
    exit.setPosition(100, 550);

    Text* options[] = { &easy, &medium, &hard, &exit };
    int selected = 0;

    // ��������� ����� � ���� ������ ������ ���������
    while (true) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return -1;
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Up)
                    selected = (selected - 1 + 4) % 4;
                if (event.key.code == Keyboard::Down)
                    selected = (selected + 1) % 4;
                if (event.key.code == Keyboard::Enter) {
                    if (selected == 0)
                        return 1;
                    if (selected == 1)
                        return 2;
                    if (selected == 2)
                        return 3;
                    if (selected == 3) {
                        window.close();
                        return -1;
                    }
                }
            }
        }

        window.clear();
        window.draw(title);
        for (int i = 0; i < 4; i++) {
            if (i == selected)
                options[i]->setFillColor(Color::Red);
            else
                options[i]->setFillColor(Color::White);
            window.draw(*options[i]);
        }
        window.display();
    }
}

// ������� ������� ���������
int main() {

    srand(static_cast<unsigned int>(time(nullptr)));  // ������������� ���������� ��������� �����

    RenderWindow window(VideoMode(width * scale, height * scale), "Snake Game");

    // �������� ������� ��� ����, ������ ������, ���� ������ � ������
    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background.png")) {
        cout << "Error loading background image" << endl;
        return -1;
    }
    Sprite background(backgroundTexture);

    Texture snakeHeadTexture;
    if (!snakeHeadTexture.loadFromFile("snake_head.png")) {
        cout << "Error loading snake head image" << endl;
        return -1;
    }
    Sprite snakeHead(snakeHeadTexture);
    snakeHead.setOrigin(snakeHeadTexture.getSize().x / 2, snakeHeadTexture.getSize().y / 2);

    Texture snakeBodyTexture;
    if (!snakeBodyTexture.loadFromFile("snake_body.png")) {
        cout << "Error loading snake body image" << endl;
        return -1;
    }
    Sprite snakeBody(snakeBodyTexture);

    Texture fruitTexture;
    if (!fruitTexture.loadFromFile("fruit.png")) {
        cout << "Error loading fruit image" << endl;
        return -1;
    }
    Sprite fruit(fruitTexture);

    // �������� ������ � �������� ������� ��� �����������
    Font font;
    if (!font.loadFromFile("arial.ttf")) {
        cerr << "Font loading error" << endl;
        return -1;
    }

    Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(10, 10);

    Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setString("Game Over");
    gameOverText.setPosition(width * scale / 2 - gameOverText.getGlobalBounds().width / 2, height * scale / 2 - gameOverText.getGlobalBounds().height / 2 - 50);

    Text againText("Again", font, 40);
    againText.setFillColor(Color::White);
    againText.setPosition(width * scale / 2 - againText.getGlobalBounds().width / 2, height * scale / 2 + 25);

    Text exitText("Exit", font, 40);
    exitText.setFillColor(Color::White);
    exitText.setPosition(width * scale / 2 - exitText.getGlobalBounds().width / 2, height * scale / 2 + 100);

    // ������ ���������� ����
    int startOption = StartMenu(window);
    if (startOption == -1)
        return 0;

    // ������ ���� ������ ������ ���������
    int difficultyLevels = LevelSelectionMenu(window);
    if (difficultyLevels == -1)
        return 0;

    // ������������� ����
    Setup(difficultyLevels);

    // �������� ������� ����
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
        }
        if (gameOver) {
            Draw(window, background, snakeHead, snakeBody, fruit, scoreText, gameOverText, againText, exitText);

            // ����������� ������ ��������� ����
            int selectedOption = 0;

            while (window.isOpen() && gameOver) {
                while (window.pollEvent(event)) {
                    if (event.type == Event::Closed) {
                        window.close();
                        return 0;
                    }
                    if (event.type == Event::KeyPressed) {
                        if (event.key.code == Keyboard::Up)
                            selectedOption = (selectedOption - 1 + 2) % 2;
                        if (event.key.code == Keyboard::Down)
                            selectedOption = (selectedOption + 1) % 2;
                        if (event.key.code == Keyboard::Enter) {
                            if (selectedOption == 0) {
                                difficultyLevels = LevelSelectionMenu(window);
                                if (difficultyLevels == -1) {
                                    return 0;
                                }
                                Setup(difficultyLevels);
                                gameOver = false;
                            }
                            else if (selectedOption == 1) {
                                window.close();
                                return 0;
                            }
                        }
                    }
                }
                againText.setFillColor(selectedOption == 0 ? Color::Red : Color::White);
                exitText.setFillColor(selectedOption == 1 ? Color::Red : Color::White);

                Draw(window, background, snakeHead, snakeBody, fruit, scoreText, gameOverText, againText, exitText);
            }
            continue;
        }

        Input();  // ��������� �����
        Logic();  // ������ ����

        // ��������� ���� �������� ������ ������ � ����������� �� �����������
        switch (dir) {
        case LEFT:
            snakeHead.setRotation(180);
            break;
        case RIGHT:
            snakeHead.setRotation(0);
            break;
        case UP:
            snakeHead.setRotation(270);
            break;
        case DOWN:
            snakeHead.setRotation(90);
            break;
        }

        Draw(window, background, snakeHead, snakeBody, fruit, scoreText, gameOverText, againText, exitText);  // ��������� ���� ���������

        sleep(milliseconds(speed));  // �������� ��� ���������� ��������� ����
    }

    return 0;
}

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream >
#include <stdlib.h>
#include <typeinfo>
#include <stdarg.h>
#include< type_traits >
#include <locale>
#include <Windows.h>

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

void drawField(sf::RectangleShape  (& object) [5][5], sf::RenderWindow& window)
{
				for (int i = 0; i < 5; i++)
				{
					for (int j = 0; j < 5; j++)
						window.draw(object[i][j]);
				}
	return;
}
void drawField(sf::Text (&object)[5][5], sf::RenderWindow& window, char(&letter)[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{

			//std::string buf = { letter[i][j] };
			char buf = 'ы';
			sf::String buff = { buf };
			object[i][j].setString(buff);
			window.draw( object[i][j]);
		}
	}
	return;
}
void clearField(sf::RectangleShape(&object)[5][5])
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
			object[i][j].setOutlineColor(sf::Color::White);
	}
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	setlocale(LC_ALL, "ru");
	sf::Font font;
	if(!font.loadFromFile("arial.ttf"))
		return EXIT_SUCCESS;
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// Define some constants
	const int gameWidth = 800;
	const int gameHeight = 800;
	char numberOfColumns=0;
	int gameStep = 2;
	std::string word;


	// Create the window of the application
	sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 32), L"Балда",
		sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	

	//sf::Font font;
	//if (!font.loadFromFile("resources/sansation.ttf"))
	//	return EXIT_FAILURE;

	// Initialize the pause message
	sf::Text startMessage;
	startMessage.setFont(font);
	startMessage.setCharacterSize(40);
	startMessage.setPosition(170.f, 150.f);
	startMessage.setFillColor(sf::Color::White);
	startMessage.setString(L"Привет Дэбил, это Балда!\nPress space to start the game");

	// Initialize settings message
	sf::Text settingMessage;
	std::wstring settingMessageString(L"введите имя первого и второго игроков");
	settingMessage.setFont(font);
	settingMessage.setCharacterSize(40);
	settingMessage.setPosition(100.f, 50.f);
	settingMessage.setFillColor(sf::Color::White);
	settingMessage.setString(settingMessageString);

	// Initialize starting Word message
	sf::Text startWordMessage;
	std::wstring startWordMessageString (L"Введи начальное слово :");
	startWordMessage.setFont(font);
	startWordMessage.setCharacterSize(40);
	startWordMessage.setPosition(100.f, 10.f);
	startWordMessage.setFillColor(sf::Color::White);
	startWordMessage.setString(startWordMessageString );

	std::wstring player1 = L"Марк";
	std::wstring player2 = L"Найк";
	sf::Text wordMessage;
	std::wstring wordMessageString = L"Твой ход, ";
	wordMessage.setFont(font);
	wordMessage.setCharacterSize(40);
	wordMessage.setPosition(100.f, 10.f);
	wordMessage.setFillColor(sf::Color::White);
	wordMessage.setString(wordMessageString + player1);


	// Initialize Field
	sf::RectangleShape rectangle [5][5];
	float indent = 100;
	float rectangleWidth = gameWidth / 5; 
	float rectangleHeight = (gameHeight - indent) / 5;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			rectangle[i][j].setSize(sf::Vector2f(rectangleWidth-20, rectangleHeight-20));
			rectangle[i][j].setOutlineColor(sf::Color::White);
			rectangle[i][j].setOutlineThickness(5);
			rectangle[i][j].setPosition(10+rectangleWidth * j, indent + 10+rectangleHeight * i);
		}
	}

	// Initialize Field of letters
	sf::Text letters [5][5];
	char letter_[5][5] = {'а','б','в','г','д',
						  ' ',' ',' ',' ',' ',
						  ' ',' ',' ',' ',' ',
						  ' ',' ',' ',' ',' ',
						  ' ',' ',' ',' ',' ', };

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			letters[i][j].setFont(font);
			letters[i][j].setCharacterSize(120);
			letters[i][j].setPosition(rectangleWidth * j + 25, indent+ rectangleHeight *i - 15);
			letters[i][j].setFillColor(sf::Color::Black);
			char buf = 'ы';
			letters[i][j].setString(buf);
		}
	}
	



	sf::Clock clock;
	bool isPlaying = false;
	bool rectangleChoosed = false;
	//координаты выбранного прямоугольника
	int I; 
	int J;
	while (window.isOpen())
	{
	
		// Handle events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Window closed or escape key pressed: exit
			if ((event.type == sf::Event::Closed) ||
				((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) //exit game
			{
				window.close();
				gameStep = 0;
				break;
			}
			else if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Space)) // start game
			{
				window.draw(settingMessage);
				window.pollEvent(event);
				gameStep = 1;		
				break;
			}
			else if ((gameStep == 1) && (event.type == sf::Event::TextEntered)) // enter number of columns
			{
				if (event.key.code >= 48 && event.key.code <= 57)
				{
					char get_sym[] = {static_cast<char>(event.key.code) };
					numberOfColumns = *(get_sym);
					
					settingMessageString += L"\n";
					settingMessageString += numberOfColumns;
					settingMessage.setString(settingMessageString);
					gameStep = 2;
				}
				break;
			}
			else if (gameStep == 2 ) // enter start word
			{
				if (event.type == sf::Event::TextEntered)
				{
					//std::cout << event.text.unicode << std::endl;

					//if (event.text.unicode < 128) 
					{
						//char buf;
						//std::cin >> buf;
						//std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
						//word += {buf};

						//letter_[2][word.size() - 1] = {buf};
						//std::cout << word;
					}
					if(word.size() == 5)
					gameStep = 3;
				}
			}
			else if (gameStep == 3) // choosing rectangle and entering letter
			{
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					int j = (int)(event.mouseButton.x / rectangleWidth);
					int i = (int)((event.mouseButton.y - indent) / rectangleHeight);
					if (rectangle[i][j].getOutlineColor() == sf::Color::White && !rectangleChoosed)
					{
						rectangle[i][j].setOutlineColor(sf::Color::Blue);
						wordMessage.setString(L"Enter your word, " + player1);
						rectangleChoosed = true;
						I = i; J = j;
					}
					else if(rectangle[i][j].getOutlineColor() == sf::Color::Blue && rectangleChoosed)
					{
						rectangle[i][j].setOutlineColor(sf::Color::White);
						wordMessage.setString(L"Enter your word, " + player1);
						rectangleChoosed = false;
					}
					else if (rectangle[i][j].getOutlineColor() == sf::Color::White && rectangleChoosed)
					{
						clearField(rectangle);
						rectangle[i][j].setOutlineColor(sf::Color::Blue);
						I = i; J = j;
					}
				}
				if (event.type == sf::Event::TextEntered && rectangleChoosed)
				{
					if (event.text.unicode < 128)
					{
						letter_[I][J] = static_cast<char>(event.text.unicode);
						letters[I][J].setString(letter_[I][J]);
						//gameStep = 4;
					}
				}

			}
			if (gameStep == 4)
			{
			
			
			}

		}
		window.clear(sf::Color(50, 200, 50));
		{
			if(gameStep == 0)
				window.draw(startMessage);
			else if(gameStep == 1)
				window.draw(settingMessage);
			else if (gameStep == 2) //typing start word
			{
				drawField(rectangle, window);
				window.draw(startWordMessage);
				drawField(letters, window, letter_);
			}
			else if (gameStep == 3) 
			{
				
				drawField(rectangle, window);
				window.draw(wordMessage);
				drawField(letters, window,letter_);
			}
			else if (gameStep == 4)
			{
				drawField(rectangle, window);
				window.draw(wordMessage);
				drawField(letters, window, letter_);
			}

		}

		// Clear the window


		// Display things on screen
		window.display();
	}

	return EXIT_SUCCESS;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

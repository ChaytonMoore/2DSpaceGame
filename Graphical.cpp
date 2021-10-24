#include "Graphical.h"


//Should probably rewrite this function as it is redundant and can cause memory leaks


void LoadImageSF(sf::Sprite* Image, std::string Name)
{
	//contains a generic location, that should be changed later in the code

	try
	{

		sf::Image* i = new sf::Image;

		sf::Texture* t = new sf::Texture;

		i->loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/" + Name);

		t->loadFromImage(*i);

		Image->setTexture(*t);
	}
	catch (std::out_of_range)
	{
		//throw std::out_of_range("Insufficient number of values in profile of: ");
		std::string message;
		message = "Can't find file at location \n C:/Users/User/Documents/2DSpaceGame/Textures/"+Name ;
		std::wstring s = std::wstring(message.begin(), message.end());

		int msgboxID = MessageBox(
			NULL,
			(message.c_str()), //contents
			"Texture Missing", //title
			MB_ICONWARNING | MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);
		switch (msgboxID)
		{
		case IDCANCEL:
				exit(-1);
		case IDTRYAGAIN:
			LoadImageSF(Image,Name);
		case IDCONTINUE:
			break;
		}
	}

}
#include "DetailsPanel.h"


void DetailsPanel::RenderPanel(sf::RenderWindow* window)
{
	if (CurrentObject != "")
	{
		window->draw(MainImage);
	}
}


void DetailsPanel::Reload()
{
	//The image will need to be reloaded.
	ImageRef.loadFromFile("C:/Users/User/Documents/2DSpaceGame/Textures/Ships/"+CurrentObject+".png");
	TextureRef.loadFromImage(ImageRef);
	MainImage.setTexture(TextureRef);
	MainImage.setPosition(400,400);

}


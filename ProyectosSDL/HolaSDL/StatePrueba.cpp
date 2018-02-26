#include "StatePrueba.h"


StatePrueba::StatePrueba(SDLApp* app): GameState(app) {

	//COLISIONABLES
	//Entity* object = new ColisionableObject(app, 500, 500, 100, 100, resources->getImageTexture(Resources::InvMarca));
	//stage.push_back(object);
	//collision.push_back(object);

	//PERSONAJE
	//Siempre 1� para que ocupe la 1� posicion en la lista
	list = new ObjectList(app);
	alena = new MainCharacter(app, 400, 300, 39, 143, resources->getImageTexture(Resources::Alena), list, collision);
	stage.push_back(alena);

	//ITEMS
	/*ItemInventario* test = new ItemInventario(app, 368, 0, 64,64, "descripcion", "tag", resources->getImageTexture(Resources::ImagenTest));
	stage.push_back(test);
	ItemInventario* test2 = new ItemInventario(app, 100, 0, 64, 64, "descripcion", "tag", resources->getImageTexture(Resources::ImagenTest));
	stage.push_back(test2);
	ItemInventario* test3 = new ItemInventario(app, 123, 0, 64, 64, "descripcion", "tag", resources->getImageTexture(Resources::ImagenTest));
	stage.push_back(test3);
	ItemInventario* test4 = new ItemInventario(app, 500, 0, 64, 64, "descripcion", "tag", resources->getImageTexture(Resources::ImagenTest));
	stage.push_back(test4);*/

	scenes.push_back(new Scene(true, app, 0));
	scenes.push_back(new Scene(false, app, 0));
}

void StatePrueba::swapScene(int nextScene)
{
	scenes[currentScene]->exitScene();
	currentScene = nextScene;
	scenes[nextScene]->loadScene();
}

void StatePrueba::handleEvent(SDL_Event &e) { //redefino el metodo HandleEvent del estado solo para cambiar de teclas con el teclado para ir probando
	if (e.key.keysym.sym == SDLK_8) {
		swapScene(1);
	}
	else if (e.key.keysym.sym == SDLK_9) {
		swapScene(0);
	}
	GameState::handleEvent(e);
}

#include "PlayState.h"

PlayState::~PlayState() {
	vector<Scene*>::iterator aux;
	scenes[currentScene]->exitScene();
	for (aux = scenes.begin(); aux != scenes.end(); aux++) {
		(*aux)->saveSceneToJson();
		delete (*aux);
	}
	delete list;
}

PlayState::PlayState(SDLApp* app): GameState(app) {

	// crea la lista vacia
	list = new ObjectList(app);

	string name = "..\\Scenes\\Scene1.json";

	// Inicializa el personaje con los datos de archivo de la primera escena
	std::ifstream i(name);
	json j;
	i >> j;

	initializeMainPJ(j);

	i.close();

	// crea las escenas 1 y 2 desde archivo
	scenes.push_back(new Scene(1, app));
	scenes.push_back(new Scene(2, app));
}

void PlayState::initializeMainPJ(json& j) {
	int n = j["mainPj"]["Texture"];
	alena = new MainCharacter(app, j["mainPj"]["x"], j["mainPj"]["y"], j["mainPj"]["w"], j["mainPj"]["h"], resources->getImageTexture(Resources::ImageId(n)), list, collision);
	
	for (int i = 0; i < j["mainPj"]["itemList"].size(); i++) {
		n = j["mainPj"]["itemList"][i]["Texture"];

		ItemInventario* item = new ItemInventario(app, j["mainPj"]["itemList"][i]["x"], j["mainPj"]["itemList"][i]["y"], 
			j["mainPj"]["itemList"][i]["w"], j["mainPj"]["itemList"][i]["h"],
			j["mainPj"]["itemList"][i]["descripcion"], j["mainPj"]["itemList"][i]["tag"], resources->getImageTexture(Resources::ImageId(n)));

		dynamic_cast<MainCharacter*>(alena)->addInventoryObject(item);
	}

	stage.push_back(alena);
}

void PlayState::swapScene(int nextScene)
{
	if (nextScene < scenes.size() && nextScene >= 0) { // basicamente se asegura de que la escena a la que quiere cambiar existe
		scenes[currentScene]->exitScene();
		currentScene = nextScene;
		scenes[nextScene]->enterScene();
	}
	else cout << "Escena no encontrada, n�mero buscado: " << nextScene << " , escenas existentes hasta: " << scenes.size() - 1;
}

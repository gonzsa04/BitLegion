#pragma once
#include "checkML.h"
#include "GODoors.h"
#include "json.hpp"
#include "ItemInventario.h"
#include "GOTransiciones.h"
#include "ColisionableObject.h"
#include <fstream>

using json = nlohmann::json;

class MainCharacter;
class Scene
{
public:
	Scene();
	Scene(int numEscena, SDLApp* app, MainCharacter* pj, bool load = false);
	~Scene();
	void enterScene();
	void exitScene();
	int getSceneNumber() const { return SceneNum; };
	void saveSceneToJson();
	Vector2D getPosIni() { return posIni; }
	Vector2D getPlayerTam() { return playerTam; }
	void setPosIni(Vector2D newPos) { posIni = newPos; }
private:
	SDLApp* app;
	int SceneNum;
	list<GameObject*> SceneItems;
	list<GameState*> SceneStates;
	enum PuzzleTypes {Match3, Lights}; //para la lectura del json
	list<GameObject*>::iterator it;
	GameState* CurrentState;
	MainCharacter* pj;
	double width = 0, height = 0, x = 0, y = 0;
	Vector2D posIni, playerTam;
	GameState* PuzzleCreator(PuzzleTypes type, const json& j); //dependiendo del tag, elige un puzzle u otro (podemos tener mas de dos)
	void addAnimsFromJSON(GameObject* obj, json& j, const int numText);
};


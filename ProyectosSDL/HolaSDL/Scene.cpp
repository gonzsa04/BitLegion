#include "Scene.h"
#include <fstream>
#include "MainCharacter.h"
#include "GOstates.h"
#include "LightsOut.h"
#include "GOConversational.h"
#include "PasswordState.h"
#include "PasswordState.h"
#include "AutoConversational.h"
#include "Decorado.h"
#include "NPC.h"
#include "GOcofres.h"

Scene::Scene()
{
	//Leer num scene
	//Idealmente lee de un archivo
}

Scene::Scene(int numEscena, SDLApp* app, MainCharacter* pj, Observer* playState, bool load):app(app), SceneNum(numEscena), pj(pj) {

	addObserver(playState);

	string name;
	if (load)name = "..\\Scenes\\saves\\Scene";
	else name = "..\\Scenes\\Scene";
	name = name + to_string(numEscena) +".json";

	std::ifstream i(name);
	
	if (i.is_open()) { // Para que no intente abrir archivos que no existen

		json j;
		i >> j;
		int n;
		string obj = "ItemInventario"; // nombre del array del objeto en el json

		// Cargado de items de inventario
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];

			bool permanente = false;
			if (!j[obj][i]["permanente"].is_null()) { permanente = j[obj][i]["permanente"]; }

			ItemInventario* item = new ItemInventario(app, j[obj][i]["x"], j[obj][i]["y"], j[obj][i]["w"], j[obj][i]["h"],
				j[obj][i]["descripcion"], j[obj][i]["tag"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), permanente);

			SceneItems.push_front(item);

			addAnimsFromJSON(item, j[obj][i], n);

			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.front()->setRotation(j[obj][i]["rotation"]);
			}
		}

		//Cargado objetos conversaciones
		for (int i = 0; i < j["GOConversational"].size(); i++) {

			n = j["GOConversational"][i]["Texture"];

			SceneItems.push_front(new GOConversational(app, j["GOConversational"][i]["x"], j["GOConversational"][i]["y"], j["GOConversational"][i]["w"], j["GOConversational"][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), j["GOConversational"][i]["convoName"]));
		}

		//Cargado objetos autoconversaciones
		for (int i = 0; i < j["AutoGOConversational"].size(); i++) {

			n = j["AutoGOConversational"][i]["Texture"];

			SceneItems.push_front(new AutoConversational(app, j["AutoGOConversational"][i]["x"], j["AutoGOConversational"][i]["y"], j["AutoGOConversational"][i]["w"], j["AutoGOConversational"][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), j["AutoGOConversational"][i]["convoName"]));
		}

		// Cargado de decoracion
		obj = "Decorado";
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];
			Decorado* aux = new Decorado(app);
			aux->setPosition(Vector2D(j[obj][i]["x"], j[obj][i]["y"]));
			aux->setHeight(j[obj][i]["h"]);
			aux->setWidth(j[obj][i]["w"]);
			ImageRenderer* im = new ImageRenderer(app->getResources()->getImageTexture(Resources::ImageId(n)));
			aux->addRenderComponent(im);
			SceneItems.push_back(aux);

			if (addAnimsFromJSON(aux, j[obj][i], n)) {
				delete im; //si tiene animaciones, borra el anterior componente de render
			}

			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}

		obj = "NPC";
		//Cargado de Puzles
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];


			NPC* npc = new NPC(app, j[obj][i]["x"], j[obj][i]["y"], j[obj][i]["w"], j[obj][i]["h"], 
				app->getResources()->getImageTexture(Resources::ImageId(n)), j[obj][i]["dialogo"]);

			SceneItems.push_back(npc);
			SceneItems.push_back(npc->getColisionable()); //pusheamos el colisionable asociado

			addAnimsFromJSON(npc, j[obj][i], n);

			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}

		obj = "GOState";
		//Cargado de Puzles
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];


			SceneStates.push_back(PuzzleCreator(j[obj][i]["type"], j[obj][i]));

			GOstates* goSt = new GOstates(app, j[obj][i]["x"], j[obj][i]["y"],
				j[obj][i]["w"], j[obj][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), SceneStates.back(), j[obj][i], playState);

			SceneItems.push_back(goSt);

			addAnimsFromJSON(goSt, j[obj][i], n);
			
			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}

		// Cargado de GOTransiciones
		obj = "GOTransiciones";
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];

			GOTransiciones* goTrans = new GOTransiciones(app, j[obj][i]["x"], j[obj][i]["y"],
				j[obj][i]["w"], j[obj][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), j[obj][i]["scneNum"]);

			SceneItems.push_back(goTrans);

			addAnimsFromJSON(goTrans, j[obj][i], n);

			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}

		// Cargado de GOCofres (valido para taquillas, cofres, cajas...)
		obj = "GOCofres";
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];
			double rotGOTrans = 0;
			if (!j[obj][i]["rotGOTr"].is_null())
				rotGOTrans = j[obj][i]["rotGOTr"];

			int id = -4;
			if (!j[obj][i]["UnlockId"].is_null())
				if (j[obj][i]["UnlockId"].is_number_integer()) {
					id = j[obj][i]["UnlockId"];
				}

			bool itPerm = false;
			if (!j[obj][i]["permanenteItem"].is_null()) {
				itPerm = j[obj][i]["permanenteItem"];
			}

			GOcofres* door = new GOcofres(app, j[obj][i]["x"], j[obj][i]["y"], j[obj][i]["w"], j[obj][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), j[obj][i]["tag"], j[obj][i]["wItem"], j[obj][i]["hItem"], j[obj][i]["descripcionItem"], j[obj][i]["tagItem"],
				app->getResources()->getImageTexture(Resources::ImageId(j[obj][i]["numTextItem"])), itPerm, id);

			SceneItems.push_back(door);
			addAnimsFromJSON(door, j[obj][i], n);


			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}

		// Cargado de Colisiones
		obj = "CollisionableObject";
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];

			ColisionableObject* newCol = new ColisionableObject(app, j[obj][i]["x"], j[obj][i]["y"],
				j[obj][i]["w"], j[obj][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)));

			SceneItems.push_back(newCol);

			addAnimsFromJSON(newCol, j[obj][i], n);

			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}

		// Cargado de GODoors
		obj = "GODoors";
		for (int i = 0; i < j[obj].size(); i++) {

			n = j[obj][i]["Texture"];
			double rotGOTrans = 0;
			if(!j[obj][i]["rotGOTr"].is_null())
				rotGOTrans = j[obj][i]["rotGOTr"];

			int id = -4;
			if (!j[obj][i]["UnlockId"].is_null())
				if (j[obj][i]["UnlockId"].is_number_integer()) {
					id = j[obj][i]["UnlockId"];
				}

			GODoors* door = new GODoors(app, j[obj][i]["x"], j[obj][i]["y"], j[obj][i]["w"], j[obj][i]["h"],
				app->getResources()->getImageTexture(Resources::ImageId(n)), j[obj][i]["tag"], j[obj][i]["scneNum"], rotGOTrans, id);

			SceneItems.push_back(door);
			addAnimsFromJSON(door, j[obj][i], n);

			
			if (!j[obj][i]["rotation"].is_null()) {
				SceneItems.back()->setRotation(j[obj][i]["rotation"]);
			}
		}


		//ESCENARIO

		if(!j["Texture"].is_null()) n = j["Texture"];
		else n = 0;

		Entity* escenario = new Entity(app);

		if (j["w"].is_null())
			escenario->setWidth(app->getWindowWidth());
		else escenario->setWidth(j["w"]);

		if (j["h"].is_null())
			escenario->setHeight(app->getWindowHeight());
		else escenario->setHeight(j["h"]);

		width = escenario->getWidth();
		height = escenario->getHeight();

		Vector2D pos;
		if (j["x"].is_null())
			pos.setX(0);
		else
			pos.setX(j["x"]);
		
		if (j["y"].is_null())
			pos.setY(0);
		else
			pos.setY(j["y"]);

		escenario->setPosition(pos);
		x = escenario->getPosition().getX();
		y = escenario->getPosition().getY();

		if (j["PlayerPos"].is_object()) {
			posIni.setX(j["PlayerPos"]["x"]);
			posIni.setY(j["PlayerPos"]["y"]);
		}

		if (!j["AlenaActiva"].is_null()) { alenaActiva = j["AlenaActiva"]; }

		//guardamos su tamaño dependiendo de lo que ponga en el json
		if (j["PlayerTam"].is_object()) {
			playerTam.setX(j["PlayerTam"]["w"]);
			playerTam.setY(j["PlayerTam"]["h"]);
		}//si en el json no se especificaba nada, se queda con un tamaño por defecto
		else playerTam.set({ pj->defaultW, pj->defaultH });

		RenderComponent* renderEscenario = new ImageRenderer(app->getResources()->getImageTexture(Resources::ImageId(n)));
		escenario->addRenderComponent(renderEscenario);
		if (addAnimsFromJSON(escenario, j, n)) {
			delete renderEscenario;
		}
		SceneItems.push_back(escenario);
		i.close();
	}
	else {
		cout << "No existe el archivo " << name;
	}
}

Scene::~Scene()
{
	list<GameObject*>::iterator aux;
	for (aux = SceneItems.begin(); aux != SceneItems.end();) {
		delete *aux;
		aux = SceneItems.erase(aux); //aux = aux++
	}

	list<GameState*>::iterator stateIt;
	for (stateIt = SceneStates.begin(); stateIt != SceneStates.end();) {
		if(!app->getStateMachine()->checkElement(*stateIt)) delete *stateIt; //si no está dentro de la pila en el momento de borrar todo, se borra. Si está, no se borra ya que ya ha sido borrado
		stateIt = SceneStates.erase(stateIt); //aux = aux++
	}
}

void Scene::enterScene() {
	CurrentState = app->getStateMachine()->currentState();
	//Iniciamos ite, saltamos primer ite(jugador) borrarmos el resto de items copiamos nuestra lista
	it = CurrentState->getStage()->begin();
	it++; it++;
	app->getStateMachine()->currentState()->changeList();
	

	while (it != CurrentState->getStage()->end()) {//Mientras no se acaben los items
		
		it = CurrentState->getStage()->erase(it);//borramos el item
	}
	CurrentState->getStage()->insert(CurrentState->getStage()->end(), SceneItems.begin(), SceneItems.end());

	send(&Mensaje(SetZBufferPlayState)); //mensaje para recolocar el zBuffer

	for (GameObject* it : SceneItems) {
		if(it->getType() == GameObject::Collider) //casteo personalizado
			pj->setNewCollision(it);
	}

	//establecemos el tamaño de la nueva escena en el jugador (para las colisiones y el mouse)
	pj->setSceneTam(width, height, x, y);

	//limpiamos las pilas de teclas para evitar errores entre cambios de escena
	pj->cleanKeys();

	//establecemos su posicion inicial
	pj->setPosIni();

	//establecemos su tamaño
	pj->setTam();

	if (!alenaActiva) {
		pj->setActive(false);
	}
	else {
		pj->setActive(true);
	}

	//genera la matriz para el mouse
	pj->collisionListWasModified();
}

void Scene::exitScene() { //al salir de la escena, todos los objetos de stage se vuelcan en la lista de la escena para que se queden guardados (menos el jugador)
	app->getStateMachine()->currentState()->changeList();
	SceneItems.clear();
	SceneItems = *(app->getStateMachine()->currentState()->getStage()); //la lista stage es igual a todos los objetos de la escena
	pj->clearCollisions(); //fuera colisiones
	SceneItems.pop_front(); //quitamos al jugador de la escena (es global). Se puede hacer as� o con un for que se salte el primero y copie los dem�s
	SceneItems.pop_front(); //quitamos al shortcut
	pj->setVelocity(Vector2D(0.0, 0.0));
	pj->getMouseComponent()->send(&Mensaje(MouseStop));
	pj->getMouseComponent()->send(&Mensaje(CambioEscena));
	this->setPosIni(pj->getPosition()); //funciona *_*
}


void Scene::saveSceneToJson() {
	string name = "..\\Scenes\\saves\\Scene" + to_string(SceneNum);
	name += ".json";

	std::ofstream i(name); //archivo donde se va a guardar
	json j;
	for (GameObject* it : SceneItems) {
		it->saveToJson(j);	//manda a todos los objetos guardarse en dichos archivos
	}

	j["PlayerPos"]["x"] = posIni.getX();
	j["PlayerPos"]["y"] = posIni.getY();

	j["PlayerTam"]["w"] = playerTam.getX();
	j["PlayerTam"]["h"] = playerTam.getY();

	j["AlenaActiva"] = alenaActiva;

	i << std::setw(3) << j; //pretty identación para leer mejor el archivo
	i.close(); //cierra el flujo
}

GameState * Scene::PuzzleCreator(PuzzleTypes type, const json& j){
	GameState* nPuzzle = nullptr;

	switch (type)
	{
	case (Match3):
	{
		int aux = -4;
		if(!j["UnlockId"].is_null())
			aux = j["UnlockId"];

		nPuzzle = new Puzzle1State(app, app->getStateMachine()->currentState(), j["numberPuzzle"], j["numText"], aux);
		break;
	}
	case (Lights):
	{
		int aux = -4;
		if (!j["UnlockId"].is_null())
			aux = j["UnlockId"];
		
		nPuzzle = new LightsOut(app, j["numCas"], j["dificultad"], aux);
		break;
	}

	case (Password): 
	{
		int aux = -4;
		if (!j["UnlockId"].is_null())
			aux = j["UnlockId"];

		int fondo = -1;
		if (!j["TexturaFondo"].is_null())
			fondo = j["TexturaFondo"];


		nPuzzle = new PasswordState(app, j["password"], aux, fondo);
		break;
	}
	default:
		break;
	}
	return nPuzzle;
}

bool Scene::addAnimsFromJSON(Entity* obj, json& j, const int numText){

	if (!j["animation"].is_null()) {
		if (j["animation"]) {
			Entity* col = obj;
			col->setAnimated(true);
			col->delEveryRenderComponent();
			for (unsigned int k = 0; k < j["Anims"].size(); k++) {
				col->addAnim("Anim" + to_string(k), j["Anims"][k], j["loop"], -1, j["vel"]);
			}

			col->addRenderComponent(new AnimationRenderer(
				app->getResources()->getImageTexture(Resources::ImageId(numText)), col->getAnimations(),
				 j["numColsFrame"], j["numFilsFrame"], j["widthFrame"], j["heightFrame"]));
			return true;
		}
	}
	return false;
}
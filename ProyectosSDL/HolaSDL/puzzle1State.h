#pragma once
#include "checkML.h"
#include "GameState.h"
#include "CasillaPuzzle1.h"
#include "MovementComponent.h"
#include "ImageRenderer.h"
#include "AnimationRenderer.h"
#include "Boton.h"

class Puzzle1State :
	public GameState
{
private:
	//--------------------ENUM DE LOS TIPOS DE CASILLAS -------------------------------
	enum TipoCasilla {CasillaVacia, CasillaLlena};
	GameState* previousState;

	//----------------------VECTORES DE OBJETOS, OBJETOS Y VECTOR AUXILIAR-------------------------
	vector<vector<CasillaPuzzle1*>> matriz;
	vector<Boton*> botones;
	vector<RenderComponent*> botonesAnim;
	vector<vector<int>> matrizOriginal;
	Boton* resetButton;
	RenderComponent* HUD;
	RenderComponent* reiniciar;
	RenderComponent* imagenCopia;
	Entity* copia = new Entity(app);
	Entity* puzzleHud = new Entity(app);

	//--------------------------VARIABLES AUXILIARES-----------------------------------------
	const int numCas = 5;
	int numRestantes = 0;
	const int espaciado = 117.0;
	pair<const double, const double> relacion = { app->getWindowWidth() / 800.0 , app->getWindowHeight() / 600.0 };
	const double topD = relacion.first*(espaciado*4 + 240 + espaciado/2), topI = relacion.first*(240-espaciado/2), topA = relacion.second*(53 - (espaciado-10) / 2), topAB = relacion.second*((espaciado-5)*4 + 53 + espaciado / 2);
	double auxI, auxD, auxA, auxAB;
	bool mover = false;
	int currentFil, currentCol;

	//--------------------------------------METODOS DE LOS BOTONES---------------------
	static void usar(GameState* state, int fil, int col);
	static void resetFunction(GameState* state) { Puzzle1State* aux = dynamic_cast<Puzzle1State*>(state); if (aux != nullptr) { if (!aux->isMoving()) { aux->restart(); } } }

	//----------------------------------------METODOS PRIVADOS ----------------------
	bool isMoving() { return this->mover; };
	void checkLine(int line, bool Vert);
	void restart();
	void deleteMatrix();
	void tresUnidos();
	void mueveMatriz();
	void eligeTipoCasilla(int tipoCas, string name, CasillaPuzzle1*& cas);
	void reestableFC(int f, int c) {
		if (f != -1) {
			for (int i = 0; i < numCas - 1; i++) swap(matriz[f][0], matriz[f][i + 1]);
			for (int i = 0; i < numCas; i++) matriz[f][i]->setPosition(Vector2D((int)topI + ((int)(relacion.first*espaciado) / 2) + (int)(i*espaciado*relacion.first), matriz[f][i]->getPosition().getY()));
		}
		else {
			for (int i = numCas - 1; i > 0; i--) swap(matriz[4][c], matriz[i - 1][c]);
			for (int i = 0; i < numCas; i++) {
				int j = (int)topAB - ((int)(relacion.second*(espaciado)) / 2) - (int)(i*(espaciado-5)*relacion.second);
				matriz[numCas - 1 - i][c]->setPosition(Vector2D(matriz[numCas - 1 - i][c]->getPosition().getX(), j));
			}
		}
	}
	void destroy();
public:
	Puzzle1State() {};
	Puzzle1State(SDLApp* game,  GameState* previousState);
	virtual ~Puzzle1State() { destroy(); stage.clear(); };

	//----------------------------HE, RENDER, UPDATE--------------------------
	virtual void handleEvent(SDL_Event& event) { 
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_p) {
				app->getStateMachine()->popState();
			}
		}
		else GameState::handleEvent(event);
	}
	virtual void render() { GameState::render(); }
	virtual void update();

	GameState* getPreviousState() { return previousState; };
	void loadFromJson(int numeroPuzzle);
};
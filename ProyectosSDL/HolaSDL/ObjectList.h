#pragma once
#include <SDL.h>
#include "CasillaInventario.h"
#include "Texture.h"
#include <list>
#include "ComponenteClickeable.h"
#include "ImageRenderer.h"

class ObjectList
{
private:
	list <CasillaInventario*> objetos; //lista de casillas (almacenan objetos)
	list <CasillaInventario*>::iterator it; //iterador de la lista
	SDLApp* app;
public:
	ObjectList();
	~ObjectList() { for (it = objetos.begin(); it != objetos.end();) { it = objetos.erase(it); } std::cout << "delete"; }; //delete
	ObjectList(SDLApp* app) : app(app) {};
	void addItem(Texture* txt, string tag, string descripcion); //a�adir un objeto
	void deleteItem(string tag) {
		bool borrado = false;
		it = objetos.begin();
		while (it != objetos.end() && !borrado) {
			if ((*it)->getTag() == tag && !borrado) {
				borrado = true;
				delete(*it);
				it = objetos.erase(it);
			}
			it++;
		}
	}
	//void deleteItem(); podemos hacerlo por tags (?)
	CasillaInventario* getItem(int num) {
		int i = 0;
		for (it = objetos.begin(); it != objetos.end(); it++) {
			if (i == num) 
				return *it;
			i++;
		}
	}
	int getLength() { return (int)objetos.size(); }
	void pushObjects(list <GameObject*>& stage) { for (it = objetos.begin(); it != objetos.end(); it++) { stage.push_back(*(it)); } } //recibe la lista de stages del inventario
	//y pushea esos objetos dentro para que realicen sus funciones b�sicas
};
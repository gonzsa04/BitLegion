#pragma once
#include "Puzzle.h"
class PasswordState :
	public Puzzle
{
private:
	const string password_; //contrasenya a averiguar
	string userPass_;
	bool checkPassword();
	//Texture text;
	Font* f;
public:
	PasswordState();
	PasswordState(SDLApp* app, string password = "hanzomain", int id = -4);
	virtual ~PasswordState();
	virtual void handleEvent(SDL_Event &e);
	virtual void render();
	virtual void win();
};


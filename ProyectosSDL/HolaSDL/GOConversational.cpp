#include "GOConversational.h"
#include "PlayState.h"

GOConversational::GOConversational()
{
}


GOConversational::~GOConversational()
{
	delete convo;
}

void GOConversational::act() {
	if (!inconversation) {

		PlayState* aux = dynamic_cast<PlayState*>(app->getStateMachine()->currentState()); 

		if (aux != nullptr) {
			inconversation = true;
			aux->setEnConversacion(true);
			if (firstTime_) {
				convo->addObserver(app->getStateMachine()->currentState());
				convo->addObserver(this);
				this->addObserver(aux->getMainPj());
				firstTime_ = false;
			}
			this->send(&Mensaje(Stop));
			convo->setActive(true);
			app->getStateMachine()->currentState()->getStage()->push_front(convo);
			app->getStateMachine()->currentState()->changeList();
		}
	}

}

void GOConversational::receive(Mensaje* msg) {
	if (repeat_) {
		switch (msg->id_) {
		default:
			inconversation = false;
		}
	}

}
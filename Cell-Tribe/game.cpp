
#pragma region Header

#include "EventController.h"
#include "EvolutionController.h"
#include "MapController.h"
#include "Entity/EntityPlayerTribe.h"

#pragma endregion


int main() {
	//SL::StartThread(WINDOW_WIDTH, WINDOW_HEIGHT);

	Entity* player1, * player2;
	MapController* mapController;
	EventController* eventController;
	EvolutionController* evolutionController1;
	EvolutionController* evolutionController2;
	evolutionController1= new EvolutionController("evolution.json");
	evolutionController2= new EvolutionController("evolution.json");

	
	player1 = new EntityPlayerTribe("Player1", evolutionController1);
	player2 = new EntityPlayerTribe("Player2", evolutionController2);


	mapController = new MapController(MAP_BEGIN_X, MAP_BEGIN_Y, MAP_END_X, MAP_END_Y);

	player1->respawn(mapController);
	player2->respawn(mapController);

	eventController = new EventController(mapController);

	while (true) {
		mapController->update();
		eventController->update();
	}
	printf("player1 : %d, player2 : %d", player1->getDeath(), player2->getDeath());

	//EntityPlayerTribe a;


	return 0;
}

/*
���Ƶ�λ����Ⱥ
	----���壺����
	----���ԣ�
		----����
		----ǿ�ȣ��������壩
		----����
	----��ʾ������Ч��
	----��Ϊ��
		----�ƶ�
		----����
		----��ʳ������
			----��ȡ�������������
		----��ֳ��
			----�������������Ӹ�������
			----����ͻ�䣺
				----�������죺
				----�������죺���Ļ�������˻�
	----������
		----���Ļ���������н���
��ͼ��
	----��ʾ����Ⱥ
	----һЩ��Ʒ
		----ʳ��

*/
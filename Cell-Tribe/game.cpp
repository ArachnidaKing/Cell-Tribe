
#pragma region Header


#include "MapController.h"
#include "./Entity/EntityPlayerTribe.h"

#pragma endregion

Entity* player1, * player2;
MapController* mapc;

int main() {
	player1 = new EntityPlayerTribe();
	player2 = new EntityPlayerTribe();
	mapc = new MapController();
	while (true) {
		int flag= 0;
		std::cout << "Player1:" << std::endl;
		if (player1->run() == TRIBE_DEAD || player2->run() == TRIBE_DEAD) {
			break;
		}
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
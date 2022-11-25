#include "Game.h"
#include "TitleScene.h"
#include "GameScene.h"

int main(int argc, char** argv)
{
	if (!GAMEINSTANCE.Initialize(1280, 768, true))
	{
		return -1;
	}
	GAMEINSTANCE.SetFirstScene(new TitleScene);
	GAMEINSTANCE.GetRenderer()->SetWindowTitle("Game");
	GAMEINSTANCE.Run();
	GAMEINSTANCE.Shutdown();
	return 0;
}
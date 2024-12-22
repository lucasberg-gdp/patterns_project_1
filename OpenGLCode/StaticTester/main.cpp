

int main()
{


	AudioManager* audioManager = new AudioManager();
	audioManager->Initialize();
	audioManager->LoadSound("audio/test.wav");
	audioManager->PlaySound();

	while (true)
	{

	}
}
#include "W32W/Win32Window.hpp"

W32W::Win32Window *pWindow;

void onEvents(W32W::Event &e);
bool onKeyPress(W32W::KeyPressedEvent &e);
bool onWindowClose(W32W::WindowCloseEvent &e);

int main() {
	pWindow = new W32W::Win32Window("Win32Window", 800, 600);
	pWindow->setEventCallback(onEvents);
	pWindow->setIcon("resources/icons/Malcret.ico");
	//pWindow->setFullscreen();
	pWindow->setVisible();

	std::string pTitle = pWindow->getTitle();

	while (!pWindow->shouldClose()) {
		pWindow->pollEvents();

		double deltaTime = W32W::getDeltaTime();
		pWindow->setTitle(pTitle + " | " + std::to_string(W32W::getFPS(deltaTime)) + " FPS");

		pWindow->swapBuffers();
	}

	int exitCode = pWindow->getExitCode();
	delete pWindow;

	return exitCode;
}

void onEvents(W32W::Event &e) {
	std::cout << "Event: " << e << std::endl;
	W32W::Event::dispatch<W32W::KeyPressedEvent>(e, onKeyPress);
	W32W::Event::dispatch<W32W::WindowCloseEvent>(e, onWindowClose);
}

bool onKeyPress(W32W::KeyPressedEvent &e) {
	switch (e.getKeyCode()) {
		case VK_ESCAPE:
			pWindow->close();
			break;
		case VK_F11:
			pWindow->setFullscreen(!pWindow->isFullscreen());
			break;
		default:
			break;
	}
	return true;
}

bool onWindowClose(W32W::WindowCloseEvent &e) {
	pWindow->close(0);
	return true;
}
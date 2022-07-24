#include "MWW/Win32Window.hpp"

MWW::Win32Window *pWindow;

void onEvents(MWW::Event &e);
bool onKeyPress(MWW::KeyPressEvent &e);
bool onWindowClose(MWW::WindowCloseEvent &e);

int main() {
	pWindow = new MWW::Win32Window("Win32Window", 800, 600);
	pWindow->setEventCallback(onEvents);
	pWindow->setIcon("resources/icons/Malcret.ico");
	pWindow->setVisible();

	std::string pTitle = pWindow->getTitle();

	while (!pWindow->shouldClose()) {
		pWindow->pollEvents();

		if (pWindow->keyPressed(MWW::KEY::RETURN)) {
			std::cout << "Return is pressed\n";
		}

		double deltaTime = MWW::getDeltaTime();
		pWindow->setTitle(pTitle + " | " + std::to_string(MWW::getFPS(deltaTime)) + " FPS");

		pWindow->swapBuffers();
	}

	int exitCode = pWindow->getExitCode();
	delete pWindow;

	return exitCode;
}

void onEvents(MWW::Event &e) {
	std::cout << "Event: " << e << std::endl;
	MWW::Event::dispatch<MWW::KeyPressEvent>(e, onKeyPress);
	MWW::Event::dispatch<MWW::WindowCloseEvent>(e, onWindowClose);
}

bool onKeyPress(MWW::KeyPressEvent &e) {
	switch (e.getKey()) {
		case MWW::KEY::ESCAPE:
			pWindow->close();
			break;
		case MWW::KEY::F11:
			pWindow->setFullscreen(!pWindow->isFullscreen());
			break;
		default:
			break;
	}

	return true;
}

bool onWindowClose(MWW::WindowCloseEvent &e) {
	pWindow->close();
	return true;
}
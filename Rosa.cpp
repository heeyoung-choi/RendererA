#include "Rosa.h"

using namespace RosaEngine;

int Application::Run()
{
	OnStart();
	while (true)
	{
		OnStart();
		// process all messages pending, but to not block for new messages
		if (const auto ecode = pWindow->ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		// execute the game logic
		//const auto dt = timer.Mark() * speed_factor;
		OnUpdate(0.016f);
		pRenderer->RenderFrame();
		pUIManager->DrawUI(pRenderer->GetBackBufferRT(),
			pRenderer->GetBrushUIMain(),
			pRenderer->GetDWriteFactory(),
			pRenderer->GetTextFormat()
            );
        pRenderer->Present();
	}
}

void Application::OnStart()
{
}

void Application::OnUpdate(float dt)
{
}

void Application::OnQuit()
{
}

Application::Application(int _sWidth, int _sHeight)
	: screenWidth(_sWidth), screenHeight(_sHeight)
{
	pWindow = std::make_unique<MyWindow>(screenWidth, screenHeight);
	pRenderer = std::make_unique<Renderer>();
	pUIManager = std::make_unique<UIManager>();
	pMouse = std::make_unique<Mouse>();

    pWindow->SetMouse(pMouse.get());
	pMouse->SetUIManager(pUIManager.get());

	pRenderer->InitD3D(pWindow->GetHwnd(), screenHeight, screenWidth);
	//InitUI

    wchar_t const* m1 = L"Choose file";
    auto mybutton = std::make_unique<UIButton>(
        RectD{ 300.0f, 50.0f, 500.0f, 150.0f },
        ColorD{ 0.8f, 0.2f, 0.2f, 1.0f },
        ColorD{ 1.0f, 1.0f, 1.0f, 1.0f },
        10.0f,
        10.0f,
        m1,
        RectD{ 300.0f, 50.0f, 500.0f, 150.0f });
    RectD labelArea = RectD{ 100.0f, 50.0f, 300.0f, 150.0f };
    auto debugLabel = std::make_unique<UIButton>(
        labelArea,
        ColorD{ 0.2f, 0.2f, 0.8f, 1.0f },
        ColorD{ 1.0f, 1.0f, 1.0f, 1.0f },
        10.0f,
        10.0f,
        L"Debug Label",
        labelArea);
    UIButton* debugLabelPtr = debugLabel.get();

    UIButton* buttonPtr = mybutton.get();
    mybutton->SetLeftMouseDownAction([debugLabelPtr, this]() {
        std::wstring filePath = OpenFileExplorer(pWindow->GetHwnd());
        std::string path = Helper::wstring_to_string(filePath);
        std::replace(path.begin(), path.end(), '\\', '/');
        std::filesystem::path pathObj(path);
        std::string fileName = pathObj.filename().string();
        std::string searchPath = pathObj.parent_path().string();
        std::string extension = pathObj.extension().string();
        if (!filePath.empty())
        {
            if (extension == ".obj")
            {
                HRESULT hr = pRenderer->InitGraphics(path, searchPath);
                if (hr == S_OK)
                {
                    debugLabelPtr->SetText(L"Loaded: " + Helper::string_to_wstring(fileName));
                }
                else
                {
                    debugLabelPtr->SetText(L"Failed to load model");

                }
            }
            else
            {
                debugLabelPtr->SetText(L"Not an .obj file");
            }


        }
        else
        {
            debugLabelPtr->SetText(L"Filepath is empty");
        }
        });
    pUIManager->CreateButton(std::move(mybutton));
    pUIManager->CreateButton(std::move(debugLabel));
}

#include "ImGUIManager.h"

ImGUIManager::ImGUIManager(Window* inWindow)
{
	window = inWindow;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(window->GetWindow(), window->GetContext());
	ImGui_ImplOpenGL3_Init("#version 130");
}

ImGUIManager::~ImGUIManager()
{
	// ImGuiCleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ImGUIManager::Run()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(window->GetWindow());
	ImGui::NewFrame();

	ImGui::Begin("Test");
	ImGui::Text("Window");
	ImGui::End();

	ImGui::Render();
}
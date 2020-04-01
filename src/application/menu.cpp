#include"menu.h"
#include "window.h"

Menu::Menu(const glm::ivec2& position, const glm::ivec2& size): position(position), size(size){

	attach([](){
		ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.60f);
		if (ImGui::CollapsingHeader("Camera Parameters", ImGuiTreeNodeFlags_None)) { }
	});
}

void Menu::attach(std::function<void()> func){
	functions.push_back(std::move(func));
}

void Menu::draw() const{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(position.x, position.y), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(size.x, size.y), ImGuiCond_FirstUseEver);

	ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_MenuBar);

	for (auto& func : functions) func();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

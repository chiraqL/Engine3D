#pragma once

#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_widgets.h"
#include "Renderer.h"

class Panel
{
private:
	Renderer* r = nullptr;
	ImGuiIO* pio;
public:
	Panel(GLFWwindow* window, Renderer* r) :r(r) { init(window); };
	void init(GLFWwindow* window)
	{
		//Setup IMGUI context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); pio = &io; (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls

		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f };
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.38f, 0.38f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.28f, 0.28f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

	}
	void pre_render() {

		// Calculate Fps
		glb.tElapsed = glfwGetTime() - glb.tDelta;
		++glb.framecount;
		if (glb.tElapsed > 1.0) {	// show fps each second
			glb.fps = (float) glb.framecount / glb.tElapsed;
			glb.framecount = 0;
			glb.tDelta = glfwGetTime();
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Scene", (bool*)0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |ImGuiWindowFlags_NoMove);
		if (ImGui::IsWindowHovered())
		{
			if (pio->KeyCtrl)
			{
				r->getCamera()->eye = r->getCamera()->eye + r->getCamera()->forwardDir * 0.5f * pio->MouseWheel;
				glb.zio = pio->MouseWheel ? math::length(r->getCamera()->target - r->getCamera()->eye) : glb.zio;
			}
			if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
			{
				ImVec2 mleftDrag = ImGui::GetMouseDragDelta();
				if (pio->KeyShift)
				{
					r->getModel()->position.x = r->getModel()->position.x + 0.0005f * mleftDrag.x;
					r->getModel()->position.y = r->getModel()->position.y - 0.0005f * mleftDrag.y;
				}
				else {
					glb.view.reset();
					r->getModel()->rotAngle.y -= 0.005f * mleftDrag.x;
					r->getModel()->rotAngle.x += 0.005f * mleftDrag.y;
					if (r->getModel()->rotAngle.y >= 360)
						r->getModel()->rotAngle.y = -360;
					if (r->getModel()->rotAngle.x >= 360)
						r->getModel()->rotAngle.x = -360;
				}
			}
		}
		glb.draw.invBgCol = WHITE - glb.draw.bgCol;
		const ImVec4 invBgCol = { glb.draw.invBgCol.r, glb.draw.invBgCol.g, glb.draw.invBgCol.b, 1.0f };
		ImGui::PushStyleColor(ImGuiCol_Text, invBgCol);
		ImGui::Text(("FPS : " + std::to_string(glb.fps).substr(0,5)).c_str());
		ImGui::PopStyleColor();
		ImGui::End();

		//properties
		ImGui::Begin("Control", (bool*)0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		if (ImGui::CollapsingHeader("Properties", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Background Color :");
			ImGui::SameLine();
			ImGui::ColorEdit3("bgcolor", (float*)&glb.draw.bgCol, ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
			int tCount = 0;
			for (int i = 0; i < r->getModel()->meshes.size(); i++)
				tCount += r->getModel()->meshes[i].tris.size();
			ImGui::Text(("Triangles : " + std::to_string(tCount)).c_str());
			ImGui::Separator();
			ImGui::Checkbox("Wireframe", &glb.controls.wireFrame);
		}
		ImGui::Separator();
		//transformation
		if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Separator();

			ImGui::Text("Translate");
			ImGui::Separator();
			imgui::draw_vec3_widget("t", r->getModel()->position, 0, 0, 80.0f);
			ImGui::Separator();

			ImGui::Text("Rotate");
			ImGui::SameLine();
			ImGui::Checkbox(" ", &glb.controls.rotateModel);
			ImGui::Separator();
			imgui::draw_vec3_widget("r", r->getModel()->rotAngle, -360, 360, 80.0f);
			if (glb.controls.rotateModel)
			{
				r->getModel()->rotAngle.y += 0.3f * glb.tDelta;
				if (r->getModel()->rotAngle.y >= 360)
					r->getModel()->rotAngle.y = -360;
			}
			ImGui::Separator();

			ImGui::Text("Scale");
			ImGui::Separator();
			imgui::draw_vec3_widget("s", r->getModel()->scaleFactor, 0, 0, 80.0f);
		}
		ImGui::Separator();
		if (ImGui::CollapsingHeader("View", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::RadioButton("Default",glb.view.isDefault() == 1)) glb.view.reset();
			if (ImGui::RadioButton("Front", glb.view.f == 1)) { glb.view.reset(); glb.view.f = true; }
			if (ImGui::RadioButton("Top", glb.view.t == 1)) { glb.view.reset(); glb.view.t = true; }
			if (ImGui::RadioButton("Left", glb.view.l == 1)) { glb.view.reset(); glb.view.l = true; }
			if (ImGui::RadioButton("Right", glb.view.r == 1)) { glb.view.reset(); glb.view.r = true; }
			ImGui::Text("Camera Position");
			ImGui::Separator();
			imgui::draw_vec3_widget("camPosition", r->getCamera()->eye, 0, 0, 80.0f);
			ImGui::Separator();
			ImGui::Text(("Left/Right  :" + std::to_string(glb.camYaw)).c_str());
			ImGui::Text(("Zoom In/Out :" + std::to_string(glb.zio).substr(0,3)).c_str());
		}
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Projection", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::RadioButton("Perspective", glb.controls.projectP == 1)) glb.controls.projectP = 1;
			if (ImGui::RadioButton("Orthogonal", glb.controls.projectP == 0)) glb.controls.projectP = 0;
		}
		ImGui::Separator();
		if (ImGui::CollapsingHeader("Shading", ImGuiTreeNodeFlags_DefaultOpen))
		{
			if (ImGui::RadioButton("Flat", glb.shading.type == FLAT)) glb.shading.type = FLAT;
			if (ImGui::RadioButton("Gouraud", glb.shading.type == GOURAUD)) glb.shading.type = GOURAUD;
		}
		if (ImGui::CollapsingHeader("Sun (Model Top View)", ImGuiTreeNodeFlags_DefaultOpen))
		{
			imgui::SunMovementWidget("movement", r->getLight()->sun->tfPosition, r->getLight()->sun->translateSph, r->getLight()->sun->isVisible);
		}
		ImGui::End();
	}
	void post_render() {
		//Render imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	void close() {
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
};


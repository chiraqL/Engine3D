#pragma once

#include "../Math.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace imgui
{
	static void draw_vec3_widget(const std::string& label, vec3& values, float min, float max, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];


		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 15.0f, lineHeight };

		ImGui::PushFont(boldFont);
		ImGui::Button("X", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(("##X" + label).c_str(), &values.x, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Y", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(("##Y" + label).c_str(), &values.y, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		ImGui::Button("Z", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(("##Z" + label).c_str(), &values.z, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();
	}

	static void draw_vec2_widget(const std::string& label, vec3& values, float min, float max, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 15.0f, lineHeight };

		ImGui::Dummy(ImVec2(130.0f, 20.0f));
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		ImGui::Button("Phi (Y)", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(("##X" + label).c_str(), &values.y, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();

		ImGui::Dummy(ImVec2(130.0f, 20.0f));
		ImGui::SameLine();
		ImGui::PushFont(boldFont);
		ImGui::Button("Theta(Z)", buttonSize);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::DragFloat(("##Y" + label).c_str(), &values.z, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PopStyleVar();
	}

	static void SunMovementWidget(const char* label, vec4& tfPos, vec3& sphPos, bool& sunIsVisible)
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();

		float radius_outer = 50.0f;
		float radius_inner = radius_outer * 0.10f;
		ImVec2 pos = ImGui::GetCursorScreenPos();
		ImVec2 center = ImVec2(pos.x + radius_outer, pos.y + radius_outer);
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		vec3 lineEnd = sphPos;//glb.view.isDefault() ? math::cartToSph(tfPos) : sphPos;
		float r = radius_outer * lineEnd.y / 90;
		bool flag = 0;
		sunIsVisible = true;
		if (lineEnd.y > 270.0f)		// phi
		{
			r = -4 * radius_outer + r;
		}
		else if (lineEnd.y > 90.0f)
		{
			sunIsVisible = false;
			flag = 1;
			r = 2 * radius_outer - r;
		}
		else if (lineEnd.y < -270.0f)
		{
			r = 4 * radius_outer + r;
		}
		else if (lineEnd.y < -90.0f)
		{
			sunIsVisible = false;
			flag = 1;
			r = -2 * radius_outer - r;
		}
		ImVec2 end = ImVec2(center.x + r * sinf(math::radians(lineEnd.z)), center.y + r * cosf(math::radians(lineEnd.z)));
		ImVec4 color;
		draw_list->AddCircleFilled(center, radius_outer, ImGui::GetColorU32(ImGuiCol_FrameBg), 16);
		if (!flag)
		{
			color = ImVec4(0.3f, 0.3f, 0.1f, 1.0f);
			ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, color);
			draw_list->AddLine(center, end, ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 2.0f);
			ImGui::PopStyleColor();

		}draw_list->AddCircleFilled(center, radius_inner, ImGui::GetColorU32(ImGuiCol_SliderGrabActive), 16);
		color = flag? ImVec4(0.5f, 0.5f, 0.0f, 1.0f) :ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, color);
		draw_list->AddCircleFilled(end, radius_inner, ImGui::GetColorU32(ImGuiCol_FrameBgActive), 16);
		ImGui::PopStyleColor();

		ImGui::Dummy(ImVec2(100, 20));
		draw_vec2_widget("sun", sphPos, -360, 360);
	}
}
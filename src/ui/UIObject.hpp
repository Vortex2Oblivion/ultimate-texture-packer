#pragma once
#include <memory>
#include <string>
#include <vector>

#include "imgui.h"

#ifndef UTP_BEGIN_END
#define UTP_BEGIN_END \
protected: \
	bool begin() override; \
	void end() override; \

#endif

namespace utp::ui {
	class UIObject {
	public:
		explicit UIObject(ImVec2 position, const std::string &name, ImGuiWindowFlags flags = ImGuiWindowFlags_None);
		virtual ~UIObject();

		bool open = true;
		ImVec2 position = {};

		std::vector<std::shared_ptr<UIObject>> children = {};

		std::string name;
		ImGuiWindowFlags flags = ImGuiWindowFlags_None;

		virtual void update(float delta);
		virtual void draw(float offsetX = 0.0f, float offsetY = 0.0f);

	protected:
		virtual bool begin();
		virtual void end();
	};
} // namespace utp::ui

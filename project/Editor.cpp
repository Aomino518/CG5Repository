#include "Editor.h"
#include "Sprite.h"
#include "Entity3D.h"
#include "EmitterManager.h"
#include "ParticleManager.h"
#include "Logger.h"
#include <nlohmann/json.hpp>
#ifdef USE_IMGUI
#include "externals/imgui/imgui.h"
#endif

Editor* Editor::GetInstance()
{
    static Editor instance;
    return &instance;
}

void Editor::Draw()
{
#ifdef USE_IMGUI
    DrawHierarchy();
    DrawInspector();
#endif
}

void Editor::RegisterSprite(const std::string& name, Sprite* sprite)
{
	sprites_[name] = sprite;
}

void Editor::RegisterModel(const std::string& name, Entity3D* model)
{
	models_[name] = model;
}

void Editor::RegisterParticle(const std::string& name)
{
	particles_[name] = EmitterManager::GetInstance()->GetEmitter(name);
}

void Editor::SaveSceneJson(const std::string& path) const
{
    json root;
    root["sprites"] = json::object();
    root["models"] = json::object();
    root["particles"] = json::object();
    root["spotLight"] = json::object();
    root["pointLight"] = json::object();

    for (const auto& [name, sprite] : sprites_) {
        if (!sprite) {
            continue;
        }
        root["sprites"][name] = sprite->SaveToJson();
    }

    for (const auto& [name, model] : models_) {
        if (!model) {
            continue;
        }

        root["models"][name] = model->SaveToJson();
    }

    for (const auto& [name, particle] : particles_) {
        if (!particle) {
            continue;
        }
        root["particles"][name] = particle->SaveToJson();
    }

    root["lights"] = LightManager::GetInstance()->SaveToJson();

    std::ofstream ofs(path);
    if (!ofs.is_open()) {
        Logger::Write(Logger::LogLevel::Error, "Failed to open file" + path);
        return;
    }

    ofs << std::setw(4) << root << std::endl;
}

void Editor::LoadSceneJson(const std::string& path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open()) {
        Logger::Write(Logger::LogLevel::Error, "Failed to open file" + path);
        return;
    }

    json root;
    ifs >> root;
    if (root.contains("sprites")) {
        for (auto& [name, data] : root["sprites"].items()) {
            auto it = sprites_.find(name);
            if (it != sprites_.end() && it->second) {
                it->second->LoadFromJson(data);
            }
        }
    }

    if (root.contains("models")) {
        for (auto& [name, data] : root["models"].items()) {
            auto it = models_.find(name);
            if (it != models_.end() && it->second) {
                it->second->LoadFromJson(data);
            }
        }
    }

    if (root.contains("particles")) {
        for (auto& [name, data] : root["particles"].items()) {
            auto it = particles_.find(name);
            if (it != particles_.end() && it->second) {
                it->second->LoadFromJson(data);
            }
        }
    }

    if (root.contains("lights")) {
        LightManager::GetInstance()->LoadFromJson(root["lights"]);
    }

}

void Editor::Clear()
{
    sprites_.clear();
    models_.clear();
    particles_.clear();
    selection_ = {};
}

void Editor::DrawHierarchy()
{
#ifdef USE_IMGUI
    ImGui::Begin("Hierarchy");

    // =========================
    // Sprite
    // =========================
    if (ImGui::CollapsingHeader("Sprite", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (auto& [name, sprite] : sprites_) {
            bool selected = (selection_.category == InspectorCategory::Sprite && selection_.name == name);
            if (ImGui::Selectable(name.c_str(), selected)) {
                selection_.category = InspectorCategory::Sprite;
                selection_.name = name;
            }
        }
    }

    // =========================
    // Model
    // =========================
    if (ImGui::CollapsingHeader("Model", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (auto& [name, model] : models_) {
            bool selected = (selection_.category == InspectorCategory::Model && selection_.name == name);
            if (ImGui::Selectable(name.c_str(), selected)) {
                selection_.category = InspectorCategory::Model;
                selection_.name = name;
            }
        }
    }

    // =========================
    // Particle
    // =========================
    if (ImGui::CollapsingHeader("Particle", ImGuiTreeNodeFlags_DefaultOpen)) {
        auto& emitters = EmitterManager::GetInstance()->GetEmittersAndNames();

        for (auto& [name, emitter] : emitters) {

            bool selected = (selection_.category == InspectorCategory::Particle && selection_.name == name);

            if (ImGui::Selectable(name.c_str(), selected)) {
                selection_.category = InspectorCategory::Particle;
                selection_.name = name;
            }
        }
    }

    ImGui::End();
#endif
}

void Editor::DrawInspector()
{
#ifdef USE_IMGUI
    ImGui::Begin("Inspector");

    switch (selection_.category) {

    case InspectorCategory::Sprite:
    {
        auto it = sprites_.find(selection_.name);
        if (it != sprites_.end()) {
            ImGui::Text("Name: %s", it->first.c_str());
            it->second->DrawImGui();
        }
        break;
    }

    case InspectorCategory::Model:
    {
        auto it = models_.find(selection_.name);
        if (it != models_.end()) {
            ImGui::Text("Name: %s", it->first.c_str());
            it->second->DrawImGui();
        }
        break;
    }

    case InspectorCategory::Particle:
    {
        auto* emitter = EmitterManager::GetInstance()->GetEmitter(selection_.name);

        if (emitter) {
            emitter->DrawImGui();
        }

        ParticleManager::GetInstance()->DrawParticleGroupImGui(selection_.name);
        break;
    }

    default:
        ImGui::Text("Nothing selected.");
        break;
    }

    ImGui::End();
#endif
}

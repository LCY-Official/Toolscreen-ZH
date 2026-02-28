if (ImGui::BeginTabItem(trc("tabs.supporters"))) {
    g_currentlyEditingMirror = "";
    g_imageDragMode.store(false);
    g_windowOverlayDragMode.store(false);

    ImGui::TextWrapped(trc("supporters.thanks"));
    ImGui::TextWrapped(trc("supporters.donate"));
    ImGui::Spacing();

    const bool loaded = g_supportersLoaded.load(std::memory_order_acquire);
    const bool failedBefore = g_supportersFetchEverFailed.load(std::memory_order_acquire);

    if (!loaded) {
        if (failedBefore) {
            ImGui::TextWrapped(trc("supporters.unable_to_load"));
        } else {
            ImGui::TextWrapped(trc("supporters.loading"));
        }
    } else {
        std::shared_lock<std::shared_mutex> readLock(g_supportersMutex);
        if (g_supporterRoles.empty()) {
            ImGui::TextDisabled(trc("supporters.no_supporters"));
        }

        for (const auto& role : g_supporterRoles) {
            const ImVec4 roleColor(role.color.r, role.color.g, role.color.b, role.color.a);
            ImGui::PushStyleColor(ImGuiCol_Text, roleColor);
            ImGui::SeparatorText(role.name.c_str());
            ImGui::PopStyleColor();

            if (role.members.empty()) {
                ImGui::TextDisabled(trc("supporters.no_members"));
            } else {
                for (const auto& member : role.members) {
                    ImGui::BulletText("%s", member.c_str());
                }
            }

            ImGui::Spacing();
        }
    }

    ImGui::EndTabItem();
}

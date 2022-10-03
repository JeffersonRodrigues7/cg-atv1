#include "window.hpp"

void Window::onCreate() {}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Calculadora Básica" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Calculadora Básica", nullptr, flags);
    doSpacing(1);

    // Texto que mostra o resultado da conta
    {
      std::string result = "teste";

      // Center text
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(result.c_str()).x) / 2);
      ImGui::Text("%s", result.c_str());
      doSpacing(1);
    }

    // ImGui::ShowDemoWindow;

    doSpacing(2);

    // Tabela onde ficam as ferramentas da calculadora
    if (ImGui::BeginTable("Game board", 2, 1)) {
      ImGui::TableNextRow();
      {
        ImGui::TableSetColumnIndex(0);

        ImGui::Indent(50.f);

        ImGui::Text("Primeiro número");
        ImGui::SliderFloat("##01", &slide01, 0.0f, 100.0f, "%.3f");

        doSpacing(3);

        ImGui::Text("Segundo número");
        ImGui::SliderFloat("##02", &slide02, 0.0f, 100.0f, "%.3f");

        doSpacing(3);

        ImGui::Indent(50.f);
        ImGui::Button("Calcular##01");
      }

      {
        ImGui::TableSetColumnIndex(1);

        ImGui::Indent(50.f);

        ImGui::Text("Primeiro número");
        ImGui::SliderFloat("##03", &slide01, 0.0f, 100.0f, "%.3f");

        doSpacing(3);

        ImGui::Text("Segundo número");
        ImGui::SliderFloat("##04", &slide02, 0.0f, 100.0f, "%.3f");

        doSpacing(3);

        ImGui::Indent(50.f);
        ImGui::Button("Calcular##02");
      }

      doSpacing(1);
      ImGui::EndTable();
    }

    ImGui::End();
  }
}

void Window::doSpacing(int spaces) {
  for (auto const i : iter::range(spaces)) {
    ImGui::Spacing();
  }
}
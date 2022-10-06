# Calculadora Básica

### Jefferson Leite Rodrigues Dantas, 11076316

---

### O que é

A aplicação consiste em uma calculadora básica que faz as operações de soma, subtração, multiplicação e divisão com 2 números.
O usuário tem duas opções para inserir os números. A primeira forma seria através de sliders que vão de 0 a 100, e a segunda é utilizando inputs onde pode-se digitar o valor desejado.
As operações serão selecionadas a partir de um slider.

---

### Implementação

#### main.cpp

Aqui foi definido o ponto de entrada da aplicação, colocando o título da tela como "Calculadora Básica"

```
#include "window.hpp"

int main(int argc, char **argv) {
  try {
    abcg::Application app(argc, argv);

    Window window;
    window.setWindowSettings(
        {.width = 600, .height = 600, .title = "Calculadora Básica"});

    app.run(window);
  } catch (std::exception const &exception) {
    fmt::print(stderr, "{}\n", exception.what());
    return -1;
  }
  return 0;
}
```

### window.hpp

Aqui definimos a classe Window, responsável pelo gerenciamento da aplicação.
Armazena-se também as variáveis e métodos utilizados no window.cpp. No código há comentários explicando a função de cada uma delas.

```
#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  // Fonte que aumenta o tamanho do texto
  ImFont *m_font{};

  // Armazena o resultado da conta
  std::string result{"0.000"};

  /*
   * Texto que mostra se a conta está sendo feita utilizando os valores dos
   * sliders ou dos inputs
   */
  std::string option{"A conta está utilizando os valores dos Sliders"};

  /*
   * Booleano que indica se estamos utilizando o slider ou o input na operação,
   * se true está se utilizando os sliders, se false está se utilizando os
   * inputs
   */
  bool sliderChoosed{true};

  // float que armazena os valores dos sliders
  float slider01{50};
  float slider02{50};

  // double que armazena os valores dos inputs
  double input01{0.0};
  double input02{0.0};

  /*
   * Aqui define-se o slider que irá conter as operações
   */
  enum Element {
    Element_Sum,
    Element_Subtraction,
    Element_Multiplication,
    Element_Division,
    Element_COUNT
  };
  const char *elems_names[Element_COUNT] = {"+", "-", "X", "÷"};
  int elem = Element_Sum;

  /* Função que faz o cálculo recebendo dois valores, ela também é responsável
   * por atualizar o texto do resultado.
   */
  void calculate(double n1, double n2);

  // Função para adicionar espaços entre as ferramentas
  void doSpacing(int spaces);
};

#endif
```

### window.cpp

No **onCreate** carrega-se a fonte utilizada para aumentar o tamanho dos textos e já se inicia calculando o resultado através dos sliders.

```
void Window::onCreate() {
  // Carregando a fonte que aumenta o tamanho da letra
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  // O resultado inicial é a partir dos valores dos sliders
  calculate(slider01, slider02);
}
```

Aqui mostra-se o resultado da conta para o usuário com um texto indicando se os valores foram retirados dos sliders ou dos inputs.

```
void Window::onPaintUI() {
  // Pegando tamanho da janela de aplicação
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Calculadora Básica" Window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Calculadora Básica", nullptr, flags);
    doSpacing(1);

    // Texto que mostra o resultado da conta
    {
      ImGui::PushFont(m_font);

      // Centralizar texto
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(result.c_str()).x) / 2);
      ImGui::Text("%s", result.c_str());
      ImGui::PopFont();

      /*
       *Texto que mostra se o resultado da conta está utilizando os valores dos
       *sliders ou dos inputs
       */
      if (sliderChoosed) {
        option = "A conta está utilizando os valores dos Sliders";
      } else {
        option = "A conta está utilizando os valores dos Inputs";
      }
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(option.c_str()).x) / 2);
      ImGui::Text("%s", option.c_str());

      doSpacing(1);
    }
```

Nesta parte vamos ter uma tabela com duas colunas. A coluna da esquerda vai fornecer ao usuário dois sliders, enquanto a coluna da direita vai fornecer dois inputs.

Toda vez que um slider for selecionado, a variável **sliderChoosed** vai para true e então calcula-se o resultado da contagem de acordo com a operação atual. O mesmo vale para os inputs, toda vez que eles forem modificados, a variável **sliderChoosed** se tornará false e então será calculado um novo resultado.

Além disso, cada coluna conta com um botão para se calcular o resultado com os valores atuais dos sliders e inputs, sem precisar alterá-los.

```
// Tabela onde ficam as ferramentas da calculadora
    if (ImGui::BeginTable("Calculator Input Options", 2, 1)) {
      ImGui::TableNextRow();

      // Coluna dos Sliders
      {
        ImGui::TableSetColumnIndex(0);

        ImGui::Indent(50.f);

        // Primeiro Slider
        ImGui::Text("Primeiro número");
        if (ImGui::SliderFloat("##01", &slider01, 0.0f, 100.0f, "%.3f")) {
          sliderChoosed = true;
          calculate(slider01, slider02);
        }

        doSpacing(3);

        // Segundo Slider
        ImGui::Text("Segundo número");
        if (ImGui::SliderFloat("##02", &slider02, 0.0f, 100.0f, "%.3f")) {
          sliderChoosed = true;
          calculate(slider01, slider02);
        }

        doSpacing(3);

        // Botão dos Sliders
        ImGui::Indent(50.f);
        if (ImGui::Button("Calcular##01")) {
          sliderChoosed = true;
          calculate(slider01, slider02);
        }
      }

      // Coluna dos inputs
      {
        ImGui::TableSetColumnIndex(1);

        ImGui::Indent(50.f);

        // Primeiro Input
        ImGui::Text("Primeiro número");
        if (ImGui::InputDouble("##03", &input01, 0, 0, "%.3f")) {
          sliderChoosed = false;
          calculate(input01, input02);
        }

        doSpacing(3);

        // Segundo Input
        ImGui::Text("Segundo número");
        if (ImGui::InputDouble("##04", &input02, 0, 0, "%.3f")) {
          sliderChoosed = false;
          calculate(input01, input02);
        }

        doSpacing(3);

        // Botão dos Inputs
        ImGui::Indent(50.f);
        if (ImGui::Button("Calcular##02")) {
          sliderChoosed = false;
          calculate(input01, input02);
        }
      }

      ImGui::Unindent(50.f);
      doSpacing(1);
      ImGui::EndTable();
    }
```

O slider da operação também chamará a função que faz o cálculo do resultado toda vez que for alterado, dependendo do valor da variável **sliderChoosed** usaremos os valores dos sliders, ou dos inputs.

```
    // Slider para escolha de operações
    const char *elem_name =
        (elem >= 0 && elem < Element_COUNT) ? elems_names[elem] : "Unknown";
    if (ImGui::SliderInt("##05", &elem, 0, Element_COUNT - 1, elem_name)) {
      if (sliderChoosed) {
        calculate(slider01, slider02);
      } else {
        calculate(input01, input02);
      }
    }
```

A função **calculate** recebe dois parâmetros double para fazer a contagem e modificar o resultado que é mostrado ao usuário.
A função **doSpacing** recebe um parâmetro int, adicionando uma quantidade de espaço de acordo com esse valor.

```
/**
 *Função que faz o cálculo através de 2 números passados, faz o cast para string
 *e armazena o valor na variável que mostra o resultado para o usuário
 */
void Window::calculate(double n1, double n2) {
  switch (elem) {
  case 0:
    result = std::to_string(n1 + n2);
    break;
  case 1:
    result = std::to_string(n1 - n2);
    break;
  case 2:
    result = std::to_string(n1 * n2);
    break;
  case 3:
    result = std::to_string(n1 / n2);
    break;
  }
}

/*
 *Adiciona uma quantidade de espaços entre as ferramentas de acordo com um
 *valor passado
 */
void Window::doSpacing(int spaces) {
  for (auto const i : iter::range(spaces)) {
    ImGui::Spacing();
  }
}
```

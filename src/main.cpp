#include <iostream>
#include <fstream>
#include <vector>

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "globals.hpp"
#include "control.hpp"
#include "map.hpp"
#include "steering.hpp"

const ImColor im_canvasColor(0xf1, 0xf2, 0xf3, 0xff);
const ImColor im_borderColor(0x58, 0x5a, 0x56, 0xff);
const ImColor im_focusColor(0x81, 0xa3, 0xa7, 0xff);
const ImColor im_red(0xf1, 0, 0, 0xff);
const sf::Color sf_bgColor(0xc2, 0xd3, 0xda);

/*
 * GLOBAL VARIABLES:  Defined and declared extern in globals.hpp
 */

State state;
std::vector<LogData> logData;

int main() {
  sf::RenderWindow window(sf::VideoMode(640, 480), "WaveRunner monitoring / diagnostics tool");
  window.setVerticalSyncEnabled(true);
  ImGui::SFML::Init(window);

  window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
  sf::Clock deltaClock;
  
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);

      if (event.type == sf::Event::Closed) {
	window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::ShowDemoWindow();
    controlWidget();
    mapWidget();
    steeringWidget();

    window.clear(sf_bgColor);
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
}


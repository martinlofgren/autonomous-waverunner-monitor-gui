#include "steering.hpp"

void steeringWidget() {
  ImGui::Begin("Throttle/nozzle");
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  // Iterate through log and find nozzle angle and throttle values
  std::vector<float> nozzle_vals, throttle_vals;
  for (auto it = logData.begin(); it != logData.end() && state.time >= it->timestamp; it++) {
    switch (it->type) {
    case LogData::throttle:
      throttle_vals.push_back(it->data.throttle);
      break;
    case LogData::nozzle_angle:
      nozzle_vals.push_back(it->data.nozzle_angle);
      break;
    }
  }
  float cur_nozzle_val = (nozzle_vals.empty()) ? 0.0 : nozzle_vals.back();
  float cur_throttle_val = (throttle_vals.empty()) ? 0.0 : throttle_vals.back();

  ImGui::Text("Throttle: %f", cur_throttle_val);

  ImVec2 canvas_size = ImGui::GetContentRegionAvail();
  if (canvas_size.x < 50.0f) canvas_size.x = 50.0f;
  if (canvas_size.y < 50.0f) canvas_size.y = 50.0f;

  ImGui::PlotLines("", throttle_vals.data(), throttle_vals.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0,80));
  ImGui::SameLine();

  // Graphical representation of current throttle value
  const int throttle_width = 80;
  const int throttle_height = 80;
  ImVec2 throttle_topleft = ImGui::GetCursorScreenPos();
  draw_list->AddRectFilled(throttle_topleft, ImVec2(throttle_topleft.x + throttle_width,
						    throttle_topleft.y + throttle_height), im_canvasColor);
  draw_list->AddLine(ImVec2(throttle_topleft.x + throttle_width / 2, throttle_topleft.y + 10),
		     ImVec2(throttle_topleft.x + throttle_width / 2, throttle_topleft.y + throttle_width - 10), im_focusColor, 1);
  draw_list->AddRectFilled(ImVec2(throttle_topleft.x + throttle_width / 2 - 10, throttle_topleft.y + 8 + (1 - cur_throttle_val) * (throttle_height - 2 * 8)),
			   ImVec2(throttle_topleft.x + throttle_width / 2 + 10, throttle_topleft.y + 12 + (1 - cur_throttle_val) * (throttle_height - 2 * 8)),
			   im_focusColor);

  ImGui::NewLine();
  ImGui::Text("Nozzle angle: %f", cur_nozzle_val);
  ImGui::PlotLines("", nozzle_vals.data(), nozzle_vals.size(), 0, NULL, -(PI / 10), PI / 10, ImVec2(0,80));
  ImGui::SameLine();

  // Graphical representation of current nozzle angle value
  const int nozzle_width = 80;
  const int nozzle_height = 80;
  ImVec2 nozzle_topleft = ImGui::GetCursorScreenPos();
  draw_list->AddRectFilled(nozzle_topleft, ImVec2(nozzle_topleft.x + nozzle_width,
						  nozzle_topleft.y + nozzle_height), im_canvasColor);
  draw_list->AddLine(ImVec2(nozzle_topleft.x + nozzle_width / 2 - cos(cur_nozzle_val) * (nozzle_width / 2 - 10),
			    nozzle_topleft.y + nozzle_height / 2 + sin(cur_nozzle_val) * (nozzle_height / 2 - 10)),
		     ImVec2(nozzle_topleft.x + nozzle_width / 2 + cos(cur_nozzle_val) * (nozzle_width / 2 - 10),
			    nozzle_topleft.y + nozzle_height / 2 - sin(cur_nozzle_val) * (nozzle_height / 2 - 10)),
		     im_focusColor, 5);

  ImGui::End();
}
